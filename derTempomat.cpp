/*
 * derTempomat.cpp
 *
 *  Created on: 16.07.2018
 *      Author: cybaer
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "avrlib/time.h"
#include "avrlib/gpio.h"
#include "avrlib/adc.h"
#include "HardwareConfig.h"
#include "clock.h"
#include "ui.h"

// __Compiler Bug__
int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {};
void __cxa_pure_virtual() {};


volatile uint8_t num_clock_ticks = 0;
volatile bool poll = false;

ISR(TIMER1_COMPA_vect)
{
  PwmChannel1A::set_frequency(clock.Tick());  // 240 Ticks per Clock from ClockInput
  if(clock.running())
  {
    ++num_clock_ticks;
  }
  Output_1::Toggle();
}

ISR(TIMER2_OVF_vect, ISR_NOBLOCK)
{
  //ca 4kHz
  while (num_clock_ticks)
  {
    --num_clock_ticks;
    ui.onClock(); // reicht Clock an die App weiter
  }
  static int8_t subClock = 0;
  subClock = (subClock + 1) & 3;

  if (subClock == 0)
  { // 1kHz
    poll = true;
  }
}



int main(void)
{
  // Configure the timers.
    static const uint8_t PRESCALER = 3;
    static const uint8_t PRESCALER_VALUE = 64;
    //PWM mit CTC
    Timer<1>::set_mode(0, _BV(WGM12), PRESCALER);
    int16_t startFreq = 500;
    int16_t counterForStartFreq = (20000000L / PRESCALER_VALUE) / startFreq -1;

    PwmChannel1A::set_frequency(counterForStartFreq);
    Timer<1>::StartCompare();
    //TIMSK |= (1<<OCIE1A);

    //     16MHz / (8 * 510) = 3906,25 Hz
    // prescaler(2)_|
    Timer<2>::set_prescaler(2);
    Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);
    Timer<2>::Start();

    sei();

  Adc::StartConversion(AdcChannelCV);

  ui.init();
  // after initialization of all port extender IOs
  initHW();
  _delay_ms(50);

  portExtender::WriteIO();


  while(1)
  {
    static uint8_t step = 3;
    if(poll)
    {
      poll = false;
      ui.poll();
      ui.doEvents();

      portExtender::WriteIO();
    }

    if (Adc::ready())
    {
        uint8_t threshold = MAX_CV - (Adc::Read(AdcChannelCV) >> 2) & 0xFF;
        Adc::StartConversion(AdcChannelCV);
    }

    if(ResetIn::isTriggered())
    {

    }

    if(ClockIn::isTriggered())
    {
      bool TaktStart = clock.ClockInEdge();
      LED_Takt::set_value(TaktStart);
      if(TaktStart)
        ui.m_StepControl.reset();
    }

  }
}

/*
 * derTempomat.cpp
 *
 *  Created on: 16.07.2018
 *      Author: cybaer
 */

#include <avr/interrupt.h>
#include "avrlib/time.h"
#include "avrlib/gpio.h"
#include "avrlib/adc.h"
#include "HardwareConfig.h"
#include "clock.h"

volatile uint8_t num_clock_ticks = 0;
volatile bool poll = false;

ISR(TIMER1_COMPA_vect)
{
  PwmChannel1A::set_frequency(clock.Tick());  // 256 Ticks per Clock from ClockInput
  if(clock.running())
  {
    ++num_clock_ticks;
  }
}

ISR(TIMER2_OVF_vect, ISR_NOBLOCK)
{
  //ca 4kHz
  while (num_clock_ticks)
  {
    --num_clock_ticks;
    //ui.OnClock(); // reicht Clock an die App weiter
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
  sei();
  initHW();
  // Configure the timers.
    Timer<1>::set_prescaler(1);
    Timer<1>::set_mode(0, _BV(WGM12), 3);
    PwmChannel1A::set_frequency(625000L/120); //ToDo: magic numbers
    Timer<1>::StartCompare();

    //     16MHz / (8 * 510) = 3906,25 Hz
    // prescaler(2)_|
    Timer<2>::set_prescaler(2);
    Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);
    Timer<2>::Start();



  Adc::StartConversion(AdcChannelCV);

  while(1)
  {
    static uint8_t step = 3;

    if (Adc::ready())
    {
        uint8_t threshold = MAX_CV - (Adc::Read(AdcChannelCV) >> 2) & 0xFF;
        Adc::StartConversion(AdcChannelCV);

    }

    if(ResetIn::isTriggered())
    {

    }

    if(ClockIn::isTriggered()) clock.ClockInEdge();

  }
}

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

//volatile uint8_t num_clock_ticks = 0;
volatile bool poll = false;

ISR(TIMER1_COMPA_vect)
{
  PwmChannel1A::set_frequency(clock.Tick());  // 240 Ticks per Clock from ClockInput
  if(clock.running())
  {
   // ++num_clock_ticks;
  }
  Output_1::Toggle();
}

ISR(TIMER2_OVF_vect, ISR_NOBLOCK)
{
  //ca 4kHz
  //while (num_clock_ticks)
  //{
  //  --num_clock_ticks;
    //ui.OnClock(); // reicht Clock an die App weiter
  //}
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
    static const uint8_t PRESCALER = 3;
    static const uint8_t PRESCALER_VALUE = 64;
    //PWM mit CTC
    Timer<1>::set_mode(0, _BV(WGM12), PRESCALER);
    int16_t startFreq = 500;
    int16_t counterForStartFreq = (20000000L / PRESCALER_VALUE) / startFreq -1;

    PwmChannel1A::set_frequency(counterForStartFreq);
    Timer<1>::StartCompare();

    //     16MHz / (8 * 510) = 3906,25 Hz
    // prescaler(2)_|
    Timer<2>::set_prescaler(2);
    Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);
    Timer<2>::Start();



  Adc::StartConversion(AdcChannelCV);
  Leds LEDs;
  SWITCHES Switches;

  LEDs.init();
  Switches.init();
  // after initialization of all port extender IOs
  initHW();
  _delay_ms(50);
  portExtender::WriteIO();

  LEDs.setColor(true, 1);

  while(1)
  {
    static uint8_t step = 3;
    if(poll)
    {
      poll = false;
      Switch_A::Read();
      Switch_B::Read();
      Switch_Mod::Read();
      portExtender::ReadIO();
      Switches.refresh();

      //Led_1.set(Sw_1.active());
      static uint16_t i=0;
      if(i++ & 0x880)
        LEDs.setWithMask(0x1f);
      else
        LEDs.clear();

      int8_t index = 0;
      if(Switches.isActive(index))
      {
        LEDs.set(index);
      }
      else
      {
        LEDs.clear();
      }
      portExtender::WriteIO();



    }
    int8_t index = 1;


    //LED_A::set_value(Switch_A::low());
    //LED_B::set_value(Switch_B::low());
    //LED_Mod::set_value(Switch_Mod::low());
    //LED_Takt::set_value(Switch_Mod::low());



        //Switch_1::active());

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
      clock.ClockInEdge();
      uint8_t takt = clock.getStepCount() == 0;
      LED_Takt::set_value(takt);
    }

  }
}

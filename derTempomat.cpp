/*
 * derTempomat.cpp
 *
 *  Created on: 16.07.2018
 *      Author: cybaer
 */


#include "avrlib/time.h"
#include "avrlib/gpio.h"
#include "avrlib/adc.h"
#include "HardwareConfig.h"

int main(void)
{
  sei();
  initHW();
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

    Clock::isTriggered();

  }
}

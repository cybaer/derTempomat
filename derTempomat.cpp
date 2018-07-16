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
#include "BernoulliGate.h"

BernoulliGate<Output_1, Output_2> bernoulliGate;

int main(void)
{
  sei();
  initHW();
  Adc::StartConversion(AdcChannelSwitch);
  bernoulliGate.setMaxValue(MAX_CV);
  bernoulliGate.setThreshold(64);

  while(1)
  {
    static uint8_t adcChannel = AdcChannelSwitch;
    static const uint8_t adcChannels[] = {AdcChannelSwitch, AdcChannelCV};
    static uint8_t step = 3;

    if (Adc::ready())
    {
      switch(adcChannel)
      {
      case AdcChannelSwitch:
      {
        step = SwitchSteps::getValue();
        Adc::StartConversion(AdcChannelCV);
        SeqSwitch.setStepCount(step);
        adcChannel = AdcChannelCV;
        break;
      }
      case AdcChannelCV:
      {
        uint8_t threshold = MAX_CV - (Adc::Read(AdcChannelCV) >> 2) & 0xFF;
        Adc::StartConversion(AdcChannelSwitch);
        bernoulliGate.setThreshold(threshold);
        adcChannel = AdcChannelSwitch;
        break;
      }
      default: adcChannel = 0;
      }
    }

    if(ResetIn::isTriggered())
    {
      //BernoulliGate.onReset();
      SeqSwitch.onReset();
    }

    if(step == 0)
    {
      const bool triggerVal = Trigger::getValue();
      if(Trigger::isTriggered())
      {
        bernoulliGate.activateNextStep();
        // prevents glitches through hardware latencies
        _delay_us(200.0);
      }
      OutputReturn::set_value(triggerVal);
    }
    else
    {
      if(Trigger::isTriggered())
      {
        SeqSwitch.activateNextStep();
      }
    }
  }
}

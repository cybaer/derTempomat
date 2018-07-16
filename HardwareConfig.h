/*
 * HardwareConfig.h
 *
 *  Created on: 09.02.2017
 *      Author: cybaer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef HARDWARECONFIG_H_
#define HARDWARECONFIG_H_

#include "lib/ui/EdgeTrigger.h"
#include "lib/ui/analogSwitch.h"
#include "avrlib/base.h"
#include "avrlib/gpio.h"
#include "avrlib/adc.h"

using namespace avrlib;
static const uint8_t MAX_CV = 122;

template <typename Out_1, typename Out_2, typename Out_3, typename Out_4>
class SequentialSwitch
{
public:
  void activateNextStep(void)
  {
    if(++m_Step > m_StepCount)
      m_Step = 0;
    inactivateAll();
    m_ActivatFunctions[m_Step]();
  }
  void setStepCount(uint8_t count)
  {
    if(count != m_StepCount)
    {
      m_StepCount = count > 3 ? 3 : count;
      if(count == 0)
      {
        onReset();
      }
    }
  }
  void onReset(void)
  {
    m_Step = 0;
    inactivateAll();
    if(m_StepCount != 0)
      m_ActivatFunctions[m_Step]();
  }

private:
  static void inactivateAll(void)
  {
    Out_1::Low();
    Out_2::Low();
    Out_3::Low();
    Out_4::Low();
  }

  typedef void (*FP)(void);

  const FP m_ActivatFunctions[4] = {Out_1::High, Out_2::High, Out_3::High, Out_4::High};
  uint8_t m_StepCount;
  uint8_t m_Step;
};

typedef EdgeTrigger<Gpio<PortD, 7>, 0> Trigger;  // 13
typedef EdgeTrigger<Gpio<PortB, 2>, 0> ResetIn;  // 16

typedef Inverter<Gpio<PortD, 4> > Output_1;  // 6
typedef Inverter<Gpio<PortD, 2> > Output_2;  // 4
typedef Inverter<Gpio<PortD, 1> > Output_3;  // 3
typedef Inverter<Gpio<PortD, 0> > Output_4;  // 2

SequentialSwitch<Output_1, Output_2, Output_3, Output_4> SeqSwitch;

typedef Inverter<Gpio<PortB, 6> > OutputReturn;  // 9
typedef Gpio<PortB, 7> Debug;                    // 10

static const uint8_t AdcChannelCV = 1;
static const uint8_t AdcChannelSwitch = 4;
typedef AnalogSwitch<Adc, 5, AdcChannelSwitch> SwitchSteps;

extern Adc adc;

inline void initInputs(void)
{
  ResetIn::init();
  Trigger::init();
}
inline void initOutputs(void)
{
  Output_1::set_mode(DIGITAL_OUTPUT);
  Output_1::set_value(false);
  Output_2::set_mode(DIGITAL_OUTPUT);
  Output_2::set_value(false);
  Output_3::set_mode(DIGITAL_OUTPUT);
  Output_3::set_value(false);
  Output_4::set_mode(DIGITAL_OUTPUT);
  Output_4::set_value(false);
  OutputReturn::set_mode(DIGITAL_OUTPUT);
  OutputReturn::set_value(false);
  Debug::set_mode(DIGITAL_OUTPUT);
  Debug::set_value(true);
}

inline void initAnalogIn(void)
{
  adc.Init();
}

inline void initHW(void)
{
  initInputs();
  initOutputs();
  initAnalogIn();
}


#endif /* HARDWARECONFIG_H_ */

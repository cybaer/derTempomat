/*
 * HardwareConfig.h
 *
 *  Created on: 19.07.2018
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
#include "avrlib/base.h"
#include "avrlib/gpio.h"
#include "avrlib/adc.h"

using namespace avrlib;
static const uint8_t MAX_CV = 122;

typedef EdgeTrigger<Gpio<PortD, 7>, 0> Clock;  // 13
typedef EdgeTrigger<Gpio<PortB, 2>, 0> ResetIn;  // 16

typedef Inverter<Gpio<PortD, 4> > Output_1;  // 6
typedef Inverter<Gpio<PortD, 2> > Output_2;  // 4
typedef Inverter<Gpio<PortD, 1> > Output_3;  // 3
typedef Inverter<Gpio<PortD, 0> > Output_4;  // 2
typedef Inverter<Gpio<PortD, 3> > Output_5;  // x

typedef Gpio<PortB, 7> Debug;                    // 10

static const uint8_t AdcChannelCV = 1;

extern Adc adc;

inline void initInputs(void)
{
  ResetIn::init();
  Clock::init();
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
  Output_5::set_mode(DIGITAL_OUTPUT);
  Output_5::set_value(false);
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

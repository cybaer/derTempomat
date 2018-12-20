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
#include "avrlib/devices/switch.h"
#include "avrlib/devices/mcp23s17.h"
#include "avrlib/devices/bicolor_led_group.h"
#include "avrlib/devices/switch_group.h"


#include "avrlib/base.h"
#include "avrlib/gpio.h"
#include "avrlib/spi.h"
#include "avrlib/adc.h"

using namespace avrlib;
static const uint8_t MAX_CV = 122;

typedef DebouncedSwitch< Gpio<PortB, 0> > Switch_A;  // 12
typedef DebouncedSwitch< Gpio<PortD, 6> > Switch_B;  // 10
typedef DebouncedSwitch< Gpio<PortD, 5> > Switch_Mod;  // 9

typedef Gpio<PortB, 1> LED_A;  // 13
typedef Gpio<PortD, 7> LED_B;  // 11
typedef Gpio<PortD, 3> LED_Mod;  // 1
typedef Gpio<PortD, 4> LED_Takt;  // 2

typedef EdgeTrigger<Gpio<PortC, 0>, 0> ClockIn;  // 23
typedef EdgeTrigger<Gpio<PortC, 1>, 0> ResetIn;  // 24
typedef Gpio<PortD, 1> ModIn;  // 31

typedef Inverter<Gpio<PortC, 2> > Output_5;  // 6
typedef Inverter<Gpio<PortC, 3> > Output_4;  // 4
typedef Inverter<Gpio<PortC, 4> > Output_3;  // 3
typedef Inverter<Gpio<PortC, 5> > Output_2;  // 2
typedef Inverter<Gpio<PortD, 0> > Output_1;  // x

// HW SPI
static const uint8_t SPI_Speed = 4;
typedef SpiMasterBase<MSB_FIRST, SPI_Speed> spi_master;

// Port Extender
typedef Gpio<PortB, 2> ExtenderSlaveSelect;
typedef MCP23S17<spi_master, ExtenderSlaveSelect, 0> portExtender;

typedef LED<PortPin<portExtender, PORT_A, 0>, PortPin<portExtender, PORT_A, 1> > LED_1;
typedef LED<PortPin<portExtender, PORT_A, 3>, PortPin<portExtender, PORT_A, 4> > LED_2;
typedef LED<PortPin<portExtender, PORT_A, 5>, PortPin<portExtender, PORT_A, 7> > LED_3;
typedef LED<PortPin<portExtender, PORT_B, 5>, PortPin<portExtender, PORT_B, 3> > LED_4;
typedef LED<PortPin<portExtender, PORT_B, 2>, PortPin<portExtender, PORT_B, 0> > LED_5;

typedef Switch<portExtender, PORT_B, 7> Switch_1;
typedef Switch<portExtender, PORT_A, 2> Switch_2;
typedef Switch<portExtender, PORT_A, 6> Switch_3;
typedef Switch<portExtender, PORT_B, 4> Switch_4;
typedef Switch<portExtender, PORT_B, 1> Switch_5;

typedef LEDGroup5 <LED_1, LED_2, LED_3, LED_4, LED_5> Leds;
typedef SwitchGroup5 <Switch_1, Switch_2, Switch_3, Switch_4, Switch_5> SWITCHES;

typedef Gpio<PortB, 7> Debug;                    // 10

static const uint8_t AdcChannelCV = 7;

extern Adc adc;

inline void initInputs(void)
{
  ResetIn::init();
  ClockIn::init();
  Switch_A::Init();
  Switch_B::Init();
  Switch_Mod::Init();
}
inline void initOutputs(void)
{
  Output_1::set_mode(DIGITAL_OUTPUT);
  Output_1::set_value(true);
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
  LED_A::set_mode(DIGITAL_OUTPUT);
  LED_A::set_value(false);
  LED_B::set_mode(DIGITAL_OUTPUT);
  LED_B::set_value(false);
  LED_Mod::set_mode(DIGITAL_OUTPUT);
  LED_Mod::set_value(false);
  LED_Takt::set_mode(DIGITAL_OUTPUT);
  LED_Takt::set_value(false);
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
  spi_master::Init();
  portExtender::Init();
}

#endif /* HARDWARECONFIG_H_ */

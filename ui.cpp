/*
 * ui.cpp
 *
 *  Created on: Apr 24, 2019
 *      Author: cybaer
 */

#include "HardwareConfig.h"
#include "ui.h"

 Ui::Ui()
 {

 }
void Ui::init()
{
  LEDs.init();
  LEDs.setColor(true, 1);
  Switches.init();
}
void Ui::poll()
{
  Switch_A::Read();
  Switch_B::Read();
  Switch_Mod::Read();
  portExtender::ReadIO();
  Switches.refresh();
}

void Ui::doEvents()
{
  int8_t index = 0;
  if(Switches.isActive(index))
  {
    LEDs.set(index);
  }
  else
  {
    LEDs.clear();
  }
}

Ui ui;

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
void Ui::poll()
{
  Switch_A::Read();
  Switch_B::Read();
  Switch_Mod::Read();
  portExtender::ReadIO();
  Switches.refresh();
}

Ui ui;

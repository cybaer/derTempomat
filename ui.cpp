/*
 * ui.cpp
 *
 *  Created on: Apr 24, 2019
 *      Author: cybaer
 */

#include "HardwareConfig.h"
#include "ui.h"

 Ui::Ui()
 : m_State(&Ui::MultividerModeState::getInstance())
 {

 }
void Ui::init()
{
  LEDs.init();
  LEDs.setColor(true, 1);
  Switches.init();
  m_StepControl.setStepCount(8);
  m_StepControl.reset();
}
void Ui::poll()
{
  Button_A::Read();
  Button_B::Read();
  Button_Mod::Read();
  portExtender::ReadIO();
  Switches.refresh();
}

void Ui::doEvents()
{
  if(Button_A::lowered()) m_State->onButtonA(*this);
  if(Button_A::raised()) m_State->onReleaseButtonA(*this);
  if(Button_B::lowered()) m_State->onButtonB(*this);
  if(Button_B::raised()) m_State->onReleaseButtonB(*this);

  /*int8_t index = 0;
  if(Switches.isActive(index))
  {
    LEDs.set(index);
  }
  else
  {
    LEDs.clear();
  }*/
}

void Ui::onClock()
{
  bool signal = m_StepControl.onTick();
  if(signal)
    {
      LEDs.set(1);
    }
    else
    {
      LEDs.clear();
    }

}

/*** State machine ***/
void Ui::MultividerModeState::onEntry(Context_t& context) const
{

}
void Ui::MultividerModeState::onExit(Context_t& context) const
{

}
void Ui::MultividerModeState::onButtonA(Context_t& context) const
{
  context.setState(Ui::HoldButtonAState::getInstance());
}

void Ui::MultividerModeState::onButtonB(Context_t& context) const
{

}

void Ui::HoldButtonAState::onEntry(Context_t& context) const
{
  LED_A::High();
}
void Ui::HoldButtonAState::onExit(Context_t& context) const
{
  LED_A::Low();
}
void Ui::HoldButtonAState::onReleaseButtonA(Context_t& context) const
{
  context.setState(Ui::MultividerModeState::getInstance());
}
void Ui::HoldButtonAState::onButtonB(Context_t& context) const
{
  LED_B::Toggle();
}
void Ui::HoldButtonAState::onButtonX(Context_t& context, int8_t button) const
{

}

Ui ui;

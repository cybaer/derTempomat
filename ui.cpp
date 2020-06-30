/*
 * ui.cpp
 *
 *  Created on: Apr 24, 2019
 *      Author: cybaer
 */

#include "HardwareConfig.h"
#include "ui.h"

 Ui::Ui()
 : LEDs()
 , Switches()
 , m_AppMultivider()
 , m_State(&Ui::UseModeState::getInstance())
 , m_Button(0)
 , m_Counter(0)
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
  int8_t pressedSwitch = 0;
  if(Switches.getPressed(pressedSwitch)) m_State->onButtonX(*this, pressedSwitch+1);
}

void Ui::onClock()
{
  m_AppMultivider.onTick();

}

/*** State machine ***/
void Ui::UseModeState::onEntry(Context_t& context) const
{

}
void Ui::UseModeState::onExit(Context_t& context) const
{

}
void Ui::UseModeState::onButtonA(Context_t& context) const
{
  context.setState(Ui::DividerState::getInstance());
}

void Ui::UseModeState::onButtonB(Context_t& context) const
{
  context.setState(Ui::MultiplierState::getInstance());
}

void Ui::UseModeState::onButtonAB(Context_t& context) const
{

}

void Ui::DividerState::onEntry(Context_t& context) const
{
  context.m_Button = 0;
  context.m_Counter = 0;
}
void Ui::DividerState::onExit(Context_t& context) const
{
  context.m_AppMultivider.setDivider(context.m_Button, context.m_Counter);
  context.m_AppMultivider.setMultiplier(context.m_Button, 1);
}
void Ui::DividerState::onReleaseButtonA(Context_t& context) const
{
  context.setState(Ui::UseModeState::getInstance());
}
void Ui::DividerState::onButtonX(Context_t& context, int8_t button) const
{
  context.m_Button = button;
  context.m_Counter++;
}

void Ui::MultiplierState::onEntry(Context_t& context) const
{
  context.m_Button = 0;
  context.m_Counter = 0;
}
void Ui::MultiplierState::onExit(Context_t& context) const
{
  context.m_AppMultivider.setMultiplier(context.m_Button, context.m_Counter);
  context.m_AppMultivider.setDivider(context.m_Button, 1);
}
void Ui::MultiplierState::onReleaseButtonB(Context_t& context) const
{
  context.setState(Ui::UseModeState::getInstance());
}
void Ui::MultiplierState::onButtonX(Context_t& context, int8_t button) const
{
  context.m_Button = button;
  context.m_Counter++;
}


void Ui::PhaseAdjustState::onEntry(Context_t& context) const
{

}
void Ui::PhaseAdjustState::onExit(Context_t& context) const
{
  context.LEDs.clear();
}
void Ui::PhaseAdjustState::onButtonA(Context_t& context) const
{

}
void Ui::PhaseAdjustState::onButtonB(Context_t& context) const
{

}
void Ui::Edit_State::onButtonX(Context_t& context, int8_t button) const
{
  context.LEDs.set(button);
  context.setState(Ui::PhaseAdjustState::getInstance());
}

Ui ui;

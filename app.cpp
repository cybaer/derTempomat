/*
 * app.cpp
 *
 *  Created on: Jun 7, 2019
 *      Author: cybaer
 */

#include "app.h"
#include "stepControl.h"

AppMultivider::AppMultivider(StepControl& stepControl)
: m_ActiveChannel(0)
{
  m_Control[0].Control = &stepControl;
  m_Control[0].isActive = true;
  m_Control[0].Divider = 1;
  m_Control[0].Multiplier = 1;
  m_Control[0].PhaseCoarse = 0;
  m_Control[0].PhaseFine = 0;
}

bool AppMultivider::setActiveChannel(int8_t channel) { m_ActiveChannel = channel; }

void AppMultivider::setDivider(int8_t divider)
{
  m_Control[m_ActiveChannel].Divider = divider;
}


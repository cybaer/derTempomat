/*
 * stepControl.cpp
 *
 *  Created on: Jul 29, 2018
 *      Author: cybaer
 */

#include "stepControl.h"

StepControl::StepControl()
: m_ActualStep(0)
, m_TotalTick(0)
, m_StepTick(0)
, m_TotalTickMax(8*STEP_RESOLUTION)
, m_StepCount(8)
{
}
void StepControl::reset()
{
  m_ActualStep = 0;
  m_TotalTick = 0;
  m_StepTick = 0;
}

void StepControl::onTick()
{
  ++m_TotalTick;
  if(m_TotalTick == m_TotalTickMax)
  {
    reset();
  }
  else
  {
    m_ActualStep = m_TotalTick / STEP_RESOLUTION;
    m_StepTick = m_TotalTick % STEP_RESOLUTION;
  }
}

void StepControl::setStepOn(uint16_t onTime)
{
  uint8_t index = onTime / STEP_RESOLUTION;
  if(index < MAX_STEPS) m_StepTable[index].on = onTime % index;
}



void StepControl::setStepCount(uint8_t count)
{
  if(count <= MAX_STEPS)
  {
    m_StepCount = count;
    m_TotalTickMax = m_StepCount * STEP_RESOLUTION;
  }
}

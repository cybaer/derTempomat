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
  // test init
    for(int8_t i=0; i<8; i++)
    {
      m_StepTable[i].on = 1;
      m_StepTable[i].off = 30;
    }
}
void StepControl::reset()
{
  m_ActualStep = 0;
  m_TotalTick = 0;
  m_StepTick = 0;

}

bool StepControl::onTick()
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
  return    m_StepTick >= m_StepTable[m_ActualStep].on
         && m_StepTick < m_StepTable[m_ActualStep].off ;
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

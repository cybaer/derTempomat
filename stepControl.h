/*
 * stepControl.h
 *
 *  Created on: Jul 29, 2018
 *      Author: cybaer
 */

#ifndef STEPCONTROL_H_
#define STEPCONTROL_H_

#include <inttypes.h>

static const uint8_t STEP_RESOLUTION = 256;

struct StepData
{
  uint8_t on;
  uint8_t off;
};

class StepControl
{
public:
  static const int8_t MAX_STEPS = 32;
  StepControl();
  void reset();
  void onTick();
  void setStepOn(uint16_t onTime);
  void setStepCount(uint8_t count);

private:
  StepData m_StepTable[MAX_STEPS];
  uint8_t m_ActualStep;
  uint16_t m_TotalTick;
  uint8_t m_StepTick;
  uint8_t m_TotalTickMax;
  uint8_t m_StepCount;
};







#endif /* STEPCONTROL_H_ */

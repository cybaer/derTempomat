/*
 * app.h
 *
 *  Created on: Jun 6, 2019
 *      Author: cybaer
 */

#ifndef APP_H_
#define APP_H_

#include "HardwareConfig.h"

class StepControl;

class App
{
public:

};

class AppMultivider : public App
{
public:
  AppMultivider(StepControl& stepControl);
  bool setActiveChannel(int8_t channel);
  void setDivider(int8_t divider);
  void setMultiplier(int8_t multiplier);
  void incrementPhaseFine();
  void incrementPhaseCoarse();
  void decrementPhaseFine();
  void decrementPhaseCoarse();

private:
  int8_t m_ActiveChannel;
  struct MultividerData
  {
    StepControl* Control;
    bool isActive;
    int8_t Divider;
    int8_t Multiplier;
    int8_t PhaseCoarse;
    int8_t PhaseFine;
  };

  MultividerData m_Control[5];
};


#endif /* APP_H_ */

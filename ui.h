/*
 * ui.h
 *
 *  Created on: Apr 24, 2019
 *      Author: cybaer
 */

#ifndef UI_H_
#define UI_H_

#include "HardwareConfig.h"

using namespace avrlib;
class Ui
{
public:
  Ui(void);
  void init();
  void poll();
  void doEvents();
  void OnClock();

  Leds LEDs;
  SWITCHES Switches;

private:

};




extern Ui ui;

#endif /* UI_H_ */

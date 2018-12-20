/*
 * clock.h
 *
 *  Created on: 26.04.2016
 *      Author: cybaer
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "avrlib/base.h"
#include "avrlib/gpio.h"
#include "HardwareConfig.h"

using namespace avrlib;
static const int8_t STEPS = 8;
class Clock
{
public:
  static inline void init(void)
  {
    m_TickCount = 0;
    m_StepCount = STEPS;
    m_Running = true;
  }
  static inline void start(void)
  {
    m_TickCount = 0;
    m_StepCount = STEPS;
    m_Running = true;
  }
  static inline void stop(void) { m_Running = false; }
  static inline void reset(void);
  static inline bool running(void) { return m_Running; }
  static inline int16_t Tick()
  {
    // better calc here the m_Interval
    m_TickCount++;
    // calc m_StepCount
    return m_Interval;
  }
  static void ClockInEdge(void)
  {
    // safe actual m_TickCount (Attention! 16bit copy not thread safe)
    uint16_t newTick = m_TickCount;
    // 240 Ticks from Clock to Clock
    m_Interval = (m_Interval * (newTick - m_OldTick)) /240;
    m_OldTick = newTick;
    /// Better: calc new m_Inetrval in context of Tick() --> no safty problem with 16bit vars

    LED_A::Toggle();

    if(++m_StepCount >= STEPS)
      m_StepCount = 0;
  }

  static int8_t getStepCount(void) { return m_StepCount; }

  void update(uint16_t bpm, uint8_t multiplier = 1, uint8_t divider = 1/* ,uint8_t groove_template, uint8_t groove_amount*/);

private:
  static bool m_Running;
  //static uint32_t m_Clock;
  static uint16_t m_TickCount;
  static uint16_t m_OldTick;
  static uint8_t m_StepCount;
  //static uint16_t m_Intervals[NumStepsInGroovePattern]; für Groove Pattern, Humanizing, ...
  static uint16_t m_Interval;
};

extern Clock clock;

#endif /* CLOCK_H_ */

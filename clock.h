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
static const uint8_t INTERVALL_TICKS = 256;
static const int8_t STEPS = 8;
class Clock
{
public:
  static inline void init(void)
  {
    m_TickCount = 0;
    m_Steps = STEPS;
    m_StepCount = STEPS;
    m_Running = true;
  }
  static inline void setStepCount(int8_t steps) { m_Steps = steps; }
  static inline void start(void)
  {
    m_TickCount = 0;
    m_StepCount = m_Steps;
    m_Running = true;
  }
  static inline void stop(void) { m_Running = false; }
  static inline void reset(void);
  static inline bool running(void) { return m_Running; }
  static inline uint16_t Tick()
  {
    m_TickCount++;
    return m_Interval;
  }
  static bool ClockInEdge(void)
  {
    uint16_t newTick;
    // safe actual m_TickCount (Attention! 16bit copy not thread safe)
    do newTick = m_TickCount;
    while(newTick != m_TickCount);
    // 240 Ticks from Clock to Clock
    uint16_t deltaTick = newTick - m_OldTick;
    uint32_t numerator = static_cast<uint32_t>(m_Interval) * deltaTick;
    volatile uint16_t interval = numerator / INTERVALL_TICKS;
    // safety copy of 16 bit
    cli();
    m_Interval = interval;
    sei();

    m_OldTick = newTick;

    LED_A::Toggle();

    if(++m_StepCount >= STEPS || deltaTick > INTERVALL_TICKS * 10l)
      m_StepCount = 0;
    return m_StepCount == 0;
  }

  static int8_t getStepCount(void) { return m_StepCount; }

  void update(uint16_t bpm, uint8_t multiplier = 1, uint8_t divider = 1/* ,uint8_t groove_template, uint8_t groove_amount*/);

private:
  static bool m_Running;
  //static uint32_t m_Clock;
  static volatile uint16_t m_TickCount;
  static uint16_t m_OldTick;
  static int8_t m_Steps;
  static int8_t m_StepCount;
  //static uint16_t m_Intervals[NumStepsInGroovePattern]; für Groove Pattern, Humanizing, ...
  static uint16_t m_Interval;
};

extern Clock clock;

#endif /* CLOCK_H_ */

/*
 * app.h
 *
 *  Created on: Jun 6, 2019
 *      Author: cybaer
 */

#ifndef APP_H_
#define APP_H_

#include "HardwareConfig.h"
#include "lib/multivider.h"

class StepControl;

class App
{
public:

};

class AppMultivider : public App
{
public:
  AppMultivider();
  void onTick(void);
  void newInput(bool in);
  bool setActiveChannel(int8_t channel);
  void setDivider(int8_t channel, int8_t div);
  void setMultiplier(int8_t channel, int8_t mul);
  void incrementPhaseFine();
  void incrementPhaseCoarse();
  void decrementPhaseFine();
  void decrementPhaseCoarse();

private:

  template<typename Output, int8_t Channel>
  uint8_t setOutput(bool in)
  {
    int8_t out = m_Multivider[Channel-1].doStep(in);
    Output::set_value(out);
    return out << Channel-1;
  }
  uint8_t setOut1(bool in) { return setOutput<Output_1, 1>(in); }
  uint8_t setOut2(bool in) { return setOutput<Output_2, 2>(in); }
  uint8_t setOut3(bool in) { return setOutput<Output_3, 3>(in); }
  uint8_t setOut4(bool in) { return setOutput<Output_4, 4>(in); }
  uint8_t setOut5(bool in) { return setOutput<Output_5, 5>(in); }

  int8_t m_ActiveChannel;
  int8_t m_Tick;
  struct MultividerData
  {
    bool isActive;
    int8_t Divider;
    int8_t Multiplier;
    int8_t PhaseCoarse;
    int8_t PhaseFine;
  };

  class Clockomat
  {
  public:
    void newInput(bool in)
    {

      calculatePulseLen(in);
      if(isRisingEdge(in))
      {
        m_Counter++;
      }
      m_OldValue = in;
    }
    inline const int32_t& getCounter(void) const {return m_Counter; };

  private:
    int16_t m_Tick;
    int32_t m_Counter{0};
    bool m_OldValue;
    uint16_t m_InPulseStart;
    uint16_t m_PulseLen;
    uint16_t m_MaxPulseLen;



    inline bool isRisingEdge(bool const in) const { return in && !m_OldValue; };
    inline bool isFallingEdge(bool const in) const { return !in && m_OldValue; };
    inline void calculatePulseLen(bool const in)
    {
      if(isRisingEdge(in))
      {
        m_InPulseStart = m_Tick;
      }
      if(isFallingEdge(in))
      {
        m_PulseLen = m_Tick - m_InPulseStart;
        if(m_PulseLen > m_MaxPulseLen) m_PulseLen = m_MaxPulseLen;
      }
    }
  };
  class NewMultivider
  {
  public:
    NewMultivider(Clockomat& clock)
    : m_Clock(clock)
    , m_Divider(1)
    , m_Factor(1)
    , m_NextStep(1)
    , m_Action(false)
    {}

    void init(void)
    {
      m_NextStep = m_Divider;
    }
    void setDivisor(int8_t div)
    {
      m_Divider = div;
    }
    void setFactor(int8_t fac)
    {
      m_Factor = fac;
    }
    bool doStep(bool in)
    {
      if(in)
      {
        if(m_Clock.getCounter() == m_NextStep)
        {
          m_Action = in;
          m_NextStep += m_Divider;
        }
      }
      else
      {
        m_Action = false;
      }
      return m_Action;
    }
    Clockomat& m_Clock;
    int8_t m_Divider;
    int8_t m_Factor;
    int32_t m_NextStep;
    bool m_Action;
  };


  Clockomat m_Clock;
  NewMultivider m_Multivider[5];
};


#endif /* APP_H_ */

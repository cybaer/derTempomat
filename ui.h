/*
 * ui.h
 *
 *  Created on: Apr 24, 2019
 *      Author: cybaer
 */

#ifndef UI_H_
#define UI_H_

#include "HardwareConfig.h"
#include "app.h"
#include "stepControl.h"

class Ui;
typedef Ui Context_t;

using namespace avrlib;
class Ui
{
public:
  Ui(void);
  void init();
  void poll();
  void doEvents();
  void onClock();

  Leds LEDs;
  SWITCHES Switches;
  StepControl m_StepControl;

private:
  AppMultivider m_Multivider;




  class IUiState
    {
    public:
      IUiState(void) {}
      virtual void onEntry(Context_t& context) const {}
      virtual void onExit(Context_t& context) const {}
      virtual void onButtonA(Context_t& context) const {}
      virtual void onReleaseButtonA(Context_t& context) const {}
      virtual void onButtonB(Context_t& context) const {}
      virtual void onReleaseButtonB(Context_t& context) const {}
      virtual void onButtonX(Context_t& context, int8_t button) const {}

    private:
      /** Forbidden */
      DISALLOW_COPY_AND_ASSIGN (IUiState);
    };

    /**
     * State machine
     */
  class MultividerModeState: public IUiState
  {
  public:
    static MultividerModeState& getInstance(void)
    {
      static MultividerModeState s_instance;
      return s_instance;
    }
    virtual void onEntry(Context_t& context) const;
    virtual void onExit(Context_t& context) const;
    virtual void onButtonA(Context_t& context) const;
    virtual void onButtonB(Context_t& context) const;

  };

  class HoldButtonAState: public MultividerModeState
  {
  public:
    static HoldButtonAState& getInstance(void)
    {
      static HoldButtonAState s_instance;
      return s_instance;
    }
    virtual void onEntry(Context_t& context) const;
    virtual void onExit(Context_t& context) const;
    virtual void onReleaseButtonA(Context_t& context) const;
    virtual void onButtonB(Context_t& context) const;
    virtual void onButtonX(Context_t& context, int8_t button) const;
  };

    void setState(IUiState& state)
    {
      m_State->onExit(*this);
      m_State = &state;
      m_State->onEntry(*this);
    }

    IUiState* m_State;


};




extern Ui ui;

#endif /* UI_H_ */

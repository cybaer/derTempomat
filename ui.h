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
#include "lib/multivider.h"

static const int8_t MODE_N = 0;
static const int8_t MODE_A = 1;
static const int8_t MODE_B = 2;
static const int8_t MODE_AB = 3;


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


  AppMultivider m_AppMultivider;   // kann 5 Kanäle als Multivider verwalten
//AppSequencer m_Sequencer;     // kann 5 Kanäle als Sequencer verwalten

private:
  int8_t m_Button;
  int8_t m_Counter;

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
      virtual void onButtonAB(Context_t& context) const {}
      virtual void onButtonX(Context_t& context, int8_t button) const {}
      virtual void onReleaseButtonX(Context_t& context, int8_t button) const {}

    private:
      /** Forbidden */
      DISALLOW_COPY_AND_ASSIGN (IUiState);
    };

    /**
     * State machine
     */
  class UseModeState: public IUiState
  {
  public:
    static UseModeState& getInstance(void)
    {
      static UseModeState s_instance;
      return s_instance;
    }
    virtual void onEntry(Context_t& context) const;
    virtual void onExit(Context_t& context) const;
    virtual void onButtonA(Context_t& context) const;
    virtual void onButtonB(Context_t& context) const;
    virtual void onButtonAB(Context_t& context) const;
  };

  class Edit_State: public IUiState
    {
    public:
      static int8_t s_Val;
      static Edit_State& getInstance(void)
      {
        static Edit_State s_instance;
        return s_instance;
      }


      virtual void onButtonX(Context_t& context, int8_t button) const;
    };

  class DividerState: public IUiState
    {
    public:

      static DividerState& getInstance(void)
      {
        static DividerState s_instance;
        return s_instance;
      }
      virtual void onEntry(Context_t& context) const;
      virtual void onExit(Context_t& context) const;
      virtual void onReleaseButtonA(Context_t& context) const;
      virtual void onButtonX(Context_t& context, int8_t button) const;
    };

  class MultiplierState: public IUiState
      {
      public:
        static MultiplierState& getInstance(void)
        {
          static MultiplierState s_instance;
          return s_instance;
        }
        virtual void onEntry(Context_t& context) const;
        virtual void onExit(Context_t& context) const;
        virtual void onReleaseButtonB(Context_t& context) const;
        virtual void onButtonX(Context_t& context, int8_t button) const;
      };

  class PhaseAdjustState: public IUiState
  {
  public:
    static PhaseAdjustState& getInstance(void)
    {
      static PhaseAdjustState s_instance;
      return s_instance;
    }
    virtual void onEntry(Context_t& context) const;
    virtual void onExit(Context_t& context) const;
    virtual void onButtonA(Context_t& context) const;
    virtual void onButtonB(Context_t& context) const;

  };

  class PhaseLeftState: public IUiState
    {
    public:
      static PhaseLeftState& getInstance(void)
      {
        static PhaseLeftState s_instance;
        return s_instance;
      }
      virtual void onEntry(Context_t& context) const;
      virtual void onExit(Context_t& context) const;
      virtual void onReleaseButtonA(Context_t& context) const;
      virtual void onButtonX(Context_t& context, int8_t button) const;
    };

  class PhaseRightState: public IUiState
      {
      public:
        static PhaseRightState& getInstance(void)
        {
          static PhaseRightState s_instance;
          return s_instance;
        }
        virtual void onEntry(Context_t& context) const;
        virtual void onExit(Context_t& context) const;
        virtual void onReleaseButtonB(Context_t& context) const;
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

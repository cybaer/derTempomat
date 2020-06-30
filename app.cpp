/*
 * app.cpp
 *
 *  Created on: Jun 7, 2019
 *      Author: cybaer
 */

#include "app.h"
#include "ui.h"


AppMultivider::AppMultivider()
: m_ActiveChannel(0)
, m_Tick(0)
, m_Clock()
, m_Multivider{m_Clock,m_Clock,m_Clock,m_Clock,m_Clock}
{

}

void AppMultivider::onTick(void)
{
  for(int8_t i=0; i<5; i++)
  {
    //m_Multivider[i].onTick();
  }

  m_Tick++;
}



void AppMultivider::newInput(bool in)
{
  m_Clock.newInput(in);
  const int32_t& counter(m_Clock.getCounter());


  int8_t out=0;
  out |= setOut1(in);
  out |= setOut2(in);
  out |= setOut3(in);
  out |= setOut4(in);
  out |= setOut5(in);
  ui.LEDs.setWithMask(out);
}


bool AppMultivider::setActiveChannel(int8_t channel) { m_ActiveChannel = channel; }
void AppMultivider::setDivider(int8_t channel, int8_t div) { m_Multivider[channel-1].setDivisor(div); }
void AppMultivider::setMultiplier(int8_t channel, int8_t mul) { m_Multivider[channel-1].setFactor(mul); }



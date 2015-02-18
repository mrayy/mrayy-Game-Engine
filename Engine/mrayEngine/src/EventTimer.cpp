#include "stdafx.h"

#include "EventTimer.h"
#include "ScheduleManager.h"


namespace mray{

EventTimer::EventTimer()
{

	ScheduleManager::getInstance().registerTimer(this);

	m_enabled=false;
	m_interval=0.1f;

	m_timeToFire=0;
}

EventTimer::~EventTimer(){
}

void EventTimer::removeTimer(){
	ScheduleManager::getInstance().removeTimer(this);
}


void EventTimer::update(float dt){
	if(!m_enabled)
		return;
	m_timeToFire-=dt;
	if(m_timeToFire<0){
		m_timeToFire+=m_interval;
		onTick(this);
	}
}


void EventTimer::setEnabled(const bool&v){
	m_enabled=v;
	m_timeToFire=m_interval;
}

void EventTimer::setInterval(const float&v){
	m_interval=v;
	m_timeToFire=m_interval;
}

}
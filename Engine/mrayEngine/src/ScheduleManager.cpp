#include "stdafx.h"

#include "ScheduleManager.h"


namespace mray{

ScheduleManager::ScheduleManager(){
}

ScheduleManager::~ScheduleManager()
{
	ClearItems();
}


void ScheduleManager::registerTimer(EventTimer* timer){
	m_timers.push_back(timer);
}
void ScheduleManager::removeTimer(EventTimer* timer){
	TimerList::iterator p=m_timers.begin();
	TimerList::iterator end=m_timers.end();
	for(;p!=end;++p)
	{
		if(*p==timer){
			m_timers.erase(p);
			return;
		}
	}
}

void ScheduleManager::addItem(IScheduleItem*item){
	ItemsList::iterator p=m_items.begin();
	ItemsList::iterator end=m_items.end();
	for(;p!=end;++p)
	{
		if((*p)->timeToFire()>item->timeToFire()){
			m_items.insert(p,item);
			return;
		}
	}
	m_items.push_back(item);
}

void ScheduleManager::ClearItems()
{
	ItemsList::iterator it=m_items.begin();
	for (;it!=m_items.end();++it)
	{
		delete *it;
	}
	m_items.clear();
}

void ScheduleManager::update(float dt){
	{
		ItemsList::iterator it=m_items.begin();
		ItemsList::iterator it2;
		while(it!=m_items.end()){
			it2=it;
			it2++;
			if((*it)->update(dt)){
				(*it)->fireEvent();
				m_items.erase(it);
			}
			it=it2;
		}
	}
	{
		TimerList::iterator it=m_timers.begin();
		while(it!=m_timers.end()){
			(*it)->update(dt);
		}
	}
}


}
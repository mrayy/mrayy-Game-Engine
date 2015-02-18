#include "stdafx.h"


#include "EventQueue.h"
#include "Event.h"
#include "IEventReciver.h"


#include "EventMemoryManager.h"


namespace mray{

EventQueue::EventQueue(){
	m_maxEvents=10;
}
EventQueue::~EventQueue(){
	clear();
}
void EventQueue::setMaxEvents(int c){
	m_maxEvents=math::Max(c,1);
	checkEventsCount();
}
int EventQueue::getMaxEvents(){
	return m_maxEvents;
}
void EventQueue::checkEventsCount(){
	int diff=math::Max<int>(m_events.size()-m_maxEvents,0);
	EventList::iterator it=m_events.end();
	--it;
	for (;diff>0 && it!=m_events.end();diff--)
	{
		EventList::iterator it2=it;
		--it2;
		m_events.erase(it);
		it=it2;
	}
}

void EventQueue::pushEvent(Event*e,bool save){

	EventHandlerList::iterator p=m_handlers.begin();
	EventHandlerList::iterator end=m_handlers.end();
	for(;p!=end;++p)
	{
		EventHandlerStruct &h=(*p);
		std::set<uint>::iterator resMask=h.mask.find(e->getType().ID());
		if(resMask!=h.mask.end() || h.mask.empty()){
			h.handler->onEvent(e);
		}
	}
	if(!e->isRecived() && save){
		m_events.push_back(e);
		checkEventsCount();
	}
}
Event*  EventQueue::popEvent(){
	std::list<Event* >::iterator it=m_events.begin();
	if(it==m_events.end())
		return 0;
	Event*  e=*it;
	m_events.erase(it);
	EventMemoryManager::getInstance().deleteEvent(e);
	return e;
}

void EventQueue::clear(){
	m_events.clear();
}
int EventQueue::getEventsCount(){
	return m_events.size();
}
void EventQueue::addEventHandler(IEventReciver*handler,const std::set<uint>& mask){
	EventHandlerList::iterator p=m_handlers.begin();
	EventHandlerList::iterator end=m_handlers.end();
	for(;p!=end;++p)
	{
		if((*p).handler==handler){
			(*p).mask.insert(mask.begin(),mask.end());
			return;
		}
	}
	m_handlers.push_back(EventHandlerStruct(handler,mask));
}

void EventQueue::removeEventHandler(IEventReciver*handler){
	EventHandlerList::iterator p=m_handlers.begin();
	EventHandlerList::iterator end=m_handlers.end();
	for(;p!=end;++p)
	{
		if((*p).handler==handler){
			m_handlers.erase(p);
			return;
		}
	}
}

}


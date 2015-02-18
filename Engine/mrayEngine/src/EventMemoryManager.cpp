#include "stdafx.h"


#include "EventMemoryManager.h"
#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include "JoystickEvent.h"
#include "GUIEvent.h"
#include "NetworkEvent.h"
#include "ResizeEvent.h"
#include "SystemEvent.h"
#include "ChangeModeEvent.h"


namespace mray{

EventMemoryManager::EventMemoryManager(){

}

EventMemoryManager::~EventMemoryManager(){

}
const EventPtr& EventMemoryManager::getDeadEvent(const GUID& type)
{
	EventList::iterator it=m_deadEvents[type.ID()].begin();
	if(it==m_deadEvents[type.ID()].end()){
		it=m_activeEvents[type.ID()].begin();
		for(;it!=m_activeEvents[type.ID()].end();++it){
			const EventPtr & e=*it;
			if(e.getRefCount()==2){
				m_activeEvents[type.ID()].erase(it);
				e->reset();
				return e;
			}
		}
	}else{
		const EventPtr & e=*it;
		m_deadEvents[type.ID()].erase(it);
		e->reset();
		return e;
	}
	return EventPtr::Null;
}


const EventPtr& EventMemoryManager::createEvent(const GUID& type){
	EventPtr e=getDeadEvent(type);
	if(!e){
		if (type==ET_Unkown) e=  new Event(ET_Unkown);
		else if (type==ET_Mouse) e=  new MouseEvent();
		else if (type==ET_Keyboard) e=  new KeyboardEvent();
		else if (type==ET_Joystick) e=  new JoystickEvent();
		else if (type==ET_GUIEvent) e=  new GUIEvent();
		else if (type==ET_NetworkEvent) e=  new NetworkEvent();
		else if (type==ET_ResizeEvent) e=  new ResizeEvent();
		else if (type==ET_SystemEvent) e=  new SystemEvent();
		else if (type==ET_ChangeModeEvent) e=  new ChangeModeEvent();
		else return EventPtr::Null;
	}
	m_activeEvents[type.ID()].push_back(e);
	EventList::iterator it=m_activeEvents[type.ID()].end();
	--it;
	return *it;
}

void EventMemoryManager::deleteEvent(const EventPtr&e){
	const GUID& type=e->getType();
	EventMap::iterator it= m_activeEvents.find(type.ID());
	if(it==m_activeEvents.end())return;
	EventList::iterator it2=it->second.begin();
	for(;it2!=it->second.end();++it2){
		if(*it2==e){
			m_activeEvents[type.ID()].erase(it2);
			break;
		}
	}
	m_deadEvents[type.ID()].push_back(e);
}


}


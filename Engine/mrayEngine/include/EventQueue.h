

/********************************************************************
	created:	2009/02/26
	created:	26:2:2009   18:49
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\EventQueue.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	EventQueue
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___EventQueue___
#define ___EventQueue___


#include "mTypes.h"
#include "ISingleton.h"
#include "IEventReciver.h"
#include <set>

namespace mray
{


class MRAY_DLL EventQueue:public ISingleton<EventQueue>
{
protected:
	struct EventHandlerStruct 
	{
		EventHandlerStruct(){}
		EventHandlerStruct(IEventReciver*handler,const std::set<uint>& mask){
			this->handler=handler;
			this->mask=mask;
		}
		IEventReciver*handler;
		std::set<uint> mask;
	};
	typedef std::list<Event* > EventList;
	EventList m_events;
	typedef std::list<EventHandlerStruct> EventHandlerList;
	EventHandlerList m_handlers;

	int m_maxEvents;

	void checkEventsCount();
public:
	EventQueue();
	virtual~EventQueue();

	void setMaxEvents(int c);
	int getMaxEvents();

	virtual void pushEvent(Event*e,bool save=true);
	virtual Event*  popEvent();

	void clear();

	void addEventHandler(IEventReciver*handler,const std::set<uint>& mask);
	void removeEventHandler(IEventReciver*handler);

	int getEventsCount();
};

}


#endif //___EventQueue___


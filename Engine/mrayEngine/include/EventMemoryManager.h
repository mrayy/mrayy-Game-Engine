
/********************************************************************
	created:	2009/02/26
	created:	26:2:2009   20:53
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\EventMemoryManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	EventMemoryManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___EventMemoryManager___
#define ___EventMemoryManager___

#include "ISingleton.h"
#include "Event.h"
#include "GCPtr.h"



namespace mray{

class MRAY_DLL EventMemoryManager:public ISingleton<EventMemoryManager>
{
protected:
	typedef std::list<EventPtr > EventList;
	typedef std::map<uint,EventList> EventMap;
	EventMap m_deadEvents;//[ET_Events_Count];
	EventMap m_activeEvents;//[ET_Events_Count];

	const EventPtr& getDeadEvent(const GUID& type);

public:
	EventMemoryManager();
	virtual~EventMemoryManager();

	const EventPtr& createEvent(const GUID& type);
	void deleteEvent(const EventPtr&e);
};

}


#endif //___EventMemoryManager___


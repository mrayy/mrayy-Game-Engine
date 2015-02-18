

/********************************************************************
	created:	2009/02/26
	created:	26:2:2009   19:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\SystemEvent.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	SystemEvent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SystemEvent___
#define ___SystemEvent___

#include "Event.h"

namespace mray{

enum ESystemEventType{
	ESET_Show,
	ESET_Hide,
	ESET_Resized,
	ESET_LostFocus,
	ESET_GotFocus,
	ESET_LostActive,
	ESET_GotActive,
	ESET_Destroy
};

class SystemEvent:public Event
{
public:
	ESystemEventType m_eventType;
public:
	SystemEvent():Event(ET_SystemEvent)
	{

	}
	virtual~SystemEvent()
	{
	}
};

}


#endif //___SystemEvent___

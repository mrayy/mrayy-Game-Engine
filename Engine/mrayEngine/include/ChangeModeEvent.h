

/********************************************************************
	created:	2009/02/26
	created:	26:2:2009   22:01
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ChangeModeEvent.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ChangeModeEvent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ChangeModeEvent___
#define ___ChangeModeEvent___

#include "Event.h"

namespace mray{

enum EChangeModeEventType{
	ECMET_ToFullscreen,
	ECMET_ToWindow,
};
class ChangeModeEvent:public Event
{
public:
	EChangeModeEventType m_eventType;
	int width;
	int height;
	int bits;
public:
	ChangeModeEvent():Event(ET_ChangeModeEvent)
	{
		width=height=0;
		bits=0;
	}
};

}


#endif //___ChangeModeEvent___

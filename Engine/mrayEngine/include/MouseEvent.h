
/********************************************************************
	created:	2009/02/26
	created:	26:2:2009   18:56
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MouseEvent.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MouseEvent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___MouseEvent___
#define ___MouseEvent___

#include "Event.h"



namespace mray{
	namespace scene{
		class ViewPort;
	}

	enum MOUSE_EVENT_TYPE
	{
		MET_NONE,
		MET_LEFTDOWN,
		MET_RIGHTDOWN,
		MET_MIDDLEDOWN,
		MET_LEFTUP,
		MET_RIGHTUP,
		MET_MIDDLEUP,
		MET_LEFTDblClick,
		MET_RIGHTDblClick,
		MET_MIDDLEDblClick,
		MET_WHEEL,
		MET_MOVED
	};

class MRAY_DLL MouseEvent:public Event
{
public:
	math::vector2d vpSize;
	math::vector2d pos;
	math::vector2d rel;
	int MouseWheel;
	int MouseWheelRel;
	MOUSE_EVENT_TYPE event;

public:
	MouseEvent():Event(ET_Mouse),MouseWheel(0),MouseWheelRel(0),event(MET_NONE)
	{		
	}

	scene::ViewPort* GetViewPort();
};

}


#endif //___MouseEvent___

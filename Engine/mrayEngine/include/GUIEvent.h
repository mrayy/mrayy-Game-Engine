
/********************************************************************
	created:	2009/02/26
	created:	26:2:2009   19:03
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUIEvent.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GUIEvent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GUIEvent___
#define ___GUIEvent___

#include "Event.h"
#include "KEYCODE.h"


namespace mray{
	namespace GUI
	{
		class IGUIElement;
	}

	enum GUI_EVENT_TYPE
	{
		GET_NONE,
		GET_HAS_FOCUS,
		GET_Mouse_DOWN,
		GET_Mouse_DOWN_AGAIN,
		GET_Mouse_UP,
		GET_Mouse_MOVE,
		GET_CHANGED,
		GET_KeyDown,
		GET_DIALOG_CLOSE,
		GET_SELECT_ITEM
	};



class GUIEvent:public Event
{
public:
	GUI::IGUIElement*  Caller;
	GUI_EVENT_TYPE event;

	EKEY_CODE key;
public:
	GUIEvent():Event(ET_GUIEvent),Caller(0)
	{
	}
};

}


#endif //___GUIEvent___

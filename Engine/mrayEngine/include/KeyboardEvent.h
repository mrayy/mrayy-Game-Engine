

/********************************************************************
	created:	2009/02/26
	created:	26:2:2009   19:00
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\KeyboardEvent.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	KeyboardEvent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___KeyboardEvent___
#define ___KeyboardEvent___


#include "Event.h"
#include "KeyCode.h"

namespace mray{

class KeyboardEvent:public Event
{
public:
	uint Char;
	EKEY_CODE key;
	bool press;
	bool shift;
	bool lshift;
	bool alt;
	bool lalt;
	bool ctrl;
	bool lctrl;
public:
	KeyboardEvent():Event(ET_Keyboard),Char(0),press(0),
	shift(0),lshift(0),alt(0),lalt(0),ctrl(0),lctrl(0)
	{
		
	}
};

}


#endif //___KeyboardEvent___

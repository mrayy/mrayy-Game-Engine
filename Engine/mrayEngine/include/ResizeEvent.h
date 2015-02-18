
/********************************************************************
	created:	2009/02/26
	created:	26:2:2009   19:26
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ResizeEvent.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ResizeEvent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ResizeEvent___
#define ___ResizeEvent___

#include "Event.h"

namespace mray{

class ResizeEvent:public Event
{
public:
	int width;
	int height;
	int bits;
public:
	ResizeEvent():Event(ET_ResizeEvent),width(0),height(0),bits(0)
	{
	}
};

}


#endif //___ResizeEvent___

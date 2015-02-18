

/********************************************************************
	created:	2009/02/26
	created:	26:2:2009   19:02
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\JoystickEvent.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	JoystickEvent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___JoystickEvent___
#define ___JoystickEvent___

#include "Event.h"
#include "CJoysticController.h"


namespace mray{
	enum JOYSTIC_EVENT_TYPE
	{
		JET_BUTTON_PRESSED,
		JET_BUTTON_RELEASED,
		JET_AXIS_MOVED,
		JET_POV_MOVED,
		JET_VECTOR3_MOVED,
	};

class JoystickEvent:public Event
{
public:
	JOYSTIC_EVENT_TYPE event;
	int id;
	int button;
	int axisIndex;
	controllers::JoysticAxis axis;
	int povIndex;
	controllers::JoysticPov pov;
	int vectorIndex;
	math::vector3d vector;
public:
	JoystickEvent():Event(ET_Joystick)
	{
	}
};

}


#endif //___JoystickEvent___

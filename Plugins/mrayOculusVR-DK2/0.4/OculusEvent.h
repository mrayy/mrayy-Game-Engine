
/********************************************************************
	created:	2013/10/06
	created:	6:10:2013   17:50
	filename: 	C:\Development\mrayEngine\Plugins\mrayOculusVR\OculusEvent.h
	file path:	C:\Development\mrayEngine\Plugins\mrayOculusVR
	file base:	OculusEvent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __OculusEvent__
#define __OculusEvent__


#include "Event.h"

namespace mray
{
	
	extern const GUID ET_OculusEvent;

class OculusEvent:public Event
{
protected:
public:

	OculusEvent():Event(ET_OculusEvent){}
	virtual~OculusEvent(){}
};

}


#endif

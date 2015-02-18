
#include "stdafx.h"

#include "WiiEvent.h"


namespace mray
{

const GUID WiiEvent::EventID("WiiEvent");

WiiEvent::WiiEvent():Event(EventID),event(EWiiEvent_Unkown),mote(0)
{
}
WiiEvent::~WiiEvent()
{
}


}

#include "stdafx.h"

#include "Event.h"
#include "common.h"


namespace mray{

	const GUID ET_Unkown="Unkown";
	const GUID ET_Mouse="Mouse";
	const GUID ET_Keyboard="Keyboard";
	const GUID ET_Joystick="Joystick";
	const GUID ET_GUIEvent="GUIEvent";
	const GUID ET_NetworkEvent="Network";
	const GUID ET_ResizeEvent="Resize";
	const GUID ET_SystemEvent="System";
	const GUID ET_ChangeModeEvent="ChangeMode";


Event::Event(const GUID& t)
{
	m_ownerWindow=0;
	recived=false;
	m_type=t;
}
Event::~Event(){
}

const GUID& Event::getType(){
	return m_type;
}

void Event::reset(){
	recived=false;
}

const core::stringc& Event::getTypeStr(){
	/*if(m_type<ET_Events_Count)
		return _EVENT_TYPE_str[m_type];
		else return core::string::Empty;
		*/
	return m_type.GetString();
}
bool Event::isRecived(){
	return recived;
}
void Event::setRecived(){
	recived=true;
}
/*

ulong Event::getMaskingValue(){
	return getEventMaskingValue((EVENT_TYPE)m_type);
}
ulong Event::getEventMaskingValue(EVENT_TYPE t){
	return BIT((ulong)t);
}*/

}

#include "stdafx.h"

#include "ISound.h"
#include "XMLElement.h"
#include "StringConverter.h"

namespace mray
{
namespace sound
{

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_PROPERTY_TYPE_HEADER(Position,ISound,math::vector3d,mT("Position"),EPBT_Struct,mT("Sound's Position"),0);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Position,ISound,math::vector3d,setPosition,getPosition,core::StringConverter::toString,core::StringConverter::toVector3d,false)


	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_PROPERTY_TYPE_HEADER(Direction,ISound,math::vector3d,mT("Direction"),EPBT_Struct,mT("Sound's Direction"),math::vector3d::ZAxis);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Direction,ISound,math::vector3d,setDirection,getDirection,core::StringConverter::toString,core::StringConverter::toVector3d,false)


	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_PROPERTY_TYPE_HEADER(Volume,ISound,float,mT("Volume"),EPBT_Basic,mT("Sound's Volume"),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Volume,ISound,float,setVolume,getVolume,core::StringConverter::toString,core::StringConverter::toFloat,false)


	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_PROPERTY_TYPE_HEADER(Pitch,ISound,float,mT("Pitch"),EPBT_Basic,mT("Sound's Pitch"),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Pitch,ISound,float,setPitch,getPitch,core::StringConverter::toString,core::StringConverter::toFloat,false)


	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_PROPERTY_TYPE_HEADER(MaxDistance,ISound,float,mT("MaxDistance"),EPBT_Basic,mT("Sound's Max Distance"),100);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(MaxDistance,ISound,float,setMaxDistance,getMaxDistance,core::StringConverter::toString,core::StringConverter::toFloat,false)


	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_PROPERTY_TYPE_HEADER(MinDistance,ISound,float,mT("MinDistance"),EPBT_Basic,mT("Sound's MinDistance"),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(MinDistance,ISound,float,setMinDistance,getMinDistance,core::StringConverter::toString,core::StringConverter::toFloat,false)


	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_PROPERTY_TYPE_HEADER(Looping,ISound,bool,mT("Looping"),EPBT_Basic,mT("Sound's Looping"),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Looping,ISound,bool,setVolume,getVolume,core::StringConverter::toString,core::StringConverter::toBool,false)


	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_PROPERTY_TYPE_HEADER(AttachedToListener,ISound,bool,mT("AttachedToListener"),EPBT_Basic,mT("Is Attached To Listener"),false);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(AttachedToListener,ISound,bool,AttachedToListener,isAttachedToListener,core::StringConverter::toString,core::StringConverter::toBool,false)





ISound::ISound(const core::string&name):m_name(name)
{

	CPropertieDictionary*dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypePosition::instance);
		dic->addPropertie(&PropertyTypeDirection::instance);
		dic->addPropertie(&PropertyTypeVolume::instance);
		dic->addPropertie(&PropertyTypePitch::instance);
		dic->addPropertie(&PropertyTypeMaxDistance::instance);
		dic->addPropertie(&PropertyTypeMinDistance::instance);
		dic->addPropertie(&PropertyTypeLooping::instance);
	}
}
ISound::~ISound()
{
}

void ISound::setName(const core::string&name){
	m_name=name;
}
const  core::string& ISound::getName(){
	return m_name;
}
/*
xml::XMLElement* ISound::exportXMLSettings(xml::XMLElement* e)
{
	xml::XMLElement* elem=new xml::XMLElement(mT("Sound"));
	e->addSubElement(elem);

	elem->addAttribute(mT("Name"),core::StringConverter::toString(getName()));
	elem->addAttribute(mT("Position"),core::StringConverter::toString(getPosition()));
	elem->addAttribute(mT("Direction"),core::StringConverter::toString(getDirection()));
	elem->addAttribute(mT("Is3D"),core::StringConverter::toString(getMode()==ESNDT_3D));
	elem->addAttribute(mT("Looping"),core::StringConverter::toString(getLooping()));
	elem->addAttribute(mT("Volume"),core::StringConverter::toString(getVolume()));
	elem->addAttribute(mT("Pitch"),core::StringConverter::toString(getPitch()));
	elem->addAttribute(mT("MinDistance"),core::StringConverter::toString(getMinDistance()));
	elem->addAttribute(mT("MaxDistance"),core::StringConverter::toString(getMaxDistance()));
	elem->addAttribute(mT("AttachToListener"),core::StringConverter::toString(isAttachedToListener()));
	return elem;
}
void ISound::loadXMLSettings(xml::XMLElement*e)
{
	xml::XMLAttribute* attr;
	attr=e->getAttribute(mT("Name"));
	if(attr)
		setName(attr->value);
	attr=e->getAttribute(mT("Position"));
	if(attr)
		setPosition(core::StringConverter::toVector3d(attr->value));
	attr=e->getAttribute(mT("Direction"));
	if(attr)
		setDirection(core::StringConverter::toVector3d(attr->value));
	attr=e->getAttribute(mT("Is3D"));
	if(attr)
		setMode(core::StringConverter::toBool(attr->value) ? ESNDT_3D:ESNDT_2D);
	attr=e->getAttribute(mT("Looping"));
	if(attr)
		setLooping(core::StringConverter::toBool(attr->value));
	attr=e->getAttribute(mT("Volume"));
	if(attr)
		setVolume(core::StringConverter::toFloat(attr->value));
	attr=e->getAttribute(mT("Pitch"));
	if(attr)
		setPitch(core::StringConverter::toFloat(attr->value));
	attr=e->getAttribute(mT("MinDistance"));
	if(attr)
		setMinDistance(core::StringConverter::toFloat(attr->value));
	attr=e->getAttribute(mT("MaxDistance"));
	if(attr)
		setMaxDistance(core::StringConverter::toFloat(attr->value));
	attr=e->getAttribute(mT("AttachToListener"));
	if(attr)
		AttachedToListener(core::StringConverter::toBool(attr->value));
}
*/

}
}

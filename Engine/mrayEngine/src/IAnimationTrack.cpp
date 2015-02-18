#include "stdafx.h"

#include "IAnimationTrack.h"


#include "IKeyFrame.h"

namespace mray{
namespace animation{

IAnimationTrack::IAnimationTrack(const core::string& name,int affectedID,ulong catagoryBits,ulong priority)
{
	m_catagoryBits=catagoryBits;
	m_priority=priority;

	m_affectedNodeID=affectedID;
	setName(name);

	m_start=0;


	CPropertieDictionary*dic=0;
	if(CreateDictionary(&dic))
		dic->addPropertie(&PropertyTypeName::instance);

	m_end=0;
}


IAnimationTrack::~IAnimationTrack(){
}


float IAnimationTrack::getStart()
{
	return m_start;
}

float IAnimationTrack::getEnd()
{
	return m_end;
}

float IAnimationTrack::getLength()
{
	return m_end-m_start;
}

void IAnimationTrack::addKeyFrame(IKeyFrame*kf)
{
	float t=kf->getTime();
	if(getKeysframsCount()==0)
	{
		m_start=m_end=t;
	}else{
		if(t<m_start)
			m_start=t;
		if(t>m_end)
			m_end=t;
	}
}


void IAnimationTrack::copy(IAnimationTrack* o){
	o->m_affectedNodeID=m_affectedNodeID;
	o->m_catagoryBits=m_catagoryBits;
	o->m_priority=m_priority;
	o->m_start=m_start;
	o->m_end=m_end;
}

xml::XMLElement* IAnimationTrack::loadXMLSettings(xml::XMLElement* elem)
{
//	xml::XMLElement* elem=e->getSubElement(mT("AnimationTrack"));
//	if(!elem)return 0;
	xml::XMLAttribute*attr=elem->getAttribute(mT("Name"));
	if(attr)
		m_name=attr->value;
	attr=elem->getAttribute(mT("AffectedNodeID"));
	if(attr)
		m_affectedNodeID = core::StringConverter::toInt(attr->value);
	attr=elem->getAttribute(mT("CatagoryBits"));
	if(attr)
		m_catagoryBits=core::StringConverter::toInt(attr->value);
	attr=elem->getAttribute(mT("Priority"));
	if(attr)
		m_priority=core::StringConverter::toInt(attr->value);
	return elem;
}
xml::XMLElement*  IAnimationTrack::exportXMLSettings(xml::XMLElement* elem){
	xml::XMLElement* e=new xml::XMLElement(mT("AnimationTrack"));
	e->addAttribute(mT("Type"),getObjectType());
	e->addAttribute(mT("Name"),m_name);
	e->addAttribute(mT("AffectedNodeID"),core::StringConverter::toString(m_affectedNodeID));
	e->addAttribute(mT("CatagoryBits"),core::StringConverter::toString(m_catagoryBits));
	e->addAttribute(mT("Priority"),core::StringConverter::toString(m_priority));
	elem->addSubElement(e);
	return e;
}


//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Name,IAnimationTrack,string,mT("Name"),EPBT_Basic,mT("Track Name"),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(Name,IAnimationTrack,core::string,setName,getName,,,false)





}
}
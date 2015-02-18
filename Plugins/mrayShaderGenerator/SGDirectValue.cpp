
#include "StdAfx.h"
#include "SGDirectValue.h"
#include "SGIVisitor.h"



namespace mray
{
namespace shader
{

SGDirectValue::SGDirectValue(video::EUniformType type,core::string v):SGIValue(type),m_value(v)
{
}
SGDirectValue::~SGDirectValue(){}


void SGDirectValue::Visit(SGIVisitor*visitor)
{
	visitor->OnVisit(this);
}

bool SGDirectValue::Load(xml::XMLElement* elem)
{
	xml::XMLAttribute* attr=0;
	attr=elem->getAttribute(mT("Type"));
	if(attr)
	{
		m_type=(video::EUniformType)EnumManager::getInstance().getValue(mT("EUniformType"),attr->value);
	}
	attr=elem->getAttribute(mT("Value"));
	if(attr)
		m_value=attr->value;
	return true;
}
xml::XMLElement* SGDirectValue::Save(xml::XMLElement* elem)
{
	elem->addAttribute(mT("ValueType"),mT("DirectValue"));
	elem->addAttribute(mT("Type"),EnumManager::getInstance().getName(mT("EUniformType"),m_type));
	elem->addAttribute(mT("Value"),m_value);
	return elem;
}

}
}


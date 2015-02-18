
#include "stdafx.h"
#include "SGValue.h"
#include "SGIVisitor.h"
#include "SGFunction.h"

namespace mray
{
namespace shader
{

SGValue::SGValue(/*const core::string&name,ushort index,*/video::EUniformType type)
:SGIValue(type)//,m_index(index),m_name(name)
{
}
SGValue::~SGValue()
{
}

void SGValue::SetValues(const std::vector<SGIValuePtr>&vals)
{
	m_values=vals;
}
const std::vector<SGIValuePtr>& SGValue::GetValues()const
{
	return m_values;
}

void SGValue::Visit(SGIVisitor*visitor)
{
	visitor->OnVisit(this);
}

bool SGValue::Load(xml::XMLElement* elem)
{
	xml::XMLAttribute* attr=0;
	attr=elem->getAttribute(mT("Type"));
	if(attr)
	{
		m_type=(video::EUniformType)EnumManager::getInstance().getValue(mT("EUniformType"),attr->value);
	}
	xml::XMLElement* node=0;

	node=elem->getSubElement(mT("Value"));
	while(node)
	{
		attr=node->getAttribute(mT("ValueType"));
		if(attr)
		{
			SGIValue*val=SGIValue::CreateValue(attr->value);
			if(val)
			{
				val->Load(node);
				m_values.push_back(val);
			}
		}
		node=node->nextSiblingElement(mT("Value"));
	}
	return true;
}
xml::XMLElement* SGValue::Save(xml::XMLElement* elem)
{
	elem->addAttribute(mT("ValueType"),mT("Value"));
	elem->addAttribute(mT("Type"),EnumManager::getInstance().getName(mT("EUniformType"),m_type));
	for(int i=0;i<m_values.size();++i)
	{
		xml::XMLElement* e=new xml::XMLElement(mT("Value"));
		m_values[i]->Save(e);
		elem->addSubElement(e);
	}
	return elem;
}

}
}


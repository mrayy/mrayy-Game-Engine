
#include "stdafx.h"
#include "SGVariable.h"
#include "SGIVisitor.h"


namespace mray
{
namespace shader
{



SGVariable::SGVariable(const core::string& name,video::EUniformType type,EVariableUsage usage,
					   EUniformSemantic semantic,ushort index,const core::string&desc)
:SGIValue(type),m_name(name),m_usage(usage),m_index(index),m_desc(desc),m_semantic(semantic),m_isConst(false)
{
}
SGVariable::~SGVariable()
{
}

void SGVariable::Visit(SGIVisitor*visitor)
{
	visitor->OnVisit(this);
}


bool SGVariable::Load(xml::XMLElement* elem)
{
	xml::XMLAttribute* attr=0;
	attr=elem->getAttribute(mT("Type"));
	if(attr)
	{
		m_type=(video::EUniformType)EnumManager::getInstance().getValue(mT("EUniformType"),attr->value);
	}
	attr=elem->getAttribute(mT("Name"));
	if(attr)
	{
		m_name=attr->value;
	}
	attr=elem->getAttribute(mT("Const"));
	if(attr)
	{
		m_isConst=core::StringConverter::toBool(attr->value);
	}
	attr=elem->getAttribute(mT("Index"));
	if(attr)
	{
		m_index=core::StringConverter::toUInt(attr->value);
	}
	attr=elem->getAttribute(mT("VariableUsage"));
	if(attr)
	{
		for(int i=0;i<EVU_COUNT;++i)
		{
			if(attr->value.equals_ignore_case(g_varUsageString[i]))
			{
				m_usage=(EVariableUsage)i;
				break;
			}
		}
	}
	xml::XMLElement* node=elem->getSubElement(mT("Description"));
	if(node)
	{
		if(node->getElementsBegin()!=node->getElementsEnd())
		{
			xml::XMLTextNode* desc=dynamic_cast<xml::XMLTextNode*>(*node->getElementsBegin());
			if(desc)
				m_desc=desc->GetValue();
		}
	}
	return true;
}
xml::XMLElement* SGVariable::Save(xml::XMLElement* elem)
{
	elem->addAttribute(mT("ValueType"),mT("Variable"));
	elem->addAttribute(mT("Name"),m_name);
	elem->addAttribute(mT("Type"),EnumManager::getInstance().getName(mT("EUniformType"),m_type));
	elem->addAttribute(mT("Const"),core::StringConverter::toString(m_isConst));
	elem->addAttribute(mT("Index"),core::StringConverter::toString(m_index));
	elem->addAttribute(mT("VariableUsage"),g_varUsageString[m_usage]);
	if(GetDescription()!=mT(""))
	{
		xml::XMLElement* desc=new xml::XMLElement(mT("Description"));
		desc->addSubElement(new xml::XMLTextNode(GetDescription()));
		elem->addSubElement(desc);
	}
	return elem;
}


}
}


#include "stdafx.h"
#include "SGFunctionParameterProtoype.h"
#include "EnumManager.h"

namespace mray
{
namespace shader
{
SGFunctionParameterProtoype::SGFunctionParameterProtoype(EVariableUsage usage):m_usage(usage),m_type(video::EUT_Unkown)
{
}
SGFunctionParameterProtoype::~SGFunctionParameterProtoype()
{
}

void SGFunctionParameterProtoype::Load(xml::XMLElement* elem)
{
	xml::XMLAttribute *attr;
	xml::XMLElement* node;

	attr=elem->getAttribute(mT("Name"));
	if(attr)
		m_name=attr->value;
	else
	{
		gLogManager.log(mT("SGFunctionParameterProtoype::Load() - Variable name is not provided!"),ELL_WARNING);
	}
	attr=elem->getAttribute(mT("Type"));
	if(attr)
	{
		m_type=(video::EUniformType)EnumManager::getInstance().getValue(mT("EUniformType"),attr->value);
	}
	else
	{
		gLogManager.log(mT("SGFunctionParameterProtoype::Load() - Variable type is not provided!"),ELL_WARNING);
	}


	node=elem->getSubElement(mT("Description"));
	if(node)
	{
		if(node->getElementsBegin()!=node->getElementsEnd())
		{
			xml::XMLTextNode* desc=dynamic_cast<xml::XMLTextNode*>(*node->getElementsBegin());
			if(desc)
				m_description=desc->GetValue();
		}
	}

}

xml::XMLElement* SGFunctionParameterProtoype::Save(xml::XMLElement* parent)
{
	xml::XMLElement* elem=0;
	if(m_usage==EVU_INPUT)
		elem=new xml::XMLElement(mT("Input"));
	else
		elem=new xml::XMLElement(mT("Output"));
	elem->addAttribute(mT("Name"),m_name);

	if(m_description!=mT(""))
	{
		xml::XMLElement* desc=new xml::XMLElement(mT("Description"));
		desc->addSubElement(new xml::XMLTextNode(m_description));
		elem->addSubElement(desc);
	}
	elem->addAttribute(mT("Type"),EnumManager::getInstance().getName(mT("EUniformType"),m_type));

	if(parent)
		parent->addSubElement(elem);
	return elem;
}


}
}
#include "stdafx.h"

#include "AttributesScheme.h"
#include "IDecisionAttribute.h"
#include "DecisionAttributes.h"
#include "XMLElement.h"
#include "StringConverter.h"

namespace mray
{
namespace AI
{

AttributesScheme::AttributesScheme()
{
}
AttributesScheme::~AttributesScheme()
{
	for(int i=0;i<m_attributes.size();++i)
	{
		delete m_attributes[i];
	}
}

void AttributesScheme::AddAttribute(IDecisionAttribute*attr)
{
	AttrNameIDMap::iterator it=m_nameID.find(attr->GetName());
	if(it==m_nameID.end())
	{
		m_nameID[attr->GetName()]=m_attributes.size();
		m_attributes.push_back(attr);
	}else
	{
		delete m_attributes[it->second];
		m_attributes[it->second]=attr;
	}
}

int AttributesScheme::GetAttributeIndex(const core::string&name)
{
	AttrNameIDMap::iterator it=m_nameID.find(name);
	if(it==m_nameID.end())
		return -1;
	return it->second;
}

IDecisionAttribute* AttributesScheme::GetAttribute(const core::string&name)
{
	AttrNameIDMap::iterator it=m_nameID.find(name);
	if(it==m_nameID.end())
		return 0;
	return m_attributes[it->second];
}
IDecisionAttribute* AttributesScheme::GetAttribute(int i)
{
	if(i>=m_attributes.size())
		return 0;
	return m_attributes[i];
}

int AttributesScheme::GetAttributesCount()
{
	return m_attributes.size();
}


void AttributesScheme::ParseAttribute(xml::XMLElement* elem)
{
	xml::XMLAttribute*attr;
	IDecisionAttribute* decAttr;
	core::string type;
	core::string name;
	attr=elem->getAttribute(mT("type"));
	if(!attr)
		return;
	type=attr->value;
	attr=elem->getAttribute(mT("name"));
	if(attr)
		name=attr->value;

	if(type.equals_ignore_case(mT("bool")))
		decAttr=new BooleanDecisionAttrib(name);
	else if(type.equals_ignore_case(mT("int"))){
		int min,max,step;
		attr=elem->getAttribute(mT("min"));
		if(!attr)
			return;
		min=core::StringConverter::toInt(attr->value);
		attr=elem->getAttribute(mT("max"));
		if(!attr)
			return;
		max=core::StringConverter::toInt(attr->value);
		attr=elem->getAttribute(mT("step"));
		if(!attr)
			return;
		step=core::StringConverter::toInt(attr->value);
		decAttr=new GenericDecisionAttrib<int>(name,min,max,step);
	}
	else if(type.equals_ignore_case(mT("float"))){
		float min,max,step;
		attr=elem->getAttribute(mT("min"));
		if(!attr)
			return;
		min=core::StringConverter::toFloat(attr->value);
		attr=elem->getAttribute(mT("max"));
		if(!attr)
			return;
		max=core::StringConverter::toFloat(attr->value);
		attr=elem->getAttribute(mT("step"));
		if(!attr)
			return;
		step=core::StringConverter::toFloat(attr->value);
		decAttr=new GenericDecisionAttrib<float>(name,min,max,step);
	}else if(type.equals_ignore_case(mT("enum"))){
		EnumDecisionAttrib*a=new EnumDecisionAttrib(name);
		a->loadXMLSettings(elem);
		decAttr=a;
	}
	if(!decAttr)
		return;
	AddAttribute(decAttr);
}
void AttributesScheme::loadXMLSettings(xml::XMLElement* elem)
{
	xml::xmlSubElementsMapIT it= elem->getElementsBegin();
	xml::xmlSubElementsMapIT end= elem->getElementsEnd();
	for(;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("Attribute")))
		{
			ParseAttribute(e);
		}
	}
}


}
}

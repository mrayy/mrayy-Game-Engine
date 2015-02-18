#include "stdafx.h"


#include "DecisionAttributes.h"
#include "XMLElement.h"

namespace mray
{
namespace AI
{

EnumDecisionAttrib::EnumDecisionAttrib(const core::string&name):IDecisionAttribute(name)
{
}
EnumDecisionAttrib::~EnumDecisionAttrib()
{
}

ushort EnumDecisionAttrib::GetBucket(const void*value)
{
	core::string v=(const mchar*)(value);
	for (int i=0;i<m_enumVals.size();++i)
	{
		if(m_enumVals[i].equals_ignore_case(v))
			return i;
	}
	return 0;
}
int EnumDecisionAttrib::GetBucketsCount()
{
	return m_enumVals.size();
}
void* EnumDecisionAttrib::GetValue(ushort bucket)
{
	return (void*)m_enumVals[bucket].c_str();
}
core::string EnumDecisionAttrib::GetValueString(ushort bucket)
{
	return m_enumVals[bucket];
}
void EnumDecisionAttrib::loadXMLSettings(xml::XMLElement* elem)
{
	xml::xmlSubElementsMapIT it= elem->getElementsBegin();
	xml::xmlSubElementsMapIT end= elem->getElementsEnd();
	for (;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("value")))
		{
			xml::XMLAttribute*attr=e->getAttribute(mT("name"));
			if(attr)
				m_enumVals.push_back(attr->value);
		}
	}
}

}
}
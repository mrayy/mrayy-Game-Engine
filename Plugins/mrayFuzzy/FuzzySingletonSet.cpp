#include "stdafx.h"

#include "FuzzySingletonSet.h"
#include "XMLElement.h"
#include "StringConverter.h"


namespace mray{
namespace AI{

FuzzySingletonSet::FuzzySingletonSet(const core::string &name,float leftRange,float rightRange)
	:FuzzySet(name,(rightRange-leftRange)*0.5f),
	m_leftRange(leftRange),m_rightRange(rightRange)
{
}
FuzzySingletonSet::~FuzzySingletonSet()
{
}

float FuzzySingletonSet::CalculateDOM(float v)
{
	if(v>=m_leftRange && v<=m_rightRange)
		return 1.0f;
	return 0.0f;
}
float FuzzySingletonSet::GetMinBound()
{
	return m_leftRange;
}
float FuzzySingletonSet::GetMaxBound()
{
	return m_rightRange;
}
xml::XMLElement* FuzzySingletonSet::exportXML(xml::XMLElement*elem)
{

	xml::XMLElement*e=new xml::XMLElement(mT("Term"));
	elem->addSubElement(e);
	e->addAttribute(mT("Type"),mT("Singleton"));
	e->addAttribute(mT("min"),core::StringConverter::toString(m_leftRange));
	e->addAttribute(mT("max"),core::StringConverter::toString(m_rightRange));
	return e;
}


}
}

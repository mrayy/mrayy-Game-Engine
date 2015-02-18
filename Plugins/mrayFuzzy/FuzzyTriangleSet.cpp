#include "stdafx.h"

#include "FuzzyTriangleSet.h"
#include "XMLElement.h"
#include "StringConverter.h"


namespace mray{
namespace AI{

FuzzyTriangleSet::FuzzyTriangleSet(const core::string &name,float minBound,float peak,float maxBound)
	:FuzzySet(name,peak),m_peak(peak),m_minBound(minBound),m_maxBound(maxBound)
{
}
FuzzyTriangleSet::~FuzzyTriangleSet()
{
}

float FuzzyTriangleSet::CalculateDOM(float val)
{
	if(val>=m_minBound && val<m_peak)
	{
		if(m_minBound==m_peak)
			return 1.0f;
		return 1.0f-(m_peak-val)/(m_peak-m_minBound);
	}else if(val>=m_peak && val<=m_maxBound)
	{
		if(m_maxBound==m_peak)
			return 1.0f;
		return 1.0f-(val-m_peak)/(m_maxBound-m_peak);
	}else
		return 0.0f;
}

float FuzzyTriangleSet::GetMinBound()
{
	return m_minBound;
}

float FuzzyTriangleSet::GetMaxBound()
{
	return m_maxBound;
}

xml::XMLElement* FuzzyTriangleSet::exportXML(xml::XMLElement*elem)
{
	xml::XMLElement*e=new xml::XMLElement(mT("Term"));
	elem->addSubElement(e);
	e->addAttribute(mT("Type"),mT("Triangle"));
	e->addAttribute(mT("min"),core::StringConverter::toString(m_minBound));
	e->addAttribute(mT("peak"),core::StringConverter::toString(m_peak));
	e->addAttribute(mT("max"),core::StringConverter::toString(m_maxBound));
	return e;
}
}
}


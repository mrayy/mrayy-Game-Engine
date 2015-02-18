#include "stdafx.h"

#include "FuzzyTrapezoidalSet.h"

#include "XMLElement.h"
#include "StringConverter.h"

namespace mray{
namespace AI{

FuzzyTrapezoidalSet::FuzzyTrapezoidalSet(const core::string &name,float minBound,float leftPeak,float rightPeak,float maxBound)
	:FuzzySet(name,((leftPeak+rightPeak)+(minBound-maxBound))*0.25f),
		m_leftPeak(leftPeak),m_rightPeak(rightPeak),m_minBound(minBound),m_maxBound(maxBound)
{
}
FuzzyTrapezoidalSet::~FuzzyTrapezoidalSet()
{
}

float FuzzyTrapezoidalSet::CalculateDOM(float val)
{
	if(val>=m_leftPeak && val<=m_rightPeak)
	{
		return 1.0f;
	}
	if(val<m_maxBound && val>m_rightPeak)
	{
		if(m_maxBound==m_rightPeak)
			return 1.0f;
		return 1.0f-(m_rightPeak-val)/(m_maxBound-m_rightPeak);
	}else if(val<m_leftPeak && val>=m_minBound)
	{
		if(m_minBound==m_leftPeak)
			return 1.0f;
		return 1.0f-(m_leftPeak-val)/(m_leftPeak-m_minBound);
	}else
		return 0.0f;
}
float FuzzyTrapezoidalSet::GetMinBound()
{
	return m_minBound;
}

float FuzzyTrapezoidalSet::GetMaxBound()
{
	return m_maxBound;
}

xml::XMLElement* FuzzyTrapezoidalSet::exportXML(xml::XMLElement*elem)
{
	xml::XMLElement*e=new xml::XMLElement(mT("Term"));
	elem->addSubElement(e);
	e->addAttribute(mT("Type"),mT("Trapezoidal"));
	e->addAttribute(mT("min"),core::StringConverter::toString(m_minBound));
	e->addAttribute(mT("leftpeak"),core::StringConverter::toString(m_leftPeak));
	e->addAttribute(mT("rightpeak"),core::StringConverter::toString(m_rightPeak));
	e->addAttribute(mT("max"),core::StringConverter::toString(m_maxBound));
	return e;
}

}
}



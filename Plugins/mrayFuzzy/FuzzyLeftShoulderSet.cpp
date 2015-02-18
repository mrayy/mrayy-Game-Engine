#include "stdafx.h"

#include "FuzzyLeftShoulderSet.h"

#include "XMLElement.h"
#include "StringConverter.h"

namespace mray{
namespace AI{

FuzzyLeftShoulderSet::FuzzyLeftShoulderSet(const core::string &name,float minBound,float peak,float maxBound)
	:FuzzySet(name,(peak+minBound)*0.5f),
	m_peak(peak),m_minBound(minBound),m_maxBound(maxBound)
{
}
FuzzyLeftShoulderSet::~FuzzyLeftShoulderSet()
{
}

float FuzzyLeftShoulderSet::CalculateDOM(float val)
{
	//is val in the left side
	if(val>=m_minBound && val<m_peak)
	{
		return 1.0f;
	}else if(val>=m_peak && val<=m_maxBound)
	{
		if(m_maxBound==m_peak)
			return 1.0f;
		return 1.0f-(val-m_peak)/(m_maxBound-m_peak);
	}else
		return 0.0f;
}

float FuzzyLeftShoulderSet::GetMinBound()
{
	return m_minBound;
}

float FuzzyLeftShoulderSet::GetMaxBound()
{
	return m_maxBound;
}

xml::XMLElement* FuzzyLeftShoulderSet::exportXML(xml::XMLElement*elem)
{
	xml::XMLElement*e=new xml::XMLElement(mT("Term"));
	elem->addSubElement(e);
	e->addAttribute(mT("Type"),mT("LeftShoulder"));
	e->addAttribute(mT("min"),core::StringConverter::toString(m_minBound));
	e->addAttribute(mT("peak"),core::StringConverter::toString(m_peak));
	e->addAttribute(mT("max"),core::StringConverter::toString(m_maxBound));
	return e;
}

}
}

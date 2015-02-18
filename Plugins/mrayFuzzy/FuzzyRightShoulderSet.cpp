#include "stdafx.h"

#include "FuzzyRightShoulderSet.h"

#include "XMLElement.h"
#include "StringConverter.h"


namespace mray{
namespace AI{

FuzzyRightShoulderSet::FuzzyRightShoulderSet(const core::string &name,float minBound,float peak,float maxBound)
	:FuzzySet(name,(peak+maxBound)*0.5f),
	m_peak(peak),m_minBound(minBound),m_maxBound(maxBound)
{
}
FuzzyRightShoulderSet::~FuzzyRightShoulderSet()
{
}

float FuzzyRightShoulderSet::CalculateDOM(float val)
{
	//is val in the right side
	if(val<=m_maxBound && val>m_peak)
	{
		return 1.0f;
	}else if(val<=m_peak && val>=m_minBound)
	{
		if(m_minBound==m_peak)
			return 1.0f;
		return 1.0f-(m_peak-val)/(m_peak-m_minBound);
	}else
		return 0.0f;
}
float FuzzyRightShoulderSet::GetMinBound()
{
	return m_minBound;
}

float FuzzyRightShoulderSet::GetMaxBound()
{
	return m_maxBound;
}

xml::XMLElement* FuzzyRightShoulderSet::exportXML(xml::XMLElement*elem)
{
	xml::XMLElement*e=new xml::XMLElement(mT("Term"));
	elem->addSubElement(e);
	e->addAttribute(mT("Type"),mT("RightShoulder"));
	e->addAttribute(mT("min"),core::StringConverter::toString(m_minBound));
	e->addAttribute(mT("peak"),core::StringConverter::toString(m_peak));
	e->addAttribute(mT("max"),core::StringConverter::toString(m_maxBound));
	return e;
}

}
}


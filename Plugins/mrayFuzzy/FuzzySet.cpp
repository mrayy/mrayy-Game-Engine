#include "stdafx.h"

#include "FuzzySet.h"


namespace mray{
namespace AI{

FuzzySet::FuzzySet(const core::string &name)
	:m_DOM(0),m_RepVal(0),m_name(name)
{
}
FuzzySet::FuzzySet(const core::string &name,float repVal)
	:m_DOM(0),m_RepVal(repVal),m_name(name)
{
}
FuzzySet::~FuzzySet()
{
}


void FuzzySet::SetDOM(float v)
{
	if(v>1)
		m_DOM=1;
	else if(v<0)
		m_DOM=0;
	else 
		m_DOM=v;
}
float FuzzySet::GetDOM()
{
	return m_DOM;
}

void FuzzySet::ClearDOM()
{
	m_DOM=0;
}
xml::XMLElement* FuzzySet::exportXML(xml::XMLElement*elem)
{
	return elem;
}


}
}

#include "stdafx.h"

#include "FuzzyNotOpt.h"
#include "XMLElement.h"
#include "StringConverter.h"


namespace mray{
namespace AI{

FuzzyNotOpt::FuzzyNotOpt(FuzzyTerm*t)
	:m_term(t)
{
}
FuzzyNotOpt::~FuzzyNotOpt()
{
}

float FuzzyNotOpt::GetDOM()
{
	return 1-m_term->GetDOM();
}

void FuzzyNotOpt::ClearDOM()
{
	m_term->ClearDOM();
}

void FuzzyNotOpt::ORWithDOM(float dom)
{
	m_term->ORWithDOM(dom);
}
xml::XMLElement* FuzzyNotOpt::exportXML(xml::XMLElement*elem)
{
	xml::XMLElement*e=new xml::XMLElement(mT("Term"));
	elem->addSubElement(e);
	e->addAttribute(mT("Type"),mT("not"));
	m_term->exportXML(e);
	return e;
}

}
}


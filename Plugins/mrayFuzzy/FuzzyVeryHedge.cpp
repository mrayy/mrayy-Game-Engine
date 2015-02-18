#include "stdafx.h"

#include "FuzzyVeryHedge.h"
#include "XMLElement.h"
#include "StringConverter.h"


namespace mray{
namespace AI{

FuzzyVeryHedge::FuzzyVeryHedge(FuzzyTerm*term)
	:m_term(term)
{
}

FuzzyVeryHedge::~FuzzyVeryHedge()
{
}


float FuzzyVeryHedge::GetDOM()
{
	float d=m_term->GetDOM();
	return d*d;
}

void FuzzyVeryHedge::ClearDOM()
{
	m_term->ClearDOM();
}
void FuzzyVeryHedge::ORWithDOM(float dom)
{
	m_term->ORWithDOM(dom*dom);
}

xml::XMLElement* FuzzyVeryHedge::exportXML(xml::XMLElement*elem)
{
	xml::XMLElement*e=new xml::XMLElement(mT("Term"));
	elem->addSubElement(e);
	e->addAttribute(mT("Type"),mT("very"));
	m_term->exportXML(e);
	return e;
}

}
}


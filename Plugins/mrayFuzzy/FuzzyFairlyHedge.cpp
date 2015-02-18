#include "stdafx.h"


#include "FuzzyFairlyHedge.h"
#include "XMLElement.h"
#include "StringConverter.h"
#include <math.h>

namespace mray{
namespace AI{

FuzzyFairlyHedge::FuzzyFairlyHedge(FuzzyTerm*term)
	:m_term(term)
{
}

FuzzyFairlyHedge::~FuzzyFairlyHedge()
{
	delete m_term;
}


float FuzzyFairlyHedge::GetDOM()
{
	float d=m_term->GetDOM();
	return sqrt(d);
}

void FuzzyFairlyHedge::ClearDOM()
{
	m_term->ClearDOM();
}

void FuzzyFairlyHedge::ORWithDOM(float dom)
{
	m_term->ORWithDOM(sqrt(dom));
}
xml::XMLElement* FuzzyFairlyHedge::exportXML(xml::XMLElement*elem)
{
	xml::XMLElement*e=new xml::XMLElement(mT("Term"));
	elem->addSubElement(e);
	e->addAttribute(mT("Type"),mT("fairly"));
	m_term->exportXML(e);
	return e;
}
}
}



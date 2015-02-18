#include "stdafx.h"

#include "FuzzyRule.h"
#include "FuzzyTerm.h"

#include "XMLElement.h"

namespace mray{
namespace AI{

FuzzyRule::FuzzyRule(FuzzyTerm* antecedent,FuzzyTerm* consequence)
:m_antecedent(antecedent),m_consequence(consequence)
{
}
FuzzyRule::~FuzzyRule()
{
	delete m_antecedent;
	delete m_consequence;
}

//set antecedent's confidence to zero
void FuzzyRule::ClearAntecedentConfidence()
{
	m_consequence->ClearDOM();
}

//update consequence DOM with antecedent's DOM
void FuzzyRule::Calculate()
{
	m_consequence->ORWithDOM(m_antecedent->GetDOM());
}


xml::XMLElement* FuzzyRule::exportXML(xml::XMLElement*elem)
{
	xml::XMLElement*e=new xml::XMLElement(mT("Rule"));
	xml::XMLElement*e2=new xml::XMLElement(mT("Antecedent"));
	e->addSubElement(e2);
	m_antecedent->exportXML(e2);

	e2=new xml::XMLElement(mT("Consequence"));
	e->addSubElement(e2);
	m_consequence->exportXML(e2);

	elem->addSubElement(e);

	return e;
}


}
}

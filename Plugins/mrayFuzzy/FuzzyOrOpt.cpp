#include "stdafx.h"

#include "FuzzyOrOpt.h"
#include "XMLElement.h"
#include "StringConverter.h"

namespace mray{
namespace AI{

FuzzyOrOpt::FuzzyOrOpt()
{

}
FuzzyOrOpt::FuzzyOrOpt(FuzzyTerm*t1,FuzzyTerm*t2)
{
	m_terms.push_back(t1);
	m_terms.push_back(t2);
}
FuzzyOrOpt::~FuzzyOrOpt()
{
	for (int i=0;i<m_terms.size();++i)
	{
		delete m_terms[i];
	}
	m_terms.clear();
}
void FuzzyOrOpt::AddTerm(FuzzyTerm*t)
{
	m_terms.push_back(t);
}
int FuzzyOrOpt::getTermsCount()
{
	return m_terms.size();
}

float FuzzyOrOpt::GetDOM()
{
	if(m_terms.size()==0)
		return 0;
	float d=m_terms[0]->GetDOM();
	for (int i=1;i<m_terms.size();++i)
	{
		float d1=m_terms[i]->GetDOM();
		if(d1>d)
			d=d1;
	}
	return d;
}

void FuzzyOrOpt::ClearDOM()
{
	for (int i=0;i<m_terms.size();++i)
	{
		m_terms[i]->ClearDOM();
	}
}

void FuzzyOrOpt::ORWithDOM(float dom)
{
	for (int i=0;i<m_terms.size();++i)
	{
		m_terms[i]->ORWithDOM(dom);
	}
}
xml::XMLElement* FuzzyOrOpt::exportXML(xml::XMLElement*elem)
{
	xml::XMLElement*e=new xml::XMLElement(mT("Term"));
	elem->addSubElement(e);
	e->addAttribute(mT("Type"),mT("or"));
	for (int i=0;i<m_terms.size();++i)
	{
		m_terms[i]->exportXML(e);
	}
	return e;
}


}
}

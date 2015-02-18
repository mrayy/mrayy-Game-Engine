#include "stdafx.h"

#include "FuzzyAndOpt.h"
#include "XMLElement.h"
#include "StringConverter.h"

namespace mray{
namespace AI{

FuzzyAndOpt::FuzzyAndOpt()
{
}
FuzzyAndOpt::FuzzyAndOpt(FuzzyTerm*t1,FuzzyTerm*t2)
{
	m_terms.push_back(t1);
	m_terms.push_back(t2);
}
FuzzyAndOpt::~FuzzyAndOpt()
{
	for (int i=0;i<m_terms.size();++i)
	{
		delete m_terms[i];
	}
	m_terms.clear();
}

void FuzzyAndOpt::AddTerm(FuzzyTerm*t)
{
	m_terms.push_back(t);
}
int FuzzyAndOpt::getTermsCount()
{
	return m_terms.size();
}
float FuzzyAndOpt::GetDOM()
{
	if(m_terms.size()==0)
		return 0;
	float d=m_terms[0]->GetDOM();
	for (int i=1;i<m_terms.size();++i)
	{
		float d1=m_terms[i]->GetDOM();
		if(d1<d)
			d=d1;
	}
	return d;
}
void FuzzyAndOpt::ClearDOM()
{
	for (int i=0;i<m_terms.size();++i)
	{
		m_terms[i]->ClearDOM();
	}
}
void FuzzyAndOpt::ORWithDOM(float dom)
{
	for (int i=0;i<m_terms.size();++i)
	{
		m_terms[i]->ORWithDOM(dom);
	}
}


xml::XMLElement* FuzzyAndOpt::exportXML(xml::XMLElement*elem)
{
	xml::XMLElement*e=new xml::XMLElement(mT("Term"));
	elem->addSubElement(e);
	e->addAttribute(mT("Type"),mT("and"));
	for (int i=0;i<m_terms.size();++i)
	{
		m_terms[i]->exportXML(e);
	}
	return e;
}

}
}

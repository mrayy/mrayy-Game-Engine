#include "stdafx.h"

#include "FuzzySetProxy.h"
#include "XMLElement.h"



namespace mray{
namespace AI{

FuzzySetProxy::FuzzySetProxy(const core::string &varName,FuzzySet*set)
:FuzzySet(set->GetName()),m_set(set),m_varName(varName)
{
}
FuzzySetProxy::~FuzzySetProxy()
{
}

float FuzzySetProxy::CalculateDOM(float v)
{
	return m_set->CalculateDOM(v);
}

float FuzzySetProxy::GetRepresentativeValue()
{
	return m_set->GetRepresentativeValue();
}

void FuzzySetProxy::SetDOM(float v)
{
	m_set->SetDOM(v);
}
float FuzzySetProxy::GetDOM()
{
	return m_set->GetDOM();
}
void FuzzySetProxy::ClearDOM()
{
	m_set->ClearDOM();
}

float FuzzySetProxy::GetMinBound()
{
	return m_set->GetMinBound();
}
float FuzzySetProxy::GetMaxBound()
{
	return m_set->GetMaxBound();
}

void FuzzySetProxy::ORWithDOM(float dom)
{
	m_set->ORWithDOM(dom);
}

xml::XMLElement* FuzzySetProxy::exportXML(xml::XMLElement*elem)
{
	xml::XMLElement*e=new xml::XMLElement(mT("Term"));
	elem->addSubElement(e);
	e->addAttribute(mT("Type"),mT("set"));
	e->addAttribute(mT("Variable"),m_varName);
	e->addAttribute(mT("set"),m_set->GetName());
	return e;
}

}
}
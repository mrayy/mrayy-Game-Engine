#include "stdafx.h"

#include "AIFuzzyComponent.h"
#include "DefuzzyCentroid.h"
#include "FuzzyXMLParser.h"
#include "AIComponentTypes.h"


namespace mray
{
namespace AI
{

AIFuzzyComponent::AIFuzzyComponent()
{
	m_module=new FuzzyModule(new DefuzzyCentroid(16));
}
AIFuzzyComponent::~AIFuzzyComponent()
{
	delete m_module;
}


void AIFuzzyComponent::AddRule(FuzzyTerm*antecedent,FuzzyTerm*consequence)
{
	m_module->AddRule(antecedent,consequence);
}
void AIFuzzyComponent::AddRule(FuzzyRule*rule)
{
	m_module->AddRule(rule);
}

FuzzyVariable* AIFuzzyComponent::CreateVariable(const core::string&name)
{
	return m_module->CreateVariable(name);
}
void AIFuzzyComponent::AddVariable(FuzzyVariable*v)
{
	m_module->AddVariable(v);
}
FuzzyVariable* AIFuzzyComponent::GetVariable(const core::string&name)
{
	return m_module->GetVariable(name);
}

void AIFuzzyComponent::Fuzzify(const core::string&varName,float val)
{
	m_module->Fuzzify(varName,val);
}

void AIFuzzyComponent::ApplyRules()
{
	m_module->ApplyRules();
}
float AIFuzzyComponent::Defuzzify(const core::string&varName)
{
	return m_module->Defuzzify(varName);
}


xml::XMLElement* AIFuzzyComponent::loadXMLSettings(xml::XMLElement* elem)
{
	FuzzyXMLParser parser;
	parser.ParseModuleElement(elem,m_module);
	return elem;
}


}
}


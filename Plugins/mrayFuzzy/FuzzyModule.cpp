
#include "stdafx.h"
#include "FuzzyModule.h"



namespace mray{
namespace AI{

FuzzyModule::FuzzyModule(IDefuzzifyOperation*deffuzifier)
:m_defuzzifier(deffuzifier)
{
}
FuzzyModule::~FuzzyModule()
{
	delete m_defuzzifier;
	{
		FuzzyRuleList::iterator it=m_rules.begin();
		for(;it!=m_rules.end();++it){
			delete *it;
		}
	}
	{
		FuzzyVariableMap::iterator it=m_variables.begin();
		for(;it!=m_variables.end();++it){
			delete it->second;
		}
	}
}


void FuzzyModule::ClearRulesConfidence()
{
	FuzzyRuleList::iterator it=m_rules.begin();
	for(;it!=m_rules.end();++it){
		(*it)->ClearAntecedentConfidence();
	}
}

void FuzzyModule::SetDefuzzier(IDefuzzifyOperation*deff)
{
	delete m_defuzzifier;
	m_defuzzifier=deff;
}

void FuzzyModule::AddRule(FuzzyTerm*antecedent,FuzzyTerm*consequence)
{
	m_rules.push_back(new FuzzyRule(antecedent,consequence));
}

void FuzzyModule::AddRule(FuzzyRule*rule)
{
	m_rules.push_back(rule);
}

FuzzyVariable* FuzzyModule::CreateVariable(const core::string&name)
{
	FuzzyVariable*v=new FuzzyVariable(name);
	if(m_variables.find(name)!=m_variables.end())
	{
		delete m_variables[name];
	}
	m_variables[name]=v;
	return v;
}

void FuzzyModule::AddVariable(FuzzyVariable*v)
{
	if(m_variables.find(v->GetName())!=m_variables.end())
	{
		delete m_variables[v->GetName()];
	}
	m_variables[v->GetName()]=v;
}

FuzzyVariable* FuzzyModule::GetVariable(const core::string&name)
{
	FuzzyVariableMap::iterator vit=m_variables.find(name);
	if(vit==m_variables.end()){
		//Var Not Found!!
		return 0;
	}
	return vit->second;
}

const FuzzyVariableMap& FuzzyModule::GetVariables()
{
	return m_variables;
}


void FuzzyModule::Fuzzify(const core::string&varName,float val)
{
	FuzzyVariableMap::iterator it=m_variables.find(varName);
	if(it==m_variables.end()){
		//Var Not Found!!
		return;
	}
	it->second->Fuzzify(val);
}

void FuzzyModule::ApplyRules()
{
	ClearRulesConfidence();

	FuzzyRuleList::iterator it=m_rules.begin();
	for(;it!=m_rules.end();++it){
		(*it)->Calculate();
	}
}

float FuzzyModule::Defuzzify(const core::string&varName)
{
	FuzzyVariableMap::iterator vit=m_variables.find(varName);
	if(vit==m_variables.end()){
		//Var Not Found!!
		return 0;
	}
	return m_defuzzifier->Defuzzify(vit->second);
}

}
}


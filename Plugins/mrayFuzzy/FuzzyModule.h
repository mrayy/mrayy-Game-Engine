
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   13:21
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzyModule.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzyModule
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FuzzyModule___
#define ___FuzzyModule___

#include "FuzzyVariable.h"
#include "FuzzyRule.h"
#include "IDefuzzifyOperation.h"
#include <map>
#include <vector>

namespace mray{
namespace AI{

typedef std::map<core::string,FuzzyVariable*> FuzzyVariableMap;
typedef std::vector<FuzzyRule*> FuzzyRuleList;

class FuzzyModule
{
private:
protected:
	FuzzyVariableMap m_variables;
	FuzzyRuleList m_rules;
	IDefuzzifyOperation* m_defuzzifier;

	void ClearRulesConfidence();
public:
	FuzzyModule(IDefuzzifyOperation*deffuzifier);
	virtual~FuzzyModule();

	void SetDefuzzier(IDefuzzifyOperation*deff);

	void AddRule(FuzzyTerm*antecedent,FuzzyTerm*consequence);
	void AddRule(FuzzyRule*rule);

	FuzzyVariable* CreateVariable(const core::string&name);
	void AddVariable(FuzzyVariable*v);
	FuzzyVariable* GetVariable(const core::string&name);

	const FuzzyVariableMap& GetVariables();

	void Fuzzify(const core::string&varName,float val);
	void ApplyRules();
	float Defuzzify(const core::string&varName);

};

}
}


#endif //___FuzzyModule___

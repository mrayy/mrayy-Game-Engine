

/********************************************************************
	created:	2010/05/05
	created:	5:5:2010   8:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AIFuzzyComponent.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AIFuzzyComponent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AIFuzzyComponent_h__
#define AIFuzzyComponent_h__


#include "FuzzyModule.h"
#include "IAIComponent.h"

namespace mray
{
	namespace xml
	{
		class XMLElement;
	}
namespace AI
{

class AIFuzzyComponent:public IAIComponent
{
	DECLARE_RTTI
private:
protected:

	FuzzyModule* m_module;

public:
	AIFuzzyComponent();
	virtual ~AIFuzzyComponent();


	void AddRule(FuzzyTerm*antecedent,FuzzyTerm*consequence);
	void AddRule(FuzzyRule*rule);

	FuzzyVariable* CreateVariable(const core::string&name);
	void AddVariable(FuzzyVariable*v);
	FuzzyVariable* GetVariable(const core::string&name);

	void Fuzzify(const core::string&varName,float val);
	void ApplyRules();
	float Defuzzify(const core::string&varName);

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
};

}
}

#endif // AIFuzzyComponent_h__

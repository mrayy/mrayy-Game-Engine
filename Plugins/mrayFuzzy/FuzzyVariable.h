
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   13:21
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\FuzzyVariable.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	FuzzyVariable
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FuzzyVariable___
#define ___FuzzyVariable___

#include "FuzzySet.h"
#include <map>
#include <mstring.h>

namespace mray{
namespace AI{

	class IDefuzzifyOperation;

typedef std::map<core::string,FuzzySet*> FuzzyMemberSetMap;

class FuzzyVariable
{
private:
protected:

	FuzzyMemberSetMap m_members;
	float m_minBound;
	float m_maxBound;

	core::string m_name;

public:
	FuzzyVariable(const core::string&name);
	virtual~FuzzyVariable();

	const FuzzyMemberSetMap& GetMemberSets()
	{
		return m_members;
	}

	const core::string& GetName(){return m_name;}

	void AddSet(const core::string&name,FuzzySet*s);
	FuzzySet* GetSet(const core::string&name);
	FuzzySet* GetSet(int id);
	int GetSetsCount();

	void Fuzzify(float v);
	//float Defuzzify(IDefuzzifyOperation* defuzzifier);

	float GetMinBound();
	float GetMaxBound();
};

}
}


#endif //___FuzzyVariable___

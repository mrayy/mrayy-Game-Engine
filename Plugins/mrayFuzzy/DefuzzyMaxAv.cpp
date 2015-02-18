#include "stdafx.h"

#include "DefuzzyMaxAv.h"
#include "FuzzyVariable.h"


namespace mray{
namespace AI{

DefuzzyMaxAv::DefuzzyMaxAv()
{
}

DefuzzyMaxAv::~DefuzzyMaxAv()
{
}

float DefuzzyMaxAv::Defuzzify(FuzzyVariable*var)
{
	const FuzzyMemberSetMap& sets=var->GetMemberSets();
	FuzzyMemberSetMap::const_iterator it=sets.begin();

	float crispVal=0;
	float confSum=0;
	for(;it!=sets.end();++it)
	{
		FuzzySet*s=it->second;
		float rep=s->GetRepresentativeValue();
		float conf=s->GetDOM();
		crispVal+=rep*conf;
		confSum+=conf;
	}

	if(confSum==0)
		return 0;

	return crispVal/confSum;
}


}
}

#include "stdafx.h"

#include "DefuzzyCentroid.h"
#include "FuzzyVariable.h"


namespace mray{
namespace AI{

DefuzzyCentroid::DefuzzyCentroid(int numSamples):m_numSamples(numSamples)
{
}

DefuzzyCentroid::~DefuzzyCentroid()
{
}

float DefuzzyCentroid::Defuzzify(FuzzyVariable*var)
{
	const FuzzyMemberSetMap& sets=var->GetMemberSets();
	FuzzyMemberSetMap::const_iterator it=sets.begin();


	float top=0;
	float DomSum=0;

	/*float varMinRange=var->GetMinBound();
	float varStep=(var->GetMaxBound()-varMinRange)/(float)m_numSamples;*/

	for(;it!=sets.end();++it)
	{
		FuzzySet*s=it->second;

		float minRange=s->GetMinBound();
		int samp;
		float stepSize=(s->GetMaxBound()-minRange)/(float)m_numSamples;

		float dom=s->GetDOM();
		if(dom==0)
			continue;
		for(samp=0;samp<=m_numSamples;++samp){

			float sample=minRange+samp*stepSize;
			//float sample=varMinRange+samp*varStep;

			float sampDom=s->CalculateDOM(sample);

			float d=dom<sampDom ? dom:sampDom;

			DomSum+=d;

			top+=(sample)*d;
		}
	}
	if(DomSum==0)
		return 0;
	return top/DomSum;

}


}
}


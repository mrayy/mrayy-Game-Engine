#include "stdafx.h"

#include "FuzzyVariable.h"
#include "IDefuzzifyOperation.h"

#include "FuzzyTriangleSet.h"
#include "FuzzyTrapezoidalSet.h"
#include "FuzzySingletonSet.h"
#include "FuzzyRightShoulderSet.h"
#include "FuzzyLeftShoulderSet.h"

namespace mray{
namespace AI{

FuzzyVariable::FuzzyVariable(const core::string&name):m_name(name)
{
	m_minBound=0;
	m_maxBound=0;
}
FuzzyVariable::~FuzzyVariable()
{
	FuzzyMemberSetMap::const_iterator it=m_members.begin();

	for(;it!=m_members.end();++it)
	{
		delete it->second;
	}
	m_members.clear();
}

/*
void FuzzyVariable::AddLeftShoulderSet(const std::string&name,float minBound,float peak,float maxBound)
{
	FuzzySet*s=new FuzzyLeftShoulderSet(peak,peak-minBound,maxBound-peak);
	AddSet(name,s,minBound,maxBound);
}
void FuzzyVariable::AddRightShoulderSet(const std::string&name,float minBound,float peak,float maxBound)
{
	FuzzySet*s=new FuzzyLeftShoulderSet(peak,peak-minBound,maxBound-peak);
	AddSet(name,s,minBound,maxBound);
}
void FuzzyVariable::AddTriangleSet(const std::string&name,float minBound,float peak,float maxBound)
{
	FuzzySet*s=new FuzzyRightShoulderSet(peak,peak-minBound,maxBound-peak);
	AddSet(name,s,minBound,maxBound);
}
void FuzzyVariable::AddTrapezoidalSet(const std::string&name,float minBound,float leftPeak,float rightPeak,float maxBound)
{
	FuzzySet*s=new FuzzyTrapezoidalSet(leftPeak,rightPeak,leftPeak-minBound,maxBound-rightPeak);
	AddSet(name,s,minBound,maxBound);
}
void FuzzyVariable::AddSingletonSet(const std::string&name,float minBound,float maxBound)
{
	FuzzySet*s=new FuzzySingletonSet(minBound,maxBound);
	AddSet(name,s,minBound,maxBound);
}
*/
void FuzzyVariable::AddSet(const core::string&name,FuzzySet*s)
{
	float minBound=s->GetMinBound();
	float maxBound=s->GetMaxBound();
	if(m_members.size()==0){
		m_minBound=minBound;
		m_maxBound=maxBound;
	}else{
		m_minBound=minBound<m_minBound ? minBound:m_minBound;
		m_maxBound=maxBound>m_maxBound ? maxBound:m_maxBound;
	}
	FuzzyMemberSetMap::iterator it=m_members.find(name);
	if(it!=m_members.end()){
		delete it->second;
		it->second=s;
	}else{
		m_members[name]=s;
	}
}


FuzzySet* FuzzyVariable::GetSet(const core::string&name)
{
	FuzzyMemberSetMap::iterator it=m_members.find(name);
	if(it==m_members.end())
		return 0;
	return it->second;
}
FuzzySet* FuzzyVariable::GetSet(int id)
{
	if(id>=m_members.size())
		return 0;

	FuzzyMemberSetMap::iterator it=m_members.begin();
	std::advance(it,id);
	if(it==m_members.end())
		return 0;
	return it->second;
}
int FuzzyVariable::GetSetsCount()
{
	return m_members.size();
}

void FuzzyVariable::Fuzzify(float v)
{
	FuzzyMemberSetMap::iterator it=m_members.begin();
	for(;it!=m_members.end();++it)
	{
		FuzzySet*s=it->second;
		s->SetDOM(s->CalculateDOM(v));
	}

}

float FuzzyVariable::GetMinBound()
{
	return m_minBound;
}
float FuzzyVariable::GetMaxBound()
{
	return m_maxBound;
}


/*
float FuzzyVariable::Defuzzify(IDefuzzifyOperation* defuzzifier)
{
	return defuzzifier->Defuzzify(this);
}*/

}
}

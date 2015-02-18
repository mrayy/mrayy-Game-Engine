#include "stdafx.h"

#include "FunctionProfileManager.h"
#include "FunctionProfiler.h"


namespace mray
{

FunctionProfileManager::ProfileSample::ProfileSample():samplesCount(0),totalTime(0),parent(0),id(0)
{
}
FunctionProfileManager::ProfileSample::~ProfileSample()
{
	Clear();
}
void FunctionProfileManager::ProfileSample::Reset()
{
	samplesCount=0;
	totalTime=0;
	parent=0;
}
void FunctionProfileManager::ProfileSample::Clear()
{
	SampleList::iterator it=childs.begin();
	for (;it!=childs.end();++it)
	{
		delete it->second;
	}
	childs.clear();
}

FunctionProfileManager::FunctionProfileManager():m_currentSample(0)
{
}

void FunctionProfileManager::BeginProfiling(FunctionProfiler*p)
{
	ProfileSample* s=0;
	if(!m_currentSample)
	{
		SampleList::iterator it=m_samples.find(p->GetID());
		if(it==m_samples.end()){
			s=new ProfileSample();
			s->name=p->GetName();
			m_samples[p->GetID()]=s;
		}
		else s=it->second;
	}else
	{
		SampleList::iterator it=m_currentSample->childs.find(p->GetID());
		if(it==m_currentSample->childs.end()){
			s=new ProfileSample();
			s->name=p->GetName();
			m_currentSample->childs[p->GetID()]=s;
		}
		else s=it->second;

		s->parent=m_currentSample;
	}
	m_currentSample=s;
	FIRE_LISTENR_METHOD(OnProfileBegin,(p));
}
void FunctionProfileManager::EndProfiling(FunctionProfiler*p)
{
	++m_currentSample->samplesCount;
	m_currentSample->totalTime+=p->GetTotalExecutionTime();
	m_currentSample=m_currentSample->parent;

	FIRE_LISTENR_METHOD(OnProfileEnd,(p));
}

}


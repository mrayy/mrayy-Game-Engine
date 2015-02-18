
#include "stdafx.h"
#include "PhComplexContactReporter.h"


namespace mray
{
namespace physics
{

#define FireEvent(eventName,r)\
	ReportersList::iterator it=m_reporters.begin();\
	for(;it!=m_reporters.end();++it)\
		(*it)->eventName(r);

PhComplexContactReporter::PhComplexContactReporter()
{

}
PhComplexContactReporter::~PhComplexContactReporter()
{
	ClearReporters();
}

void PhComplexContactReporter::AddReporter(IPhysicUserContactReport*r)
{
	m_reporters.push_back(r);
}
void PhComplexContactReporter::RemoveReporter(IPhysicUserContactReport*r)
{
	ReportersList::iterator it=m_reporters.begin();
	for(;it!=m_reporters.end();++it)
	{
		if(*it==r)
		{
			m_reporters.erase(it);
			break;
		}
	}
}
void PhComplexContactReporter::ClearReporters()
{
	m_reporters.clear();
}

void PhComplexContactReporter::OnPairStartTouch(PhysicContactPair* pair)
{
	FireEvent(OnPairStartTouch,pair);
}
void PhComplexContactReporter::OnPairTouch(PhysicContactPair* pair)
{
	FireEvent(OnPairTouch,pair);
}
void PhComplexContactReporter::OnPairEndTouch(PhysicContactPair* pair)
{
	FireEvent(OnPairEndTouch,pair);
}
void PhComplexContactReporter::OnPairImpact(PhysicContactPair* pair)
{
	FireEvent(OnPairImpact,pair);
}
void PhComplexContactReporter::OnPairRoll(PhysicContactPair* pair)
{
	FireEvent(OnPairRoll,pair);
}
void PhComplexContactReporter::OnPairSlide(PhysicContactPair* pair)
{
	FireEvent(OnPairSlide,pair);
}

void PhComplexContactReporter::OnPairStartTouchForceThreshold(PhysicContactPair* pair)
{
	FireEvent(OnPairStartTouchForceThreshold,pair);
}
void PhComplexContactReporter::OnPairTouchForceThreshold(PhysicContactPair* pair)
{
	FireEvent(OnPairTouchForceThreshold,pair);
}
void PhComplexContactReporter::OnPairEndTouchForceThreshold(PhysicContactPair* pair)
{
	FireEvent(OnPairEndTouchForceThreshold,pair);
}



}
}



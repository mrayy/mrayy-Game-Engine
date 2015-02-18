

/********************************************************************
	created:	2012/02/14
	created:	14:2:2012   15:21
	filename: 	d:\Development\mrayEngine\Engine\include\PhComplexContactReporter.h
	file path:	d:\Development\mrayEngine\Engine\include
	file base:	PhComplexContactReporter
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhComplexContactReporter__
#define __PhComplexContactReporter__

#include "IPhysicUserContactReport.h"


namespace mray
{
namespace physics
{

class PhComplexContactReporter:public IPhysicUserContactReport
{
private:
protected:
	typedef std::list<IPhysicUserContactReport*> ReportersList;
	ReportersList m_reporters;
public:
	PhComplexContactReporter();
	virtual~PhComplexContactReporter();

	void AddReporter(IPhysicUserContactReport*r);
	void RemoveReporter(IPhysicUserContactReport*r);
	void ClearReporters();

	void OnPairStartTouch(PhysicContactPair* pair);
	void OnPairTouch(PhysicContactPair* pair);
	void OnPairEndTouch(PhysicContactPair* pair);
	void OnPairImpact(PhysicContactPair* pair);
	void OnPairRoll(PhysicContactPair* pair);
	void OnPairSlide(PhysicContactPair* pair);

	void OnPairStartTouchForceThreshold(PhysicContactPair* pair);
	void OnPairTouchForceThreshold(PhysicContactPair* pair);
	void OnPairEndTouchForceThreshold(PhysicContactPair* pair);
};


}
}

#endif




/********************************************************************
	created:	2011/07/14
	created:	14:7:2011   14:58
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\CompositeContactReporter.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	CompositeContactReporter
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __CompositeContactReporter__
#define __CompositeContactReporter__

#include "IPhysicUserContactReport.h"
#include "ListenerContainer.h"

namespace mray
{
namespace physics
{

class CompositeContactReporter:public IPhysicUserContactReport,public ListenerContainer<IPhysicUserContactReport*>
{
private:
protected:
	DECLARE_FIRE_METHOD(OnPairStartTouch,(PhysicContactPair* pair),(pair));
	DECLARE_FIRE_METHOD(OnPairTouch,(PhysicContactPair* pair),(pair));
	DECLARE_FIRE_METHOD(OnPairEndTouch,(PhysicContactPair* pair),(pair));
	DECLARE_FIRE_METHOD(OnPairImpact,(PhysicContactPair* pair),(pair));
	DECLARE_FIRE_METHOD(OnPairRoll,(PhysicContactPair* pair),(pair));
	DECLARE_FIRE_METHOD(OnPairSlide,(PhysicContactPair* pair),(pair));
	DECLARE_FIRE_METHOD(OnPairStartTouchForceThreshold,(PhysicContactPair* pair),(pair));
	DECLARE_FIRE_METHOD(OnPairTouchForceThreshold,(PhysicContactPair* pair),(pair));
	DECLARE_FIRE_METHOD(OnPairEndTouchForceThreshold,(PhysicContactPair* pair),(pair));
public:
	CompositeContactReporter();
	virtual~CompositeContactReporter();

	virtual void OnPairStartTouch(PhysicContactPair* pair);
	virtual void OnPairTouch(PhysicContactPair* pair);
	virtual void OnPairEndTouch(PhysicContactPair* pair);
	virtual void OnPairImpact(PhysicContactPair* pair);
	virtual void OnPairRoll(PhysicContactPair* pair);
	virtual void OnPairSlide(PhysicContactPair* pair);

	virtual void OnPairStartTouchForceThreshold(PhysicContactPair* pair);
	virtual void OnPairTouchForceThreshold(PhysicContactPair* pair);
	virtual void OnPairEndTouchForceThreshold(PhysicContactPair* pair);
};

}
}

#endif

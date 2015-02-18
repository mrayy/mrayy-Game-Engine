


/********************************************************************
	created:	2011/05/12
	created:	12:5:2011   23:03
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IPhysicUserReport.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IPhysicUserReport
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IPhysicUserContactReport__
#define __IPhysicUserContactReport__


namespace mray
{
namespace physics
{
	class PhysicContactPair;

class IPhysicUserContactReport
{
private:
protected:
public:
	IPhysicUserContactReport(){}
	virtual~IPhysicUserContactReport(){}

	virtual void OnPairStartTouch(PhysicContactPair* pair){}
	virtual void OnPairTouch(PhysicContactPair* pair){}
	virtual void OnPairEndTouch(PhysicContactPair* pair){}
	virtual void OnPairImpact(PhysicContactPair* pair){}
	virtual void OnPairRoll(PhysicContactPair* pair){}
	virtual void OnPairSlide(PhysicContactPair* pair){}

	virtual void OnPairStartTouchForceThreshold(PhysicContactPair* pair){}
	virtual void OnPairTouchForceThreshold(PhysicContactPair* pair){}
	virtual void OnPairEndTouchForceThreshold(PhysicContactPair* pair){}
};

}
}

#endif

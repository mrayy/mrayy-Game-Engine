


/********************************************************************
	created:	2011/05/13
	created:	13:5:2011   21:47
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IPhysicUserRaycaseReport.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IPhysicUserRaycaseReport
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IPhysicUserRaycaseReport__
#define __IPhysicUserRaycaseReport__

#include "PhysicsRaycaseHit.h"

namespace mray
{
namespace physics
{

class IPhysicUserRaycaseReport
{
private:
protected:
public:
	IPhysicUserRaycaseReport(){}
	virtual~IPhysicUserRaycaseReport(){}

	virtual bool OnRayHit(const PhysicsRaycaseHit& hit)=0;

};


}
}

#endif

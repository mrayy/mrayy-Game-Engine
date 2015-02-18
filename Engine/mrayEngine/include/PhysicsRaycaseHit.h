


/********************************************************************
	created:	2011/05/13
	created:	13:5:2011   21:49
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\PhysicsRaycaseHit.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	PhysicsRaycaseHit
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhysicsRaycaseHit__
#define __PhysicsRaycaseHit__



#include "IPhysical3DDef.h"

namespace mray
{
namespace physics
{
	class IPhysicalShape;
	class IPhysicMaterial;

class PhysicsRaycaseHit
{
private:
protected:
public:
	PhysicsRaycaseHit():shape(0),distance(0),materialID(0)
	{}
	virtual~PhysicsRaycaseHit(){}


	IPhysicalShape* shape;
	math::vector3d worldImpact;
	math::vector3d worldNormal;
	float distance;
	MaterialID materialID;

};


}
}

#endif

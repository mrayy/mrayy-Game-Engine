

/********************************************************************
	created:	2011/05/12
	created:	12:5:2011   23:01
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\PhysicContactPair.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	PhysicContactPair
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhysicContactPair__
#define __PhysicContactPair__

#include <point3d.h>

namespace mray
{
namespace physics
{
	class IPhysicalNode;
	class IPhysicContactStreamIterator;

class PhysicContactPair
{
private:
protected:
public:
	PhysicContactPair()
	{
		nodes[0]=0;
		nodes[1]=0;
		contactStream=0;
	}
	virtual~PhysicContactPair(){}

	IPhysicalNode* nodes[2];

	IPhysicContactStreamIterator *contactStream;

	math::vector3d sumNormalForce;
	math::vector3d sumFrictionForce;
};

}
}

#endif

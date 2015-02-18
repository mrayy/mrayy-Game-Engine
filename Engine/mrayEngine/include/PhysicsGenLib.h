
/********************************************************************
	created:	2009/10/19
	created:	19:10:2009   7:49
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\PhysicsGenLib.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	PhysicsGenLib
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PhysicsGenLib___
#define ___PhysicsGenLib___

#include "ISingleton.h"
#include "PhysicsGenLib.h"
#include "IPhysical3DDef.h"

namespace mray{
namespace physics{

class PhysicsGenLib:public ISingleton<PhysicsGenLib>
{
private:
protected:
public:


	virtual void JointDesc_SetGlobalAxis(IPhysicalJoint3DDesc*desc,const math::vector3d&axis)=0;
	virtual void JointDesc_SetGlobalAnchor(IPhysicalJoint3DDesc*desc,const math::vector3d&anchor)=0;

};

}
}


#endif //___PhysicsGenLib___

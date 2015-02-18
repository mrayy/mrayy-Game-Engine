
/********************************************************************
	created:	2009/10/19
	created:	19:10:2009   7:51
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx\physXPhysicsGenLib.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx
	file base:	physXPhysicsGenLib
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___physXPhysicsGenLib___
#define ___physXPhysicsGenLib___

#include <PhysicsGenLib.h>

namespace mray{
namespace physics{

class physXPhysicsGenLib:public PhysicsGenLib
{
private:
protected:
public:
	physXPhysicsGenLib();
	virtual~physXPhysicsGenLib();

	virtual void JointDesc_SetGlobalAxis(IPhysicalJoint3DDesc*desc,const math::vector3d&axis);
	virtual void JointDesc_SetGlobalAnchor(IPhysicalJoint3DDesc*desc,const math::vector3d&anchor);

};

}
}

#endif //___physXPhysicsGenLib___

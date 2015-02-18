#include "stdafx.h"

#include "physXPhysicsGenLib.h"


namespace mray{
namespace physics{

physXPhysicsGenLib::physXPhysicsGenLib()
{
}
physXPhysicsGenLib::~physXPhysicsGenLib()
{
}

void physXPhysicsGenLib::JointDesc_SetGlobalAxis(IPhysicalJoint3DDesc*desc,const math::vector3d&axis)
{
}
void physXPhysicsGenLib::JointDesc_SetGlobalAnchor(IPhysicalJoint3DDesc*desc,const math::vector3d&anchor)
{
	//NxGetUtilLib()->NxJointDesc_SetGlobalAnchor(*this, anchor);
}


}
}


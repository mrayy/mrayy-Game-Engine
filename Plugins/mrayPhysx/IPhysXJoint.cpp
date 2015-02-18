
#include "stdafx.h"
#include "IPhysXJoint.h"
#include "NxJointDesc.h"
#include "IPhysical3DDef.h"
#include "CPhysXNode.h"
#include "PhysXCommon.h"

namespace mray{
namespace physics{
	
IPhysXJoint::IPhysXJoint(NxJoint*j)
{
	m_joint=j;


	m_node2RelativeQuat=getNode2LocalOrintation();
	math::vector3d angles;
	m_node2RelativeQuat.toEulerAngles(angles);
}

math::quaternion IPhysXJoint::getNode2LocalOrintation()
{
	NxActor*a,*b;

	m_joint->getActors(&a,&b);
	if(!a || !b)
		return math::quaternion::Identity;
	math::quaternion node1Quat;
	math::quaternion node2Quat;
	NxQuat q;
	q=a->getGlobalOrientationQuat();
	node1Quat=math::quaternion(q.w,q.x,q.y,q.z);
	q=b->getGlobalOrientationQuat();
	node2Quat=math::quaternion(q.w,q.x,q.y,q.z);

	return node1Quat.inverse()*node2Quat;
}

void IPhysXJoint::ConvertToDesc(const IPhysicalJoint3DDesc* srcDesc,NxJointDesc* dstDesc)
{
	if(srcDesc->body1)
		dstDesc->actor[0]=((CPhysXNode*)srcDesc->body1)->getActor();
	else dstDesc->actor[0]=0;
	if(srcDesc->body2)
		dstDesc->actor[1]=((CPhysXNode*)srcDesc->body2)->getActor();
	else dstDesc->actor[1]=0;

	dstDesc->localNormal[0]=ToNxVec3(srcDesc->localNormal[0]);
	dstDesc->localNormal[1]=ToNxVec3(srcDesc->localNormal[1]);

	dstDesc->localAxis[0]=ToNxVec3(srcDesc->localAxis[0]);
	dstDesc->localAxis[1]=ToNxVec3(srcDesc->localAxis[1]);

	dstDesc->localAnchor[0]=ToNxVec3(srcDesc->localAnchor[0]);
	dstDesc->localAnchor[1]=ToNxVec3(srcDesc->localAnchor[1]);

	dstDesc->maxForce=srcDesc->maxForce;
	dstDesc->maxTorque=srcDesc->maxTorque;
	dstDesc->solverExtrapolationFactor=srcDesc->solverExtrapolationFactor;
	dstDesc->useAccelerationSpring=srcDesc->useAccelerationSpring;
//	dstDesc->userData=srcDesc->userData;

	dstDesc->jointFlags=NX_JF_COLLISION_ENABLED * srcDesc->flags[EJFlag_CollisionEnable] | 
		NX_JF_VISUALIZATION * srcDesc->flags[EJFlag_Visualization];

	if(srcDesc->m_fromGlobalAxis){
		dstDesc->setGlobalAxis(ToNxVec3(srcDesc->m_globalAxis));
	}
	if(srcDesc->m_fromGlobalAnchor){
		dstDesc->setGlobalAnchor(ToNxVec3(srcDesc->m_globalAnchor));
	}
}
void IPhysXJoint::ConvertToDesc(const NxJointDesc* srcDesc,IPhysicalJoint3DDesc* dstDesc)
{
	if(srcDesc->actor[0])
		dstDesc->body1=(CPhysXNode*)(srcDesc->actor[0]->userData);
	else
		dstDesc->body1=0;
	if(srcDesc->actor[1])
		dstDesc->body2=(CPhysXNode*)(srcDesc->actor[1]->userData);
	else
		dstDesc->body2=0;

	dstDesc->localNormal[0]=ToVec3(srcDesc->localNormal[0]);
	dstDesc->localNormal[1]=ToVec3(srcDesc->localNormal[1]);

	dstDesc->localAxis[0]=ToVec3(srcDesc->localAxis[0]);
	dstDesc->localAxis[1]=ToVec3(srcDesc->localAxis[1]);

	dstDesc->localAnchor[0]=ToVec3(srcDesc->localAnchor[0]);
	dstDesc->localAnchor[1]=ToVec3(srcDesc->localAnchor[1]);

	dstDesc->maxForce=srcDesc->maxForce;
	dstDesc->maxTorque=srcDesc->maxTorque;
	dstDesc->solverExtrapolationFactor=srcDesc->solverExtrapolationFactor;
	dstDesc->useAccelerationSpring=srcDesc->useAccelerationSpring;
// 	dstDesc->userData=srcDesc->userData;

	dstDesc->flags[EJFlag_CollisionEnable]=srcDesc->jointFlags & NX_JF_COLLISION_ENABLED;
	dstDesc->flags[EJFlag_Visualization]=srcDesc->jointFlags & NX_JF_VISUALIZATION;
	dstDesc->m_fromGlobalAxis=false;
	dstDesc->m_fromGlobalAnchor=false;
}

}
}

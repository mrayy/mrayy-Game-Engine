#include "stdafx.h"

#include <NxJoint.h>
#include "PhysXManager.h"
#include "physXCommon.h"
#include "CPhysXNode.h"

#include "PhysXPointOnPlaneJoint.h"
#include "NxPointInPlaneJoint.h"
#include "NxPointInPlaneJointDesc.h"
#include <NxActor.h>
namespace mray{
namespace physics{


PhysXPointOnPlaneJoint::PhysXPointOnPlaneJoint(IPhysicManager*c,NxJoint*joint):IPointOnPlaneJoint3D(c)
,IPhysXJoint(joint){
	m_joint=joint;
}

PhysXPointOnPlaneJoint::~PhysXPointOnPlaneJoint(){
}

IPhysicalNode* PhysXPointOnPlaneJoint::getNode1(){


	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!a)return 0;
	return (IPhysicalNode*)a->userData;
}
IPhysicalNode* PhysXPointOnPlaneJoint::getNode2(){

	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!b)return 0;
	return (IPhysicalNode*)b->userData;
}
//Sets the point where the two nodes are attached, specified in global coordinates.
void PhysXPointOnPlaneJoint::setGlobalAnchor(const math::vector3d &v){
	m_joint->setGlobalAnchor(ToNxVec3(v));
}


math::vector3d PhysXPointOnPlaneJoint::getGlobalAnchor(){
	NxVec3 vec=m_joint->getGlobalAnchor();
	return ToVec3(vec);
}

//Sets the direction of the joint's primary axis, specified in global coordinates.
void PhysXPointOnPlaneJoint::setGlobalAxis(const math::vector3d &v){
	m_joint->setGlobalAxis(ToNxVec3(v));
}

math::vector3d PhysXPointOnPlaneJoint::getGlobalAxis(){
	NxVec3 vec=m_joint->getGlobalAxis();
	return ToVec3(vec);
}

EJoint3DState PhysXPointOnPlaneJoint::getJointState(){
	NxJointState s=m_joint->getState();
	if(s==NX_JS_UNBOUND)
		return EJ3S_Unbound;
	if(s==NX_JS_SIMULATING)
		return EJ3S_Simulating;
	return EJ3S_Broken;
}

void PhysXPointOnPlaneJoint::setBreakable(float maxForce,float maxTorque){
	m_joint->setBreakable(maxForce,maxTorque);
}

// Sets the solver extrapolation factor.
void PhysXPointOnPlaneJoint::setSolverExtrapolationFactor(float f){
	m_joint->setSolverExtrapolationFactor(f);
}

float PhysXPointOnPlaneJoint::getSolverExtrapolationFactor(){
	return m_joint->getSolverExtrapolationFactor();
}

void PhysXPointOnPlaneJoint::setUseAccelerationSpring(bool u){
	m_joint->setUseAccelerationSpring(u);
}
bool PhysXPointOnPlaneJoint::getUseAccelerationSpring(){
	return m_joint->getUseAccelerationSpring();
}

void PhysXPointOnPlaneJoint::setLimitPoint(const math::vector3d&p,bool pointOnNode2){
	m_joint->setLimitPoint(ToNxVec3(p),pointOnNode2);
}

// returns true if point is on node2
bool PhysXPointOnPlaneJoint::getLimitPoint(math::vector3d&p){
	return m_joint->getLimitPoint(ToNxVec3(p));
}


bool PhysXPointOnPlaneJoint::addLimitPlane(const math::vector3d& normal, const math::vector3d & pointInPlane, 
	float restitution ) 
{
	return m_joint->addLimitPlane(ToNxVec3(normal),ToNxVec3(pointInPlane),restitution);
}

void PhysXPointOnPlaneJoint::clearLimitPlanes(){
	m_joint->purgeLimitPlanes();
}
void PhysXPointOnPlaneJoint::SaveToDesc(IPhysicalJointPointOnPlaneDesc* desc)
{
	if(!m_joint)return;
	NxPointInPlaneJointDesc d;

	((NxPointInPlaneJoint*)m_joint)->saveToDesc(d);
	IPhysXJoint::ConvertToDesc(&d,desc);
}
void PhysXPointOnPlaneJoint::LoadFromDesc(const IPhysicalJointPointOnPlaneDesc* desc)
{
	if(!m_joint)return;
	NxPointInPlaneJointDesc d;
	IPhysXJoint::ConvertToDesc(desc,&d);

	((NxPointInPlaneJoint*)m_joint)->loadFromDesc(d);
}

}
}





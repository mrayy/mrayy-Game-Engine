#include "stdafx.h"

#include <NxJoint.h>
#include "PhysXManager.h"
#include "physXCommon.h"
#include "CPhysXNode.h"

#include "PhysXPointOnLineJoint.h"
#include "NxPointOnLineJoint.h"
#include "NxPointOnLineJointDesc.h"
#include <NxActor.h>
namespace mray{
namespace physics{


PhysXPointOnLineJoint::PhysXPointOnLineJoint(IPhysicManager*c,NxJoint*joint):IPointOnLineJoint3D(c),IPhysXJoint(joint)
{
	m_joint=joint;
}

PhysXPointOnLineJoint::~PhysXPointOnLineJoint(){
}

IPhysicalNode* PhysXPointOnLineJoint::getNode1(){


	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!a)return 0;
	return (IPhysicalNode*)a->userData;
}
IPhysicalNode* PhysXPointOnLineJoint::getNode2(){

	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!b)return 0;
	return (IPhysicalNode*)b->userData;
}

//Sets the point where the two nodes are attached, specified in global coordinates.
void PhysXPointOnLineJoint::setGlobalAnchor(const math::vector3d &v){
	m_joint->setGlobalAnchor(ToNxVec3(v));
}


math::vector3d PhysXPointOnLineJoint::getGlobalAnchor(){
	NxVec3 vec=m_joint->getGlobalAnchor();
	return ToVec3(vec);
}

//Sets the direction of the joint's primary axis, specified in global coordinates.
void PhysXPointOnLineJoint::setGlobalAxis(const math::vector3d &v){
	m_joint->setGlobalAxis(ToNxVec3(v));
}

math::vector3d PhysXPointOnLineJoint::getGlobalAxis(){
	NxVec3 vec=m_joint->getGlobalAxis();
	return ToVec3(vec);
}

EJoint3DState PhysXPointOnLineJoint::getJointState(){
	NxJointState s=m_joint->getState();
	if(s==NX_JS_UNBOUND)
		return EJ3S_Unbound;
	if(s==NX_JS_SIMULATING)
		return EJ3S_Simulating;
	return EJ3S_Broken;
}

void PhysXPointOnLineJoint::setBreakable(float maxForce,float maxTorque){
	m_joint->setBreakable(maxForce,maxTorque);
}

// Sets the solver extrapolation factor.
void PhysXPointOnLineJoint::setSolverExtrapolationFactor(float f){
	m_joint->setSolverExtrapolationFactor(f);
}

float PhysXPointOnLineJoint::getSolverExtrapolationFactor(){
	return m_joint->getSolverExtrapolationFactor();
}

void PhysXPointOnLineJoint::setUseAccelerationSpring(bool u){
	m_joint->setUseAccelerationSpring(u);
}
bool PhysXPointOnLineJoint::getUseAccelerationSpring(){
	return m_joint->getUseAccelerationSpring();
}

void PhysXPointOnLineJoint::setLimitPoint(const math::vector3d&p,bool pointOnNode2){
	m_joint->setLimitPoint(ToNxVec3(p),pointOnNode2);
}

// returns true if point is on node2
bool PhysXPointOnLineJoint::getLimitPoint(math::vector3d&p){
	return m_joint->getLimitPoint(ToNxVec3(p));
}


bool PhysXPointOnLineJoint::addLimitPlane(const math::vector3d& normal, const math::vector3d & pointInPlane, 
	float restitution ) 
{
	return m_joint->addLimitPlane(ToNxVec3(normal),ToNxVec3(pointInPlane),restitution);
}

void PhysXPointOnLineJoint::clearLimitPlanes(){
	m_joint->purgeLimitPlanes();
}

void PhysXPointOnLineJoint::SaveToDesc(IPhysicalJointPointOnLineDesc* desc)
{
	if(!m_joint)return;
	NxPointOnLineJointDesc d;

	((NxPointOnLineJoint*)m_joint)->saveToDesc(d);
	IPhysXJoint::ConvertToDesc(&d,desc);
}
void PhysXPointOnLineJoint::LoadFromDesc(const IPhysicalJointPointOnLineDesc* desc)
{
	if(!m_joint)return;
	NxPointOnLineJointDesc d;
	IPhysXJoint::ConvertToDesc(desc,&d);

	((NxPointOnLineJoint*)m_joint)->loadFromDesc(d);
}
}
}




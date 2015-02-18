#include "stdafx.h"

#include <NxJoint.h>
#include "PhysXManager.h"
#include "physXCommon.h"
#include "CPhysXNode.h"

#include "NxPrismaticJoint.h"
#include "NxPrismaticJointDesc.h"

#include "PhysXPrismaticJoint.h"
#include <NxActor.h>


namespace mray{
namespace physics{


PhysXPrismaticJoint::PhysXPrismaticJoint(IPhysicManager*c,NxJoint*joint):IPrismaticJoint3D(c),IPhysXJoint(joint)
{
	m_joint=joint;
}

PhysXPrismaticJoint::~PhysXPrismaticJoint(){
}

IPhysicalNode* PhysXPrismaticJoint::getNode1(){


	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!a)return 0;
	return (IPhysicalNode*)a->userData;
}
IPhysicalNode* PhysXPrismaticJoint::getNode2(){

	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!b)return 0;
	return (IPhysicalNode*)b->userData;
}

//Sets the point where the two nodes are attached, specified in global coordinates.
void PhysXPrismaticJoint::setGlobalAnchor(const math::vector3d &v){
	m_joint->setGlobalAnchor(ToNxVec3(v));
}


math::vector3d PhysXPrismaticJoint::getGlobalAnchor(){
	NxVec3 vec=m_joint->getGlobalAnchor();
	return ToVec3(vec);
}

//Sets the direction of the joint's primary axis, specified in global coordinates.
void PhysXPrismaticJoint::setGlobalAxis(const math::vector3d &v){
	m_joint->setGlobalAxis(ToNxVec3(v));
}

math::vector3d PhysXPrismaticJoint::getGlobalAxis(){
	NxVec3 vec=m_joint->getGlobalAxis();
	return ToVec3(vec);
}

EJoint3DState PhysXPrismaticJoint::getJointState(){
	NxJointState s=m_joint->getState();
	if(s==NX_JS_UNBOUND)
		return EJ3S_Unbound;
	if(s==NX_JS_SIMULATING)
		return EJ3S_Simulating;
	return EJ3S_Broken;
}

void PhysXPrismaticJoint::setBreakable(float maxForce,float maxTorque){
	m_joint->setBreakable(maxForce,maxTorque);
}

// Sets the solver extrapolation factor.
void PhysXPrismaticJoint::setSolverExtrapolationFactor(float f){
	m_joint->setSolverExtrapolationFactor(f);
}

float PhysXPrismaticJoint::getSolverExtrapolationFactor(){
	return m_joint->getSolverExtrapolationFactor();
}

void PhysXPrismaticJoint::setUseAccelerationSpring(bool u){
	m_joint->setUseAccelerationSpring(u);
}
bool PhysXPrismaticJoint::getUseAccelerationSpring(){
	return m_joint->getUseAccelerationSpring();
}

void PhysXPrismaticJoint::setLimitPoint(const math::vector3d&p,bool pointOnNode2){
	m_joint->setLimitPoint(ToNxVec3(p),pointOnNode2);
}

// returns true if point is on node2
bool PhysXPrismaticJoint::getLimitPoint(math::vector3d&p){
	return m_joint->getLimitPoint(ToNxVec3(p));
}


bool PhysXPrismaticJoint::addLimitPlane(const math::vector3d& normal, const math::vector3d & pointInPlane, 
	float restitution ) 
{
	return m_joint->addLimitPlane(ToNxVec3(normal),ToNxVec3(pointInPlane),restitution);
}

void PhysXPrismaticJoint::clearLimitPlanes(){
	m_joint->purgeLimitPlanes();
}
void PhysXPrismaticJoint::SaveToDesc(IPhysicalJointPrismaticDesc* desc)
{
	if(!m_joint)return;
	NxPrismaticJointDesc d;

	((NxPrismaticJoint*)m_joint)->saveToDesc(d);
	IPhysXJoint::ConvertToDesc(&d,desc);
}
void PhysXPrismaticJoint::LoadFromDesc(const IPhysicalJointPrismaticDesc* desc)
{
	if(!m_joint)return;
	NxPrismaticJointDesc d;
	IPhysXJoint::ConvertToDesc(desc,&d);

	((NxPrismaticJoint*)m_joint)->loadFromDesc(d);
}

}
}

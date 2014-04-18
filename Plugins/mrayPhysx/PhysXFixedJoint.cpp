#include "stdafx.h"


#include <NxJoint.h>
#include "PhysXManager.h"
#include "physXCommon.h"
#include "CPhysXNode.h"
#include <NxActor.h>

#include "PhysXFixedJoint.h"
#include "NxFixedJointDesc.h"
#include "NxFixedJoint.h"

namespace mray{
namespace physics{


PhysXFixedJoint::PhysXFixedJoint(IPhysicManager*c,NxJoint*joint):IFixedJoint3D(c),IPhysXJoint(joint)
{
	m_joint=joint;
}

PhysXFixedJoint::~PhysXFixedJoint(){
}

IPhysicalNode* PhysXFixedJoint::getNode1(){


	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!a)return 0;
	return (IPhysicalNode*)a->userData;
}
IPhysicalNode* PhysXFixedJoint::getNode2(){

	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!b)return 0;
	return (IPhysicalNode*)b->userData;
}

//Sets the point where the two nodes are attached, specified in global coordinates.
void PhysXFixedJoint::setGlobalAnchor(const math::vector3d &v){
	m_joint->setGlobalAnchor(ToNxVec3(v));
}


math::vector3d PhysXFixedJoint::getGlobalAnchor(){
	NxVec3 vec=m_joint->getGlobalAnchor();
	return ToVec3(vec);
}

//Sets the direction of the joint's primary axis, specified in global coordinates.
void PhysXFixedJoint::setGlobalAxis(const math::vector3d &v){
	m_joint->setGlobalAxis(ToNxVec3(v));
}

math::vector3d PhysXFixedJoint::getGlobalAxis(){
	NxVec3 vec=m_joint->getGlobalAxis();
	return ToVec3(vec);
}

EJoint3DState PhysXFixedJoint::getJointState(){
	NxJointState s=m_joint->getState();
	if(s==NX_JS_UNBOUND)
		return EJ3S_Unbound;
	if(s==NX_JS_SIMULATING)
		return EJ3S_Simulating;
	return EJ3S_Broken;
}

void PhysXFixedJoint::setBreakable(float maxForce,float maxTorque){
	m_joint->setBreakable(maxForce,maxTorque);
}

// Sets the solver extrapolation factor.
void PhysXFixedJoint::setSolverExtrapolationFactor(float f){
	m_joint->setSolverExtrapolationFactor(f);
}

float PhysXFixedJoint::getSolverExtrapolationFactor(){
	return m_joint->getSolverExtrapolationFactor();
}

void PhysXFixedJoint::setUseAccelerationSpring(bool u){
	m_joint->setUseAccelerationSpring(u);
}
bool PhysXFixedJoint::getUseAccelerationSpring(){
	return m_joint->getUseAccelerationSpring();
}

void PhysXFixedJoint::setLimitPoint(const math::vector3d&p,bool pointOnNode2){
	m_joint->setLimitPoint(ToNxVec3(p),pointOnNode2);
}

// returns true if point is on node2
bool PhysXFixedJoint::getLimitPoint(math::vector3d&p){
	return m_joint->getLimitPoint(ToNxVec3(p));
}


bool PhysXFixedJoint::addLimitPlane(const math::vector3d& normal, const math::vector3d & pointInPlane, 
	float restitution ) 
{
	return m_joint->addLimitPlane(ToNxVec3(normal),ToNxVec3(pointInPlane),restitution);
}

void PhysXFixedJoint::clearLimitPlanes(){
	m_joint->purgeLimitPlanes();
}

void PhysXFixedJoint::SaveToDesc(IPhysicalJointFixedDesc* desc)
{
	if(!m_joint)return;
	NxFixedJointDesc d;

	((NxFixedJoint*)m_joint)->saveToDesc(d);
	IPhysXJoint::ConvertToDesc(&d,desc);
}
void PhysXFixedJoint::LoadFromDesc(const IPhysicalJointFixedDesc* desc)
{
	if(!m_joint)return;
	NxFixedJointDesc d;
	IPhysXJoint::ConvertToDesc(desc,&d);

	((NxFixedJoint*)m_joint)->loadFromDesc(d);
}

}
}





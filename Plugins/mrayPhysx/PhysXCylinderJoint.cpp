#include "stdafx.h"

#include <NxJoint.h>
#include "PhysXManager.h"
#include "physXCommon.h"
#include "CPhysXNode.h"

#include "NxCylindricalJoint.h"
#include "NxCylindricalJointDesc.h"
#include "PhysXCylinderJoint.h"
#include <NxActor.h>
namespace mray{
namespace physics{


PhysXCylinderJoint::PhysXCylinderJoint(IPhysicManager*c,NxJoint*joint):ICylinderJoint3D(c),IPhysXJoint(joint)
{
	m_joint=joint;
}

PhysXCylinderJoint::~PhysXCylinderJoint(){
}

IPhysicalNode* PhysXCylinderJoint::getNode1(){


	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!a)return 0;
	return (IPhysicalNode*)a->userData;
}
IPhysicalNode* PhysXCylinderJoint::getNode2(){

	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!b)return 0;
	return (IPhysicalNode*)b->userData;
}

//Sets the point where the two nodes are attached, specified in global coordinates.
void PhysXCylinderJoint::setGlobalAnchor(const math::vector3d &v){
	m_joint->setGlobalAnchor(ToNxVec3(v));
}


math::vector3d PhysXCylinderJoint::getGlobalAnchor(){
	NxVec3 vec=m_joint->getGlobalAnchor();
	return ToVec3(vec);
}

//Sets the direction of the joint's primary axis, specified in global coordinates.
void PhysXCylinderJoint::setGlobalAxis(const math::vector3d &v){
	m_joint->setGlobalAxis(ToNxVec3(v));
}

math::vector3d PhysXCylinderJoint::getGlobalAxis(){
	NxVec3 vec=m_joint->getGlobalAxis();
	return ToVec3(vec);
}

EJoint3DState PhysXCylinderJoint::getJointState(){
	NxJointState s=m_joint->getState();
	if(s==NX_JS_UNBOUND)
		return EJ3S_Unbound;
	if(s==NX_JS_SIMULATING)
		return EJ3S_Simulating;
	return EJ3S_Broken;
}

void PhysXCylinderJoint::setBreakable(float maxForce,float maxTorque){
	m_joint->setBreakable(maxForce,maxTorque);
}

// Sets the solver extrapolation factor.
void PhysXCylinderJoint::setSolverExtrapolationFactor(float f){
	m_joint->setSolverExtrapolationFactor(f);
}

float PhysXCylinderJoint::getSolverExtrapolationFactor(){
	return m_joint->getSolverExtrapolationFactor();
}

void PhysXCylinderJoint::setUseAccelerationSpring(bool u){
	m_joint->setUseAccelerationSpring(u);
}
bool PhysXCylinderJoint::getUseAccelerationSpring(){
	return m_joint->getUseAccelerationSpring();
}

void PhysXCylinderJoint::setLimitPoint(const math::vector3d&p,bool pointOnNode2){
	m_joint->setLimitPoint(ToNxVec3(p),pointOnNode2);
}

// returns true if point is on node2
bool PhysXCylinderJoint::getLimitPoint(math::vector3d&p){
	return m_joint->getLimitPoint(ToNxVec3(p));
}


bool PhysXCylinderJoint::addLimitPlane(const math::vector3d& normal, const math::vector3d & pointInPlane, 
	float restitution ) 
{
	return m_joint->addLimitPlane(ToNxVec3(normal),ToNxVec3(pointInPlane),restitution);
}

void PhysXCylinderJoint::clearLimitPlanes(){
	m_joint->purgeLimitPlanes();
}


void PhysXCylinderJoint::SaveToDesc(IPhysicalJointCylinderDesc* desc)
{
	if(!m_joint)return;
	NxCylindricalJointDesc d;

	((NxCylindricalJoint*)m_joint)->saveToDesc(d);
	IPhysXJoint::ConvertToDesc(&d,desc);
}
void PhysXCylinderJoint::LoadFromDesc(const IPhysicalJointCylinderDesc* desc)
{
	if(!m_joint)return;
	NxCylindricalJointDesc d;
	IPhysXJoint::ConvertToDesc(desc,&d);

	((NxCylindricalJoint*)m_joint)->loadFromDesc(d);
}

}
}



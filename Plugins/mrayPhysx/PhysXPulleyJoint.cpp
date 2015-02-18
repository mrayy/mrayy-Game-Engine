#include "stdafx.h"




#include <NxPulleyJoint.h>
#include <NxMotorDesc.h>
#include "PhysXManager.h"
#include "physXCommon.h"
#include "CPhysXNode.h"

#include "PhysXPulleyJoint.h"
#include "NxPulleyJointDesc.h"
#include "NxPulleyJoint.h"
#include <NxActor.h>

namespace mray{
namespace physics{


PhysXPulleyJoint::PhysXPulleyJoint(IPhysicManager*c,NxJoint*joint):IPulleyJoint3D(c),IPhysXJoint(joint)
{
	m_joint=joint;
}

PhysXPulleyJoint::~PhysXPulleyJoint(){
}

IPhysicalNode* PhysXPulleyJoint::getNode1(){


	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!a)return 0;
	return (IPhysicalNode*)a->userData;
}
IPhysicalNode* PhysXPulleyJoint::getNode2(){

	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!b)return 0;
	return (IPhysicalNode*)b->userData;
}

//Sets the point where the two nodes are attached, specified in global coordinates.
void PhysXPulleyJoint::setGlobalAnchor(const math::vector3d &v){
	m_joint->setGlobalAnchor(ToNxVec3(v));
}


math::vector3d PhysXPulleyJoint::getGlobalAnchor(){
	NxVec3 vec=m_joint->getGlobalAnchor();
	return ToVec3(vec);
}

//Sets the direction of the joint's primary axis, specified in global coordinates.
void PhysXPulleyJoint::setGlobalAxis(const math::vector3d &v){
	m_joint->setGlobalAxis(ToNxVec3(v));
}

math::vector3d PhysXPulleyJoint::getGlobalAxis(){
	NxVec3 vec=m_joint->getGlobalAxis();
	return ToVec3(vec);
}

EJoint3DState PhysXPulleyJoint::getJointState(){
	NxJointState s=m_joint->getState();
	if(s==NX_JS_UNBOUND)
		return EJ3S_Unbound;
	if(s==NX_JS_SIMULATING)
		return EJ3S_Simulating;
	return EJ3S_Broken;
}

void PhysXPulleyJoint::setBreakable(float maxForce,float maxTorque){
	m_joint->setBreakable(maxForce,maxTorque);
}

// Sets the solver extrapolation factor.
void PhysXPulleyJoint::setSolverExtrapolationFactor(float f){
	m_joint->setSolverExtrapolationFactor(f);
}

float PhysXPulleyJoint::getSolverExtrapolationFactor(){
	return m_joint->getSolverExtrapolationFactor();
}

void PhysXPulleyJoint::setUseAccelerationSpring(bool u){
	m_joint->setUseAccelerationSpring(u);
}
bool PhysXPulleyJoint::getUseAccelerationSpring(){
	return m_joint->getUseAccelerationSpring();
}

void PhysXPulleyJoint::setLimitPoint(const math::vector3d&p,bool pointOnNode2){
	m_joint->setLimitPoint(ToNxVec3(p),pointOnNode2);
}

// returns true if point is on node2
bool PhysXPulleyJoint::getLimitPoint(math::vector3d&p){
	return m_joint->getLimitPoint(ToNxVec3(p));
}


bool PhysXPulleyJoint::addLimitPlane(const math::vector3d& normal, const math::vector3d & pointInPlane, 
	float restitution ) 
{
	return m_joint->addLimitPlane(ToNxVec3(normal),ToNxVec3(pointInPlane),restitution);
}

void PhysXPulleyJoint::clearLimitPlanes(){
	m_joint->purgeLimitPlanes();
}

//////////////////////////////////////////////////////////////////////////


void PhysXPulleyJoint::setMotor(const PhysicalMotor3DDesc&d){
	NxMotorDesc desc;
	desc.velTarget=d.velTarget;
	desc.freeSpin=d.freeSpin;
	desc.maxForce=d.maxForce;
	((NxPulleyJoint*) m_joint)->setMotor(desc);
}
bool PhysXPulleyJoint::getMotor(PhysicalMotor3DDesc&d){
	NxMotorDesc desc;
	bool r=((NxPulleyJoint*) m_joint)->getMotor(desc);

	d.velTarget=desc.velTarget;
	d.freeSpin=desc.freeSpin;
	d.maxForce=desc.maxForce;

	return r;
}

void PhysXPulleyJoint::SaveToDesc(IPhysicalJointPulleyDesc* desc)
{
	if(!m_joint)return;
	NxPulleyJointDesc d;

	((NxPulleyJoint*)m_joint)->saveToDesc(d);
	ConvertToDesc(&d,desc);
}
void PhysXPulleyJoint::LoadFromDesc(const IPhysicalJointPulleyDesc* desc)
{
	if(!m_joint)return;
	NxPulleyJointDesc d;
	ConvertToDesc(desc,&d);

	((NxPulleyJoint*)m_joint)->loadFromDesc(d);
}
void PhysXPulleyJoint::ConvertToDesc(const IPhysicalJointPulleyDesc* srcDesc,NxPulleyJointDesc* dstDesc)
{
	IPhysXJoint::ConvertToDesc(srcDesc,dstDesc);

	dstDesc->pulley[0]=ToNxVec3(srcDesc->pulley[0]);
	dstDesc->pulley[1]=ToNxVec3(srcDesc->pulley[1]);

	dstDesc->flags=NX_PJF_IS_RIGID * srcDesc->isRigid | 
		NX_PJF_MOTOR_ENABLED * srcDesc->motorEnable ;

	dstDesc->distance=srcDesc->distance;
	dstDesc->stiffness=srcDesc->stiffness;
	dstDesc->ratio=srcDesc->ratio;

}
void PhysXPulleyJoint::ConvertToDesc(const NxPulleyJointDesc* srcDesc,IPhysicalJointPulleyDesc* dstDesc)
{
	IPhysXJoint::ConvertToDesc(srcDesc,dstDesc);

	dstDesc->pulley[0]=ToVec3(srcDesc->pulley[0]);
	dstDesc->pulley[1]=ToVec3(srcDesc->pulley[1]);

	dstDesc->isRigid= NX_PJF_IS_RIGID & srcDesc->flags;
	dstDesc->motorEnable= NX_PJF_MOTOR_ENABLED & srcDesc->flags;

	dstDesc->distance=srcDesc->distance;
	dstDesc->stiffness=srcDesc->stiffness;
	dstDesc->ratio=srcDesc->ratio;

}

}
}

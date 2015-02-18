#include "stdafx.h"

#include <NxD6Joint.h>
#include <NxD6JointDesc.h>
#include "PhysXManager.h"
#include "physXCommon.h"
#include "CPhysXNode.h"

#include "PhysXDOF6Joint.h"
#include "PhysXObjectConverter.h"


namespace mray{
namespace physics{


PhysXDOF6Joint::PhysXDOF6Joint(IPhysicManager*c,NxJoint*joint):IDOF6Joint3D(c),IPhysXJoint(joint)
{
}

PhysXDOF6Joint::~PhysXDOF6Joint(){
}

IPhysicalNode* PhysXDOF6Joint::getNode1(){


	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!a)return 0;
	return (IPhysicalNode*)a->userData;
}
IPhysicalNode* PhysXDOF6Joint::getNode2(){

	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!b)return 0;
	return (IPhysicalNode*)b->userData;
}

//Sets the point where the two nodes are attached, specified in global coordinates.
void PhysXDOF6Joint::setGlobalAnchor(const math::vector3d &v){
	m_joint->setGlobalAnchor(ToNxVec3(v));
}


math::vector3d PhysXDOF6Joint::getGlobalAnchor(){
	NxVec3 vec=m_joint->getGlobalAnchor();
	return ToVec3(vec);
}

//Sets the direction of the joint's primary axis, specified in global coordinates.
void PhysXDOF6Joint::setGlobalAxis(const math::vector3d &v){
	m_joint->setGlobalAxis(ToNxVec3(v));
}

math::vector3d PhysXDOF6Joint::getGlobalAxis(){
	NxVec3 vec=m_joint->getGlobalAxis();
	return ToVec3(vec);
}

EJoint3DState PhysXDOF6Joint::getJointState(){
	NxJointState s=m_joint->getState();
	if(s==NX_JS_UNBOUND)
		return EJ3S_Unbound;
	if(s==NX_JS_SIMULATING)
		return EJ3S_Simulating;
	return EJ3S_Broken;
}

void PhysXDOF6Joint::setBreakable(float maxForce,float maxTorque){
	m_joint->setBreakable(maxForce,maxTorque);
}

// Sets the solver extrapolation factor.
void PhysXDOF6Joint::setSolverExtrapolationFactor(float f){
	m_joint->setSolverExtrapolationFactor(f);
}

float PhysXDOF6Joint::getSolverExtrapolationFactor(){
	return m_joint->getSolverExtrapolationFactor();
}

void PhysXDOF6Joint::setUseAccelerationSpring(bool u){
	m_joint->setUseAccelerationSpring(u);
}
bool PhysXDOF6Joint::getUseAccelerationSpring(){
	return m_joint->getUseAccelerationSpring();
}

void PhysXDOF6Joint::setLimitPoint(const math::vector3d&p,bool pointOnNode2){
	m_joint->setLimitPoint(ToNxVec3(p),pointOnNode2);
}

// returns true if point is on node2
bool PhysXDOF6Joint::getLimitPoint(math::vector3d&p){
	return m_joint->getLimitPoint(ToNxVec3(p));
}


bool PhysXDOF6Joint::addLimitPlane(const math::vector3d& normal, const math::vector3d & pointInPlane, 
	float restitution ) 
{
	return m_joint->addLimitPlane(ToNxVec3(normal),ToNxVec3(pointInPlane),restitution);
}

void PhysXDOF6Joint::clearLimitPlanes(){
	m_joint->purgeLimitPlanes();
}

void PhysXDOF6Joint::setDrivePosition(const math::vector3d&p){
	((NxD6Joint*)m_joint)->setDrivePosition(ToNxVec3(p));
}

 
void PhysXDOF6Joint::setDriveOrientation(const math::quaternion&p){
	NxQuat q;
	q.x=p.x;
	q.y=p.y;
	q.z=p.z;
	q.w=p.w;
	((NxD6Joint*)m_joint)->setDriveOrientation(q);
}


void PhysXDOF6Joint::setDriveLinearVelocity(const math::vector3d&p){
	((NxD6Joint*)m_joint)->setDriveLinearVelocity(ToNxVec3(p));
}


void PhysXDOF6Joint::setDriveAngularVelocity(const math::vector3d&p){
	((NxD6Joint*)m_joint)->setDriveAngularVelocity(ToNxVec3(p));
}

void PhysXDOF6Joint::setAngularDriveProperties(PhysicalJointDriveDesc twistDriveDesc,PhysicalJointDriveDesc swingDriveDesc)
{
	NxD6Joint* joint = (NxD6Joint*)m_joint;
	NxD6JointDesc jointDesc;
	joint->saveToDesc(jointDesc);
	PhysXObjectConverter::copyJointDriveDesc(jointDesc.twistDrive ,twistDriveDesc);
	PhysXObjectConverter::copyJointDriveDesc(jointDesc.swingDrive, swingDriveDesc);
	joint->loadFromDesc(jointDesc);

}
void PhysXDOF6Joint::SaveToDesc(IPhysicalJointD6Desc* desc)
{
	if(!m_joint)return;
	NxD6JointDesc d;

	((NxD6Joint*)m_joint)->saveToDesc(d);
	ConvertToDesc(&d,desc);
}
void PhysXDOF6Joint::LoadFromDesc(const IPhysicalJointD6Desc* desc)
{
	if(!m_joint)return;
	NxD6JointDesc d;
	ConvertToDesc(desc,&d);

	((NxD6Joint*)m_joint)->loadFromDesc(d);
}


void PhysXDOF6Joint::ConvertToDesc(const IPhysicalJointD6Desc* srcDesc,NxD6JointDesc* dstDesc)
{
	IPhysXJoint::ConvertToDesc(srcDesc,dstDesc);

	dstDesc->xMotion=PhysXObjectConverter::convertJointMotion(srcDesc->xMotion);
	dstDesc->yMotion=PhysXObjectConverter::convertJointMotion(srcDesc->yMotion);
	dstDesc->zMotion=PhysXObjectConverter::convertJointMotion(srcDesc->zMotion);

	dstDesc->swing1Motion=PhysXObjectConverter::convertJointMotion(srcDesc->swing1Motion);
	dstDesc->swing2Motion=PhysXObjectConverter::convertJointMotion(srcDesc->swing2Motion);
	dstDesc->twistMotion=PhysXObjectConverter::convertJointMotion(srcDesc->twistMotion);

	PhysXObjectConverter::copyJointLimitSoftDesc(dstDesc->linearLimit,srcDesc->linearLimit);
	PhysXObjectConverter::copyJointLimitSoftDesc(dstDesc->swing1Limit,srcDesc->swing1Limit);
	PhysXObjectConverter::copyJointLimitSoftDesc(dstDesc->swing2Limit,srcDesc->swing2Limit);
	PhysXObjectConverter::copyLimitPairDesc(dstDesc->twistLimit,srcDesc->twistLimit);
	dstDesc->twistLimit.low.spring=dstDesc->twistLimit.high.spring=10000;


	PhysXObjectConverter::copyJointDriveDesc(dstDesc->xDrive,srcDesc->xDrive);
	PhysXObjectConverter::copyJointDriveDesc(dstDesc->yDrive,srcDesc->yDrive);
	PhysXObjectConverter::copyJointDriveDesc(dstDesc->zDrive,srcDesc->zDrive);
	PhysXObjectConverter::copyJointDriveDesc(dstDesc->swingDrive,srcDesc->swingDrive);
	PhysXObjectConverter::copyJointDriveDesc(dstDesc->twistDrive,srcDesc->twistDrive);
	PhysXObjectConverter::copyJointDriveDesc(dstDesc->slerpDrive,srcDesc->slerpDrive);
	
	dstDesc->drivePosition=ToNxVec3(srcDesc->drivePosition);
	dstDesc->driveAngularVelocity=ToNxVec3(srcDesc->driveAngularVelocity);
	dstDesc->driveOrientation.setXYZW(srcDesc->driveOrientation.x,srcDesc->driveOrientation.y,srcDesc->driveOrientation.z,srcDesc->driveOrientation.w);

	dstDesc->projectionMode=PhysXObjectConverter::convertJoinProjectionMode(srcDesc->projectionMode);
	dstDesc->projectionDistance=srcDesc->projectionDistance;
	dstDesc->projectionAngle=srcDesc->projectionAngle;
	dstDesc->gearRatio=srcDesc->gearRatio;

	dstDesc->flags=	srcDesc->slerpDriveFlag ? NX_D6JOINT_SLERP_DRIVE : 0 |
					srcDesc->gearEnabledFlag ? NX_D6JOINT_GEAR_ENABLED : 0 ;
}
void PhysXDOF6Joint::ConvertToDesc(const NxD6JointDesc* srcDesc,IPhysicalJointD6Desc* dstDesc)
{
	IPhysXJoint::ConvertToDesc(srcDesc,dstDesc);

	dstDesc->xMotion=PhysXObjectConverter::convertJointMotion(srcDesc->xMotion);
	dstDesc->yMotion=PhysXObjectConverter::convertJointMotion(srcDesc->yMotion);
	dstDesc->zMotion=PhysXObjectConverter::convertJointMotion(srcDesc->zMotion);

	dstDesc->swing1Motion=PhysXObjectConverter::convertJointMotion(srcDesc->swing1Motion);
	dstDesc->swing2Motion=PhysXObjectConverter::convertJointMotion(srcDesc->swing2Motion);
	dstDesc->twistMotion=PhysXObjectConverter::convertJointMotion(srcDesc->twistMotion);

	PhysXObjectConverter::copyJointLimitSoftDesc(dstDesc->linearLimit,srcDesc->linearLimit);
	PhysXObjectConverter::copyJointLimitSoftDesc(dstDesc->swing1Limit,srcDesc->swing1Limit);
	PhysXObjectConverter::copyJointLimitSoftDesc(dstDesc->swing2Limit,srcDesc->swing2Limit);
	PhysXObjectConverter::copyLimitPairDesc(dstDesc->twistLimit,srcDesc->twistLimit);

	PhysXObjectConverter::copyJointDriveDesc(dstDesc->xDrive,srcDesc->xDrive);
	PhysXObjectConverter::copyJointDriveDesc(dstDesc->yDrive,srcDesc->yDrive);
	PhysXObjectConverter::copyJointDriveDesc(dstDesc->zDrive,srcDesc->zDrive);
	PhysXObjectConverter::copyJointDriveDesc(dstDesc->swingDrive,srcDesc->swingDrive);
	PhysXObjectConverter::copyJointDriveDesc(dstDesc->twistDrive,srcDesc->twistDrive);
	PhysXObjectConverter::copyJointDriveDesc(dstDesc->slerpDrive,srcDesc->slerpDrive);

	dstDesc->drivePosition=ToVec3(srcDesc->drivePosition);
	dstDesc->driveAngularVelocity=ToVec3(srcDesc->driveAngularVelocity);
	dstDesc->driveOrientation.x=srcDesc->driveOrientation.x;
	dstDesc->driveOrientation.y=srcDesc->driveOrientation.y;
	dstDesc->driveOrientation.z=srcDesc->driveOrientation.z;
	dstDesc->driveOrientation.w=srcDesc->driveOrientation.w;

	dstDesc->projectionMode=PhysXObjectConverter::convertJoinProjectionMode(srcDesc->projectionMode);
	dstDesc->projectionDistance=srcDesc->projectionDistance;
	dstDesc->projectionAngle=srcDesc->projectionAngle;
	dstDesc->gearRatio=srcDesc->gearRatio;

	dstDesc->slerpDriveFlag= srcDesc->flags & NX_D6JOINT_SLERP_DRIVE ;
	dstDesc->gearEnabledFlag= srcDesc->flags & NX_D6JOINT_GEAR_ENABLED ;
}

}
}





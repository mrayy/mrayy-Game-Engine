#include "stdafx.h"



#include <NxRevoluteJoint.h>
#include "PhysXManager.h"
#include "physXCommon.h"
#include "CPhysXNode.h"
#include "NxRevoluteJointDesc.h"
#include "NxRevoluteJoint.h"

#include "PhysXRevoluteJoint.h"
#include "PhysXObjectConverter.h"

namespace mray{
namespace physics{


PhysXRevoluteJoint::PhysXRevoluteJoint(IPhysicManager*c,NxJoint*joint):IRevoluteJoint3D(c),IPhysXJoint(joint)
{
	m_joint=joint;
}

PhysXRevoluteJoint::~PhysXRevoluteJoint(){
}

IPhysicalNode* PhysXRevoluteJoint::getNode1(){


	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!a)return 0;
	return (IPhysicalNode*)a->userData;
}
IPhysicalNode* PhysXRevoluteJoint::getNode2(){

	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!b)return 0;
	return (IPhysicalNode*)b->userData;
}

//Sets the point where the two nodes are attached, specified in global coordinates.
void PhysXRevoluteJoint::setGlobalAnchor(const math::vector3d &v){
	m_joint->setGlobalAnchor(ToNxVec3(v));
}


math::vector3d PhysXRevoluteJoint::getGlobalAnchor(){
	NxVec3 vec=m_joint->getGlobalAnchor();
	return ToVec3(vec);
}

//Sets the direction of the joint's primary axis, specified in global coordinates.
void PhysXRevoluteJoint::setGlobalAxis(const math::vector3d &v){
	m_joint->setGlobalAxis(ToNxVec3(v));
}

math::vector3d PhysXRevoluteJoint::getGlobalAxis(){
	NxVec3 vec=m_joint->getGlobalAxis();
	return ToVec3(vec);
}

EJoint3DState PhysXRevoluteJoint::getJointState(){
	NxJointState s=m_joint->getState();
	if(s==NX_JS_UNBOUND)
		return EJ3S_Unbound;
	if(s==NX_JS_SIMULATING)
		return EJ3S_Simulating;
	return EJ3S_Broken;
}

void PhysXRevoluteJoint::setBreakable(float maxForce,float maxTorque){
	m_joint->setBreakable(maxForce,maxTorque);
}

// Sets the solver extrapolation factor.
void PhysXRevoluteJoint::setSolverExtrapolationFactor(float f){
	m_joint->setSolverExtrapolationFactor(f);
}

float PhysXRevoluteJoint::getSolverExtrapolationFactor(){
	return m_joint->getSolverExtrapolationFactor();
}

void PhysXRevoluteJoint::setUseAccelerationSpring(bool u){
	m_joint->setUseAccelerationSpring(u);
}
bool PhysXRevoluteJoint::getUseAccelerationSpring(){
	return m_joint->getUseAccelerationSpring();
}

void PhysXRevoluteJoint::setLimitPoint(const math::vector3d&p,bool pointOnNode2){
	m_joint->setLimitPoint(ToNxVec3(p),pointOnNode2);
}

// returns true if point is on node2
bool PhysXRevoluteJoint::getLimitPoint(math::vector3d&p){
	return m_joint->getLimitPoint(ToNxVec3(p));
}


bool PhysXRevoluteJoint::addLimitPlane(const math::vector3d& normal, const math::vector3d & pointInPlane, 
	float restitution ) 
{
	return m_joint->addLimitPlane(ToNxVec3(normal),ToNxVec3(pointInPlane),restitution);
}

void PhysXRevoluteJoint::clearLimitPlanes(){
	m_joint->purgeLimitPlanes();
}



bool PhysXRevoluteJoint::getLimit(PhysicalJointLimitPairDesc&d){
	NxJointLimitPairDesc pair;
	bool r=((NxRevoluteJoint*)m_joint)->getLimits(pair);
	PhysXObjectConverter::copyLimitPairDesc(d,pair);
	return r;
	
}

void PhysXRevoluteJoint::setMotor(const PhysicalMotor3DDesc&d){
	NxMotorDesc desc;
	PhysXObjectConverter::copyMotorDesc(desc,d);
	((NxRevoluteJoint*)m_joint)->setMotor(desc);
}
bool PhysXRevoluteJoint::getMotor(PhysicalMotor3DDesc&d){
	NxMotorDesc desc;
	bool r=((NxRevoluteJoint*)m_joint)->getMotor(desc);
	PhysXObjectConverter::copyMotorDesc(d,desc);
	return r;
}

void PhysXRevoluteJoint::setSpring(const PhysicalSpringDesc&d){
	NxSpringDesc desc;
	PhysXObjectConverter::copySpringDesc(desc,d);
	((NxRevoluteJoint*)m_joint)->setSpring(desc);
}
bool PhysXRevoluteJoint::getSpring(PhysicalSpringDesc&d){
	NxSpringDesc desc;
	bool r=((NxRevoluteJoint*)m_joint)->getSpring(desc);
	PhysXObjectConverter::copySpringDesc(d,desc);
	return r;
}

float PhysXRevoluteJoint::getAngle(){
	return ((NxRevoluteJoint*)m_joint)->getAngle();
}
float PhysXRevoluteJoint::getVelocity(){
	return ((NxRevoluteJoint*)m_joint)->getVelocity();
}

void PhysXRevoluteJoint::setProjectionMode(EJointProjectionMode m){
	switch (m)
	{
	case EJPM_None:((NxRevoluteJoint*)m_joint)->setProjectionMode(NX_JPM_NONE);break;
	case EJPM_LinearMindist:((NxRevoluteJoint*)m_joint)->setProjectionMode(NX_JPM_LINEAR_MINDIST);break;
	case EJPM_PointMindist:((NxRevoluteJoint*)m_joint)->setProjectionMode(NX_JPM_POINT_MINDIST);break;
	}
}
EJointProjectionMode PhysXRevoluteJoint::getProjectionMode(){
	NxJointProjectionMode m= ((NxRevoluteJoint*)m_joint)->getProjectionMode();
	switch (m)
	{
	case NX_JPM_LINEAR_MINDIST:
		return EJPM_LinearMindist;
	case NX_JPM_POINT_MINDIST:
		return EJPM_PointMindist;
	}
	return EJPM_None;
}


void PhysXRevoluteJoint::SaveToDesc(IPhysicalJointRevoluteDesc* desc)
{
	if(!m_joint)return;
	NxRevoluteJointDesc d;

	((NxRevoluteJoint*)m_joint)->saveToDesc(d);
	ConvertToDesc(&d,desc);
}
void PhysXRevoluteJoint::LoadFromDesc(const IPhysicalJointRevoluteDesc* desc)
{
	if(!m_joint)return;
	NxRevoluteJointDesc d;
	ConvertToDesc(desc,&d);

	((NxRevoluteJoint*)m_joint)->loadFromDesc(d);
}
void PhysXRevoluteJoint::ConvertToDesc(const IPhysicalJointRevoluteDesc* srcDesc,NxRevoluteJointDesc* dstDesc)
{
	IPhysXJoint::ConvertToDesc(srcDesc,dstDesc);
	PhysXObjectConverter::copyLimitPairDesc(dstDesc->limit,srcDesc->limit);

	PhysXObjectConverter::copySpringDesc(dstDesc->spring,srcDesc->spring);


	dstDesc->flags=NX_RJF_LIMIT_ENABLED * srcDesc->limitEnable | 
		NX_RJF_MOTOR_ENABLED * srcDesc->motorEnable |
		NX_RJF_SPRING_ENABLED * srcDesc->springEnable;

	dstDesc->projectionDistance=srcDesc->projectionDistance;
	dstDesc->projectionAngle=srcDesc->projectionAngle;
}
void PhysXRevoluteJoint::ConvertToDesc(const NxRevoluteJointDesc* srcDesc,IPhysicalJointRevoluteDesc* dstDesc)
{
	IPhysXJoint::ConvertToDesc(srcDesc,dstDesc);
	PhysXObjectConverter::copyLimitPairDesc(dstDesc->limit,srcDesc->limit);

	PhysXObjectConverter::copySpringDesc(dstDesc->spring,srcDesc->spring);


	dstDesc->limitEnable=NX_RJF_LIMIT_ENABLED & srcDesc->flags ;
	dstDesc->motorEnable=NX_RJF_MOTOR_ENABLED & srcDesc->flags ;
	dstDesc->springEnable=NX_RJF_SPRING_ENABLED & srcDesc->flags ;

	dstDesc->projectionDistance=srcDesc->projectionDistance;
	dstDesc->projectionAngle=srcDesc->projectionAngle;
}

}
}





#include "stdafx.h"

#include <NxJoint.h>
#include "PhysXManager.h"
#include "physXCommon.h"
#include "CPhysXNode.h"

#include "PhysXDistanceJoint.h"
#include "NxDistanceJointDesc.h"
#include "NxDistanceJoint.h"
#include "PhysXObjectConverter.h"
#include <NxActor.h>


namespace mray{
namespace physics{


PhysXDistanceJoint::PhysXDistanceJoint(IPhysicManager*c,NxJoint*joint):IDistanceJoint3D(c),IPhysXJoint(joint)
{
	m_joint=joint;
}

PhysXDistanceJoint::~PhysXDistanceJoint(){
}

IPhysicalNode* PhysXDistanceJoint::getNode1(){


	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!a)return 0;
	return (IPhysicalNode*)a->userData;
}
IPhysicalNode* PhysXDistanceJoint::getNode2(){

	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!b)return 0;
	return (IPhysicalNode*)b->userData;
}

//Sets the point where the two nodes are attached, specified in global coordinates.
void PhysXDistanceJoint::setGlobalAnchor(const math::vector3d &v){
	m_joint->setGlobalAnchor(ToNxVec3(v));
}


math::vector3d PhysXDistanceJoint::getGlobalAnchor(){
	NxVec3 vec=m_joint->getGlobalAnchor();
	return ToVec3(vec);
}

//Sets the direction of the joint's primary axis, specified in global coordinates.
void PhysXDistanceJoint::setGlobalAxis(const math::vector3d &v){
	m_joint->setGlobalAxis(ToNxVec3(v));
}

math::vector3d PhysXDistanceJoint::getGlobalAxis(){
	NxVec3 vec=m_joint->getGlobalAxis();
	return ToVec3(vec);
}

EJoint3DState PhysXDistanceJoint::getJointState(){
	NxJointState s=m_joint->getState();
	if(s==NX_JS_UNBOUND)
		return EJ3S_Unbound;
	if(s==NX_JS_SIMULATING)
		return EJ3S_Simulating;
	return EJ3S_Broken;
}

void PhysXDistanceJoint::setBreakable(float maxForce,float maxTorque){
	m_joint->setBreakable(maxForce,maxTorque);
}

// Sets the solver extrapolation factor.
void PhysXDistanceJoint::setSolverExtrapolationFactor(float f){
	m_joint->setSolverExtrapolationFactor(f);
}

float PhysXDistanceJoint::getSolverExtrapolationFactor(){
	return m_joint->getSolverExtrapolationFactor();
}

void PhysXDistanceJoint::setUseAccelerationSpring(bool u){
	m_joint->setUseAccelerationSpring(u);
}
bool PhysXDistanceJoint::getUseAccelerationSpring(){
	return m_joint->getUseAccelerationSpring();
}

void PhysXDistanceJoint::setLimitPoint(const math::vector3d&p,bool pointOnNode2){
	m_joint->setLimitPoint(ToNxVec3(p),pointOnNode2);
}

// returns true if point is on node2
bool PhysXDistanceJoint::getLimitPoint(math::vector3d&p){
	return m_joint->getLimitPoint(ToNxVec3(p));
}


bool PhysXDistanceJoint::addLimitPlane(const math::vector3d& normal, const math::vector3d & pointInPlane, 
	float restitution ) 
{
	return m_joint->addLimitPlane(ToNxVec3(normal),ToNxVec3(pointInPlane),restitution);
}

void PhysXDistanceJoint::clearLimitPlanes(){
	m_joint->purgeLimitPlanes();
}
void PhysXDistanceJoint::ConvertToDesc(const IPhysicalJointDistanceDesc* srcDesc,NxDistanceJointDesc* dstDesc)
{
	IPhysXJoint::ConvertToDesc(srcDesc,dstDesc);

	dstDesc->minDistance=srcDesc->minDistance;
	dstDesc->maxDistance=srcDesc->maxDistance;

	dstDesc->flags=	srcDesc->maxDistanceEnable	? NX_DJF_MAX_DISTANCE_ENABLED : 0 | 
					srcDesc->minDistanceEnable	? NX_DJF_MIN_DISTANCE_ENABLED : 0 |
					srcDesc->springEnable		? NX_DJF_SPRING_ENABLED : 0;

	PhysXObjectConverter::copySpringDesc(dstDesc->spring,srcDesc->spring);

}
void PhysXDistanceJoint::ConvertToDesc(const NxDistanceJointDesc* srcDesc,IPhysicalJointDistanceDesc* dstDesc)
{
	IPhysXJoint::ConvertToDesc(srcDesc,dstDesc);

	dstDesc->minDistance=srcDesc->minDistance;
	dstDesc->maxDistance=srcDesc->maxDistance;

	dstDesc->maxDistanceEnable = srcDesc->flags & NX_DJF_MAX_DISTANCE_ENABLED ;
	dstDesc->springEnable = srcDesc->flags & NX_DJF_SPRING_ENABLED ;

	PhysXObjectConverter::copySpringDesc(dstDesc->spring,srcDesc->spring);

}

void PhysXDistanceJoint::SaveToDesc(IPhysicalJointDistanceDesc* desc)
{
	if(!m_joint)return;
	NxDistanceJointDesc d;

	((NxDistanceJoint*)m_joint)->saveToDesc(d);
	ConvertToDesc(&d,desc);
}
void PhysXDistanceJoint::LoadFromDesc(const IPhysicalJointDistanceDesc* desc)
{
	if(!m_joint)return;
	NxDistanceJointDesc d;
	ConvertToDesc(desc,&d);

	((NxDistanceJoint*)m_joint)->loadFromDesc(d);
}


}
}


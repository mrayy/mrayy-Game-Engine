#include "stdafx.h"

#include <NxSphericalJoint.h>
#include "PhysXManager.h"
#include "physXCommon.h"
#include "CPhysXNode.h"

#include "PhysXSphericalJoint3D.h"
#include "NxSphericalJointDesc.h"
#include "PhysXObjectConverter.h"


namespace mray{
namespace physics{


PhysXSphericalJoint3D::PhysXSphericalJoint3D(IPhysicManager*c,NxJoint*joint):ISphericalJoint3D(c),IPhysXJoint(joint)
{
	m_joint=joint;
}

PhysXSphericalJoint3D::~PhysXSphericalJoint3D(){
}
IPhysicalNode* PhysXSphericalJoint3D::getNode1(){


	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!a)return 0;
	return (IPhysicalNode*)a->userData;
}
IPhysicalNode* PhysXSphericalJoint3D::getNode2(){

	NxActor*a,*b;

	m_joint->getActors(&a,&b);

	if(!b)return 0;
	return (IPhysicalNode*)b->userData;
}

//Sets the point where the two nodes are attached, specified in global coordinates.
void PhysXSphericalJoint3D::setGlobalAnchor(const math::vector3d &v){
	m_joint->setGlobalAnchor(ToNxVec3(v));
}


math::vector3d PhysXSphericalJoint3D::getGlobalAnchor(){
	NxVec3 vec=m_joint->getGlobalAnchor();
	return ToVec3(vec);
}

//Sets the direction of the joint's primary axis, specified in global coordinates.
void PhysXSphericalJoint3D::setGlobalAxis(const math::vector3d &v){
	m_joint->setGlobalAxis(ToNxVec3(v));
}

math::vector3d PhysXSphericalJoint3D::getGlobalAxis(){
	NxVec3 vec=m_joint->getGlobalAxis();
	return ToVec3(vec);
}

EJoint3DState PhysXSphericalJoint3D::getJointState(){
	NxJointState s=m_joint->getState();
	if(s==NX_JS_UNBOUND)
		return EJ3S_Unbound;
	if(s==NX_JS_SIMULATING)
		return EJ3S_Simulating;
	return EJ3S_Broken;
}

void PhysXSphericalJoint3D::setBreakable(float maxForce,float maxTorque){
	m_joint->setBreakable(maxForce,maxTorque);
}

// Sets the solver extrapolation factor.
void PhysXSphericalJoint3D::setSolverExtrapolationFactor(float f){
	m_joint->setSolverExtrapolationFactor(f);
}

float PhysXSphericalJoint3D::getSolverExtrapolationFactor(){
	return m_joint->getSolverExtrapolationFactor();
}

void PhysXSphericalJoint3D::setUseAccelerationSpring(bool u){
	m_joint->setUseAccelerationSpring(u);
}
bool PhysXSphericalJoint3D::getUseAccelerationSpring(){
	return m_joint->getUseAccelerationSpring();
}

void PhysXSphericalJoint3D::setLimitPoint(const math::vector3d&p,bool pointOnNode2){
	m_joint->setLimitPoint(ToNxVec3(p),pointOnNode2);
}

// returns true if point is on node2
bool PhysXSphericalJoint3D::getLimitPoint(math::vector3d&p){
	return m_joint->getLimitPoint(ToNxVec3(p));
}


bool PhysXSphericalJoint3D::addLimitPlane(const math::vector3d& normal, const math::vector3d & pointInPlane, 
	float restitution ) 
{
	return m_joint->addLimitPlane(ToNxVec3(normal),ToNxVec3(pointInPlane),restitution);
}

void PhysXSphericalJoint3D::clearLimitPlanes(){
	m_joint->purgeLimitPlanes();
}

//////////////////////////////////////////////////////////////////////////

void PhysXSphericalJoint3D::setProjectionMode(EJointProjectionMode m){

	NxJointProjectionMode mx=PhysXObjectConverter::convertJoinProjectionMode(m);
	((NxSphericalJoint*) m_joint)->setProjectionMode(mx);
}
EJointProjectionMode PhysXSphericalJoint3D::getProjectionMode(){

	NxJointProjectionMode mx=((NxSphericalJoint*) m_joint)->getProjectionMode();
	return PhysXObjectConverter::convertJoinProjectionMode(mx);
}


void PhysXSphericalJoint3D::ConvertToDesc(const IPhysicalJointSphericalDesc* srcDesc,NxSphericalJointDesc* dstDesc)
{
	IPhysXJoint::ConvertToDesc(srcDesc,dstDesc);

	dstDesc->swingAxis=ToNxVec3(srcDesc->swingAxis);
	dstDesc->projectionDistance=srcDesc->projectionDistance;

	PhysXObjectConverter::copyLimitPairDesc(dstDesc->twistLimit,srcDesc->twistLimit);
	PhysXObjectConverter::copyLimitDesc(dstDesc->swingLimit,srcDesc->swingLimit);

	PhysXObjectConverter::copySpringDesc(dstDesc->twistSpring,srcDesc->twistSpring);
	PhysXObjectConverter::copySpringDesc(dstDesc->swingSpring,srcDesc->swingSpring);
	PhysXObjectConverter::copySpringDesc(dstDesc->jointSpring,srcDesc->jointSpring);


	dstDesc->projectionMode=PhysXObjectConverter::convertJoinProjectionMode(srcDesc->projectionMode);

}
void PhysXSphericalJoint3D::ConvertToDesc(const NxSphericalJointDesc* srcDesc,IPhysicalJointSphericalDesc* dstDesc)
{
	IPhysXJoint::ConvertToDesc(srcDesc,dstDesc);

	dstDesc->swingAxis=ToVec3(srcDesc->swingAxis);
	dstDesc->projectionDistance=srcDesc->projectionDistance;

	PhysXObjectConverter::copyLimitPairDesc(dstDesc->twistLimit,srcDesc->twistLimit);
	PhysXObjectConverter::copyLimitDesc(dstDesc->swingLimit,srcDesc->swingLimit);

	PhysXObjectConverter::copySpringDesc(dstDesc->twistSpring,srcDesc->twistSpring);
	PhysXObjectConverter::copySpringDesc(dstDesc->swingSpring,srcDesc->swingSpring);
	PhysXObjectConverter::copySpringDesc(dstDesc->jointSpring,srcDesc->jointSpring);


	dstDesc->projectionMode=PhysXObjectConverter::convertJoinProjectionMode(srcDesc->projectionMode);

}

void PhysXSphericalJoint3D::SaveToDesc(IPhysicalJointSphericalDesc* desc)
{
	if(!m_joint)return;
	NxSphericalJointDesc d;

	((NxSphericalJoint*)m_joint)->saveToDesc(d);
	ConvertToDesc(&d,desc);
}
void PhysXSphericalJoint3D::LoadFromDesc(const IPhysicalJointSphericalDesc* desc)
{
	if(!m_joint)return;
	NxSphericalJointDesc d;
	ConvertToDesc(desc,&d);

	((NxSphericalJoint*)m_joint)->loadFromDesc(d);
}

}
}

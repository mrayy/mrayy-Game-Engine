#include "stdafx.h"

#include "CPhysXNode.h"
#include "matrix4x4.h"
#include "quaternion.h"
#include <NxPhysics.h>
#include "physXCommon.h"

#include "PhysXShape.h"
#include "PhysXWheelShape.h"
#include "PhysXManager.h"
#include "PhysXObjectConverter.h"

namespace mray{
namespace physics{


CPhysXNode::CPhysXNode( NxActor* actor,IPhysicManager*creator)
:IPhysicalNode(creator),m_actor(0)
{
	core::char_to_string(actor->getName(),m_name);
	setActor(actor);
}
CPhysXNode::~CPhysXNode()
{
	for(int i=0;i<m_shapes.size();++i)
		delete m_shapes[i];
}


PhysXShape* CPhysXNode::_AddShape(NxShape*s)
{
	PhysXShape* ps;
	if(s->getType()==NX_SHAPE_WHEEL)
		ps=(new PhysXWheelShape((NxWheelShape*)s,this));
	else
		ps=new PhysXShape(s,this);
	m_shapes.push_back(ps);
	ps->SetUserData(s->userData);
	s->userData=ps;
	return ps;
}
bool CPhysXNode::SetName(const core::string&n)
{
	m_name=n;
	m_actor->setName(string_to_char(m_name).c_str());
	return true;
}

void CPhysXNode::setActor(NxActor*d)
{
	m_actor=d;
	if(m_actor)
	{
		m_shapes.resize(0);
		int cnt=d->getNbShapes();
		for (int i=0;i<cnt;++i)
		{
			_AddShape(d->getShapes()[i]);
		}
	}
}

IPhysicalShape* CPhysXNode::GetShape(int i)
{
	if(i>=m_shapes.size())
		return 0;
	return m_shapes[i];
}
IPhysicalShape* CPhysXNode::GetShapeByName(const core::string& name)
{
	for(int i=0;i<m_shapes.size();++i)
	{
		if(m_shapes[i]->getName()==name)
			return m_shapes[i];
	}
	return 0;
}

IPhysicalShape* CPhysXNode::CreateShape(IPhysicalShapeDesc*desc)
{
	
	NxShapeDesc* d=PhysXObjectConverter::ConvertShapeDesc(desc,(PhysXManager*)m_creator);
	if(!d)
		return 0;
	NxShape*s= m_actor->createShape(*d);
	delete d;
	if(!s)
		return 0;
	return _AddShape(s);
}



void CPhysXNode::moveGlobalPosition(const math::vector3d&pos)
{
	m_actor->moveGlobalPosition(ToNxVec3(pos));
}
void CPhysXNode::moveGlobalOrientation(const math::quaternion&or)
{
	NxQuat q;
	q.setWXYZ(or.w,or.x,or.y,or.z);
	m_actor->moveGlobalOrientationQuat(q);
}

bool CPhysXNode::setGlobalPosition(const math::vector3d&pos){
	m_actor->setGlobalPosition(ToNxVec3(pos));
	return true;
}


math::vector3d CPhysXNode::getGlobalPosition()const
{
	return ToVec3(m_actor->getGlobalPosition());
}

bool CPhysXNode::setGlobalOrintation(const math::quaternion&or){
	NxQuat q;
	q.setWXYZ(or.w,or.x,or.y,or.z);
	m_actor->setGlobalOrientationQuat(q);
	return true;
}
math::quaternion CPhysXNode::getGlobalOrintation()const
{/*
	NxMat33 orient;
	math::matrix4x4 mat;
	orient=m_actor->getGlobalOrientation();
	orient.getRowMajorStride4(mat.pointer());
	//clear the elements we don't need:
	mat.f41= mat.f42= mat.f43 = 0.0f;
	mat.f44 = 1.0f;*/

	NxQuat qq=m_actor->getGlobalOrientationQuat();
	//math::quaternion q(mat);
	return math::quaternion(qq.w,qq.x,qq.y,qq.z);/*
	
	NxQuat q=m_actor->getGlobalOrientationQuat();
	return math::quaternion(q.x,q.y,q.z,q.w);*/
}


int CPhysXNode::getNumberOfShapes(){
	return m_actor->getNbShapes();
}

void CPhysXNode::SetBodyFlag(EBodyFlag f,bool enable)
{
	NxBodyFlag pf;
	switch (f)
	{
	case EBF_DisableGravity:pf=NX_BF_DISABLE_GRAVITY;break;

	case EBF_Frozen_PosX:pf=NX_BF_FROZEN_POS_X;break;
	case EBF_Frozen_PosY:pf=NX_BF_FROZEN_POS_Y;break;
	case EBF_Frozen_PosZ:pf=NX_BF_FROZEN_POS_Z;break;

	case EBF_Frozen_RotX:pf=NX_BF_FROZEN_ROT_X;break;
	case EBF_Frozen_RotY:pf=NX_BF_FROZEN_ROT_Y;break;
	case EBF_Frozen_RotZ:pf=NX_BF_FROZEN_ROT_Z;break;

	case EBF_Frozen_Pos:pf=NX_BF_FROZEN_POS;break;
	case EBF_Frozen_Rot:pf=NX_BF_FROZEN_ROT;break;

	case EBF_Frozen:pf=NX_BF_FROZEN;break;

	case EBF_Kinematic:pf=NX_BF_KINEMATIC;break;

	case EBF_FilterSleepVel:pf=NX_BF_FILTER_SLEEP_VEL;break;

	case EBF_EnergySleepTest:pf=NX_BF_ENERGY_SLEEP_TEST;break;
	default:
		return;
	}
	if(enable)
		m_actor->raiseBodyFlag(pf);
	else
		m_actor->clearBodyFlag(pf);
}
bool CPhysXNode::GetBodyFlag(EBodyFlag f)
{
	NxBodyFlag pf;
	switch (f)
	{
	case EBF_DisableGravity:pf=NX_BF_DISABLE_GRAVITY;break;

	case EBF_Frozen_PosX:pf=NX_BF_FROZEN_POS_X;break;
	case EBF_Frozen_PosY:pf=NX_BF_FROZEN_POS_Y;break;
	case EBF_Frozen_PosZ:pf=NX_BF_FROZEN_POS_Z;break;

	case EBF_Frozen_RotX:pf=NX_BF_FROZEN_ROT_X;break;
	case EBF_Frozen_RotY:pf=NX_BF_FROZEN_ROT_Y;break;
	case EBF_Frozen_RotZ:pf=NX_BF_FROZEN_ROT_Z;break;

	case EBF_Frozen_Pos:pf=NX_BF_FROZEN_POS;break;
	case EBF_Frozen_Rot:pf=NX_BF_FROZEN_ROT;break;

	case EBF_Frozen:pf=NX_BF_FROZEN;break;

	case EBF_Kinematic:pf=NX_BF_KINEMATIC;break;

	case EBF_FilterSleepVel:pf=NX_BF_FILTER_SLEEP_VEL;break;

	case EBF_EnergySleepTest:pf=NX_BF_ENERGY_SLEEP_TEST;break;
	default:
		return 0;
	}
	return m_actor->readBodyFlag(pf);
}

void CPhysXNode::setMaterialIndex(int matIndex,int shape){
	if(shape>=m_actor->getNbShapes())
		return;
	NxShape *const*s=m_actor->getShapes();
	s[shape]->setMaterial(matIndex);
}
int CPhysXNode::getMaterialIndex(int shape){
	if(shape>=m_actor->getNbShapes())
		return 0;
	NxShape *const*s=m_actor->getShapes();
	return s[shape]->getMaterial();
}

bool	CPhysXNode::setMass(float m){
	m_actor->setMass(m);
	return true;
}
float	CPhysXNode::getMass()const{
	return m_actor->getMass();

}

bool CPhysXNode::setLinearDamping(float d){
	m_actor->setLinearDamping(d);
	return true;
}
float CPhysXNode::getLinearDamping()const{
	return m_actor->getLinearDamping();
}


bool CPhysXNode::setAngularDamping(float d){
	m_actor->setAngularDamping(d);
	return true;

}
float CPhysXNode::getAngularDamping()const{
	return m_actor->getAngularDamping();

}

bool CPhysXNode::isDynamic()const{
	return m_actor->isDynamic();

}

bool CPhysXNode::setLinearVelocity(const math::vector3d& v){
	m_actor->setLinearVelocity(ToNxVec3(v));
	return true;

}
bool CPhysXNode::setAngularVelocity(const math::vector3d& v){
	m_actor->setLinearVelocity(ToNxVec3(v));
	return true;

}

math::vector3d CPhysXNode::getLinearVelocity()const{
	return ToVec3(m_actor->getLinearVelocity());

}
math::vector3d CPhysXNode::getAngularVelocity()const{
	return ToVec3(m_actor->getAngularVelocity());

}

bool CPhysXNode::setMaxAngularVelocity(float m){
	m_actor->setMaxAngularVelocity(m);
	return true;

}
float CPhysXNode::getMaxAngularVelocity()const{
	return m_actor->getMaxAngularVelocity();
	return true;

}

bool CPhysXNode::setLinearMomentum(const math::vector3d& v){
	m_actor->setLinearMomentum(ToNxVec3(v));
	return true;

}
math::vector3d CPhysXNode::getLinearMomentum()const{
	return ToVec3(m_actor->getLinearMomentum());

}

bool CPhysXNode::setAngularMomentum(const math::vector3d& v){
	m_actor->setAngularMomentum(ToNxVec3(v));
	return true;

}
math::vector3d CPhysXNode::getAngularMomentum()const{
	return ToVec3(m_actor->getAngularMomentum());

}

void CPhysXNode::addForce(const math::vector3d& force, EForceMode mode , bool wakeup ) {

	NxForceMode m;
	switch(mode){
		case EFM_Force:
			m=NX_FORCE;
			break;
		case EFM_Impulse:
			m=NX_IMPULSE;
			break;
		case EFM_VelocityChange:
			m=NX_VELOCITY_CHANGE;
			break;
		case EFM_SmoothImpulse:
			m=NX_SMOOTH_IMPULSE;
			break;
		case EFM_SmoothVelocityChange:
			m=NX_SMOOTH_VELOCITY_CHANGE;
			break;
		case EFM_Acceleration:
			m=NX_ACCELERATION;
			break;
	}

	m_actor->addForce(ToNxVec3(force),m,wakeup);
}
void CPhysXNode::addLocalForce(const  math::vector3d& force, EForceMode mode , bool wakeup ) {

	NxForceMode m;
	switch(mode){
		case EFM_Force:
			m=NX_FORCE;
			break;
		case EFM_Impulse:
			m=NX_IMPULSE;
			break;
		case EFM_VelocityChange:
			m=NX_VELOCITY_CHANGE;
			break;
		case EFM_SmoothImpulse:
			m=NX_SMOOTH_IMPULSE;
			break;
		case EFM_SmoothVelocityChange:
			m=NX_SMOOTH_VELOCITY_CHANGE;
			break;
		case EFM_Acceleration:
			m=NX_ACCELERATION;
			break;
	}

	m_actor->addLocalForce(ToNxVec3(force),m,wakeup);
}

void CPhysXNode::addForceAtPos(const math::vector3d& force, const math::vector3d& pos, EForceMode mode , bool wakeup) {

	NxForceMode m;
	switch(mode){
		case EFM_Force:
			m=NX_FORCE;
			break;
		case EFM_Impulse:
			m=NX_IMPULSE;
			break;
		case EFM_VelocityChange:
			m=NX_VELOCITY_CHANGE;
			break;
		case EFM_SmoothImpulse:
			m=NX_SMOOTH_IMPULSE;
			break;
		case EFM_SmoothVelocityChange:
			m=NX_SMOOTH_VELOCITY_CHANGE;
			break;
		case EFM_Acceleration:
			m=NX_ACCELERATION;
			break;
	}

	m_actor->addForceAtPos(ToNxVec3(force),ToNxVec3(pos),m,wakeup);

}
void CPhysXNode::addForceAtLocalPos(const math::vector3d& force, const math::vector3d& pos, EForceMode mode , bool wakeup ) {

	NxForceMode m;
	switch(mode){
		case EFM_Force:
			m=NX_FORCE;
			break;
		case EFM_Impulse:
			m=NX_IMPULSE;
			break;
		case EFM_VelocityChange:
			m=NX_VELOCITY_CHANGE;
			break;
		case EFM_SmoothImpulse:
			m=NX_SMOOTH_IMPULSE;
			break;
		case EFM_SmoothVelocityChange:
			m=NX_SMOOTH_VELOCITY_CHANGE;
			break;
		case EFM_Acceleration:
			m=NX_ACCELERATION;
			break;
	}

	m_actor->addForceAtLocalPos(ToNxVec3(force),ToNxVec3(pos),m,wakeup);

}
void CPhysXNode::addLocalForceAtPos(const math::vector3d& force, const math::vector3d& pos, EForceMode mode , bool wakeup ) {

	NxForceMode m;
	switch(mode){
		case EFM_Force:
			m=NX_FORCE;
			break;
		case EFM_Impulse:
			m=NX_IMPULSE;
			break;
		case EFM_VelocityChange:
			m=NX_VELOCITY_CHANGE;
			break;
		case EFM_SmoothImpulse:
			m=NX_SMOOTH_IMPULSE;
			break;
		case EFM_SmoothVelocityChange:
			m=NX_SMOOTH_VELOCITY_CHANGE;
			break;
		case EFM_Acceleration:
			m=NX_ACCELERATION;
			break;
	}

	m_actor->addLocalForceAtPos(ToNxVec3(force),ToNxVec3(pos),m,wakeup);

}
void CPhysXNode::addLocalForceAtLocalPos(const math::vector3d& force, const math::vector3d& pos, EForceMode mode , bool wakeup ) {

	NxForceMode m;
	switch(mode){
		case EFM_Force:
			m=NX_FORCE;
			break;
		case EFM_Impulse:
			m=NX_IMPULSE;
			break;
		case EFM_VelocityChange:
			m=NX_VELOCITY_CHANGE;
			break;
		case EFM_SmoothImpulse:
			m=NX_SMOOTH_IMPULSE;
			break;
		case EFM_SmoothVelocityChange:
			m=NX_SMOOTH_VELOCITY_CHANGE;
			break;
		case EFM_Acceleration:
			m=NX_ACCELERATION;
			break;
	}

	m_actor->addLocalForceAtLocalPos(ToNxVec3(force),ToNxVec3(pos),m,wakeup);

}


void CPhysXNode::addTorque(const math::vector3d& torque, EForceMode mode , bool wakeup ) {

	NxForceMode m;
	switch(mode){
		case EFM_Force:
			m=NX_FORCE;
			break;
		case EFM_Impulse:
			m=NX_IMPULSE;
			break;
		case EFM_VelocityChange:
			m=NX_VELOCITY_CHANGE;
			break;
		case EFM_SmoothImpulse:
			m=NX_SMOOTH_IMPULSE;
			break;
		case EFM_SmoothVelocityChange:
			m=NX_SMOOTH_VELOCITY_CHANGE;
			break;
		case EFM_Acceleration:
			m=NX_ACCELERATION;
			break;
	}

	m_actor->addTorque(ToNxVec3(torque),m,wakeup);

}
void CPhysXNode::addLocalTorque(const math::vector3d& torque, EForceMode mode , bool wakeup ) {

	NxForceMode m;
	switch(mode){
		case EFM_Force:
			m=NX_FORCE;
			break;
		case EFM_Impulse:
			m=NX_IMPULSE;
			break;
		case EFM_VelocityChange:
			m=NX_VELOCITY_CHANGE;
			break;
		case EFM_SmoothImpulse:
			m=NX_SMOOTH_IMPULSE;
			break;
		case EFM_SmoothVelocityChange:
			m=NX_SMOOTH_VELOCITY_CHANGE;
			break;
		case EFM_Acceleration:
			m=NX_ACCELERATION;
			break;
	}

	m_actor->addLocalTorque(ToNxVec3(torque),m,wakeup);

}

float CPhysXNode::computeKineticEnergy() const {
	return m_actor->computeKineticEnergy();

}


math::vector3d CPhysXNode::getPointVelocity(const math::vector3d& point) const {
	return ToVec3(m_actor->getPointVelocity(ToNxVec3(point)));

}
math::vector3d CPhysXNode::getLocalPointVelocity(const math::vector3d& point) const {
	return ToVec3(m_actor->getLocalPointVelocity(ToNxVec3(point)));

}


bool CPhysXNode::isSleeping() const {
	return m_actor->isSleeping();

}
bool CPhysXNode::isGroupSleeping() const {
	return m_actor->isGroupSleeping();

}

bool CPhysXNode::setSleepLinearVelocity(float threshold) {
	m_actor->setSleepLinearVelocity(threshold);
	return true;
}
float CPhysXNode::getSleepLinearVelocity() const {
	return m_actor->getSleepLinearVelocity();

}


bool CPhysXNode::setSleepAngularVelocity(float threshold) {
	m_actor->setSleepAngularVelocity(threshold);
	return true;

}
float CPhysXNode::getSleepAngularVelocity() const {
	return m_actor->getSleepAngularVelocity();

}

bool CPhysXNode::setSleepEnergyThreshold(float threshold) {
	m_actor->setSleepEnergyThreshold(threshold);
	return true;

}
float CPhysXNode::getSleepEnergyThreshold() const {
	return m_actor->getSleepEnergyThreshold();

}

void CPhysXNode::wakeUp(float wakeCounterValue)	{
	m_actor->wakeUp(wakeCounterValue);

}
void CPhysXNode::goToSleep()	{
	m_actor->putToSleep();

}

bool CPhysXNode::SetSolverIterationCount(uint c)
{
	m_actor->setSolverIterationCount(c);
	return true;
}
uint CPhysXNode::GetSolverIterationCount()const
{
	return m_actor->getSolverIterationCount();
}

bool CPhysXNode::SetContactReportThreshold(float th)
{
	m_actor->setContactReportThreshold(th);
	return true;
}
float CPhysXNode::GetContactReportThreshold()const
{
	return m_actor->getContactReportThreshold();
}

void CPhysXNode::SetCMassOffsetLocalMat(const math::matrix4x4& m)
{
	NxMat34 mat;
	mat.setRowMajor44(m.getMatPointer());
	m_actor->setCMassOffsetLocalPose(mat);
}

void CPhysXNode::SetCMassOffsetLocalPosition(const math::vector3d&pos)
{
	m_actor->setCMassOffsetLocalPosition(ToNxVec3(pos));
}

void CPhysXNode::SetCMassOffsetLocalOrientation(const math::quaternion&or)
{
	NxMat33 m;
	NxQuat q;
	q.setXYZW(or.x,or.y,or.z,or.w);
	m.fromQuat(q);
	m_actor->setCMassOffsetLocalOrientation(m);
}


void CPhysXNode::SetCMassOffsetGlobalMat(const math::matrix4x4& m)
{
	NxMat34 mat;
	mat.setRowMajor44(m.getMatPointer());
	m_actor->setCMassOffsetGlobalPose(mat);
}

void CPhysXNode::SetCMassOffsetGlobalPosition(const math::vector3d&pos)
{
	m_actor->setCMassOffsetGlobalPosition(ToNxVec3(pos));
}

void CPhysXNode::SetCMassOffsetGlobalOrientation(const math::quaternion&or)
{
	NxMat33 m;
	NxQuat q;
	q.setXYZW(or.x,or.y,or.z,or.w);
	m.fromQuat(q);
	m_actor->setCMassOffsetGlobalOrientation(m);
}


void CPhysXNode::SetCMassGlobalMat(const math::matrix4x4& m)
{
	NxMat34 mat;
	mat.setRowMajor44(m.getMatPointer());
	m_actor->setCMassGlobalPose(mat);
}

void CPhysXNode::SetCMassGlobalPosition(const math::vector3d&pos)
{
	m_actor->setCMassGlobalPosition(ToNxVec3(pos));
}

void CPhysXNode::SetCMassGlobalOrientation(const math::quaternion&or)
{
	NxMat33 m;
	NxQuat q;
	q.setXYZW(or.x,or.y,or.z,or.w);
	m.fromQuat(q);
	m_actor->setCMassGlobalOrientation(m);
}


void CPhysXNode::GetCMassLocalMat(math::matrix4x4& m)const
{
	NxMat34 mat= m_actor->getCMassLocalPose();
	mat.getRowMajor44(m.pointer());
}

void CPhysXNode::GetCMassLocalPosition(math::vector3d& p)const
{
	NxVec3 v=m_actor->getCMassLocalPosition();
	p=ToVec3(v);
}

void CPhysXNode::GetCMassLocalOrientation(math::quaternion&or)const
{

	NxMat33 m;
	m=m_actor->getCMassGlobalOrientation();
	NxQuat q;
	m.toQuat(q);
	or=math::quaternion(q.w,q.x,q.y,q.z);
}


void CPhysXNode::GetCMassGlobalMat(math::matrix4x4& m)const
{
	NxMat34 mat= m_actor->getCMassGlobalPose();
	mat.getRowMajor44(m.pointer());
}

void CPhysXNode::GetCMassGlobalPosition(math::vector3d& p)const
{
	NxVec3 v=m_actor->getCMassGlobalPosition();
	p=ToVec3(v);
}

void CPhysXNode::GetCMassGlobalOrientation( math::quaternion&or)const
{
	NxMat33 m;
	m=m_actor->getCMassGlobalOrientation();
	NxQuat q;
	m.toQuat(q);
	or=math::quaternion(q.w,q.x,q.y,q.z);
}

void CPhysXNode::EnableContactReportFlag(EContactReportFlag flag)
{
	NxU32 flags= m_actor->getContactReportFlags();
	flags|=PhysXObjectConverter::convertContactFlag(flag);
	m_actor->setContactReportFlags(flags);
}
void CPhysXNode::DisableContactReportFlag(EContactReportFlag flag)
{
	NxU32 flags= m_actor->getContactReportFlags();
	flags&=~PhysXObjectConverter::convertContactFlag(flag);
	m_actor->setContactReportFlags(flags);
}

void CPhysXNode::SaveToDesc(PhysicalNodeDesc* desc)
{
	NxActorDesc nxDesc;
	m_actor->saveToDesc(nxDesc);
	core::char_to_string(nxDesc.name,desc->name);
	nxDesc.globalPose.getRowMajor44(desc->globalPos.mat);
	desc->density= nxDesc.density;
	desc->actorGroupID= nxDesc.group;
}

bool CPhysXNode::SaveToDesc(PhysicalBodyDesc* desc)
{
	NxBodyDesc nxDesc;
	if(!m_actor->saveBodyToDesc(nxDesc))
		return false;
	nxDesc.massLocalPose.getRowMajor44(desc->massLocalPos.mat);

	desc->massSpaceInertia=ToVec3(nxDesc.massSpaceInertia);
	desc->mass=nxDesc.mass;
	desc->linearVelocity=ToVec3(nxDesc.linearVelocity);
	desc->angularVelocity=ToVec3(nxDesc.angularVelocity);
	desc->wakeupCounter=nxDesc.wakeUpCounter;
	desc->linearDamping=nxDesc.linearDamping;
	desc->angularDamping=nxDesc.angularDamping;
	desc->maxAngularVelocity=nxDesc.maxAngularVelocity;
	desc->CCDMotionThreshold=nxDesc.CCDMotionThreshold;
	desc->sleepLinearVelocity=nxDesc.sleepLinearVelocity;
	desc->sleepAngularVelocity=nxDesc.sleepAngularVelocity;
	desc->solverIterationCount=nxDesc.solverIterationCount;
	desc->sleepEnergyThreshold=nxDesc.sleepEnergyThreshold;
	desc->sleepDamping=nxDesc.sleepDamping;
	desc->contactReportThreshold=nxDesc.contactReportThreshold;
	desc->kinematic = nxDesc.flags& NX_BF_KINEMATIC;
	desc->disableGravity = nxDesc.flags& NX_BF_DISABLE_GRAVITY;

	return true;
}

}
}
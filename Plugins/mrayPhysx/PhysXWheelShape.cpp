
#include "stdafx.h"
#include "PhysXWheelShape.h"
#include <NxWheelShape.h>
#include "PhysXObjectConverter.h"

namespace mray
{
namespace physics
{

PhysXWheelShape::PhysXWheelShape(NxWheelShape*shape,IPhysicalNode* node)
:PhysXShape(shape,node),m_wheelShape(shape)
{
}
PhysXWheelShape::~PhysXWheelShape()
{
}

void PhysXWheelShape::SetRadius(float r)
{
	m_wheelShape->setRadius(r);
}
float PhysXWheelShape::GetRadius()const
{
	return m_wheelShape->getRadius();
}

void PhysXWheelShape::SetSuspensionTravel(float v)
{
	m_wheelShape->setSuspensionTravel(v);
}
float PhysXWheelShape::GetSuspensionTravel()const
{
	return m_wheelShape->getSuspensionTravel();
}

void PhysXWheelShape::SetSuspension(const WheelShapeDesc::SuspensionSpring&s)
{
}
void PhysXWheelShape::GetSuspension(WheelShapeDesc::SuspensionSpring&s)const
{
}

void PhysXWheelShape::SetLongitudalTireForceFunction(const WheelShapeDesc::TireFunctionDesc&s)
{
}
void PhysXWheelShape::GetLongitudalTireForceFunction(WheelShapeDesc::TireFunctionDesc&s)
{
}	

void PhysXWheelShape::SetLateralTireForceFunction(const WheelShapeDesc::TireFunctionDesc&s)
{
}	
void PhysXWheelShape::GetLateralTireForceFunction(WheelShapeDesc::TireFunctionDesc&s)const
{
}	

void PhysXWheelShape::SetInverseWheelMass(float invM)
{
	m_wheelShape->setInverseWheelMass(invM);
}
float PhysXWheelShape::GetInverseWheelMass()const
{
	return m_wheelShape->getInverseWheelMass();
}

void PhysXWheelShape::SetMotorTorque(float t)
{
	m_wheelShape->setMotorTorque(t);
}
float PhysXWheelShape::GetMotorTorque()const
{
	return m_wheelShape->getMotorTorque();
}

void PhysXWheelShape::SetBrakeTorque(float t)
{
	m_wheelShape->setBrakeTorque(t);
}
float PhysXWheelShape::GetBrakeTorque()const
{
	return m_wheelShape->getBrakeTorque();
}

void PhysXWheelShape::SetSteerAngle(float t)
{
	m_wheelShape->setSteerAngle(t);
}
float PhysXWheelShape::GetSteerAngle()const
{
	return math::toDeg(m_wheelShape->getSteerAngle());
}

void PhysXWheelShape::SetAxleSpeed(float s)
{
	m_wheelShape->setAxleSpeed(s);
}
float PhysXWheelShape::GetAxleSpeed()const
{
	return m_wheelShape->getAxleSpeed();
}

IPhysicalShape* PhysXWheelShape::GetContact(WheelContactData& contact)
{
	NxWheelContactData wcd;
	NxShape*s= m_wheelShape->getContact(wcd);
	if(!s)
		return 0;
	IPhysicalShape* ps=(IPhysicalShape*) s->userData;
	NxShapeType t=s->getType();
	if(!ps)
		return 0;
 	PhysXObjectConverter::copyWheelContactData(contact,wcd);
	return ps;
}



}
}
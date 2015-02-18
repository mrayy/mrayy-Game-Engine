
#include "stdafx.h"
#include "PhysicalObject.h"
#include "IPhysicalNode.h"



namespace mray
{
namespace GameMod
{

PhysicalObject::PhysicalObject(physics::IPhysicalNode* node):m_node(node),m_maxSpeed(100)
{
}
PhysicalObject::~PhysicalObject()
{
}

float PhysicalObject::GetMass()
{
	return m_node->getMass();
}

void PhysicalObject::SetGlobalPosition(const math::vector3d&pos)
{
	m_node->setGlobalPosition(pos);
}
math::vector3d PhysicalObject::GetGlobalPosition()
{
	return m_node->getGlobalPosition();
}

void PhysicalObject::SetGlobalOrintation(const math::quaternion&or)
{
	m_node->setGlobalOrintation(or);
}
math::quaternion PhysicalObject::GetGlobalOrintation()
{
	return m_node->getGlobalOrintation();
}

void PhysicalObject::SetLinearDamping(float d)
{
	m_node->setLinearDamping(d);
}
float PhysicalObject::GetLinearDamping()
{
	return m_node->getLinearDamping();
}

void PhysicalObject::SetLinearVelocity(const math::vector3d&v)
{
	m_node->setLinearVelocity(v);
}
math::vector3d PhysicalObject::GetLinearVelocity()
{
	return m_node->getLinearVelocity();
}

void PhysicalObject::SetAngularVelocity(const math::vector3d& v)
{
	m_node->setAngularVelocity(v);
}
math::vector3d PhysicalObject::GetAngularVelocity()
{
	return m_node->getAngularVelocity();
}

void PhysicalObject::SetMaxAngularVelocity(float m)
{
	m_node->setMaxAngularVelocity(m);
}
float PhysicalObject::GetMaxAngularVelocity()const
{
	return m_node->getMaxAngularVelocity();
}

void PhysicalObject::SetMaxSpeed(float s)
{
	m_maxSpeed=s;
}
float PhysicalObject::GetMaxSpeed()const
{
	return m_maxSpeed;
}



}
}

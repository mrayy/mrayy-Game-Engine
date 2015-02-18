
#include "stdafx.h"
#include "SimplePhysics.h"


namespace mray
{
namespace GameMod
{

SimplePhysics::SimplePhysics()
{
}
SimplePhysics::~SimplePhysics()
{
}

float SimplePhysics::GetMass()
{
	return 1;
}

void SimplePhysics::Update(float dt)
{
}

void SimplePhysics::SetGlobalPosition(const math::vector3d&pos)
{
	m_position=pos;
}
math::vector3d SimplePhysics::GetGlobalPosition()
{
	return m_position;
}

void SimplePhysics::SetGlobalOrintation(const math::quaternion&or)
{
	m_ori=or;
}
math::quaternion SimplePhysics::GetGlobalOrintation()
{
	return m_ori;
}

void SimplePhysics::SetLinearDamping(float d)
{
}
float SimplePhysics::GetLinearDamping()
{
	return 0;
}

void SimplePhysics::SetLinearVelocity(const math::vector3d&v)
{
}
math::vector3d SimplePhysics::GetLinearVelocity()
{
	return math::vector3d::Zero;
}

void SimplePhysics::SetAngularVelocity(const math::vector3d& v)
{
}
math::vector3d SimplePhysics::GetAngularVelocity()
{
	return math::vector3d::Zero;
}

void SimplePhysics::SetMaxAngularVelocity(float m)
{
}
float SimplePhysics::GetMaxAngularVelocity()const
{
	return 0;
}

void SimplePhysics::SetMaxSpeed(float s)
{
}
float SimplePhysics::GetMaxSpeed()const
{
	return 0;
}


}
}

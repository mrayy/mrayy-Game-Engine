#include "stdafx.h"

#include "AIBasedPhysicControl.h"
#include "IAIPhysics.h"

namespace mray
{
namespace game
{

AIBasedPhysicControl::AIBasedPhysicControl(AI::IAIPhysics* physics):m_physics(physics)
{
}
AIBasedPhysicControl::~AIBasedPhysicControl()
{
}

float AIBasedPhysicControl::GetMass()
{
	if(!m_physics)
		return 0;
	return m_physics->GetMass();
}

void AIBasedPhysicControl::SetPosition(const math::vector3d&pos)
{
	if(!m_physics)
		return;
	m_physics->SetGlobalPosition(pos);
}
math::vector3d AIBasedPhysicControl::GetPosition()
{
	if(!m_physics)
		return math::vector3d::Zero;
	return m_physics->GetGlobalPosition();
}

void AIBasedPhysicControl::SetOrintation(const math::quaternion&or)
{
	if(!m_physics)
		return;
	m_physics->SetGlobalOrintation(or);
}
math::quaternion AIBasedPhysicControl::GetOrintation()
{
	if(!m_physics)
		return math::vector3d::Zero;
	return m_physics->GetGlobalOrintation();
}

void AIBasedPhysicControl::SetLinearVelocity(const math::vector3d&v)
{
	if(!m_physics)
		return;
	m_physics->SetLinearVelocity(v);
}
math::vector3d AIBasedPhysicControl::GetLinearVelocity()
{
	if(!m_physics)
		return math::vector3d::Zero;
	return m_physics->GetLinearVelocity();
}

void AIBasedPhysicControl::SetAngularVelocity(const math::vector3d& v)
{
	if(!m_physics)
		return;
	m_physics->SetAngularVelocity(v);
}
math::vector3d AIBasedPhysicControl::GetAngularVelocity()
{
	if(!m_physics)
		return math::vector3d::Zero;
	return m_physics->GetAngularVelocity();
}

void AIBasedPhysicControl::SetMaxAngularVelocity(float m)
{
	if(!m_physics)
		return;
	m_physics->SetMaxAngularVelocity(m);
}
float AIBasedPhysicControl::GetMaxAngularVelocity()const
{
	if(!m_physics)
		return 0;
	return m_physics->GetMaxAngularVelocity();
}

void AIBasedPhysicControl::SetMaxSpeed(float s)
{
	if(!m_physics)
		return;
	m_physics->SetMaxSpeed(s);
}
float AIBasedPhysicControl::GetMaxSpeed()const
{
	if(!m_physics)
		return 0;
	return m_physics->GetMaxSpeed();
}	

math::vector3d AIBasedPhysicControl::GetUp()
{
	if(!m_physics)
		return math::vector3d::Zero;
	return m_physics->GetUp();
}
math::vector3d AIBasedPhysicControl::GetHeading()
{
	if(!m_physics)
		return math::vector3d::Zero;
	return m_physics->GetHeading();
}


}
}
#include "stdafx.h"

#include "AIPhysicsProxy.h"



namespace mray
{
namespace AI
{

AIPhysicsProxy::AIPhysicsProxy(IAIPhysics*ph):m_physics(ph)
{
}
AIPhysicsProxy::~AIPhysicsProxy()
{
	delete m_physics;
}
void AIPhysicsProxy::SetPhysics(IAIPhysics*ph)
{
	m_physics=ph;
}
IAIPhysics* AIPhysicsProxy::GetPhysics()
{
	return m_physics;
}
void AIPhysicsProxy::Update(float dt)
{
	if(m_physics)
		m_physics->Update(dt);
}
void AIPhysicsProxy::Visualize(IRenderDevice*dev)
{
	if(m_physics)
		m_physics->Visualize(dev);
}

float AIPhysicsProxy::GetMass()
{
	if(m_physics)
		return m_physics->GetMass();
	return 0;
}

void AIPhysicsProxy::SetGlobalPosition(const math::vector3d&pos)
{
	if(m_physics)
		m_physics->SetGlobalPosition(pos);
}
math::vector3d AIPhysicsProxy::GetGlobalPosition()
{
	if(m_physics)
		return m_physics->GetGlobalPosition();
	return math::vector3d::Zero;
}

void AIPhysicsProxy::SetGlobalOrintation(const math::quaternion&or)
{
	if(m_physics)
		m_physics->SetGlobalOrintation(or);
}
math::quaternion AIPhysicsProxy::GetGlobalOrintation()
{
	if(m_physics)
		return m_physics->GetGlobalOrintation();
	return math::quaternion::Identity;
}

void AIPhysicsProxy::SetLinearDamping(float d)
{
	if(m_physics)
		m_physics->SetLinearDamping(d);
}
float AIPhysicsProxy::GetLinearDamping()
{
	if(m_physics)
		return m_physics->GetLinearDamping();
	return 0;
}

void AIPhysicsProxy::SetLinearVelocity(const math::vector3d&v)
{
	if(m_physics)
		m_physics->SetLinearVelocity(v);
}
math::vector3d AIPhysicsProxy::GetLinearVelocity()
{
	if(m_physics)
		return m_physics->GetLinearVelocity();
	return math::vector3d::Zero;
}

void AIPhysicsProxy::SetAngularVelocity(const math::vector3d& v)
{
	if(m_physics)
		m_physics->SetAngularVelocity(v);
}
math::vector3d AIPhysicsProxy::GetAngularVelocity()
{
	if(m_physics)
		return m_physics->GetAngularVelocity();
	return math::vector3d::Zero;
}

void AIPhysicsProxy::SetMaxAngularVelocity(float m)
{
	if(m_physics)
		m_physics->SetMaxAngularVelocity(m);
}
float AIPhysicsProxy::GetMaxAngularVelocity()const
{
	if(m_physics)
		return m_physics->GetMaxAngularVelocity();
	return 0;
}

void AIPhysicsProxy::SetMaxSpeed(float s)
{
	if(m_physics)
		m_physics->SetMaxSpeed(s);
}
float AIPhysicsProxy::GetMaxSpeed()const
{
	if(m_physics)
		return m_physics->GetMaxSpeed();
	return 0;
}

math::vector3d AIPhysicsProxy::GetUp()
{
	if(m_physics)
		return m_physics->GetUp();
	return math::vector3d::Zero;
}
math::vector3d AIPhysicsProxy::GetHeading()
{
	if(m_physics)
		return m_physics->GetHeading();
	return math::vector3d::Zero;
}


}
}



#include "stdafx.h"
#include "AgentPhysics.h"

namespace mray{
namespace AI{

AgentPhysics::AgentPhysics(physics::ICharacterController*phNode)
{
	m_phNode=phNode;
	SetMaxAngularVelocity(180);
}
AgentPhysics::~AgentPhysics()
{
}

float AgentPhysics::GetMass()
{
	return 1;//m_phNode->getActor()->getMass();
}

void AgentPhysics::Update(float dt)
{
	m_ori*=m_angularVeloctity*dt;
}

void AgentPhysics::SetGlobalPosition(const math::vector3d&pos)
{
	m_phNode->setPosition(pos);
}
math::vector3d AgentPhysics::GetGlobalPosition()
{
	//math::vector3d p= m_phNode->getPosition();
	math::vector3d p2=m_phNode->getActor()->getGlobalPosition();
	//p2.y=0;
	return p2;
}

void AgentPhysics::SetGlobalOrintation(const math::quaternion&or)
{
	m_ori=or;//m_phNode->getActor()->setGlobalOrintation(or);
}
math::quaternion AgentPhysics::GetGlobalOrintation()
{
	return m_ori;//m_phNode->getActor()->getGlobalOrintation();
}

void AgentPhysics::SetLinearDamping(float d)
{
	m_phNode->getActor()->setLinearDamping(d);
}
float AgentPhysics::GetLinearDamping()
{
	return m_phNode->getActor()->getLinearDamping();
}

void AgentPhysics::SetLinearVelocity(const math::vector3d&v)
{
	math::vector3d vel(v.x,0,v.z);
	float len=vel.LengthSQ();
	if(len>GetMaxSpeed()*GetMaxSpeed())
	{
		vel.Normalize();
		vel*=GetMaxSpeed();
	}
	vel.y=v.y;
	m_phNode->setMovingDir(vel);
}
math::vector3d AgentPhysics::GetLinearVelocity()
{
	return m_phNode->getActor()->getLinearVelocity();
}

void AgentPhysics::SetAngularVelocity(const math::vector3d& v)
{
	//m_phNode->getActor()->setAngularVelocity(v);
	m_angularVeloctity=v;
	float len=m_angularVeloctity.LengthSQ();
	if(len>GetMaxAngularVelocity()*GetMaxAngularVelocity())
	{
		m_angularVeloctity.Normalize();
		m_angularVeloctity*=GetMaxAngularVelocity();
	}
	m_oriVel=math::quaternion(m_angularVeloctity);
}
math::vector3d AgentPhysics::GetAngularVelocity()
{
	return m_angularVeloctity;//m_phNode->getActor()->getAngularVelocity();
}

void AgentPhysics::SetMaxAngularVelocity(float m)
{
	m_phNode->getActor()->setMaxAngularVelocity(m);
}
float AgentPhysics::GetMaxAngularVelocity()const
{
	return m_phNode->getActor()->getMaxAngularVelocity();
}

void AgentPhysics::SetMaxSpeed(float s)
{
}
float AgentPhysics::GetMaxSpeed()const
{
	return 50;
}


}
}


#include "stdafx.h"
#include "PhCharacterPM.h"

namespace mray
{
namespace GameMod
{

PhCharacterPM::PhCharacterPM(PhysicsBasedCharacter::CharacterGait* gait,float YOffset)
{
	m_gait=gait;
	m_maxSpeed=3;
	m_YOffset=YOffset;
}
PhCharacterPM::~PhCharacterPM()
{
}


float	PhCharacterPM::GetMass()
{
	return m_gait->GetMass();
}

void	PhCharacterPM::Update(float dt)
{
	m_gait->update(dt);
}

void PhCharacterPM::SetGlobalPosition(const math::vector3d&pos)
{
	math::vector3d p =pos;
	p.y+=m_YOffset;
	m_gait->SetGlobalPosition(p);
}
math::vector3d PhCharacterPM::GetGlobalPosition()
{
	math::vector3d p2=m_gait->GetGlobalPosition();
	//p2.y=0;
	p2.y-=m_YOffset;
	return p2;
}

void PhCharacterPM::SetGlobalOrintation(const math::quaternion&or)
{
	m_gait->SetGlobalOrintation(or);
}
math::quaternion PhCharacterPM::GetGlobalOrintation()
{/*
	math::vector3d zAxis= m_gait->GetGlobalOrintation()*math::vector3d::ZAxis;
	math::vector3d xAxis=math::vector3d::YAxis.crossProduct(zAxis);
	zAxis=xAxis.crossProduct(math::vector3d::YAxis);
	math::quaternion q;
	q.fromAxis(xAxis,math::vector3d::YAxis,zAxis);
*/
	return m_gait->GetGlobalOrintation();
}

void PhCharacterPM::SetLinearDamping(float d)
{
	m_gait->SetLinearDamping(d);
}
float PhCharacterPM::GetLinearDamping()
{
	return m_gait->GetLinearDamping();
}

void PhCharacterPM::SetLinearVelocity(const math::vector3d&v)
{
	m_gait->SetLinearVelocity(v);
}
math::vector3d PhCharacterPM::GetLinearVelocity()
{
	return m_gait->GetLinearVelocity();
}

void PhCharacterPM::SetAngularVelocity(const math::vector3d& v)
{
	m_gait->SetAngularVelocity(v);
}
math::vector3d PhCharacterPM::GetAngularVelocity()
{
	return m_gait->GetAngularVelocity();
}

void PhCharacterPM::SetMaxAngularVelocity(float m)
{
	m_gait->SetMaxAngularVelocity(m);
}
float PhCharacterPM::GetMaxAngularVelocity()const
{
	return m_gait->GetMaxAngularVelocity();
}

void PhCharacterPM::SetMaxSpeed(float s)
{
	m_maxSpeed=s;
}
float PhCharacterPM::GetMaxSpeed()const
{
	return m_maxSpeed;
}



}
}




#include "stdafx.h"
#include "CharControlerPM.h"
#include "ICharacterController.h"

namespace mray{
namespace GameMod{

CharControlerPM::CharControlerPM(physics::ICharacterController*phNode,float YOffset,float gravity)
{
	m_phNode=phNode;
	SetMaxAngularVelocity(350);
	m_gravity=gravity;
	SetLinearVelocity(0);
	m_YOffset=YOffset;
}
CharControlerPM::~CharControlerPM()
{
}

float CharControlerPM::GetMass()
{
	return 1;//m_phNode->getActor()->getMass();
}

void CharControlerPM::Update(float dt)
{
	float kd=0.7,kp=0.8;
	math::vector3d currAngle;
	GetGlobalOrintation().toEulerAngles(currAngle);
	currAngle.x=currAngle.z=0;
	
	math::vector3d speed=m_angularVeloctity;
	speed.Normalize();
	speed*=GetMaxAngularVelocity();
	math::vector3d torque;
	torque=(m_angularVeloctity/* - currAngle*/)*kp;
	if(torque.LengthSQ()>GetMaxAngularVelocity()*GetMaxAngularVelocity())
	{
		torque.Normalize();
		torque*=GetMaxAngularVelocity();
	}
	m_ori*=torque*dt;
	//m_ori*=m_angularVeloctity*dt;
	m_ori.Normalize();
}

void CharControlerPM::SetGlobalPosition(const math::vector3d&pos)
{
	math::vector3d p =pos;
	p.y+=m_YOffset;
	m_phNode->setPosition(p);
}
math::vector3d CharControlerPM::GetGlobalPosition()
{
	//math::vector3d p= m_phNode->getPosition();
	math::vector3d p2=m_phNode->getActor()->getGlobalPosition();
	//p2.y=0;
	p2.y-=m_YOffset;
	return p2;
}

void CharControlerPM::SetGlobalOrintation(const math::quaternion&or)
{
	m_ori=or;//m_phNode->getActor()->setGlobalOrintation(or);
}
math::quaternion CharControlerPM::GetGlobalOrintation()
{
	return m_ori;//m_phNode->getActor()->getGlobalOrintation();
}

void CharControlerPM::SetLinearDamping(float d)
{
	m_phNode->getActor()->setLinearDamping(d);
}
float CharControlerPM::GetLinearDamping()
{
	return m_phNode->getActor()->getLinearDamping();
}

void CharControlerPM::SetLinearVelocity(const math::vector3d&v)
{
	math::vector3d vel(v.x,0,v.z);
	float len=vel.LengthSQ();
	if(len>GetMaxSpeed()*GetMaxSpeed())
	{
		vel.Normalize();
		vel*=GetMaxSpeed();
	}
	vel.y=v.y+m_gravity;
	m_phNode->setMovingDir(vel);
}
math::vector3d CharControlerPM::GetLinearVelocity()
{
	return m_phNode->getActor()->getLinearVelocity();
}

void CharControlerPM::SetAngularVelocity(const math::vector3d& v)
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
math::vector3d CharControlerPM::GetAngularVelocity()
{
	return m_angularVeloctity;//m_phNode->getActor()->getAngularVelocity();
}

void CharControlerPM::SetMaxAngularVelocity(float m)
{
	m_maxAngularVel=m;
}
float CharControlerPM::GetMaxAngularVelocity()const
{
	return m_maxAngularVel;//m_phNode->getActor()->getMaxAngularVelocity();
}

void CharControlerPM::SetMaxSpeed(float s)
{
}
float CharControlerPM::GetMaxSpeed()const
{
	return 3;
}


}
}

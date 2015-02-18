
#include "stdafx.h"
#include "AccelerometerComponent.h"
#include "PhysicsComponent.h"
#include "GameEntityManager.h"
#include "IPhysicManager.h"


namespace mray
{
namespace VT
{
IMPLEMENT_VTCOMPONENT(AccelerometerComponent,MountableComponent)

AccelerometerComponent::AccelerometerComponent(game::GameEntityManager*mngr)
	:MountableComponent(mngr)
{
	m_phMngr=mngr->GetPhysicsManager();
	m_timeAcc=0;
	m_noiseAmount=PropertyTypeNoiseAmount::instance.GetDefaultValue();
	m_refreshFreq=PropertyTypeRefreshFrequency::instance.GetDefaultValue();
	m_invRefFreq=1.0f/m_refreshFreq;
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeNoiseAmount::instance);
		dic->addPropertie(&PropertyTypeRefreshFrequency::instance);
	}
}

AccelerometerComponent::~AccelerometerComponent()
{
}

bool AccelerometerComponent::InitComponent()
{

	m_timeAcc=0;
	if(!MountableComponent::InitComponent())
		return false;
	m_phComp=game::IGameComponent::RetriveComponent<game::PhysicsComponent>(m_ownerComponent,m_targetNode);
	return true;
}

IMPLEMENT_SETGET_PROP(AccelerometerComponent,NoiseAmount,float,m_noiseAmount,0.05,core::StringConverter::toFloat,core::StringConverter::toString);

IMPLEMENT_PROPERTY_TYPE_HEADER(RefreshFrequency,AccelerometerComponent,float,"RefreshFrequency",EPBT_Basic,mT(""),60.0f);
IMPLEMENT_PROPERTY_TYPE_GENERIC(RefreshFrequency,AccelerometerComponent,float,SetRefreshFrequency,GetRefreshFrequency,core::StringConverter::toString,core::StringConverter::toFloat,false);

bool AccelerometerComponent::SetRefreshFrequency(const float& v)
{

	if(!v)
		return false;
	m_refreshFreq=v;
	m_invRefFreq=1.0f/m_refreshFreq;
	return true;
}
const float& AccelerometerComponent::GetRefreshFrequency()const{return m_refreshFreq;}

math::vector3d AccelerometerComponent::GetAccelerometerValues()
{
	return m_accelValues;
}


void AccelerometerComponent::Update(float dt)
{
	MountableComponent::Update(dt);
	m_timeAcc+=dt;
	if(m_timeAcc>m_invRefFreq)
	{
		m_accelValues=GetAbsoluteOrientation()*m_phMngr->GetGravity();//gravity Acceleration
		if(m_phComp)//local Acceleration
		{
			m_vel[0]=m_phComp->GetPhysicalNode()->getLinearVelocity();
			m_accelValues+=(m_vel[0]-m_vel[1])/m_timeAcc;
			m_vel[1]=m_vel[0];
		}
		m_accelValues.x+=m_noiseAmount*(math::Randomizer::rand01()*2-1);
		m_accelValues.y+=m_noiseAmount*(math::Randomizer::rand01()*2-1);
		m_accelValues.z+=m_noiseAmount*(math::Randomizer::rand01()*2-1);
		m_timeAcc=0;
	}
}



}
}



#include "stdafx.h"
#include "GPSComponent.h"
#include "PhysicsComponent.h"
#include "GameEntityManager.h"
#include "IPhysicManager.h"


namespace mray
{
namespace VT
{
	IMPLEMENT_VTCOMPONENT(GPSComponent,MountableComponent);

	IMPLEMENT_SETGET_PROP(GPSComponent,NoiseAmount,float,m_noiseAmount,0.05,core::StringConverter::toFloat,core::StringConverter::toString);

	IMPLEMENT_PROPERTY_TYPE_HEADER(RefreshFrequency,GPSComponent,float,"RefreshFrequency",EPBT_Basic,mT(""),60.0f);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(RefreshFrequency,GPSComponent,float,SetRefreshFrequency,GetRefreshFrequency,core::StringConverter::toString,core::StringConverter::toFloat,false);

GPSComponent::GPSComponent(game::GameEntityManager*mngr)
	:MountableComponent(mngr)
{
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

GPSComponent::~GPSComponent()
{
}


bool GPSComponent::InitComponent()
{
	m_timeAcc=0;
	return MountableComponent::InitComponent();
}

bool GPSComponent::SetRefreshFrequency(const float& v)
{

	if(!v)
		return false;
	m_refreshFreq=v;
	m_invRefFreq=1.0f/m_refreshFreq;
	return true;
}
const float& GPSComponent::GetRefreshFrequency()const{return m_refreshFreq;}

math::vector3d GPSComponent::GetGPSValues()
{
	return m_posValues;
}


void GPSComponent::Update(float dt)
{
	MountableComponent::Update(dt);
	m_timeAcc+=dt;
	if(m_timeAcc>m_invRefFreq)
	{
		m_posValues=GetAbsolutePosition();
		m_posValues.x+=m_noiseAmount*(math::Randomizer::rand01()*2-1);
		m_posValues.y+=m_noiseAmount*(math::Randomizer::rand01()*2-1);
		m_posValues.z+=m_noiseAmount*(math::Randomizer::rand01()*2-1);
		m_timeAcc=0;
	}
}



}
}


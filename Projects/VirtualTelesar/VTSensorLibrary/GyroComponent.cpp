

#include "stdafx.h"
#include "GyroComponent.h"



namespace mray
{
namespace VT
{

	IMPLEMENT_VTCOMPONENT(GyroComponent,MountableComponent)
GyroComponent::GyroComponent(game::GameEntityManager*mngr)
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

GyroComponent::~GyroComponent()
{
}


bool GyroComponent::InitComponent()
{
	
	m_timeAcc=0;
	return MountableComponent::InitComponent();
}

IMPLEMENT_SETGET_PROP(GyroComponent,NoiseAmount,float,m_noiseAmount,0.05,core::StringConverter::toFloat,core::StringConverter::toString);

IMPLEMENT_PROPERTY_TYPE_HEADER(RefreshFrequency,GyroComponent,float,"RefreshFrequency",EPBT_Basic,mT(""),60.0f);
IMPLEMENT_PROPERTY_TYPE_GENERIC(RefreshFrequency,GyroComponent,float,SetRefreshFrequency,GetRefreshFrequency,core::StringConverter::toString,core::StringConverter::toFloat,false);

bool GyroComponent::SetRefreshFrequency(const float& v)
{

	if(!v)
		return false;
	m_refreshFreq=v;
	m_invRefFreq=1.0f/m_refreshFreq;
	return true;
}
const float& GyroComponent::GetRefreshFrequency()const{return m_refreshFreq;}

math::vector3d GyroComponent::GetGyroValues()
{
	return m_gyroValues;
}


void GyroComponent::Update(float dt)
{
	MountableComponent::Update(dt);
	m_timeAcc+=dt;
	if(m_timeAcc>m_invRefFreq)
	{
		math::vector3d currentVals;
		GetAbsoluteOrientation().toEulerAngles(currentVals);
		m_gyroValues=(currentVals-m_lastMeasured)/m_timeAcc;
		m_gyroValues.x+=m_noiseAmount*(math::Randomizer::rand01()*2-1);
		m_gyroValues.y+=m_noiseAmount*(math::Randomizer::rand01()*2-1);
		m_gyroValues.z+=m_noiseAmount*(math::Randomizer::rand01()*2-1);
		m_lastMeasured=currentVals;
		m_timeAcc=0;//-=m_invRefFreq;
	}
}

}
}



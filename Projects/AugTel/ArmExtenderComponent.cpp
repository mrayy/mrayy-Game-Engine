

#include "stdafx.h"
#include "ArmExtenderComponent.h"
#include "IMovableComponent.h"



namespace mray
{
namespace AugTel
{
	IMPLEMENT_SETGET_PROP(ArmExtenderComponent, Threshold, float, m_threshold, 0.4, core::StringConverter::toFloat, core::StringConverter::toString);

	const float VEL_DELTA_TIME = 1.0f / 30.0f;
	ArmExtenderComponent::ArmExtenderComponent(game::GameEntityManager*m)
		:ArmComponent(m)
	{
		m_threshold = 0.4;
		CPropertieDictionary* dic;
		if (CreateDictionary(&dic))
		{
			dic->addPropertie(&PropertyTypeThreshold::instance);
		}
		m_time = 99999999;
		m_extension = 0;
	}
	ArmExtenderComponent::~ArmExtenderComponent()
	{
	}
	void ArmExtenderComponent::_SetExtension(int e)
	{
		m_extension = e;
		if (m_extension > 3)
			m_extension = 3;
		else if (m_extension < 0)
			m_extension = 0;
		SetArmLength(m_extension*0.5);
	}
	void ArmExtenderComponent::Update(float dt)
	{
		ArmComponent::Update(dt);
		if (!m_wrist || !m_wrist->GetMovableObject())
			return;
		math::vector3d pos = m_wrist->GetMovableObject()->getAbsolutePosition();
		m_time += dt;

		if (m_time >= VEL_DELTA_TIME)
		{
			m_avgVelocity = (pos - m_lastPos) / m_time;
			m_lastPos = pos;
			if (m_avgVelocity.z > m_threshold)
			{
				_SetExtension(m_extension+1);
			}
			else if (m_avgVelocity.z < -m_threshold)
			{
				_SetExtension(m_extension - 1);
			}
			m_time = 0;
		}
	}


}
}


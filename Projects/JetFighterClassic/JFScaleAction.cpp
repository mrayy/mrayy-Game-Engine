
#include "stdafx.h"
#include "JFScaleAction.h"
#include "JFObject.h"
#include "SceneComponent.h"


namespace mray
{
namespace game
{

JFScaleAction::JFScaleAction(JFObject* o,float speed,math::vector3d scale)
:m_object(o),m_scale(scale),m_time(0)
{
	m_speed=abs(speed);
}

JFScaleAction::~JFScaleAction()
{
}


void JFScaleAction::OnErrupt()
{
	m_object->SetScale(m_scale);
}
bool JFScaleAction::Execute(float dt)
{
	if(m_time==0)
	{
		m_oldScale=m_object->GetScale();
	}

	math::vector3d l;
	l.x=math::lerp(m_oldScale.x,m_scale.x,m_time);
	l.y=math::lerp(m_oldScale.y,m_scale.y,m_time);
	l.z=math::lerp(m_oldScale.z,m_scale.z,m_time);
	m_object->SetScale(l);


	m_time=math::Min(m_time+m_speed*dt,1.0f);

	if(m_time==1)
		return true;

	return false;
}


}
}


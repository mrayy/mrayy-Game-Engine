
#include "stdafx.h"
#include "DistanceComponent.h"
#include "GameEntityManager.h"
#include "DefaultColors.h"
#include "IDebugDrawManager.h"
#include "SceneComponent.h"
#include "DebugRenderSettings.h"


namespace mray
{
namespace VT
{

IMPLEMENT_VTCOMPONENT(DistanceComponent,MountableComponent);

IMPLEMENT_PROPERTY_TYPE_HEADER(Distance,DistanceComponent,float,mT("Distance"),EPBT_Basic,mT(""),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Distance,DistanceComponent,float,SetDistance,GetDistance,core::StringConverter::toString,core::StringConverter::toFloat,false);


DistanceComponent::DistanceComponent(game::GameEntityManager*mngr):MountableComponent(mngr)
{
	m_phManager=mngr->GetPhysicsManager();
	m_hasHit=false;
	m_distance=0;
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeDistance::instance);
	}
}
DistanceComponent::~DistanceComponent()
{
}

bool DistanceComponent::InitComponent()
{
	return MountableComponent::InitComponent();
}
bool DistanceComponent::SetDistance(float v)
{
	m_distance=v;
	return true;
}
float DistanceComponent::GetDistance()
{
	return m_distance;
}

void DistanceComponent::Update(float dt)
{
	if(m_node)
	{
		math::Ray3d ray;
		ray.Start=GetAbsolutePosition();
		ray.Dir=GetAbsoluteOrientation()*math::vector3d::ZAxis;
		ray.Dir.Normalize();
		m_hasHit= m_phManager->RaycastClosestShape(ray,physics::EShape_All,m_hitData,m_distance);
	}

	game::IGameComponent::Update(dt);
}
void DistanceComponent::DebugRender(scene::IDebugDrawManager* renderer)
{
	if(m_node)
	{
		math::vector3d start=GetAbsolutePosition();
		math::vector3d end;
		math::quaternion ori=GetAbsoluteOrientation();
		float d=m_distance;
		if(m_hasHit)
		{
			end=start+ori*(math::vector3d::ZAxis*m_hitData.distance);
			renderer->AddLine(start,end,video::DefaultColors::Red);
			start=end;
			d-=m_hitData.distance;
		}
		end=start+ori*(math::vector3d::ZAxis*d);
		renderer->AddLine(start,end,video::DefaultColors::Green,DebugRenderSettings::LineWidth);
	}
	MountableComponent::DebugRender(renderer);
}


}
}
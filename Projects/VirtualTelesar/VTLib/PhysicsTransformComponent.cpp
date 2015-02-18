
#include "stdafx.h"
#include "PhysicsTransformComponent.h"

#include "PhysicsComponent.h"

namespace mray
{
namespace VT
{


	IMPLEMENT_PROPERTY_TYPE_HEADER(Position,PhysicsTransformComponent,math::vector3d,mT("Position"),EPBT_Basic,mT(""),0);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Position,PhysicsTransformComponent,math::vector3d,SetPosition,GetPosition,core::StringConverter::toString,core::StringConverter::toVector3d,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(Rotation,PhysicsTransformComponent,math::vector3d,mT("Rotation"),EPBT_Basic,mT(""),0);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Rotation,PhysicsTransformComponent,math::vector3d,SetRotation,GetRotation,core::StringConverter::toString,core::StringConverter::toVector3d,false);

PhysicsTransformComponent::PhysicsTransformComponent(game::GameEntityManager*mngr)
{
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypePosition::instance);
		dic->addPropertie(&PropertyTypeRotation::instance);
	}
}
PhysicsTransformComponent::~PhysicsTransformComponent()
{
}
bool PhysicsTransformComponent::InitComponent()
{
	game::IGameComponent::InitComponent();

	if(m_ownerComponent)
	{
		const std::list<IObjectComponent*>& lst =m_ownerComponent->GetComponent(game::PhysicsComponent::getClassRTTI());
		std::list<IObjectComponent*>::const_iterator it=lst.begin();
		for(;it!=lst.end();++it)
		{
			game::PhysicsComponent* comp=dynamic_cast<game::PhysicsComponent*>(*it);
			if(comp )
			{
				comp->SetPosition(m_pos+ comp->GetPosition());
				comp->SetRotation(m_rotation+ comp->GetRotation());
			}
		}
	}
	return true;
}



}
}

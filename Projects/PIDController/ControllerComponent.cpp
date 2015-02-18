

#include "stdafx.h"
#include "ControllerComponent.h"
#include "PhysicsComponent.h"
#include "GameEntity.h"

namespace mray
{
namespace game
{

IMPLEMENT_RTTI(ControllerComponent,IGameComponent)
IMPLEMENT_GAMECOMPONENT_FACTORY(ControllerComponent)

ControllerComponent::ControllerComponent(GameEntityManager*)
{
}

ControllerComponent::~ControllerComponent()
{
}


bool ControllerComponent::InitComponent()
{
	if(!IGameComponent::InitComponent())
		return false;

	m_phComp=m_ownerEntity->RetriveComponent<PhysicsComponent>(m_ownerComponent,"");
	return true;
}


void ControllerComponent::Update(float dt)
{
}


void ControllerComponent::SetCurrent(float v)
{
}

float ControllerComponent::GetPosition()
{
	return 0;
}

float ControllerComponent::GetForce()
{
	return 0;
}

}
}




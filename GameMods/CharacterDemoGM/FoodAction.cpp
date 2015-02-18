
#include "stdafx.h"
#include "FoodAction.h"
#include "CHouse.h"
#include "PropertyValue.h"
#include "IMovable.h"

#include "AISystem.h"
#include "AIAgent.h"
#include "GameLevel.h"
#include "GameEntityManager.h"

#include "AIFoodArch.h"
#include "SimplePhysics.h"

#include "AIObstacleCircle.h"

#include "ValueProxy.h"

namespace mray
{
namespace GameMod
{

FoodAction::FoodAction():ObstacleAction(mT("FoodAction"))
{
	m_physics=0;
	m_owner=0;
	m_food=new CFood();

	CHouse::getInstance().AddFood(m_food);

	AddAttribute(new ValueProxy(&m_food->Amount));
	AddAttribute(new ValueProxy(&m_food->Type));
}
FoodAction::~FoodAction()
{
}

void FoodAction::Init()
{
	ObstacleAction::Init();
	/*
	m_physics=new SimplePhysics();

	m_actor=new AI::AIAgent(m_physics,mT(""));
	m_arch=new AIFoodArch(m_actor);
	m_arch->GetObstacle()->SetRadius(1);
	m_arch->GetObstacle()->SetShape(new AI::AIObstacleCircle(1));
	m_actor->AttachController(m_arch);*/

}

void FoodAction::AttachToEntity(game::GameEntity*ent)
{
	ObstacleAction::AttachToEntity(ent);
	m_food->SetEntity(ent);
}

void FoodAction::Update(float dt)
{
	ObstacleAction::Update(dt);
}



}
}

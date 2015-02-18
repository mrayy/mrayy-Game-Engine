
#include "stdafx.h"
#include "ObstacleAction.h"
#include "IMovable.h"

#include "AISystem.h"
#include "AIAgent.h"
#include "GameLevel.h"
#include "GameEntityManager.h"

#include "SimplePhysics.h"
#include "AIObstacleObject.h"
#include "CharControlerPM.h"
#include "AIObstacleCircle.h"

namespace mray
{
namespace GameMod
{

ObstacleAction::ObstacleAction():game::IGameAction(mT("ObstacleAction")),m_phNode(0)
{
	m_radius=new FloatValue(mT("Radius"),1);
	AddAttribute(m_radius);
}

ObstacleAction::ObstacleAction(const core::string&name):game::IGameAction(name)
{
	m_radius=new FloatValue(mT("Radius"),1);
	AddAttribute(m_radius);
}
ObstacleAction::~ObstacleAction()
{
	if(m_owner){
		m_owner->GetGameEntityManager()->GetGameLevel()->GetAISystem()->RemoveActor(m_actor);
		if(m_phNode)
			m_owner->GetGameEntityManager()->GetGameLevel()->GetPhysicsManager()->removeController(m_phNode);
	}
}

void ObstacleAction::Init()
{
	//m_physics=new SimplePhysics();

	physics::PhysicsCCCapsuleDesc ccDesc;
	ccDesc.radius=m_radius->value;
	ccDesc.height=2;
	ccDesc.position=0;
	ccDesc.slopeLimit=0;

	m_phNode=m_owner->GetGameEntityManager()->GetGameLevel()->GetPhysicsManager()->createCharacterController(&ccDesc);

	m_physics=new CharControlerPM(m_phNode);

	m_actor=new AI::AIAgent(m_physics,mT(""));
	m_actor->EnableVisualize(true);
	m_arch=new AIObstacleObject(m_actor);
	m_arch->GetObstacle()->SetRadius(m_radius->value);
	m_arch->GetObstacle()->SetShape(new AI::AIObstacleCircle(m_radius->value));
	m_actor->AttachController(m_arch);
}

void ObstacleAction::AttachToEntity(game::GameEntity*ent)
{
	m_owner=ent;
	Init();
	if(ent)
	{
		if(ent->GetShape())
			m_physics->SetGlobalPosition(ent->GetShape()->getAbsolutePosition());

		m_owner->GetGameEntityManager()->GetGameLevel()->GetAISystem()->AddActor(m_actor);
	}
}

void ObstacleAction::Update(float dt)
{
	if(m_owner && m_owner->GetShape()){
		m_physics->SetGlobalPosition(m_owner->GetShape()->getAbsolutePosition());
	}
}


}
}




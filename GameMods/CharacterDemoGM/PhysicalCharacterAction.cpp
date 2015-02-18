
#include "Stdafx.h"
#include "PhysicalCharacterAction.h"
#include "CHouse.h"
#include "GameEntityManager.h"
#include "GameLevel.h"


namespace mray
{
namespace GameMod
{

PhysicalCharacterAction::PhysicalCharacterAction()
:game::IGameAction(mT("PhysicalCharacterAction"))
{
	m_phNode=0;
	m_owner=0;
}
PhysicalCharacterAction::~PhysicalCharacterAction()
{
}

void PhysicalCharacterAction::AttachToEntity(game::GameEntity*ent)
{
	m_owner=ent;
	if(m_owner)
	{
		std::list<physics::IPhysicalNode*> grounds=CHouse::getInstance().GetCollisionGrounds();
	/*	m_character=new PhysicsBasedCharacter::Character(mT("Human.xml"),0,
			m_owner->GetGameEntityManager()->GetGameLevel()->GetPhysicsManager(),
			grounds,
			m_owner->GetGameEntityManager()->GetGameLevel()->GetSceneManager(),
			m_owner->GetGameEntityManager()->GetGameLevel()->GetRootSceneNode());

		m_phNode=new PhCharacterPM(m_character->getGait(),0);
		m_phNode->SetGlobalPosition(m_owner->GetShape()->getPosition());
		m_phNode->SetGlobalOrintation(m_owner->GetShape()->getOrintation());
		m_oldPos=m_owner->GetShape()->getPosition();
		m_oldOrient=m_owner->GetShape()->getOrintation();*/
	}
	
}

void PhysicalCharacterAction::Update(float dt)
{
	m_character->update(dt);
	if(m_owner && m_owner->GetShape()){
		if(m_owner->GetGameEntityManager()->GetGameLevel()->IsEditorMode()){
			math::vector3d pos=m_owner->GetShape()->getPosition();
			/*pos.y-=m_YOffset.value;*/
// 			if(m_oldPos.getDistSQ(pos)>0.1){
// 				m_oldPos=pos;
			// 			}
			m_character->deactivate();
			m_phNode->SetGlobalPosition(pos);
 			if((m_oldOrient-m_owner->GetShape()->getOrintation()).LengthSQ()>0.00){
			 	m_oldOrient=m_owner->GetShape()->getOrintation();
				m_phNode->SetGlobalOrintation(m_owner->GetShape()->getOrintation());
 			}
			((scene::IRenderable*)m_owner->GetShape())->setVisible(true);

		}
		else{
			m_character->activate();
			((scene::IRenderable*)m_owner->GetShape())->setVisible(false);
			math::vector3d pos=m_phNode->GetGlobalPosition();
			//pos.y+=m_YOffset.value;
			m_owner->GetShape()->setPosition(pos);
			//m_owner->GetShape()->setOrintation(m_phNode->GetGlobalOrintation());
		}
	}
}

}
}
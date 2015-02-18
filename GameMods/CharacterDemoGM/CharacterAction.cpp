
#include "stdafx.h"
#include "CharacterAction.h"
#include "AIAgent.h"
#include "AIPhysicsProxy.h"
#include "AICharacterArch.h"

#include "GameEntity.h"
#include "IMovable.h"

#include "AISystem.h"
#include "AIAgent.h"

#include "IPhysical3DDef.h"
#include "IPhysicManager.h"

#include "CharControlerPM.h"
#include "GameLevel.h"
#include "GameEntityManager.h"
#include "AIObstacleCircle.h"

#include "ValueProxy.h"
#include "AIArchitectParser.h"
#include "IFileSystem.h"

#include "PhCharacterPM.h"

namespace mray
{
namespace GameMod
{

CharacterAction::CharacterAction():IGameAction(mT("CharacterAction")),m_owner(0),m_isPhysicalBased(mT("PhysicalBased"),false),m_phNode(0),
		m_radius(mT("Radius"),0.3),m_height(mT("Height"),1.7),m_YOffset(mT("YOffset"),0),m_definitionFile(mT("Definition"),mT("")),
		m_scenarioFile(mT("Scenario"),mT(""))
{
	m_physics=new AI::AIPhysicsProxy(0);
	m_actor=new AI::AIAgent(m_physics,mT(""));
	m_actor->EnableVisualize(false);
	m_archeticture=0;
	m_phCharacter=0;


	AddAttribute(new ValueProxy(&m_isPhysicalBased));
	AddAttribute(new ValueProxy(&m_radius));
	AddAttribute(new ValueProxy(&m_height));
	AddAttribute(new ValueProxy(&m_YOffset));
	AddAttribute(new ValueProxy(&m_definitionFile));
	AddAttribute(new ValueProxy(&m_scenarioFile));

}
CharacterAction::~CharacterAction()
{
	if(m_owner){
		m_owner->GetGameEntityManager()->GetGameLevel()->GetAISystem()->RemoveActor(m_actor);
	}
	//delete m_archeticture;
	//delete m_physics;
	m_actor=0;

}

void CharacterAction::AttachToEntity(game::GameEntity*ent)
{
	m_owner=ent;
	if(m_owner ){
		m_owner->SetAIActor(m_actor);

		if(m_isPhysicalBased.value==false){
			
			physics::PhysicsCCCapsuleDesc ccDesc;
			ccDesc.radius=m_radius.value;
			ccDesc.height=m_height.value;
			/*
			physics::PhysicsCCBoxDesc ccDesc;
			ccDesc.extent.set(m_radius.value,m_height.value*0.5f,m_radius.value);*/
			ccDesc.position.y=0;
			ccDesc.slopeLimit=0;
			ccDesc.skinWidth =math::Epsilon;

			m_phNode=m_owner->GetGameEntityManager()->GetGameLevel()->GetPhysicsManager()->createCharacterController(&ccDesc);

			m_physics->SetPhysics(new CharControlerPM(m_phNode,m_YOffset.value,-9.8));
		}else
		{
			scene::ISceneNode*node=(scene::ISceneNode*) m_owner->GetShape();
			if(!node->getMesh())
				return;
			scene::Skeleton* skeleton = node->getMesh()->getSkeleton();
			if(!skeleton )
				return;
			//use physical based character control
			std::list<physics::IPhysicalNode*> grounds=CHouse::getInstance().GetCollisionGrounds();
			m_phCharacter=CHouse::getInstance().GetCharacterManager()->addCharacter(mT("Human2.xml"),skeleton);
				/*new PhysicsBasedCharacter::Character(mT("Human.xml"),0,
				m_owner->GetGameEntityManager()->GetGameLevel()->GetPhysicsManager(),
				grounds,
				m_owner->GetGameEntityManager()->GetGameLevel()->GetSceneManager(),
				m_owner->GetGameEntityManager()->GetGameLevel()->GetRootSceneNode());
*/
			m_physics->SetPhysics(new PhCharacterPM(m_phCharacter->getGait(),m_YOffset.value));
			m_physics->SetGlobalPosition(m_owner->GetShape()->getPosition());
			m_physics->SetGlobalOrintation(m_owner->GetShape()->getOrintation());
		}

		scene::ISceneNode*node= (scene::ISceneNode*)m_owner->GetShape();
		animation::IAnimationMixer* mixer=0;
		if(node)
		{
			scene::SMesh*mesh=node->getMesh();
			if(mesh && mesh->getSkeleton())
			{
				mixer=mesh->getSkeleton()->getAnimationMixer();
			}
		}

		m_owner->GetGameEntityManager()->GetGameLevel()->GetAISystem()->AddActor(m_actor);
		if(m_archeticture)
			delete m_archeticture;
		m_archeticture=new AICharacterArch(m_owner,mixer,!m_isPhysicalBased.value,m_isPhysicalBased.value);
		m_archeticture->GetObstacle()->SetRadius(m_radius.value);
		m_archeticture->GetObstacle()->SetShape(new AI::AIObstacleCircle(m_radius.value));

		if(m_definitionFile.value!=mT("")){
			AI::AIArchitectParser parser;
			GCPtr<OS::IStream> archDesc=gFileSystem.openFile(m_definitionFile.value);
			if(archDesc){
				parser.ParseXML(m_archeticture,archDesc);
				archDesc->close();
			}
		}

		m_actor->AttachController(m_archeticture);

		if(m_owner->GetShape())
			m_physics->SetGlobalPosition(m_owner->GetShape()->getPosition());

		if(m_scenarioFile.value!=mT(""))
			m_archeticture->GetCharacterObject()->RunScenario(m_scenarioFile.value);
	}
}

void CharacterAction::Update(float dt)
{

	if(m_phCharacter){
		
		if(m_owner && m_owner->GetShape()){
			if(m_owner->GetGameEntityManager()->GetGameLevel()->IsEditorMode()){
				math::vector3d pos=m_owner->GetShape()->getPosition();
				/*pos.y-=m_YOffset.value;*/
				// 			if(m_oldPos.getDistSQ(pos)>0.1){
				// 				m_oldPos=pos;
				// 			}
				m_phCharacter->deactivate();
				m_physics->SetGlobalPosition(pos);
			//	m_physics->SetGlobalOrintation(m_owner->GetShape()->getOrintation());
				//((scene::IRenderable*)m_owner->GetShape())->setVisible(true);

			}
			else{
				m_phCharacter->activate();
				//((scene::IRenderable*)m_owner->GetShape())->setVisible(false);
				math::vector3d pos=m_physics->GetGlobalPosition();
				//pos.y+=m_YOffset.value;
				m_owner->GetShape()->setPosition(pos);
				m_owner->GetShape()->setOrintation(math::quaternion::Identity);
			}
		}
	}else{
		if(m_owner && m_owner->GetShape()){
			if(m_owner->GetGameEntityManager()->GetGameLevel()->IsEditorMode()){
				math::vector3d pos=m_owner->GetShape()->getPosition();
				/*pos.y-=m_YOffset.value;*/
				m_physics->SetGlobalPosition(pos);
				m_physics->SetGlobalOrintation(m_owner->GetShape()->getOrintation());
			}
			else{
				math::vector3d pos=m_physics->GetGlobalPosition();
				//pos.y+=m_YOffset.value;
				m_owner->GetShape()->setPosition(pos);
				m_owner->GetShape()->setOrintation(m_physics->GetGlobalOrintation());
			}
		}
	}
}

}
}




/********************************************************************
	created:	2010/06/27
	created:	27:6:2010   18:57
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\CharacterAction.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	CharacterAction
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef CharacterAction_h__
#define CharacterAction_h__

#include "IGameAction.h"
#include "AIActor.h"
#include "IGameActionCreator.h"
#include "CHouse.h"
#include "GenericValues.h"
#include "ICharacterController.h"

#include <Character.h>

namespace mray
{
namespace AI
{
	class AIActor;
	class AIPhysicsProxy;
}
namespace GameMod
{

	class AICharacterArch;

class CharacterAction:public game::IGameAction
{
private:
protected:
	AI::AIPhysicsProxy* m_physics;
	GCPtr<AI::AIActor> m_actor;
	AICharacterArch* m_archeticture;
	game::GameEntity* m_owner;
	physics::ICharacterController* m_phNode;

	PhysicsBasedCharacter::Character* m_phCharacter;

	BoolValue m_isPhysicalBased;
	FloatValue m_radius;
	FloatValue m_height;
	FloatValue m_YOffset;
	StringValue m_definitionFile;
	StringValue m_scenarioFile;


public:
	CharacterAction();
	virtual ~CharacterAction();

	virtual void AttachToEntity(game::GameEntity*ent);

	virtual void Update(float dt);

	void SetPhysicalBased(const bool&v);
	const bool& GetPhysicalBased();

};


class CharacterActionCreator:public game::IGameActionCreator
{
public:
	virtual core::string GetActionName()
	{
		return mT("CharacterAction");
	}
	virtual core::string GetDescription()
	{
		return mT("Contains a physical model and the AI model of the character, with the required goals");
	}

	virtual game::IGameAction* CreateAction()
	{
		return new CharacterAction();
	}
};


}
}


#endif // CharacterAction_h__

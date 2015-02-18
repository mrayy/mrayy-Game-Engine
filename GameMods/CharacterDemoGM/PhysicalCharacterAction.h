


/********************************************************************
	created:	2010/07/11
	file base:	PhysicalCharacterAction
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhysicalCharacterAction__
#define __PhysicalCharacterAction__

#include "IGameAction.h"
#include "IGameActionCreator.h"
#include "PhCharacterPM.h"

#include <Character.h>

namespace mray
{
namespace GameMod
{

class AICharacterArch;

class PhysicalCharacterAction:public game::IGameAction
{
private:
protected:
	game::GameEntity* m_owner;
	PhCharacterPM* m_phNode;
	GCPtr<PhysicsBasedCharacter::Character> m_character;
	math::vector3d m_oldPos;
	math::quaternion m_oldOrient;
public:
	PhysicalCharacterAction();
	virtual ~PhysicalCharacterAction();

	virtual void AttachToEntity(game::GameEntity*ent);

	virtual void Update(float dt);

};


class PhysicalCharacterActionCreator:public game::IGameActionCreator
{
public:
	virtual core::string GetActionName()
	{
		return mT("PhysicalCharacterAction");
	}
	virtual core::string GetDescription()
	{
		return mT("Contains a physical model and the AI model of the character, with the required goals");
	}

	virtual game::IGameAction* CreateAction()
	{
		return new PhysicalCharacterAction();
	}
};


}
}

#endif

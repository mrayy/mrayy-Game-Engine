

/********************************************************************
	created:	2010/07/01
	created:	1:7:2010   16:33
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\ObstacleAction.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	ObstacleAction
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ObstacleAction_h__
#define ObstacleAction_h__


#include "CompileConfig.h"
#include "IGameAction.h"
#include "IGameActionCreator.h"
#include "AIActor.h"
#include "GenericValues.h"
#include "ICharacterController.h"

namespace mray
{
namespace AI
{
	class IAIPhysics;
}
namespace GameMod
{
	class AIObstacleObject;

class CHARACTERDEMOGM_API ObstacleAction:public game::IGameAction
{
private:
protected:
	AI::IAIPhysics* m_physics;
	game::GameEntity* m_owner;
	GCPtr<AI::AIActor> m_actor;

	physics::ICharacterController* m_phNode;
	AIObstacleObject* m_arch;

	virtual void Init();
	FloatValue *m_radius;

public:
	ObstacleAction();
	ObstacleAction(const core::string&name);
	virtual ~ObstacleAction();

	virtual void AttachToEntity(game::GameEntity*ent);

	virtual void Update(float dt);

};

class ObstacleActionCreator:public game::IGameActionCreator
{
public:
	virtual core::string GetActionName()
	{
		return mT("ObstacleAction");
	}
	virtual core::string GetDescription()
	{
		return mT("Adding an obstacle object to the scene");
	}

	virtual game::IGameAction* CreateAction()
	{
		return new ObstacleAction();
	}
};

}
}

#endif // ObstacleAction_h__

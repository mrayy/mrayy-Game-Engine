


/********************************************************************
	created:	2010/07/04
	created:	4:7:2010   22:01
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\CollisionModelAction.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	CollisionModelAction
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef CollisionModelAction_h__
#define CollisionModelAction_h__

#include "CompileConfig.h"
#include "IGameActionCreator.h"
#include "AIPhysicsProxy.h"
#include "IGameAction.h"
#include "IPhysicalNode.h"

namespace mray
{
namespace GameMod
{

class CHARACTERDEMOGM_API CollisionModelAction:public game::IGameAction
{
private:
protected:
	AI::AIPhysicsProxy* m_physics;
	game::GameEntity* m_owner;
	physics::IPhysicalNode* m_phNode;
public:
	CollisionModelAction();
	virtual ~CollisionModelAction();

	virtual void AttachToEntity(game::GameEntity*ent);

	virtual void Update(float dt);
	
};
class CollisionModelActionCreator:public game::IGameActionCreator
{
public:
	virtual core::string GetActionName()
	{
		return mT("CollisionModelAction");
	}
	virtual core::string GetDescription()
	{
		return mT("Convert the scene node into a static collision model");
	}

	virtual game::IGameAction* CreateAction()
	{
		return new CollisionModelAction();
	}
};


}
}

#endif // CollisionModelAction_h__




/********************************************************************
	created:	2010/07/01
	created:	1:7:2010   17:55
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\PhysicalObjectAction.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	PhysicalObjectAction
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef PhysicalObjectAction_h__
#define PhysicalObjectAction_h__

#include "CompileConfig.h"
#include "IGameActionCreator.h"
#include "AIPhysicsProxy.h"
#include "IGameAction.h"
#include "GenericValues.h"
#include "IPhysicalNode.h"

namespace mray
{
namespace GameMod
{

class CHARACTERDEMOGM_API PhysicalObjectAction:public game::IGameAction
{
private:
protected:
	FloatValue *m_radius;
	AI::AIPhysicsProxy* m_physics;
	game::GameEntity* m_owner;
	physics::IPhysicalNode* m_phNode;
	float m_i;
	math::quaternion m_targetAngle1;
	math::quaternion m_targetAngle2;
	bool m_useShortPath;
public:
	PhysicalObjectAction();
	virtual ~PhysicalObjectAction();

	virtual void AttachToEntity(game::GameEntity*ent);

	virtual void Update(float dt);
	
};

class PhysicalObjectActionCreator:public game::IGameActionCreator
{
public:
	virtual core::string GetActionName()
	{
		return mT("PhysicalObjectAction");
	}
	virtual core::string GetDescription()
	{
		return mT("Adding physical object to the scene");
	}

	virtual game::IGameAction* CreateAction()
	{
		return new PhysicalObjectAction();
	}
};



}
}

#endif // PhysicalObjectAction_h__

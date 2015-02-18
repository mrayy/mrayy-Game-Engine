


/********************************************************************
	created:	2010/07/06
	created:	6:7:2010   12:30
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\SelectionSurfaceAction.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	SelectionSurfaceAction
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef SelectionSurfaceAction_h__
#define SelectionSurfaceAction_h__

#include "CompileConfig.h"
#include "IGameActionCreator.h"
#include "IGameAction.h"

namespace mray
{
namespace GameMod
{

class CHARACTERDEMOGM_API SelectionSurfaceAction:public game::IGameAction
{
private:
protected:
	game::GameEntity* m_owner;
public:
	SelectionSurfaceAction();
	virtual ~SelectionSurfaceAction();

	virtual void AttachToEntity(game::GameEntity*ent);

	virtual void Update(float dt);
};

class SelectionSurfaceActionCreator:public game::IGameActionCreator
{
public:
	virtual core::string GetActionName()
	{
		return mT("SelectionSurfaceAction");
	}
	virtual core::string GetDescription()
	{
		return mT("Set the selection surface in game");
	}

	virtual game::IGameAction* CreateAction()
	{
		return new SelectionSurfaceAction();
	}
};


}
}


#endif // SelectionSurfaceAction_h__

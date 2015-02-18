

/********************************************************************
	created:	2010/07/01
	created:	1:7:2010   13:39
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\FoodAction.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	FoodAction
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef FoodAction_h__
#define FoodAction_h__

#include "CFood.h"
#include "ObstacleAction.h"
#include "IGameActionCreator.h"

namespace mray
{
namespace GameMod
{

class CHARACTERDEMOGM_API FoodAction:public ObstacleAction
{
private:
protected:
	CFood* m_food;
	virtual void Init();
public:
	FoodAction();
	virtual ~FoodAction();

	virtual void AttachToEntity(game::GameEntity*ent);

	virtual void Update(float dt);
	
};

class FoodActionCreator:public game::IGameActionCreator
{
public:
	virtual core::string GetActionName()
	{
		return mT("FoodAction");
	}
	virtual core::string GetDescription()
	{
		return mT("Adding food to the scene");
	}

	virtual game::IGameAction* CreateAction()
	{
		return new FoodAction();
	}
};


}
}

#endif // FoodAction_h__

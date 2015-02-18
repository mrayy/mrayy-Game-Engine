

/********************************************************************
	created:	2010/07/21
	created:	21:7:2010   11:34
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\CameraStartAction.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	CameraStartAction
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef CameraStartAction_h__
#define CameraStartAction_h__

#include "IGameAction.h"
#include "IGameActionCreator.h"

namespace mray
{
namespace GameMod
{

class CameraStartAction:public game::IGameAction
{
private:
protected:
	math::vector3d m_startPos;
	math::quaternion m_startRot;
	game::GameEntity* m_owner;
	bool m_switched;
public:
	CameraStartAction();
	virtual ~CameraStartAction();

	virtual void AttachToEntity(game::GameEntity*ent);

	virtual void Update(float dt);
};

class CameraStartActionCreator:public game::IGameActionCreator
{
public:
	virtual core::string GetActionName()
	{
		return mT("CameraStartAction");
	}
	virtual core::string GetDescription()
	{
		return mT("Specifies start location for the camera");
	}

	virtual game::IGameAction* CreateAction()
	{
		return new CameraStartAction();
	}
};

}
}


#endif // CameraStartAction_h__

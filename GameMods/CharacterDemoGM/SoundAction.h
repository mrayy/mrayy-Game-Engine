


/********************************************************************
	created:	2010/07/13
	created:	13:7:2010   15:29
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\SoundAction.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	SoundAction
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef SoundAction_h__
#define SoundAction_h__


#include "IGameAction.h"
#include "IGameActionCreator.h"
#include "ISound.h"

namespace mray
{
namespace GameMod
{

class SoundAction:public game::IGameAction
{
private:
protected:
	game::GameEntity* m_owner;

	GCPtr<sound::ISound> m_sound;

	StringValue m_soundPath;
	FloatValue m_soundVolume;
	FloatValue m_minDistance;
	FloatValue m_maxDistance;
public:
	SoundAction();
	virtual ~SoundAction();

	virtual void AttachToEntity(game::GameEntity*ent);

	virtual void Update(float dt);

};

class SoundActionCreator:public game::IGameActionCreator
{
public:
	virtual core::string GetActionName()
	{
		return mT("SoundAction");
	}
	virtual core::string GetDescription()
	{
		return mT("add sound source to the scene");
	}

	virtual game::IGameAction* CreateAction()
	{
		return new SoundAction();
	}
};

}
}
#endif // SoundAction_h__

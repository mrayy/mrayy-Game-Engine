


/********************************************************************
	created:	2012/02/13
	created:	13:2:2012   0:45
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\SoundComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	SoundComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SoundComponent__
#define __SoundComponent__

#include "IGameComponent.h"
#include "ISound.h"

namespace mray
{
namespace game
{

class MRAY_GAME_DLL SoundComponent:public IGameComponent
{
	DECLARE_RTTI;
private:
protected:
	sound::ISound* m_node;
public:
	SoundComponent(GameEntityManager*):m_node(0){}
	SoundComponent(sound::ISound* node):m_node(node)
	{}
	virtual~SoundComponent(){}

	sound::ISound* GetSound(){return m_node;}
	void SetSound(sound::ISound* node){m_node=node;}

	virtual IObject* GetAttachedObject(){return m_node;}
};
DECLARE_GAMECOMPONENT_FACTORY(SoundComponent);


}
}

#endif

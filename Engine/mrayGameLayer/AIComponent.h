

/********************************************************************
	created:	2012/02/13
	created:	13:2:2012   0:34
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\AIComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	AIComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __AIComponent__
#define __AIComponent__

#include "IGameComponent.h"
#include "AIActor.h"

namespace mray
{
namespace game
{

class MRAY_GAME_DLL AIComponent:public IGameComponent
{
	DECLARE_RTTI;
private:
protected:
	AI::AIActor* m_node;
public:
	AIComponent(GameEntityManager* mngr):m_node(0)
	{}
	AIComponent():m_node(0)
	{}
	virtual~AIComponent(){}

	AI::AIActor* GetAIActor(){return m_node;}
	void SetAIActor(AI::AIActor* node){m_node=node;}

	virtual IObject* GetAttachedObject(){return m_node;}
};

DECLARE_GAMECOMPONENT_FACTORY(AIComponent);

}
}

#endif


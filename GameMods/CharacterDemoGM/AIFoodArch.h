

/********************************************************************
	created:	2010/07/01
	created:	1:7:2010   14:53
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\AIFoodArch.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	AIFoodArch
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AIFoodArch_h__
#define AIFoodArch_h__

#include "AIObstacleObject.h"

namespace mray
{
namespace AI
{
}
namespace GameMod
{

class CHARACTERDEMOGM_API AIFoodArch:public AIObstacleObject
{
private:
protected:
public:
	AIFoodArch(AI::AIActor*owner);
	virtual ~AIFoodArch();

	virtual void Update(float dt);

	virtual bool OnMessageTelegram(const AI::AIMessageTelegram& msg);
};

}
}

#endif // AIFoodArch_h__

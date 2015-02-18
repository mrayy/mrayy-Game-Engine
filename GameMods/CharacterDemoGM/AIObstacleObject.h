


/********************************************************************
	created:	2010/07/01
	created:	1:7:2010   16:00
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\AIObstacleObject.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	AIObstacleObject
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AIObstacleObject_h__
#define AIObstacleObject_h__

#include "AIGameArch.h"


namespace mray
{
namespace GameMod
{

class CHARACTERDEMOGM_API AIObstacleObject:public AIGameArch
{
private:

	void Init();
protected:
	AI::AgentObstacle* m_obstacle;
public:
	AIObstacleObject(AI::AIActor*owner);
	AIObstacleObject(AI::AIActor*owner,EAgentType type);
	virtual ~AIObstacleObject();

	virtual AI::AgentObstacle* GetObstacle();

	virtual void Update(float dt);

	virtual bool OnMessageTelegram(const AI::AIMessageTelegram& msg);
	
};

}
}


#endif // AIObstacleObject_h__

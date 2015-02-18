

/********************************************************************
	created:	2011/06/03
	created:	3:6:2011   21:53
	filename: 	i:\Programing\GameEngine\mrayEngine\Projects\BallanceBall\AIBallArchitecture.h
	file path:	i:\Programing\GameEngine\mrayEngine\Projects\BallanceBall
	file base:	AIBallArchitecture
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AIBallArchitecture_h__
#define AIBallArchitecture_h__

#include "IAIArchitect.h"

namespace mray
{
namespace game
{
	class GameEntity;
}
namespace Ball
{

class AIBallArchitecture:public AI::IAIArchitect
{
private:
protected:
	AI::AIActor* m_owner;
	AI::AgentObstacle* m_obstacle;
	game::GameEntity* m_entity;
public:
	AIBallArchitecture(game::GameEntity*entity);
	virtual ~AIBallArchitecture();

	void SetOwner(AI::AIActor*owner){m_owner=owner;}
	AI::AIActor* GetOwner(){return m_owner;}

	virtual AI::AgentObstacle* GetObstacle();

	virtual bool IsAIControlled();
	virtual void Update(float dt);

	virtual bool OnMessageTelegram(const AI::AIMessageTelegram&msg);
};

}
}


#endif // AIBallArchitecture_h__

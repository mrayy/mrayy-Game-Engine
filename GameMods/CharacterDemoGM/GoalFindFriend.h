


/********************************************************************
	created:	2010/07/10
	created:	10:7:2010   14:41
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\GoalFindFriend.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	GoalFindFriend
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GoalFindFriend_h__
#define GoalFindFriend_h__

#include <GoalPipe.h>
#include "IGoalFactory.h"
#include "GenericValues.h"

namespace mray
{
	namespace AI
	{
		class VisionSensor;
	}
namespace GameMod
{

	class CharacterObject;
class GoalFindFriend:public AI::GoalPipe
{
private:
protected:
	AI::EGoalStatus m_state;
	AI::VisionSensor* m_vision;
	CharacterObject* m_character;
	CharacterObject* m_friend;

	bool m_requestMade;

	AI::EGoalStatus CheckForFriends();
public:
	GoalFindFriend(CharacterObject* character);
	virtual ~GoalFindFriend();

	void pause();
	void resume();

	virtual int GetGoalType();
	virtual void activate();
	virtual AI::EGoalStatus process();
	virtual void terminate();

	virtual AI::EGoalStatus getStatus();


	virtual bool OnMessageTelegram(const AI::AIMessageTelegram&msg);

	virtual core::string getType(){return mT("GoalChatToFriend");}
	virtual void Visualize(AI::IRenderDevice*dev,const core::string&prefix);
};

}
}


#endif // GoalFindFriend_h__


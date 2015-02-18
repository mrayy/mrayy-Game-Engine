


/********************************************************************
	created:	2010/07/10
	created:	10:7:2010   14:07
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\GoalChatToFriend.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	GoalChatToFriend
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GoalChatToFriend_h__
#define GoalChatToFriend_h__

#include <GoalPipe.h>
#include "IGoalFactory.h"
#include "GenericValues.h"

namespace mray
{
namespace GameMod
{

	class CharacterObject;
class GoalChatToFriend:public AI::GoalPipe
{
private:
protected:
	AI::EGoalStatus m_state;
	CharacterObject* m_character;

	FloatValue m_timeToChat;
public:
	GoalChatToFriend(CharacterObject* character,float chat);
	virtual ~GoalChatToFriend();

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
class GoalChatToFriendFactory:public AI::IGoalFactory
{
public:
	virtual core::string GetType(){
		return mT("ChatToFriend");
	}
	virtual AI::IGoal* CreateGoal(AI::IAIArchitect*arch);
};


}
}


#endif // GoalChatToFriend_h__

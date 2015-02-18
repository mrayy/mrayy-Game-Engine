

/********************************************************************
	created:	2010/07/10
	created:	10:7:2010   14:44
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\GoalMakeChat.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	GoalMakeChat
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GoalMakeChat_h__
#define GoalMakeChat_h__

#include "IGoal.h"

namespace mray
{
namespace GameMod
{

	class CharacterObject;

class GoalMakeChat:public AI::IGoal
{
private:
protected:
	AI::EGoalStatus m_state;
	CharacterObject* m_character;

	float m_timeToChat;
public:
	GoalMakeChat(CharacterObject*character, float timeToChat);
	virtual ~GoalMakeChat();

	virtual int GetGoalType();

	virtual void pause();
	virtual void resume();

	virtual void activate();
	virtual AI::EGoalStatus process();
	virtual void terminate();

	virtual AI::EGoalStatus getStatus();

	virtual bool isPrimitive(){return true;}

	virtual core::string getType();
};

}
}


#endif // GoalMakeChat_h__



/********************************************************************
	created:	2010/07/14
	created:	14:7:2010   0:27
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\GoalWaitForChat.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	GoalWaitForChat
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GoalWaitForChat_h__
#define GoalWaitForChat_h__

#include "IGoal.h"

namespace mray
{
namespace GameMod
{

	class CharacterObject;

class GoalWaitForChat:public AI::IGoal
{
private:
protected:
	AI::EGoalStatus m_state;
	CharacterObject* m_character;
public:
	GoalWaitForChat(CharacterObject*character);
	virtual ~GoalWaitForChat();

	void pause();
	void resume();

	virtual int GetGoalType();
	virtual void activate();
	virtual AI::EGoalStatus process();
	virtual void terminate();

	virtual AI::EGoalStatus getStatus();


	virtual bool isPrimitive(){return true;}
	virtual bool OnMessageTelegram(const AI::AIMessageTelegram&msg);

	virtual core::string getType(){return mT("GoalWaitForChat");}
	virtual void Visualize(AI::IRenderDevice*dev,const core::string&prefix);
};

}
}


#endif // GoalWaitForChat_h__

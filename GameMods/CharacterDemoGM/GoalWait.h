


/********************************************************************
	created:	2010/07/06
	created:	6:7:2010   21:06
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\GoalWait.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	GoalWait
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GoalWait_h__
#define GoalWait_h__


#include <IGoal.h>
#include <GenericValues.h>
#include <IGoalFactory.h>

namespace mray
{
namespace GameMod
{

	class CharacterObject;

	enum EWaitType
	{
		EWT_Infinite,
		EWT_Timed,
		EWT_ChatStart,
		EWT_ChatEnd
	};

class GoalWait:public AI::IGoal
{
private:
protected:
	FloatValue m_timeToWait;
	AI::EGoalStatus m_status;
	EWaitType m_chatType;
	bool m_animConst;
	CharacterObject* m_character;
public:
	GoalWait(CharacterObject* character,float waitTime=10,EWaitType chatType=EWT_Timed,bool animConst=false);
	virtual ~GoalWait();

	virtual void pause();
	virtual void resume();

	virtual int GetGoalType();
	virtual void activate();
	virtual AI::EGoalStatus process();
	virtual void terminate();

	virtual AI::EGoalStatus getStatus();

	virtual bool OnMessageTelegram(const AI::AIMessageTelegram&msg);

	virtual core::string getType();
	bool isPrimitive(){return true;}
};

class GoalWaitFactory:public AI::IGoalFactory
{
public:
	virtual core::string GetType(){
		return mT("Wait");
	}
	virtual AI::IGoal* CreateGoal(AI::IAIArchitect*arch);
};

}
}


#endif // GoalWait_h__

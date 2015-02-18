

/********************************************************************
	created:	2010/07/10
	created:	10:7:2010   11:50
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\GoalListenToUser.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	GoalListenToUser
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GoalListenToUser_h__
#define GoalListenToUser_h__

#include "GoalPipe.h"

namespace mray
{
namespace GameMod
{

class CharacterObject;
class GoalListenToUser:public AI::GoalPipe
{
private:
protected:
	CharacterObject* m_character;
public:
	GoalListenToUser(CharacterObject*character);
	virtual ~GoalListenToUser();

	virtual int GetGoalType();
	virtual void activate();
	virtual AI::EGoalStatus process();
	virtual void terminate();

	virtual AI::EGoalStatus getStatus();


	virtual bool OnMessageTelegram(const AI::AIMessageTelegram&msg);

	virtual core::string getType(){return mT("GoalListenToUser");}
	virtual void Visualize(AI::IRenderDevice*dev,const core::string&prefix);
};

}
}


#endif // GoalListenToUser_h__

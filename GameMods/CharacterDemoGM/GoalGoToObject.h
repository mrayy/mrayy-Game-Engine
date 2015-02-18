

/********************************************************************
	created:	2010/07/13
	created:	13:7:2010   11:08
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\GoalGoToObject.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	GoalGoToObject
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GoalGoToObject_h__
#define GoalGoToObject_h__


#include <GoalPipe.h>
#include "IGoalFactory.h"
#include "GenericValues.h"

namespace mray
{
namespace GameMod
{

class CharacterObject;
class GoalGoToObject:public AI::GoalPipe
{
private:
protected:
	AI::EGoalStatus m_state;
	CharacterObject* m_character;

	StringValue m_objectType;

public:
	GoalGoToObject(CharacterObject* character);
	virtual ~GoalGoToObject();

	void pause();
	void resume();

	virtual int GetGoalType();
	virtual void activate();
	virtual AI::EGoalStatus process();
	virtual void terminate();

	virtual AI::EGoalStatus getStatus();


	virtual bool OnMessageTelegram(const AI::AIMessageTelegram&msg);

	virtual core::string getType(){return mT("GoalGoToObject");}
	virtual void Visualize(AI::IRenderDevice*dev,const core::string&prefix);
	
};
class GoalGoToObjectFactory:public AI::IGoalFactory
{
public:
	virtual core::string GetType(){
		return mT("GoToObject");
	}
	virtual AI::IGoal* CreateGoal(AI::IAIArchitect*arch);
};

}
}


#endif // GoalGoToObject_h__

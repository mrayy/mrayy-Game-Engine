

/********************************************************************
	created:	2010/07/09
	created:	9:7:2010   14:16
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\GoalFindFood.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	GoalFindFood
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GoalFindFood_h__
#define GoalFindFood_h__

#include "GoalPipe.h"
#include "CFood.h"
#include "IGoalFactory.h"
#include "GenericValues.h"

namespace mray
{
namespace GameMod
{
class CharacterObject;

class GoalFindFood:public AI::GoalPipe
{
private:
protected:
	AI::EGoalStatus m_state;
	CharacterObject* m_character;

	FloatValue m_requiredAmountOfFood;
	IntValue m_requiredType;
	CFood *m_food;
public:
	GoalFindFood(CharacterObject*character,float amount,EFoodType requiredType);
	virtual ~GoalFindFood();

	void pause();
	void resume();

	virtual int GetGoalType();
	virtual void activate();
	virtual AI::EGoalStatus process();
	virtual void terminate();

	virtual AI::EGoalStatus getStatus();


	virtual bool OnMessageTelegram(const AI::AIMessageTelegram&msg);

	virtual core::string getType(){return mT("GoalFindFood");}
	virtual void Visualize(AI::IRenderDevice*dev,const core::string&prefix);
};

class GoalFindFoodFactory:public AI::IGoalFactory
{
public:
	virtual core::string GetType(){
		return mT("FindFood");
	}
	virtual AI::IGoal* CreateGoal(AI::IAIArchitect*arch);
};

}
}


#endif // GoalFindFood_h__


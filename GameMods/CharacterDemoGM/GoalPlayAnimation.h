


/********************************************************************
	created:	2010/07/13
	created:	13:7:2010   14:44
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\GoalPlayAnimation.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	GoalPlayAnimation
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GoalPlayAnimation_h__
#define GoalPlayAnimation_h__


#include <IGoal.h>
#include <GenericValues.h>
#include <IGoalFactory.h>

namespace mray
{
namespace GameMod
{

	class CharacterObject;

class GoalPlayAnimation:public AI::IGoal
{
private:
protected:
	AI::EGoalStatus m_status;
	CharacterObject* m_character;
	StringValue m_animationName;
	FloatValue m_animationSpeed;

	animation::AnimationController* m_group;
public:
	GoalPlayAnimation(CharacterObject* character,const core::string& animationName,float speed);
	virtual ~GoalPlayAnimation();

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

class GoalPlayAnimationFactory:public AI::IGoalFactory
{
public:
	virtual core::string GetType(){
		return mT("PlayAnimation");
	}
	virtual AI::IGoal* CreateGoal(AI::IAIArchitect*arch);
};


}
}


#endif // GoalPlayAnimation_h__
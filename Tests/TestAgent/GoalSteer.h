
/********************************************************************
	created:	2010/04/04
	created:	4:4:2010   9:27
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent\GoalSteer.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent
	file base:	GoalSteer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GoalSteer___
#define ___GoalSteer___

#include <IGoal.h>
#include <IGoalEvaluator.h>

namespace mray{
namespace AI{
	class SteerBehavior;

class GoalSteer:public IGoal
{
private:
protected:
	EGoalStatus m_state;
	SteerBehavior* m_steer;
public:
	GoalSteer(SteerBehavior*steer);
	virtual~GoalSteer();

	virtual int GetGoalType();

	virtual void activate();
	virtual EGoalStatus process();
	virtual void terminate();

	virtual EGoalStatus getStatus();

	virtual bool isPrimitive(){return true;}

	virtual core::string getType(){return mT("GoalSteer");}

};

class GoalSteerEvaluator:public IGoalEvaluator
{
protected:
	SteerBehavior* m_steer;
	virtual float innerCalcDeserability();
public:
	GoalSteerEvaluator(SteerBehavior* steer);
	virtual void CreateGoal(GoalPipe*pipe);

	virtual core::string getInfo();
};

}
}


#endif //___GoalSteer___

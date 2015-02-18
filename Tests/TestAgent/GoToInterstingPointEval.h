
/********************************************************************
	created:	2010/04/04
	created:	4:4:2010   20:33
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent\GoToInterstingPointEval.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent
	file base:	GoToInterstingPointEval
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GoToInterstingPointEval___
#define ___GoToInterstingPointEval___

#include <IGoalEvaluator.h>

namespace mray{
namespace AI{

	class SteerBehavior;
	class PathFindPlanner;

class GoToInterstingPointEval:public IGoalEvaluator
{
protected:
	AIActor* m_actor;
	SteerBehavior* m_steer;
	PathFindPlanner* m_planner;
	virtual float innerCalcDeserability();
public:
	GoToInterstingPointEval(AIActor* actor);
	virtual void CreateGoal(GoalPipe*pipe);

	virtual core::string getInfo();

};

}
}

#endif //___GoToInterstingPointEval___

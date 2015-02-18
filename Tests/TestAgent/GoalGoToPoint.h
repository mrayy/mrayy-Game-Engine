
/********************************************************************
	created:	2010/04/04
	created:	4:4:2010   17:58
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent\GoalGoToPoint.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent
	file base:	GoalGoToPoint
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GoalGoToPoint___
#define ___GoalGoToPoint___


#include <GoalPipe.h>
#include <IGoalEvaluator.h>
#include <Path.h>

namespace mray{
namespace AI{

	class PathFindPlanner;
	class SteerBehavior;

class GoalGoToPoint:public GoalPipe
{
private:
protected:
	EGoalStatus m_state;
	AIActor* m_actor;
	PathFindPlanner* m_planner;
	SteerBehavior* m_steer;
	math::vector3d m_pos;

public:
	GoalGoToPoint(AIActor* actor,SteerBehavior* steer,PathFindPlanner*planner,const math::vector3d& pos);
	virtual~GoalGoToPoint();

	virtual int GetGoalType();
	virtual void activate();
	virtual EGoalStatus process();
	virtual void terminate();

	math::vector3d getTargetPoint(){return m_pos;}

	virtual EGoalStatus getStatus();


	virtual bool OnMessageTelegram(const AIMessageTelegram&msg);

	virtual core::string getType(){return mT("GoalGoToPoint");}
	virtual void Visualize(IRenderDevice*dev);
};

}
}


#endif //___GoalGoToPoint___


#include "stdafx.h"
#include "GoToInterstingPointEval.h"
#include "GoalsTypeEnum.h"
#include "InterstingPointManager.h"
#include "GoalGoToPoint.h"
#include "AIComponentTypes.h"
#include <GoalPipe.h>

namespace mray{
namespace AI{

GoToInterstingPointEval::GoToInterstingPointEval(AIActor* actor)
{
	m_actor=actor;
	m_steer=(SteerBehavior*)actor->GetController()->GetComponent(GetComponentName(ECT_Steering));
	m_planner=(PathFindPlanner*)actor->GetController()->GetComponent(GetComponentName(ECT_PathFinder));
}

float GoToInterstingPointEval::innerCalcDeserability()
{
	if(InterstingPointManager::getInstance().NewPointPresented())
		return 1;
	return 0;
}


void GoToInterstingPointEval::CreateGoal(GoalPipe*pipe)
{
	IGoal* goal=pipe->getSubGoal(EGoal_GoToPoint);
	math::vector3d p=InterstingPointManager::getInstance().GetInterstingPoint();
	if(goal){
		GoalGoToPoint* g=(GoalGoToPoint*)goal;
		if(g->getTargetPoint().getDistSQ(p)<0.5)
			return;
	}
	pipe->removeAllGoals();
	pipe->addSubGoal(new GoalGoToPoint(m_actor,m_steer,m_planner,p));
}

core::string GoToInterstingPointEval::getInfo()
{
	return mT("");
}

}
}
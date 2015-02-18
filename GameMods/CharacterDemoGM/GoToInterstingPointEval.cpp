
#include "stdafx.h"
#include "GoToInterstingPointEval.h"
#include "GoalsTypeEnum.h"
#include "GoalGoToPoint.h"
#include "AIComponentTypes.h"
#include "AIActor.h"
#include "IAIArchitect.h"
#include <GoalPipe.h>

namespace mray{
namespace GameMod{

GoToInterstingPointEval::GoToInterstingPointEval(AI::AIActor* actor)
{
	m_actor=actor;
	m_steer=(AI::SteerController*)actor->GetController()->GetComponent(GetComponentName(AI::ECT_Steering));
	m_planner=(AI::PathFindPlanner*)actor->GetController()->GetComponent(GetComponentName(AI::ECT_PathFinder));
}

float GoToInterstingPointEval::innerCalcDeserability()
{
	return 0;
}


void GoToInterstingPointEval::CreateGoal(AI::GoalPipe*pipe)
{
	/*
	IGoal* goal=pipe->getSubGoal(EGoal_GoToPoint);
	math::vector3d p=InterstingPointManager::getInstance().GetInterstingPoint();
	if(goal){
		GoalGoToPoint* g=(GoalGoToPoint*)goal;
		if(g->getTargetPoint().getDistSQ(p)<0.5)
			return;
	}
	pipe->removeAllGoals();
	pipe->addSubGoal(new GoalGoToPoint(m_actor,m_steer,m_planner,p));*/
}

core::string GoToInterstingPointEval::getInfo()
{
	return mT("");
}

}
}
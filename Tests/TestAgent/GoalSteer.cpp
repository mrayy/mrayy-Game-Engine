
#include "stdafx.h"
#include "GoalSteer.h"
#include "GoalsTypeEnum.h"
#include <SteerBehavior.h>
#include <GoalPipe.h>

namespace mray{
namespace AI{

GoalSteer::GoalSteer(SteerBehavior*steer)
{
	m_state=EGS_Inactive;
	m_steer=steer;
}
GoalSteer::~GoalSteer()
{
}

int GoalSteer::GetGoalType()
{
	return EGoal_Steer;
}

void GoalSteer::activate()
{
	m_state=EGS_Active;
	m_steer->wanderOn();
}
EGoalStatus GoalSteer::process()
{
	if(m_state!=EGS_Active)
		activate();
	return m_state;
}
void GoalSteer::terminate()
{
	m_steer->wanderOff();
}

EGoalStatus GoalSteer::getStatus()
{
	return m_state;
}

//////////////////////////////////////////////////////////////////////////

GoalSteerEvaluator::GoalSteerEvaluator(SteerBehavior* steer)
{
	m_steer=steer;
}

float GoalSteerEvaluator::innerCalcDeserability()
{
	return 1;
}


void GoalSteerEvaluator::CreateGoal(GoalPipe*pipe)
{
	if(pipe->isContainSubGoals())
		return;
	pipe->addSubGoal(new GoalSteer(m_steer));
}

core::string GoalSteerEvaluator::getInfo()
{
	return mT("");
}

}
}
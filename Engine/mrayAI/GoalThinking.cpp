#include "stdafx.h"

#include "GoalThinking.h"

#include "AIActor.h"
#include "AIComponentTypes.h"
#include "AIDefaultGoalsTypes.h"


namespace mray{
namespace AI{

GoalThinking::GoalThinking()
{
	m_state=EGS_Inactive;
}
GoalThinking::~GoalThinking(){
}

int GoalThinking::GetGoalType()
{
	return EGoal_Think;
}

EGoalStatus GoalThinking::getStatus()
{
	return m_state;
}

core::string GoalThinking::getType()
{
	return mT("GoalThinking");
}

EGoalStatus GoalThinking::process(){
	m_state=GoalPipe::process();
	return m_state;
}

}
}


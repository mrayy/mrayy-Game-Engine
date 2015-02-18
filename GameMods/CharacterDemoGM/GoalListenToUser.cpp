
#include "stdafx.h"
#include "GoalListenToUser.h"
#include "GoalsTypeEnum.h"



namespace mray
{
namespace GameMod
{

GoalListenToUser::GoalListenToUser(CharacterObject*character)
{
	m_character=character;
}
GoalListenToUser::~GoalListenToUser()
{
}

int GoalListenToUser::GetGoalType()
{
	return EGoal_ListenToUser;
}
void GoalListenToUser::activate()
{
}
AI::EGoalStatus GoalListenToUser::process()
{
	GoalPipe::process();
	return AI::EGS_Active;
}
void GoalListenToUser::terminate()
{
	GoalPipe::terminate();
}

AI::EGoalStatus GoalListenToUser::getStatus()
{
	return AI::EGS_Active;
}


bool GoalListenToUser::OnMessageTelegram(const AI::AIMessageTelegram&msg)
{
	return GoalPipe::OnMessageTelegram(msg);
}

void GoalListenToUser::Visualize(AI::IRenderDevice*dev,const core::string&prefix)
{
	GoalPipe::Visualize(dev,prefix);
}


}
}

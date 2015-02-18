
#include "stdafx.h"
#include "GoalWaitForChat.h"

#include "CharacterObject.h"

namespace mray
{
namespace GameMod
{

GoalWaitForChat::GoalWaitForChat(CharacterObject*character)
{
	m_character=character;
	m_state=AI::EGS_Inactive;
}
GoalWaitForChat::~GoalWaitForChat()
{
}

void GoalWaitForChat::pause()
{
	m_state=AI::EGS_Paused;
}
void GoalWaitForChat::resume()
{
	m_state=AI::EGS_Failed;
}

int GoalWaitForChat::GetGoalType()
{
	return 0;
}
void GoalWaitForChat::activate()
{
	m_state=AI::EGS_Active;
}
AI::EGoalStatus GoalWaitForChat::process()
{
	if(m_state==AI::EGS_Inactive)
		activate();

	if(!m_character->WaitingForChat())
		m_state=AI::EGS_Failed;

	return m_state;
}
void GoalWaitForChat::terminate()
{
}

AI::EGoalStatus GoalWaitForChat::getStatus()
{
	return m_state;
}


bool GoalWaitForChat::OnMessageTelegram(const AI::AIMessageTelegram&msg)
{
	return IGoal::OnMessageTelegram(msg);
}

void GoalWaitForChat::Visualize(AI::IRenderDevice*dev,const core::string&prefix)
{
	IGoal::Visualize(dev,prefix);
}


}
}


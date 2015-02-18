
#include "stdafx.h"
#include "GoalMakeChat.h"
#include "CharacterObject.h"

#include "GoalsTypeEnum.h"
#include "Randomizer.h"
#include "CFPS.h"


namespace mray
{
namespace GameMod
{

GoalMakeChat::GoalMakeChat(CharacterObject*character, float timeToChat)
:m_timeToChat(timeToChat),m_character(character),m_state(AI::EGS_Inactive)
{
}
GoalMakeChat::~GoalMakeChat()
{
}

int GoalMakeChat::GetGoalType()
{
	return EGoal_MakeChat;
}

void GoalMakeChat::pause()
{
	m_state=AI::EGS_Paused;
}
void GoalMakeChat::resume()
{
	activate();
}

void GoalMakeChat::activate()
{
	m_state=AI::EGS_Active;
	m_character->StartChat();
	m_character->GetTargetFriend()->StartListenToChat();
}
AI::EGoalStatus GoalMakeChat::process()
{
	if(m_state==AI::EGS_Inactive)
		activate();
	m_timeToChat-=gFPS.dt();
	if(m_timeToChat<=0)
		m_state=AI::EGS_Completed;
	return m_state;
}
void GoalMakeChat::terminate()
{
	m_character->GetTargetFriend()->EndChat();
	m_character->EndChat();
}

AI::EGoalStatus GoalMakeChat::getStatus()
{
	return m_state;
}


core::string GoalMakeChat::getType()
{
	return mT("GoalMakeChat");
}


}
}



#include "stdafx.h"
#include "GoalChatToFriend.h"
#include "CharacterObject.h"

#include "GoalsTypeEnum.h"

#include "AICharacterArch.h"
#include "GoalFindFriend.h"
#include "GoalMakeChat.h"

namespace mray
{
namespace GameMod
{

GoalChatToFriend::GoalChatToFriend(CharacterObject* character,float chat)
:m_timeToChat(mT("ChatLength"),chat),m_character(character),m_state(AI::EGS_Inactive)
{
	SetSequence(true);
	AddAttribute(&m_timeToChat);
}
GoalChatToFriend::~GoalChatToFriend()
{
}
int GoalChatToFriend::GetGoalType()
{
	return EGoal_ChatToFriend;
}

void GoalChatToFriend::pause()
{
	m_state=AI::EGS_Paused;
	removeAllGoals();
}
void GoalChatToFriend::resume()
{
	activate();
}

void GoalChatToFriend::activate()
{
	m_state=AI::EGS_Active;

	//adding goals like a stack
	addSubGoal(new GoalMakeChat(m_character,m_timeToChat.value)); // -2
	addSubGoal(new GoalFindFriend(m_character));		// -1

}
AI::EGoalStatus GoalChatToFriend::process()
{
	if(m_state==AI::EGS_Inactive)
		activate();
	if(m_state!=AI::EGS_Active)
		return m_state;

	m_state=GoalPipe::process();
	if(m_state==AI::EGS_Completed)
	{
	}
	return m_state;
}
void GoalChatToFriend::terminate()
{
}

AI::EGoalStatus GoalChatToFriend::getStatus()
{
	return m_state;
}


bool GoalChatToFriend::OnMessageTelegram(const AI::AIMessageTelegram&msg)
{
	return GoalPipe::OnMessageTelegram(msg);
}

void GoalChatToFriend::Visualize(AI::IRenderDevice*dev,const core::string&prefix)
{
	GoalPipe::Visualize(dev,prefix);
}


AI::IGoal* GoalChatToFriendFactory::CreateGoal(AI::IAIArchitect*arch)
{
	AICharacterArch*charArch=dynamic_cast<AICharacterArch*>(arch);
	if(!charArch)
		return 0;

	return new GoalChatToFriend(charArch->GetCharacterObject(),0);
}



}
}

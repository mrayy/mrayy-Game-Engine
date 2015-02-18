

#include "stdafx.h"
#include "GoalWait.h"
#include "GoalsTypeEnum.h"
#include "CFPS.h"
#include "CharacterObject.h"
#include "AICharacterArch.h"



namespace mray
{
namespace GameMod
{

GoalWait::GoalWait(CharacterObject* character,float waitTime,EWaitType chatType,bool animConst)
:m_timeToWait(mT("Time"),waitTime),m_chatType(chatType),m_character(character),m_animConst(animConst)
{
	m_status=AI::EGS_Inactive;
	AddAttribute(&m_timeToWait);
}
GoalWait::~GoalWait()
{
}

void GoalWait::pause()
{
	m_status=AI::EGS_Paused;
}
void GoalWait::resume()
{
	activate();
}

int GoalWait::GetGoalType()
{
	return EGoal_Wait;
}
void GoalWait::activate()
{
	m_status=AI::EGS_Active;
	if(m_animConst)
		m_character->SetAnimationConstrained(true);
}
AI::EGoalStatus GoalWait::process()
{
	if(m_status==AI::EGS_Inactive)
		activate();

	switch (m_chatType)
	{
	case EWT_Infinite:
		break;
	case EWT_Timed:
		m_timeToWait.value-=gFPS.dt();

		if(m_timeToWait.value<=0)
			m_status=AI::EGS_Completed;
		break;
	case EWT_ChatStart:
		if(m_character->IsChating())
			m_status=AI::EGS_Completed;
		break;
	case EWT_ChatEnd:
		if(!m_character->IsChating())
			m_status=AI::EGS_Completed;
		break;
	}
	return m_status;
}
void GoalWait::terminate()
{
	if(m_animConst)
		m_character->SetAnimationConstrained(false);
}


AI::EGoalStatus GoalWait::getStatus()
{
	return m_status;
}


bool GoalWait::OnMessageTelegram(const AI::AIMessageTelegram&msg)
{
	return false;
}

core::string GoalWait::getType()
{
	return mT("GoalWait");
}

//////////////////////////////////////////////////////////////////////////
AI::IGoal* GoalWaitFactory::CreateGoal(AI::IAIArchitect*arch)
{
	AICharacterArch*charArch=dynamic_cast<AICharacterArch*>(arch);
	if(!charArch)
		return 0;

	return new GoalWait(charArch->GetCharacterObject());
}


}
}

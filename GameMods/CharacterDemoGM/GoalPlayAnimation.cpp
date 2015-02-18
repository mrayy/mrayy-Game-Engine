

#include "stdafx.h"
#include "GoalPlayAnimation.h"

#include "GoalsTypeEnum.h"
#include "CFPS.h"
#include "CharacterObject.h"
#include "AICharacterArch.h"


namespace mray
{
namespace GameMod
{

GoalPlayAnimation::GoalPlayAnimation(CharacterObject* character,const core::string& animationName,float speed)
:m_animationName(mT("Time"),animationName),m_animationSpeed(mT("Speed"),speed),m_character(character)
{
	m_status=AI::EGS_Inactive;
	AddAttribute(&m_animationName);
	AddAttribute(&m_animationSpeed);
	m_group=0;
}
GoalPlayAnimation::~GoalPlayAnimation()
{
}

void GoalPlayAnimation::pause()
{
	m_status=AI::EGS_Paused;
}
void GoalPlayAnimation::resume()
{
	activate();
}

int GoalPlayAnimation::GetGoalType()
{
	return EGoal_PlayAnimation;
}
void GoalPlayAnimation::activate()
{
	m_status=AI::EGS_Active;
	m_character->SetAnimationConstrained(true);
	m_character->PlayAnimation(m_animationName.value,m_animationSpeed.value);
	m_group=m_character->GetAnimationGroup(m_animationName.value);
	if(!m_group)
	{
		m_status=AI::EGS_Failed;
	}else
	{
		m_group->setAnimationMode(animation::EAM_Clamp);
		m_group->setAnimationDirection(1);
		m_group->setTime(0);
	}
}
AI::EGoalStatus GoalPlayAnimation::process()
{
	if(m_status==AI::EGS_Inactive)
		activate();

	if(m_status!=AI::EGS_Active)
		return m_status;

	if(!m_group->isPlaying())
		m_status=AI::EGS_Completed;
	return m_status;
}
void GoalPlayAnimation::terminate()
{
	m_character->SetAnimationConstrained(false);
	m_group=0;
}

AI::EGoalStatus GoalPlayAnimation::getStatus()
{
	return m_status;
}

bool GoalPlayAnimation::OnMessageTelegram(const AI::AIMessageTelegram&msg)
{
	return false;
}

core::string GoalPlayAnimation::getType()
{
	return mT("GoalPlayAnimation");
}

AI::IGoal* GoalPlayAnimationFactory::CreateGoal(AI::IAIArchitect*arch)
{
	AICharacterArch*charArch=dynamic_cast<AICharacterArch*>(arch);
	if(!charArch)
		return 0;

	return new GoalPlayAnimation(charArch->GetCharacterObject(),mT(""),1);
}



}
}



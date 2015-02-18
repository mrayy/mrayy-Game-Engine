
#include "stdafx.h"
#include "GoalFindFriend.h"
#include "CharacterObject.h"

#include "GoalsTypeEnum.h"
#include "CHouse.h"
#include "Randomizer.h"
#include "GoalGoToPoint.h"
#include "IAIPhysics.h"

#include "AICharacterArch.h"

#include "VisionSensor.h"
#include "AIComponentTypes.h"
#include "GoalPlayAnimation.h"

namespace mray
{
namespace GameMod
{

GoalFindFriend::GoalFindFriend(CharacterObject* character)
	:m_friend(0),m_character(character),m_state(AI::EGS_Inactive),m_requestMade(false)
{
	m_vision=dynamic_cast<AI::VisionSensor*>(character->GetAIActor()->GetController()->GetComponent(AI::GetComponentName(AI::ECT_Vision)));
}
GoalFindFriend::~GoalFindFriend()
{
}
int GoalFindFriend::GetGoalType()
{
	return EGoal_GoToFriend;
}

void GoalFindFriend::pause()
{
	m_state=AI::EGS_Paused;
	removeAllGoals();
}
void GoalFindFriend::resume()
{
	activate();
}

AI::EGoalStatus GoalFindFriend::CheckForFriends()
{
	if(m_vision && !m_requestMade)
	{
		const AI::AIActorList& actors= m_vision->GetActorsList();
		AI::AIActorList::const_iterator it=actors.begin();
		for(;it!=actors.end();++it)
		{
			AI::AIActor* actor=*it;
			AICharacterArch* arch=dynamic_cast<AICharacterArch*>(actor->GetController());
			if(arch)
			{
				if(arch->GetCharacterObject()==m_friend)
				{
					if(!m_friend->RequestToTalk(m_character))
					{
						return AI::EGS_Failed;
					}
					m_requestMade=true;
					m_character->SetTargetFriend(m_friend);
					m_character->LookAtTarget();
					removeAllGoals();
					m_state=AI::EGS_Active;
					//request to talk was successfull, animate shout
					addSubGoal(new GoalGoToPoint(m_character,m_friend->GetPhysics()->GetGlobalPosition()));
					addSubGoal(new GoalPlayAnimation(m_character,mT("Shout"),25));
				}
			}
		}
	}

	return m_state;
}

void GoalFindFriend::activate()
{
	const CharactersList& characters= CHouse::getInstance().GetCharacters();

	if(characters.size()<=1){
		m_state=AI::EGS_Failed;
		return;
	}

	int charID=math::Randomizer::rand(characters.size());

	CharactersList::const_iterator it=characters.begin();

	m_friend=0;
	for (int i=0;i<charID;++it,++i);

	if(it==characters.end()){
		m_state=AI::EGS_Failed;
		return;
	}

	m_friend=*it;

	if(m_friend==m_character)
	{
		++it;
		if(it==characters.end())
			it=characters.begin();
		m_friend=*it;
	}

	if(m_friend==0)
	{
		m_state=AI::EGS_Failed;
	}else
	{
		m_state=AI::EGS_Active;
		addSubGoal(new GoalGoToPoint(m_character,m_friend->GetPhysics()->GetGlobalPosition()));
	}
}
AI::EGoalStatus GoalFindFriend::process()
{
	if(m_state==AI::EGS_Inactive)
		activate();
	if(m_state!=AI::EGS_Active)
		return m_state;


	m_state=GoalPipe::process();
	m_state=CheckForFriends();

	if(m_state==AI::EGS_Completed)
	{
	}
	return m_state;
}
void GoalFindFriend::terminate()
{
}

AI::EGoalStatus GoalFindFriend::getStatus()
{
	return m_state;
}


bool GoalFindFriend::OnMessageTelegram(const AI::AIMessageTelegram&msg)
{
	return GoalPipe::OnMessageTelegram(msg);
}

void GoalFindFriend::Visualize(AI::IRenderDevice*dev,const core::string&prefix)
{
	GoalPipe::Visualize(dev,prefix);
}

}
}

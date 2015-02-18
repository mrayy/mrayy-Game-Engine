
#include "stdafx.h"
#include "GoalGoToObject.h"
#include "GoalsTypeEnum.h"

#include "AICharacterArch.h"

namespace mray
{
namespace GameMod
{

GoalGoToObject::GoalGoToObject(CharacterObject* character):
m_objectType(mT("Object"),mT(""))
{
	AddAttribute(&m_objectType);
	m_state=AI::EGS_Inactive;
}

GoalGoToObject::~GoalGoToObject()
{
}


void GoalGoToObject::pause()
{
	m_state=AI::EGS_Paused;
	removeAllGoals();
}

void GoalGoToObject::resume()
{
	activate();
}


int GoalGoToObject::GetGoalType()
{
	return EGoal_GoToObject;
}

void GoalGoToObject::activate()
{
	if(m_objectType.value==mT(""))
	{
		m_state=AI::EGS_Completed;
	}else
	{
	}
}

AI::EGoalStatus GoalGoToObject::process()
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

void GoalGoToObject::terminate()
{
}


AI::EGoalStatus GoalGoToObject::getStatus()
{
	return m_state;

}



bool GoalGoToObject::OnMessageTelegram(const AI::AIMessageTelegram&msg)
{
	return GoalPipe::OnMessageTelegram(msg);
}


void GoalGoToObject::Visualize(AI::IRenderDevice*dev,const core::string&prefix)
{
	GoalPipe::Visualize(dev,prefix);
}



AI::IGoal* GoalGoToObjectFactory::CreateGoal(AI::IAIArchitect*arch)
{
	AICharacterArch*charArch=dynamic_cast<AICharacterArch*>(arch);
	if(!charArch)
		return 0;

	return new GoalGoToObject(charArch->GetCharacterObject());
}


}
}


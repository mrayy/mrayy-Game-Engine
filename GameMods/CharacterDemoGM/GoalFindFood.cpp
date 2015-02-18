
#include "stdafx.h"
#include "GoalFindFood.h"

#include "GoalsTypeEnum.h"

#include "CHouse.h"
#include "GoalGoToPoint.h"
#include "CharacterObject.h"

#include "AICharacterArch.h"

namespace mray
{
namespace GameMod
{

GoalFindFood::GoalFindFood(CharacterObject*character,float amount,EFoodType requiredType)
:m_requiredAmountOfFood(mT("Amount"),amount),m_requiredType(mT("Type"),requiredType)
{
	m_character=character;
	m_state=AI::EGS_Inactive;
	m_food=0;

	AddAttribute(&m_requiredType);
	AddAttribute(&m_requiredAmountOfFood);
}
GoalFindFood::~GoalFindFood()
{
}

int GoalFindFood::GetGoalType()
{
	return EGoal_FindFood;
}
void GoalFindFood::pause()
{
	m_state=AI::EGS_Paused;
	removeAllGoals();
}
void GoalFindFood::resume()
{
	activate();
}
void GoalFindFood::activate()
{
	const FoodList& foodList= CHouse::getInstance().GetFoods();
	
	FoodList::const_iterator it=foodList.begin();

	m_food=0;
	for (;it!=foodList.end();++it)
	{
		CFood* f=*it;
		if(f->GetAmount()>0 && f->GetFoodType()==m_requiredType.value)
		{
			m_food=f;
			break;
		}
	}
	if(m_food==0)
	{
		m_state=AI::EGS_Failed;
	}else
	{
		m_character->SetBusy(true);
		m_state=AI::EGS_Active;
		addSubGoal(new GoalGoToPoint(m_character,m_food->GetEntity()->GetShape()->getPosition()));
	}
}
AI::EGoalStatus GoalFindFood::process()
{
	if(m_state==AI::EGS_Inactive)
		activate();
	if(m_state!=AI::EGS_Active)
		return m_state;

	m_state=GoalPipe::process();
	if(m_state==AI::EGS_Completed)
	{
		m_character->Eat(m_food);
	}
	return m_state;
}
void GoalFindFood::terminate()
{

	m_character->SetBusy(false);
}

AI::EGoalStatus GoalFindFood::getStatus()
{
	return m_state;
}


bool GoalFindFood::OnMessageTelegram(const AI::AIMessageTelegram&msg)
{
	return GoalPipe::OnMessageTelegram(msg);
}

void GoalFindFood::Visualize(AI::IRenderDevice*dev,const core::string&prefix)
{
	GoalPipe::Visualize(dev,prefix);
}

//////////////////////////////////////////////////////////////////////////
AI::IGoal* GoalFindFoodFactory::CreateGoal(AI::IAIArchitect*arch)
{
	AICharacterArch*charArch=dynamic_cast<AICharacterArch*>(arch);
	if(!charArch)
		return 0;

	return new GoalFindFood(charArch->GetCharacterObject(),0,EFT_Meat);
}



}
}

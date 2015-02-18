
#include "stdafx.h"
#include "GoalGoToPoint.h"
#include "PathFindPlanner.h"
//#include "GoalSteer.h"
#include "GoalFollowPath.h"
#include "GoalsTypeEnum.h"
#include "GoalWait.h"
#include "CharacterObject.h"
#include <AIMessages.h>
#include <IRenderDevice.h>
#include "ILogManager.h"

namespace mray{
namespace GameMod{


GoalGoToPoint::GoalGoToPoint(CharacterObject*character, const math::vector3d& pos)
{
	m_character=character;
	m_state=AI::EGS_Inactive;
	m_pos=pos;
}

GoalGoToPoint::~GoalGoToPoint()
{

}

int GoalGoToPoint::GetGoalType()
{
	return EGoal_GoToPoint;
}

void GoalGoToPoint::pause()
{
	m_state=AI::EGS_Paused;
	removeAllGoals();
}
void GoalGoToPoint::resume()
{
	activate();
}

void GoalGoToPoint::activate()
{
	m_state=AI::EGS_Active;
	if(!m_character->GetPathfinder()->IsRequsting() || m_character->GetPathfinder()->GetDestination().getDistSQ(m_pos)>0.5){
		m_character->GetPathfinder()->RequestPathToPosition(m_pos,false,0);
		addSubGoal(new GoalWait(m_character));
	}
/*	if(m_planner->GetSearchResult()!=ESR_NotFound)
		addSubGoal(new GoalFollowPath(m_actor,m_steer,m_planner->GetPath()));
	*/
}

AI::EGoalStatus GoalGoToPoint::process()
{
	if(m_state==AI::EGS_Inactive)
		activate();
	if(m_state!=AI::EGS_Active)
		return m_state;
	m_state= AI::GoalPipe::process();
	if(m_state==AI::EGS_Failed)
		activate();
	if(m_state==AI::EGS_Completed)
	{
		/*m_character->StopAndIdle();*/
	}
	return m_state;
}

void GoalGoToPoint::terminate()
{
	GoalPipe::terminate();
}


AI::EGoalStatus GoalGoToPoint::getStatus()
{
	return m_state;
}

bool GoalGoToPoint::OnMessageTelegram(const AI::AIMessageTelegram&msg)
{
	bool ret=false;
	if(msg.message==AI::EDAM_PathFound)
	{
		removeAllGoals();
		addSubGoal(new GoalFollowPath(m_character->GetAIActor(),m_character->GetSteering(),m_character->GetPathfinder()->GetPath()));
		ret=true;
		m_state=AI::EGS_Active;
		//gLogManager.log(mT("Path Found"),ELL_SUCCESS);
	}else if(msg.message==AI::EDAM_PathNotFound)
	{
		gLogManager.log(mT("Failed to find path"),ELL_WARNING);
		m_character->StopAndIdle();
		m_state=AI::EGS_Failed;
		ret=true;
	}else
		ret = GoalPipe::OnMessageTelegram(msg);
	return ret;
}

void GoalGoToPoint::Visualize(AI::IRenderDevice*dev,const core::string&prefix)
{
	math::line3d l(m_pos,m_pos);
	l.pStart.x-=2;
	l.pEnd.x+=2;
	l.pStart.y+=1;
	l.pEnd.y+=1;
	dev->drawLine(l,255);
	
	l.pStart=m_pos;
	l.pEnd=m_pos;
	l.pStart.z-=2;
	l.pEnd.z+=2;
	l.pStart.y+=1;
	l.pEnd.y+=1;
	dev->drawLine(l,255);

	AI::GoalPipe::Visualize(dev,prefix);
}

}
}
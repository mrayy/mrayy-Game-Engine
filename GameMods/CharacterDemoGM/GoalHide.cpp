
#include "stdafx.h"
#include "GoalHide.h"
#include "GoalsTypeEnum.h"
#include "GoalGoToPoint.h"
#include <TacticalPointManager.h>

#include "GameGlobals.h"
#include "AIComponentTypes.h"

namespace mray
{
namespace GameMod
{

GoalHide::GoalHide(AI::AIActor* actor,const math::vector3d& target)
{
	m_actor=actor;
	SetTarget(target);
	m_status=AI::EGS_Inactive;
}
GoalHide::~GoalHide()
{
}

int GoalHide::GetGoalType()
{
	return EGoal_Hide;
}
void GoalHide::activate()
{
	core::array<AI::TacticalPoint*> candPoints;

	core::BitVector targetVis,actorVis,candidates;
	/*
	Game::GameGlobals::g_tacticalManager->GetVisibleNodesVec(m_targetNode,m_target,targetVis);
	Game::GameGlobals::g_tacticalManager->GetVisibleNodesVec(m_actor->GetWorldNode(),m_actor->GetPosition(),actorVis);
	actorVis=Game::GameGlobals::g_tacticalManager->ExtendVisibility(actorVis);
	candidates=(!targetVis) & actorVis;

	Game::GameGlobals::g_tacticalManager->GetNodesFromVector(candidates,candPoints);
	AI::TacticalPoint* best=0;
	float bdist=math::Infinity;

	math::vector3d pos=m_actor->GetPosition();

	for (int i=0;i<candPoints.size();++i)
	{
		float d=pos.getDistSQ(candPoints[i]->pos);
		if(d<bdist)
		{
			bdist=d;
			best=candPoints[i];
		}
	}
	if(best){
		addSubGoal(new GoalGoToPoint(m_actor,(SteerBehavior*)m_actor->GetController()->GetComponent(GetComponentName(ECT_Steering))
			,(PathFindPlanner*)m_actor->GetController()->GetComponent(GetComponentName(ECT_PathFinder)),best->pos));
		m_status=EGS_Active;
	}else
		m_status=EGS_Failed;*/
}
AI::EGoalStatus GoalHide::process()
{
	if(m_status==AI::EGS_Inactive)
	{
		activate();
	}
	m_status=AI::GoalPipe::process();
	if(m_status!=AI::EGS_Active)
	{
		m_status=AI::EGS_Completed;
	}
	return m_status;
}
void GoalHide::terminate()
{
	GoalPipe::terminate();
	m_status=AI::EGS_Inactive;
}

AI::EGoalStatus GoalHide::getStatus()
{
	return m_status;
}
void GoalHide::SetTarget(const math::vector3d&t)
{
	/*
	AI::NavWorldNodeListPtr tmp;
	m_target=t;
	m_targetNode=Game::GameGlobals::g_system->GetNavigationWorld()->QueryPolys(m_target,tmp);
	removeAllGoals();
	m_status=EGS_Inactive;*/
}


}
}

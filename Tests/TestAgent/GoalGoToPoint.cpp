
#include "stdafx.h"
#include "GoalGoToPoint.h"
#include "PathFindPlanner.h"
#include "GoalSteer.h"
#include "GoalFollowPath.h"
#include "GoalsTypeEnum.h"
#include <AIMessages.h>
#include <IRenderDevice.h>

namespace mray{
namespace AI{

	class GoalWait:public IGoal
	{
	public:
		GoalWait(){}
		virtual~GoalWait(){}

		virtual int GetGoalType(){return EGoal_Wait;}
		virtual void activate(){}
		virtual EGoalStatus process(){return EGS_Active;}
		virtual void terminate(){}


		virtual EGoalStatus getStatus(){return EGS_Active;}


		virtual bool OnMessageTelegram(const AIMessageTelegram&msg){return false;}

		virtual core::string getType(){return mT("GoalWait");}
		bool isPrimitive(){return true;}
	};

GoalGoToPoint::GoalGoToPoint(AIActor* actor,SteerBehavior* steer,PathFindPlanner*planner,
							 const math::vector3d& pos)
{
	m_actor=actor;
	m_state=EGS_Inactive;
	m_planner=planner;
	m_pos=pos;
	m_steer=steer;
}

GoalGoToPoint::~GoalGoToPoint()
{

}

int GoalGoToPoint::GetGoalType()
{
	return EGoal_GoToPoint;
}


void GoalGoToPoint::activate()
{
	m_state=EGS_Active;
	if(!m_planner->IsRequsting() || m_planner->GetDestination().getDistSQ(m_pos)>0.5){
		m_planner->RequestPathToPosition(m_pos,false,0);
		addSubGoal(new GoalWait());
	}
/*	if(m_planner->GetSearchResult()!=ESR_NotFound)
		addSubGoal(new GoalFollowPath(m_actor,m_steer,m_planner->GetPath()));
	*/
}

EGoalStatus GoalGoToPoint::process()
{
	if(m_state==EGS_Inactive)
		activate();
	m_state= GoalPipe::process();
	if(m_state==EGS_Failed)
		activate();
	return m_state;
}

void GoalGoToPoint::terminate()
{
	GoalPipe::terminate();
}


EGoalStatus GoalGoToPoint::getStatus()
{
	return m_state;
}

bool GoalGoToPoint::OnMessageTelegram(const AIMessageTelegram&msg)
{
	bool ret=false;
	if(msg.message==EDAM_PathFound)
	{
		removeAllGoals();
		addSubGoal(new GoalFollowPath(m_actor,m_steer,m_planner->GetPath()));
		ret=true;
		m_state=EGS_Active;
	}else if(msg.message==EDAM_PathNotFound)
	{
		removeAllGoals();
		m_state=EGS_Failed;
		ret=true;
	}else
		ret = GoalPipe::OnMessageTelegram(msg);
	return ret;
}

void GoalGoToPoint::Visualize(IRenderDevice*dev)
{
	GoalPipe::Visualize(dev);
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
}

}
}
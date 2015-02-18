
#include "stdafx.h"
#include "GoalFollowPath.h"
#include "GoalTraverseEdge.h"
#include "GoalsTypeEnum.h"
#include <IRenderDevice.h>
#include <DefaultColors.h>
#include <INavWorld.h>


namespace mray{
namespace GameMod{

GoalFollowPath::GoalFollowPath(AI::AIActor* actor,AI::SteerController* steer,const AI::Path& p)
	:m_path(p),m_actor(actor)
{
	m_steer=steer;

	AI::NavWorldNodeListPtr plist;
	AI::PathEdgeList::iterator it=m_path.GetPath().begin();
	for (;it!=m_path.GetPath().end();++it)
	{
		m_nodes.push_back(m_actor->GetNavWorld()->QueryPolys((*it).GetEnd(),plist));
	}

	m_state=AI::EGS_Inactive;
	m_currNode=0;
}
GoalFollowPath::~GoalFollowPath()
{
}

int GoalFollowPath::GetGoalType()
{
	return EGoal_FollowPath;
}

void GoalFollowPath::pause()
{
	m_state=AI::EGS_Paused;
	GoalPipe::pause();
}
void GoalFollowPath::activate()
{
	m_state=AI::EGS_Active;
	m_currEdge=m_path.GetPath().front();
	m_currNode=m_nodes.front();
	m_path.GetPath().pop_front();
	m_nodes.pop_front();
	addSubGoal(new GoalTraverseEdge(m_steer,m_currEdge,m_path.GetPath().empty()));
}
AI::EGoalStatus GoalFollowPath::process()
{
	m_state=processSubGoals();
	if(m_state==AI::EGS_Completed && !m_path.GetPath().empty()){
		activate();
	}
	math::vector3d pos=m_steer->getPhysics()->GetGlobalPosition();
	//check all points starting from the last point if the agent can see or not
	const AI::PathEdgeList& edges=m_path.GetPath();
	bool see = false;
	if(!edges.empty()){
		const AI::PathEdge& e=edges.front();
		AI::INavWorldNode* node=m_nodes.front();
		if(e.GetAttribute()==m_currEdge.GetAttribute()){
			see=m_actor->GetNavWorld()->lineOfSightTest(pos,m_actor->GetWorldNode(),e.GetEnd(),node,0);
			if(see){
				removeAllGoals();
				activate();
			}
		}
		/*
		PathEdgeList::const_iterator it=edges.end();
		WorldNodesList::iterator wit=m_nodes.end();
		for (--it,--wit;;--it,--wit)
		{
			const PathEdge& e=*it;
			see=m_actor->GetNavWorld()->lineOfSightTest(pos,m_actor->GetWorldNode(),e.end,*wit,0);
			if(see){
				//clear all nodes before this
				while (m_path.GetPath().begin()!=it)
				{
					m_path.GetPath().pop_front();
				}
				removeAllGoals();
				activate();
				break;
			}
			if(it==edges.begin())
				break;
		}*/
	}
	if(!see){
		see=m_actor->GetNavWorld()->lineOfSightTest(pos,m_actor->GetWorldNode(),m_currEdge.GetEnd(),m_currNode,0);
		if(!see)
			m_state=AI::EGS_Failed;
	}
	return m_state;
}
void GoalFollowPath::terminate()
{
	AI::GoalPipe::terminate();
} 

AI::EGoalStatus GoalFollowPath::getStatus()
{
	return m_state;
}


void GoalFollowPath::Visualize(AI::IRenderDevice*dev,const core::string&prefix)
{
	math::line3d l;
	AI::PathEdgeList::const_iterator it= m_path.GetPath().begin();
	AI::PathEdgeList::const_iterator end= m_path.GetPath().end();

	for (;it!=end;++it)
	{
		const AI::PathEdge& e=*it;
		l.pStart=e.GetBegin();
		l.pEnd=e.GetEnd();
		l.pStart.y+=1;
		l.pEnd.y+=1;
		dev->drawLine(l,video::DefaultColors::White);
	}
	AI::GoalPipe::Visualize(dev,prefix);

}



}
}
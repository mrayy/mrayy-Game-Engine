
#include "stdafx.h"
#include "GoalFollowPath.h"
#include "GoalTraverseEdge.h"
#include "GoalsTypeEnum.h"
#include <IRenderDevice.h>
#include <DefaultColors.h>


namespace mray{
namespace AI{

GoalFollowPath::GoalFollowPath(AIActor* actor,SteerBehavior* steer,const Path& p)
	:m_path(p),m_actor(actor)
{
	m_steer=steer;

	NavWorldNodeListPtr plist;
	PathEdgeList::iterator it=m_path.GetPath().begin();
	for (;it!=m_path.GetPath().end();++it)
	{
		m_nodes.push_back(m_actor->GetNavWorld()->QueryPolys((*it).end,plist));
	}
	
	m_currNode=0;
}
GoalFollowPath::~GoalFollowPath()
{
}

int GoalFollowPath::GetGoalType()
{
	return EGoal_FollowPath;
}

void GoalFollowPath::activate()
{
	m_state=EGS_Active;
	m_currEdge=m_path.GetPath().front();
	m_currNode=m_nodes.front();
	m_path.GetPath().pop_front();
	m_nodes.pop_front();
	addSubGoal(new GoalTraverseEdge(m_steer,m_currEdge,m_path.GetPath().empty()));
}
EGoalStatus GoalFollowPath::process()
{
	m_state=processSubGoals();
	if(m_state==EGS_Completed && !m_path.GetPath().empty()){
		activate();
	}
	math::vector3d pos=m_steer->getPhysics()->GetGlobalPosition();
	//check all points starting from the last point if the agent can see or not
	const PathEdgeList& edges=m_path.GetPath();
	bool see = false;
	if(!edges.empty()){
		const PathEdge& e=edges.front();
		INavWorldNode* node=m_nodes.front();
		if(e.attributes==m_currEdge.attributes){
			see=m_actor->GetNavWorld()->lineOfSightTest(pos,m_actor->GetWorldNode(),e.end,node,0);
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
		see=m_actor->GetNavWorld()->lineOfSightTest(pos,m_actor->GetWorldNode(),m_currEdge.end,m_currNode,0);
		if(!see)
			m_state=EGS_Failed;
	}
	return m_state;
}
void GoalFollowPath::terminate()
{
	GoalPipe::terminate();
} 

EGoalStatus GoalFollowPath::getStatus()
{
	return m_state;
}


void GoalFollowPath::Visualize(IRenderDevice*dev)
{
	math::line3d l;
	PathEdgeList::const_iterator it= m_path.GetPath().begin();
	PathEdgeList::const_iterator end= m_path.GetPath().end();

	for (;it!=end;++it)
	{
		const PathEdge& e=*it;
		l.pStart=e.begin;
		l.pEnd=e.end;
		l.pStart.y+=1;
		l.pEnd.y+=1;
		dev->drawLine(l,video::DefaultColors::White);
	}
	GoalPipe::Visualize(dev);

}



}
}
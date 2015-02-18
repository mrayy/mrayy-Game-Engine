#include "stdafx.h"

#include "PathFindPlanner.h"
#include "IGraphSearch.h"
#include "AIActor.h"
#include "INavWorld.h"
#include "AIMessages.h"
#include "AISystem.h"
#include "PathFindingManager.h"
#include "IAIPhysics.h"
#include "MessageDispatcher.h"
#include "AIComponentTypes.h"

namespace mray{
namespace AI{

PathFindPlanner::PathFindPlanner(AIActor* owner):m_owner(owner)
{
	m_searchPlanner = m_owner->GetNavWorld()->CreatePathFinder();
	m_destNode=0;
	m_costFunc=0;
	m_isRequesting=0;
	m_searchResult=ESR_NotFound;
}

PathFindPlanner::~PathFindPlanner()
{
	delete m_searchPlanner;
	delete m_costFunc;
}
void PathFindPlanner::SetCostFunction(ICostFunction* f)
{
	delete m_costFunc;
	m_costFunc=f;
}

bool PathFindPlanner::IsRequsting()
{
	return m_isRequesting;
}
const math::vector3d& PathFindPlanner::GetDestination()
{
	return m_destination;
}
void PathFindPlanner::OnPathFound()
{
	m_path.Clear();
	m_owner->GetNavWorld()->OptimizePath(m_owner->GetPhysicalProp()->GetGlobalPosition(),m_owner->GetWorldNode(),
		m_destination,m_destNode,m_searchPlanner->GetPath(),m_path.GetPath());
}
bool PathFindPlanner::RequestPathToNodePos(const math::vector3d&startPos,INavWorldNode*startNode,
						  const math::vector3d&endPos,INavWorldNode*endNode,bool immediat,int preSteps)
{
	m_owner->GetAISystem()->GetPathFindingManager()->RemovePathfindRequest(this);
	m_destination=endPos;
	m_destNode=endNode;
	m_isRequesting=true;

	if(immediat){
		ESearchResult res=m_searchPlanner->Search(startNode->GetGraphNode(),
			m_destNode->GetGraphNode(),0);
		if(res==ESR_FullFound){
			OnPathFound();
			return true;
		}
		else
			return false;
	}else{
		m_searchPlanner->SetupForNewSearch(startNode->GetNearestGraphNode(startPos),m_destNode->GetNearestGraphNode(endPos),m_costFunc);
		if(preSteps>0){
			for (int i=0;i<preSteps;++i)
			{
				if(Process())
					break;
			}
			if(m_searchPlanner->GetLastState()!=ESR_NotFound){
				m_searchPlanner->GeneratePath();
				OnPathFound();
			}
		}
		if(m_searchPlanner->GetLastState()==ESR_NotComplete)
			m_owner->GetAISystem()->GetPathFindingManager()->AddPathfindRequest(this);
	}
	return true;
}

bool PathFindPlanner::RequestPathToPosition(const math::vector3d& pos,bool immediat,int preSteps)
{
	AI::NavWorldNodeListPtr tmp;
	math::vector3d start=m_owner->GetPhysicalProp()->GetGlobalPosition();
//	m_owner->UpdateWorldNode();
	INavWorldNode*startNode= m_owner->GetWorldNode();
	if(!startNode){
		return false;//cann't retrive owner's world node..
	}
	m_destNode= m_owner->GetNavWorld()->QueryPolys(pos,tmp);
	if(!m_destNode){
		return false;//cann't target's world node..
	}

	return RequestPathToNodePos(start,startNode,pos,m_destNode,immediat,preSteps);
}

bool PathFindPlanner::RequestPathToActor(AIActor*actor,bool immediat,int preSteps)
{
	//m_owner->UpdateWorldNode();
	INavWorldNode*startNode= m_owner->GetWorldNode();
	if(!startNode){
		return false;//cann't retrive owner's world node..
	}
	//actor->UpdateWorldNode();
	INavWorldNode*destNode= actor->GetWorldNode();
	if(!startNode){
		return false;//cann't retrive owner's world node..
	}

	return RequestPathToNodePos(m_owner->GetPhysicalProp()->GetGlobalPosition(),startNode,
		actor->GetPhysicalProp()->GetGlobalPosition(),destNode,immediat,preSteps);
}

bool PathFindPlanner::Process()
{
	if(!m_searchPlanner)
		return true;

	m_searchResult= m_searchPlanner->DoSearchStep();
	if(m_searchResult==ESR_FullFound){
		m_isRequesting=false;
		OnPathFound();
		m_owner->GetAISystem()->
			GetMessageDispatcher()->DispatchMessage(0,m_owner,0,EDAM_PathFound,0,0);
		return true;
	}
	if(m_searchResult==ESR_NotFound){
		m_isRequesting=false;
		m_owner->GetAISystem()->
			GetMessageDispatcher()->DispatchMessage(0,m_owner,0,EDAM_PathNotFound,0,0);
		return true;
	}

	//search not completed
	return false;
}

ESearchResult PathFindPlanner::Process_MT()
{
	if(!m_searchPlanner)
		return ESR_NotFound;
	m_searchResult= m_searchPlanner->DoSearchStep();
	return m_searchResult;
}
void PathFindPlanner::OnSearchDone()
{
	if(m_searchResult==ESR_FullFound){
		m_isRequesting=false;
		OnPathFound();
		m_owner->GetAISystem()->
			GetMessageDispatcher()->DispatchMessage(0,m_owner,0,EDAM_PathFound,0,0);
	}
	if(m_searchResult==ESR_NotFound){
		m_isRequesting=false;
		m_owner->GetAISystem()->
			GetMessageDispatcher()->DispatchMessage(0,m_owner,0,EDAM_PathNotFound,0,0);
	}

}

AI::Path& PathFindPlanner::GetPath()
{
	return m_path;
}
ESearchResult PathFindPlanner::GetSearchResult()
{
	return m_searchPlanner->GetLastState();
}


}
}

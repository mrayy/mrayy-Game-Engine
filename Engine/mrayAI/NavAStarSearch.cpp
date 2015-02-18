#include "stdafx.h"

#include "NavAStarSearch.h"
#include "NavMesh.h"


namespace mray{
namespace AI{ 

NavAStarSearch::NavAStarSearch(NavMesh*world):
	m_world(world),m_begin(0),m_end(0),m_costFunc(0)
{
	m_costs.resize(m_world->GetGraphNodes().size());
	m_actualCosts.resize(m_world->GetGraphNodes().size());
	m_closeListDirect.resize(m_world->GetGraphNodes().size());
	m_openListDirect.resize(m_world->GetGraphNodes().size());
	m_parents.resize(m_world->GetGraphNodes().size());
}
NavAStarSearch::~NavAStarSearch()
{
}


void NavAStarSearch::cleanUp()
{
	m_costs.assign(m_costs.size(),-1);
	m_actualCosts.assign(m_actualCosts.size(),-1);
	m_closeListDirect.assign(m_closeListDirect.size(),0);
	m_openListDirect.assign(m_openListDirect.size(),0);
	m_parents.assign(m_openListDirect.size(),0);
	m_openList.clear();
	m_begin=0;
	m_end=0;
	m_costFunc=0;
}

void NavAStarSearch::insertToList(GraphNodesList&l,IGraphNode*node)
{
	float cost=m_actualCosts[node->getID()];
	GraphNodesList::iterator it=l.begin();
	for(;it!=l.end();++it)
	{
		IGraphNode*p=*it;
		float c=m_actualCosts[p->getID()];
		if(c>cost){
			l.insert(it,node);
			return;
		}
	}
	l.push_back(node);
}

void NavAStarSearch::removeFromList(GraphNodesList&l,IGraphNode*node)
{
	GraphNodesList::iterator it=l.begin();
	for(;it!=l.end();++it)
	{
		IGraphNode*p=*it;
		if(p==node){
			l.erase(it);
			return;
		}
	}
}

void NavAStarSearch::createPath(int begin,int end)
{
	m_path.clear();
	if(begin==end){
		return;
	}
	int i=end;
	while(true)
	{
		if(i==begin)
			break;
		m_path.push_front(m_parents[i]);
		i=m_parents[i]->getOtherNode(i)->getID();
	}
}

ESearchResult NavAStarSearch::DoSearchStep()
{
	GraphLinksList::iterator it;
	m_lastRes=ESR_NotComplete;
	if(m_openList.begin()!=m_openList.end())
	{
		IGraphNode* node=*m_openList.begin();
		m_openList.erase(m_openList.begin());
		m_openListDirect[node->getID()]=false;
		if(node==m_end){
			createPath(m_begin->getID(),m_end->getID());
			m_lastRes=ESR_FullFound;
			return m_lastRes;
		}
		else
		{
			float nodeCost=m_costs[node->getID()];
			m_closeListDirect[node->getID()]=true;
			//m_closeList.push_back(node);
			it=node->links.begin();
			for(;it!=node->links.end();++it)
			{
				IGraphLink*link=*it;
				IGraphNode*adj;
				float cost=0;
				float weight=1.0f;
				if(m_costFunc){
					weight=m_costFunc->getWeight(link);
				}
				if(weight==0)
					continue;

				if(link->isBegin(node)){
					adj=link->m_end;
					cost=nodeCost+link->getWeightBeginEnd()/weight;
				}else{
					adj=link->m_begin;
					cost=nodeCost+link->getWeightEndBegin()/weight;
				}
				if(m_costs[adj->getID()]>=0){
					if(cost>m_costs[adj->getID()])
						continue;
				}
				m_costs[adj->getID()]=cost;
				if(m_costFunc)
					m_actualCosts[adj->getID()]=cost+m_costFunc->getHeuristic(adj,m_end);
				else
					m_actualCosts[adj->getID()]=cost;
				m_parents[adj->getID()]=link;
				if(m_openListDirect[adj->getID()]){
					removeFromList(m_openList,adj);
					insertToList(m_openList,adj);
				}else if(!m_closeListDirect[adj->getID()])
				{
					m_openListDirect[adj->getID()]=true;
					insertToList(m_openList,adj);
				}
			}
		}
	}else
		m_lastRes=ESR_NotFound;
	return m_lastRes;
}


ESearchResult NavAStarSearch::Search(IGraphNode*begin,IGraphNode*end,ICostFunction*h)
{

	SetupForNewSearch(begin,end,h);
	while(true)
	{
		m_lastRes=DoSearchStep();
		if(m_lastRes!=ESR_NotComplete)
			break;
	}
	return m_lastRes;
}

void NavAStarSearch::SetupForNewSearch(IGraphNode*begin,IGraphNode*end,ICostFunction*h)
{
	//clean data,and prepare for new search
	cleanUp();
	m_lastRes=ESR_NotComplete;

	m_begin=begin;
	m_end=end;
	m_costFunc=h;

	//add begin node to open list
	m_openList.push_back(begin);
	m_openListDirect[begin->getID()]=true;
	m_costs[begin->getID()]=0;
	if(h)
		m_actualCosts[begin->getID()]=h->getHeuristic(begin,end);
	else
		m_actualCosts[begin->getID()]=0;
}

bool NavAStarSearch::GeneratePath()
{
	if(m_openList.begin()==m_openList.end())return false;
	IGraphNode* node=*m_openList.begin();
	createPath(m_begin->getID(),node->getID());
	return true;
}
ESearchResult NavAStarSearch::GetLastState()
{
	return m_lastRes;
}

}
}

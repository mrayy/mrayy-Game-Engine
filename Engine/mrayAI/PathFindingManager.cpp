#include "stdafx.h"

#include "PathFindingManager.h"
#include "PathFindPlanner.h"
#include "JobOrder.h"
#include "JobPool.h"
#include "IGraphSearch.h"
#include <algorithm>

namespace mray{
namespace AI{
	
	class PathFindingJobOrder:public JobOrder
	{
		JobLoadCompleteDelegate m_delegate;
		PathFindPlanner* m_planner;
		//PathFindingManager* m_manager;
		int m_maxTicks;
	public:
		PathFindingJobOrder(PathFindPlanner* p,PathFindingManager*mngr,JobLoadCompleteDelegate d):/*m_manager(mngr),*/m_planner(p),
			m_delegate(d)
		{
			m_maxTicks=mngr->GetMaxNumberTicks();
		}
		PathFindPlanner* GetPlanner()
		{
			return m_planner;
		}
		bool ExecuteJob()
		{
			int ticks=m_maxTicks;
			bool done=false;
			while( ticks>0)
			{
				if(m_planner->Process_MT()!=ESR_NotFound)
				{
					done=true;
					break;
				}
				--ticks;
			}
			if(!done)
			{
				JobPool::getInstance().AddRequest(this,m_delegate);
			}
			return true;
		}
	};

PathFindingManager::PathFindingManager():m_allowedTicks(80),m_jobCompleted(0)
{
	m_currentPath=m_planners.end();
}
PathFindingManager::~PathFindingManager()
{
	ClearRequests();
}

void PathFindingManager::SetMaxNumberTicks(int c)
{
	m_allowedTicks=c;
}
int PathFindingManager::GetMaxNumberTicks()
{
	return m_allowedTicks;
}

void PathFindingManager::AddPathfindRequest(PathFindPlanner*req)
{
	if(!IsPlannerActive(req))
	{
		m_planners.push_back(req);
		JobPool::getInstance().AddRequest(new PathFindingJobOrder(req,this,m_jobCompleted),m_jobCompleted);
	}
}

bool PathFindingManager::IsPlannerActive(PathFindPlanner*req)
{
	if(std::find(m_planners.begin(),m_planners.end(),req)!=m_planners.end())
		return true;
	return false;
}

void PathFindingManager::RemovePathfindRequest(PathFindPlanner*req)
{
	if(m_currentPath!=m_planners.end())
	{
		if(*m_currentPath==req)
			++m_currentPath;
	}
	m_planners.remove(req);
}

void PathFindingManager::ClearRequests()
{
	m_planners.clear();
	m_currentPath=m_planners.end();
}

int PathFindingManager::GetCurrentPlannersCount()
{
	return (int)m_planners.size();
}

void PathFindingManager::ProcessRequests()
{
	/*
	if(m_currentPath==m_planners.end())
		m_currentPath=m_planners.begin();
	int iterations=m_allowedTicks;
	while(iterations-- && !m_planners.empty())
	{
		PathFindPlanner*p=*m_currentPath;
		if(p->Process()){
			m_currentPath=m_planners.erase(m_currentPath);
		}else
		{
			++m_currentPath;
		}
		if(m_currentPath==m_planners.end())
			m_currentPath=m_planners.begin();
	}*/

	PlannersList::iterator it= m_planners.begin();
	PlannersList::iterator it2=it;
	for(;it!=m_planners.end();)
	{
		if((*it)->IsRequsting()==false)
		{
			(*it)->OnSearchDone();
			it2=it;++it2;
			m_planners.erase(it);
			it=it2;
		}else
			++it;
	}
}


void PathFindingManager::LoadingComplete(JobOrder*const &job,const bool&res)
{/*
	if(res)
	{
		PathFindingJobOrder* order=(PathFindingJobOrder*)job;
		m_plannersMutex->lock();

		PlannersList::iterator it= m_planners.begin();
		for(;it!=m_planners.end();)
		{
		}

		m_plannersMutex->unlock();
	}*/

}

}
}

/********************************************************************
	created:	2010/03/27
	created:	27:3:2010   14:56
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\PathFindingManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	PathFindingManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	manages current pathfinding requests,and process them in time slicing manner
*********************************************************************/

#ifndef ___PathFindingManager___
#define ___PathFindingManager___

#include <list>
#include "compileConfig.h"
#include "JobOrder.h"


namespace mray{
namespace OS{
	class IMutex;
}
namespace AI{

class PathFindPlanner;

class MRAY_AI_DLL PathFindingManager
{
private:
protected:
	typedef std::list<PathFindPlanner*> PlannersList;
	PlannersList m_planners;

	PlannersList::iterator m_currentPath;
	JobLoadCompleteDelegate m_jobCompleted;

	int m_allowedTicks;

	OS::IMutex* m_plannersMutex;

	void LoadingComplete(JobOrder*const &job,const bool&res);
public:
	PathFindingManager();
	virtual~PathFindingManager();

	void SetMaxNumberTicks(int c);
	int GetMaxNumberTicks();

	void AddPathfindRequest(PathFindPlanner*req);
	void RemovePathfindRequest(PathFindPlanner*req);

	bool IsPlannerActive(PathFindPlanner*req);

	void ClearRequests();
	int GetCurrentPlannersCount();

	void ProcessRequests();
};

}
}


#endif //___PathFindingManager___

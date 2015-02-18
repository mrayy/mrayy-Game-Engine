

/********************************************************************
	created:	2010/03/27
	created:	27:3:2010   15:05
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\PathFindPlanner.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	PathFindPlanner
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PathFindPlanner___
#define ___PathFindPlanner___

#include <Point3d.h>
#include "Path.h"
#include "IAIComponent.h"

namespace mray{
namespace AI{

	class IGraphSearch;
	class ICostFunction;
	class AIActor;
	class INavWorldNode;
	enum ESearchResult;

class MRAY_AI_DLL PathFindPlanner:public IAIComponent
{
	DECLARE_RTTI
private:
protected:
	IGraphSearch* m_searchPlanner;
	AIActor* m_owner;

	ESearchResult m_searchResult;

	math::vector3d m_destination;
	INavWorldNode* m_destNode;
	Path m_path;
	ICostFunction* m_costFunc;
	bool m_isRequesting;

	void OnPathFound();

	bool RequestPathToNodePos(const math::vector3d&startPos,INavWorldNode*startNode,
		const math::vector3d&endPos,INavWorldNode*endNode,bool immediat,int preSteps);
protected:


public:
	PathFindPlanner(AIActor* owner);
	virtual~PathFindPlanner();

	bool RequestPathToPosition(const math::vector3d& pos,bool imediate=false,int preSteps=0);
	bool RequestPathToActor(AIActor*actor,bool imediate=false,int preSteps=0);

	void SetCostFunction(ICostFunction* f);

	bool IsRequsting();
	const math::vector3d& GetDestination();

	//returns true if search completed
	bool Process();
	ESearchResult GetSearchResult();

	Path& GetPath();



	//process method for Multi-threading
	ESearchResult Process_MT();
	void OnSearchDone();
};

}
}


#endif //___PathFindPlanner___

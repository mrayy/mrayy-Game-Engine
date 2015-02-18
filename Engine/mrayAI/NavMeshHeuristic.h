
/********************************************************************
	created:	2010/04/03
	created:	3:4:2010   18:03
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\NavMeshHeuristic.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	NavMeshHeuristic
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___NavMeshHeuristic___
#define ___NavMeshHeuristic___

#include "ICostFunction.h"
#include "NavGraphNode.h"


namespace mray{
namespace AI{

class NavMeshHeuristic:public ICostFunction
{
private:
protected:
public:
	NavMeshHeuristic()
	{
	}
	virtual~NavMeshHeuristic()
	{
	}

	virtual float getHeuristic(IGraphNode*a,IGraphNode*b)
	{
		return ((NavGraphNode*)a)->position.getDistSQ(((NavGraphNode*)b)->position);
	}
	virtual float getWeight(IGraphLink*link)
	{
		return 1;
	}

};

}
}


#endif //___NavMeshHeuristic___
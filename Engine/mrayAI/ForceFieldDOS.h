

/********************************************************************
	created:	2010/04/18
	created:	18:4:2010   7:43
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\ForceFieldDOS.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	ForceFieldDOS
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	consider all obstacles as force generators against the agent
*********************************************************************/

#ifndef ForceFieldDOS_h__
#define ForceFieldDOS_h__

#include "compileConfig.h"
#include "IDynamicObstacleSolver.h"

namespace mray
{
namespace AI
{

	class IActorQuery;

class MRAY_AI_DLL ForceFieldDOS:public IDynamicObstacleSolver
{
	DECLARE_RTTI
private:
protected:
	AgentObstacle* m_agent;
	IActorQuery* m_query;
public:
	ForceFieldDOS(AgentObstacle* agent,IActorQuery* query);
	virtual ~ForceFieldDOS();

	virtual void CalculateAvoidanceForce();
};

}
}

#endif // ForceFieldDOS_h__

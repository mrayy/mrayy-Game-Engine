
/********************************************************************
	created:	2010/04/04
	created:	4:4:2010   18:29
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent\GoalTraverseEdge.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent
	file base:	GoalTraverseEdge
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GoalTraverseEdge___
#define ___GoalTraverseEdge___


#include <IGoal.h>
#include <PathEdge.h>

namespace mray{
namespace AI{

class SteerBehavior;

class GoalTraverseEdge:public IGoal
{
private:
protected:
	EGoalStatus m_state;
	PathEdge m_edge;
	bool m_lastEdge;
	SteerBehavior* m_steer;
	int m_stuckCounter;
public:
	GoalTraverseEdge(SteerBehavior*steer,const PathEdge& e,bool lastEdge);
	virtual~GoalTraverseEdge();

	virtual int GetGoalType();
	virtual void activate();
	virtual EGoalStatus process();
	virtual void terminate();

	virtual EGoalStatus getStatus();

	virtual bool isPrimitive(){return true;}

	virtual core::string getType(){return mT("GoalTraverseEdge");}
	virtual void Visualize(IRenderDevice*dev);
};

}
}


#endif //___GoalTraverseEdge___

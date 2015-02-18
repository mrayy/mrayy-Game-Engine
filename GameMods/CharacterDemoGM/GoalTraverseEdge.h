
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
	class SteerController;
}
namespace GameMod{


class GoalTraverseEdge:public AI::IGoal
{
private:
protected:
	AI::EGoalStatus m_state;
	AI::PathEdge m_edge;
	bool m_lastEdge;
	AI::SteerController* m_steer;
	int m_stuckCounter;


public:
	GoalTraverseEdge(AI::SteerController*steer,const AI::PathEdge& e,bool lastEdge);
	virtual~GoalTraverseEdge();

	virtual void pause();
	virtual void resume();

	virtual int GetGoalType();
	virtual void activate();
	virtual AI::EGoalStatus process();
	virtual void terminate();

	virtual AI::EGoalStatus getStatus();

	virtual bool isPrimitive(){return true;}

	virtual core::string getType(){return mT("GoalTraverseEdge");}
	virtual void Visualize(AI::IRenderDevice*dev,const core::string&prefix);
};

}
}


#endif //___GoalTraverseEdge___

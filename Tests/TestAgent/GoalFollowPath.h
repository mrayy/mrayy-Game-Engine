
/********************************************************************
	created:	2010/04/04
	created:	4:4:2010   18:16
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent\GoalFollowPath.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent
	file base:	GoalFollowPath
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GoalFollowPath___
#define ___GoalFollowPath___


#include <GoalPipe.h>
#include <Path.h>

namespace mray{
namespace AI{

class GoalFollowPath:public GoalPipe
{
private:
protected:
	typedef std::list<INavWorldNode*> WorldNodesList;
	EGoalStatus m_state;
	SteerBehavior* m_steer;
	AIActor* m_actor;
	Path m_path;
	WorldNodesList m_nodes;
	PathEdge m_currEdge;
	INavWorldNode* m_currNode;

	
public:
	GoalFollowPath(AIActor* actor,SteerBehavior* steer,const Path& p);
	virtual~GoalFollowPath();

	virtual int GetGoalType();
	virtual void activate();
	virtual EGoalStatus process();
	virtual void terminate();

	virtual EGoalStatus getStatus();

	virtual core::string getType(){return mT("GoalFollowPath");}

	virtual void Visualize(IRenderDevice*dev);
};

}
}

#endif //___GoalFollowPath___

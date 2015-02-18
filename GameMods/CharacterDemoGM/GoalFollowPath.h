
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
#include <INavWorldNode.h>
#include <SteerController.h>

namespace mray{
namespace GameMod{

class GoalFollowPath:public AI::GoalPipe
{
private:
protected:
	typedef std::list<AI::INavWorldNode*> WorldNodesList;
	AI::EGoalStatus m_state;
	AI::SteerController* m_steer;
	AI::AIActor* m_actor;
	AI::Path m_path;
	WorldNodesList m_nodes;
	AI::PathEdge m_currEdge;
	AI::INavWorldNode* m_currNode;

	
public:
	GoalFollowPath(AI::AIActor* actor,AI::SteerController* steer,const AI::Path& p);
	virtual~GoalFollowPath();


	void pause();

	virtual int GetGoalType();
	virtual void activate();
	virtual AI::EGoalStatus process();
	virtual void terminate();

	virtual AI::EGoalStatus getStatus();

	virtual core::string getType(){return mT("GoalFollowPath");}

	virtual void Visualize(AI::IRenderDevice*dev,const core::string&prefix);
};

}
}

#endif //___GoalFollowPath___

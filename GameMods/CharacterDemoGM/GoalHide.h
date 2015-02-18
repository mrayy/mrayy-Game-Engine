

/********************************************************************
	created:	2010/05/23
	created:	23:5:2010   23:52
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent\GoalHide.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent
	file base:	GoalHide
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GoalHide_h__
#define GoalHide_h__

#include <GoalPipe.h>

namespace mray
{
namespace AI
{
	class INavWorldNode;
}
namespace GameMod
{

class GoalHide:public AI::GoalPipe
{
private:
protected:
	AI::AIActor* m_actor;
	math::vector3d m_target;
	AI::INavWorldNode* m_targetNode;
	AI::EGoalStatus m_status;
public:
	GoalHide(AI::AIActor* actor,const math::vector3d& target);
	virtual ~GoalHide();

	virtual int GetGoalType();
	virtual void activate();
	virtual AI::EGoalStatus process();
	virtual void terminate();

	const math::vector3d& GetTarget(){return m_target;}
	void SetTarget(const math::vector3d&t);

	virtual AI::EGoalStatus getStatus();

	virtual core::string getType(){return mT("GoalHide");}

	
};

}
}

#endif // GoalHide_h__

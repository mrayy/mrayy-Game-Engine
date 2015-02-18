
/********************************************************************
	created:	2010/04/04
	created:	4:4:2010   17:58
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent\GoalGoToPoint.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent
	file base:	GoalGoToPoint
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GoalGoToPoint___
#define ___GoalGoToPoint___


#include <GoalPipe.h>
#include <IGoalEvaluator.h>
#include <Path.h>

namespace mray{
namespace GameMod{

class CharacterObject;
class GoalGoToPoint:public AI::GoalPipe
{
private:
protected:
	AI::EGoalStatus m_state;
	math::vector3d m_pos;
	CharacterObject* m_character;

public:
	GoalGoToPoint(CharacterObject*character,const math::vector3d& pos);
	virtual~GoalGoToPoint();

	void pause();
	void resume();

	virtual int GetGoalType();
	virtual void activate();
	virtual AI::EGoalStatus process();
	virtual void terminate();

	math::vector3d getTargetPoint(){return m_pos;}

	virtual AI::EGoalStatus getStatus();


	virtual bool OnMessageTelegram(const AI::AIMessageTelegram&msg);

	virtual core::string getType(){return mT("GoalGoToPoint");}
	virtual void Visualize(AI::IRenderDevice*dev,const core::string&prefix);
};

}
}


#endif //___GoalGoToPoint___

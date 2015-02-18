

/********************************************************************
	created:	2009/07/17
	created:	17:7:2009   15:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\GoalPipe.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	GoalPipe
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GoalPipe___
#define ___GoalPipe___

#include "mString.h"
#include "GCPtr.h"
#include "IGoal.h"
#include "CompileConfig.h"

#include <list>

namespace mray{
namespace AI{

class MRAY_AI_DLL GoalPipe:public IGoal
{
private:
protected:
	typedef std::list<IGoal*> GoalList;

	//if isSequence is true, then when one sub-goal fail, all the pipe will fail as well
	bool m_isSequence;
	GoalList m_goals;
	virtual EGoalStatus processSubGoals();
public:
	GoalPipe();
	virtual~GoalPipe();

	void SetSequence(bool s);
	bool IsSequence();

	virtual void pause();
	virtual void resume();

	virtual void activate();
	virtual EGoalStatus process();
	virtual void terminate();

	bool isContainSubGoals();

	virtual void addSubGoal(IGoal* goal);
	virtual IGoal* getSubGoal(int goalType);
	virtual void removeAllGoals();

	virtual bool isPrimitive(){return false;}
	virtual core::string getType(){return mT("GoalPipe");}

	//check if the goal is inside the current processing goals
	bool isGoalPresented(int goalType);

	virtual bool OnMessageTelegram(const AIMessageTelegram&msg); 


	virtual void Visualize(IRenderDevice*dev,const core::string&prefix);
	virtual void loadXMLSettings(xml::XMLElement*elem);
};


}
}


#endif //___GoalPipe___

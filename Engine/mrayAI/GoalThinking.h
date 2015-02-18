
/********************************************************************
	created:	2009/07/26
	created:	26:7:2009   20:33
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\GoalThinking.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	GoalThinking
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GoalThinking___
#define ___GoalThinking___

#include "GoalPipe.h"

namespace mray{
namespace AI{

class GoalThinking:public GoalPipe
{
private:
protected:
	EGoalStatus m_state;

public:
	GoalThinking();
	virtual~GoalThinking();

	int GetGoalType();
	virtual EGoalStatus process();

	virtual EGoalStatus getStatus();

	virtual core::string getType();
};

}
}


#endif //___GoalThinking___


/********************************************************************
	created:	2009/07/26
	created:	26:7:2009   20:33
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\Thinking.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	Thinking
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Thinking___
#define ___Thinking___

#include "IAIComponent.h"
#include "IGoalEvaluator.h"
#include "GoalThinking.h"

namespace mray{
namespace AI{

class MRAY_AI_DLL Thinking:public IAIComponent
{
	DECLARE_RTTI
private:
protected:
	typedef std::list<GCPtr<IGoalEvaluator>> EvaluatorList;
	EvaluatorList m_evaluators;

	GoalThinking m_goal;


public:
	Thinking();
	virtual~Thinking();

	virtual void addEvaluator(GCPtr<IGoalEvaluator> eval);

	void selectBestEvaluator();

	GoalThinking* GetThinkingGoal();


	virtual void activate();
	virtual void Update(float dt);
	//virtual void terminate();

	virtual bool OnMessageTelegram(const AIMessageTelegram&msg); 

	virtual void Visualize(IRenderDevice*dev);
};

}
}


#endif //___Thinking___

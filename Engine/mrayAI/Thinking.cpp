#include "stdafx.h"

#include "Thinking.h"

#include "AIComponentTypes.h"
#include "AIDefaultGoalsTypes.h"
#include "IRenderDevice.h"


namespace mray{
namespace AI{


Thinking::Thinking()
{
}
Thinking::~Thinking(){
		
	m_evaluators.clear();
}

void Thinking::addEvaluator(GCPtr<IGoalEvaluator> eval){
	m_evaluators.push_back(eval);
}


GoalThinking* Thinking::GetThinkingGoal()
{
	return &m_goal;
}

void Thinking::selectBestEvaluator(){
	float bestScore=0;
	IGoalEvaluator* eval=0;
	EvaluatorList::iterator p=m_evaluators.begin();
	EvaluatorList::iterator end=m_evaluators.end();
	for(;p!=end;++p)
	{
		float score=(*p)->calcDeserability();
		if(score>bestScore){
			bestScore=score;
			eval=*p;
		}
	}
	if(eval){
		eval->CreateGoal(&m_goal);
	}
}
void Thinking::activate(){
	selectBestEvaluator();
	m_goal.activate();
}
void Thinking::Update(float dt){
	//if(!isActive())
	activate();
	EGoalStatus s=m_goal.process();
/*
	if(s==EGS_Completed || s==EGS_Failed){
		if(m_owner->IsAIControlled()){
			m_state=EGS_Inactive;
		}
	}
	return m_state;*/
}
/*
void Thinking::terminate(){
}
*/

bool Thinking::OnMessageTelegram(const AIMessageTelegram&msg){
	return m_goal.OnMessageTelegram(msg);
} 

void Thinking::Visualize(IRenderDevice*dev)
{
	dev->clearTextLines();
	m_goal.Visualize(dev,mT(""));
}

}
}


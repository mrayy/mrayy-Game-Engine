
/********************************************************************
	created:	2009/07/26
	created:	26:7:2009   20:35
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\IGoalEvaluator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	IGoalEvaluator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IGoalEvaluator___
#define ___IGoalEvaluator___

#include "mString.h"

namespace mray{
namespace AI{

	class GoalPipe;

class IGoalEvaluator
{
private:
protected:
	// this value determine the weight of this evaluater,
	//
	float m_bias;
	virtual float innerCalcDeserability()=0;

public:
	IGoalEvaluator():m_bias(1){}
	virtual~IGoalEvaluator(){}

	void SetBias(float b){m_bias=b;}
	float GetBias(){return m_bias;}

	// this function will return value between 0..1
	virtual float calcDeserability(){
		return m_bias*innerCalcDeserability();
	}

	virtual void CreateGoal(GoalPipe*pipe)=0;

	virtual core::string getInfo()=0;
};

}
}


#endif //___IGoalEvaluator___

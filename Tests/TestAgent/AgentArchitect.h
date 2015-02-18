

/********************************************************************
	created:	2010/04/14
	created:	14:4:2010   12:24
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent\AgentArchitect.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent
	file base:	AgentArchitect
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AgentArchitect_h__
#define AgentArchitect_h__

#include "GenericArchitect.h"

namespace mray
{
namespace AI
{

class SensorActuator;
class SensorSet;
class FiniteStateMachine;
class DecisionTree;

class AgentArchitect:public GenericArchitect
{
private:
protected:

	SensorActuator* m_actuator;
	FiniteStateMachine* m_stateMachine;
	SensorSet* m_sensor;
	DecisionTree* m_decTree;

	virtual void UpdateInternal(float dt);
public:
	AgentArchitect(AIActor* owner);
	virtual ~AgentArchitect();

	SensorSet* GetSensors();

	SensorActuator* GetActuator();


	virtual void Visualize(IRenderDevice*dev);
};

}
}
#endif // AgentArchitect_h__

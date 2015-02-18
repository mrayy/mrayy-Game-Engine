
#include "Stdafx.h"
#include "AgentArchitect.h"
#include "AgentWorldSensor.h"
#include "SensorActuator.h"
#include "SensorSet.h"
#include "FiniteStateMachine.h"

#include "NavGraphLink.h"
#include "DecisionTree.h"
#include "NavGraphLink.h"
#include <PathFindPlanner.h>
namespace mray
{
namespace AI
{

class AgentCostFunction:public ICostFunction
{
public:
	virtual float getHeuristic(IGraphNode*a,IGraphNode*b)
	{
		NavGraphNode*an=(NavGraphNode*)a;
		NavGraphNode*bn=(NavGraphNode*)b;
		return an->position.getDistSQ(bn->position);
	}
	virtual float getWeight(IGraphLink*link)
	{
		NavGraphLink*l=(NavGraphLink*)link;
		if(l->getOwnerNode()->GetAttribute()==2)
			return 0.02;
		return 1;
	}
};


AgentArchitect::AgentArchitect(AIActor* owner)
:GenericArchitect(owner)
{

	m_sensor=new SensorSet(owner->GetNavWorld(),GetObstacle(),m_visionSensor);
	m_actuator=new SensorActuator(GetObstacle(),m_sensor);
	m_pathPlanner->SetCostFunction(new AgentCostFunction());
	m_stateMachine=new FiniteStateMachine();
	m_decTree=new DecisionTree();


	AddComponent(m_sensor);
	//AddComponent(m_actuator);
	AddComponent(m_stateMachine);
	AddComponent(m_decTree);

/*
	AgentWorldSensor*leftSensor=new AgentWorldSensor(owner->GetNavWorld(),GetObstacle());
	AgentWorldSensor*rightSensor=new AgentWorldSensor(owner->GetNavWorld(),GetObstacle());
	AgentWorldSensor*frontSensor=new AgentWorldSensor(owner->GetNavWorld(),GetObstacle());

	leftSensor->SetLength(10);
	leftSensor->SetRelativeAngle(math::vector3d(0,-90,0));

	rightSensor->SetLength(10);
	rightSensor->SetRelativeAngle(math::vector3d(0,+90,0));

	frontSensor->SetLength(10);
	frontSensor->SetRelativeAngle(math::vector3d(0,0,0));

	m_actuator->AddActuator(leftSensor,math::vector3d(50,0,0),0);
	m_actuator->AddActuator(rightSensor,math::vector3d(-50,0,0),0);
	m_actuator->AddActuator(frontSensor,math::vector3d(0,0,-50),0);
*/
}
AgentArchitect::~AgentArchitect()
{
	delete m_sensor;
	delete m_actuator;
	delete m_stateMachine;
	delete m_decTree;
}
SensorSet* AgentArchitect::GetSensors()
{
	return m_sensor;
}

SensorActuator* AgentArchitect::GetActuator()
{
	return m_actuator;
}

void AgentArchitect::UpdateInternal(float dt)
{
	GenericArchitect::UpdateInternal(dt);
	//m_actuator->Update(dt);
	//AccumulateLinearForce(m_actuator->GetResultLinearForce());
	//AccumulateAngularForce(m_actuator->GetResultAngularForce());

}

void AgentArchitect::Visualize(IRenderDevice*dev)
{
	GenericArchitect::Visualize(dev);
	//m_actuator->Visualize(dev);
}

}
}
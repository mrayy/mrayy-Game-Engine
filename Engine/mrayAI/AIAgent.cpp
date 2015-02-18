#include "stdafx.h"

#include "AIAgent.h"
#include "INavWorld.h"
#include "AISystem.h"
#include "IAIArchitect.h"
#include <IPhysicalNode.h>
#include "IRenderDevice.h"
#include "IWorldVisualizer.h"



namespace mray{
namespace AI{


AIAgent::AIAgent(IAIPhysics* phNode,const core::string &name):
		m_physicalProp(phNode),m_enableVis(false),m_ID(0)
		//,	m_brakingRate(0.75)
{
	SetName(name);
	m_system=0;

	m_worldNode=0;

	m_heading=math::vector3d::ZAxis;
	m_up=math::vector3d::YAxis;

	m_controller=0;


	CPropertieDictionary* dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeName::instance);
	}
}
AIAgent::~AIAgent(){
	delete m_controller;
	delete m_physicalProp;
	/*
	delete m_brain;
	delete m_steering;
	delete m_visionSensor;
	delete m_memory;*/

}

bool AIAgent::SetID(uint id)
{
	m_ID=id;
	return true;
}
uint AIAgent::GetID()
{
	return m_ID;
}
void AIAgent::SetAISystem(AISystem*s)
{
	m_system=s;
}
AISystem* AIAgent::GetAISystem()
{
	return m_system;
}
AgentObstacle* AIAgent::GetObstacle()
{
	return m_controller->GetObstacle();
}
INavWorld* AIAgent::GetNavWorld()
{
	if(!m_system)
		return 0;
	return m_system->GetNavigationWorld();
}

void AIAgent::AttachController(IAIArchitect*controller)
{
	m_controller=controller;
}
IAIArchitect* AIAgent::GetController()
{
	return m_controller;
}
/*
void AIAgent::SetSpeed(const math::vector3d& s){
	math::vector3d vel=s;
	float l=s.LengthSQ();
	if(l>m_maxSpeed*m_maxSpeed){
		vel.Normalize();
		vel*=m_maxSpeed;
	}
	if(m_physicalProp)
		m_physicalProp->setLinearVelocity(vel);
}
const math::vector3d& AIAgent::GetSpeed()const{
	if(m_physicalProp)
		return m_physicalProp->getLinearVelocity();
	return math::vector3d::Zero;
}

void AIAgent::SetMaxSpeed(float s){
	m_maxSpeed=s;
}
float AIAgent::GetMaxSpeed()const{
	return m_maxSpeed;
}*/
void AIAgent::Update(float dt){

	UpdateWorldNode();
	m_controller->Update(dt);

	if(m_physicalProp){
		m_heading=m_physicalProp->GetHeading();
		m_up=m_physicalProp->GetUp();
	}
	//UpdateMovement(dt);
/*
	m_brain->process();


	if(IsAIControlled()){
		m_brain->selectBestEvaluator();
	}
	m_visionSensor->Update();
	m_memory->Update();*/
}
/*
void AIAgent::SetForce(const math::vector3d& s)
{
	m_force=s;
}
const math::vector3d& AIAgent::GetForce()
{
	return m_force;
}
void AIAgent::UpdateMovement(float dt){
	if(!m_physicalProp)
		return;

	//math::vector3d force=m_steering->calculate();
	math::vector3d vel=GetSpeed();
	if(m_force==0){
		vel*=m_brakingRate;
	}

	math::vector3d accel=m_force/m_physicalProp->getMass();
	SetSpeed(vel+accel*dt);



	m_force=0;

}*/
/*
ActorMemory* AIAgent::GetMemory()
{
	return m_memory;
}
*/
INavWorldNode* AIAgent::GetWorldNode()
{
	return m_worldNode;
}
void AIAgent::UpdateWorldNode()
{
	if(!GetPhysicalProp())
		return;
	NavWorldNodeListPtr tmp;
	m_worldNode=GetNavWorld()->QueryPolys(GetPhysicalProp()->GetGlobalPosition(),tmp);
}
/*
PathFindPlanner* AIAgent::GetPathFinder()
{
	return m_pathPlanner;
}*/

bool AIAgent::OnMessageTelegram(const AIMessageTelegram&msg)
{
	//return m_brain->OnMessageTelegram(msg);
	return m_controller->OnMessageTelegram(msg);
}

bool AIAgent::IsAIControlled()
{
	if(!m_controller)
		return true;
	return m_controller->IsAIControlled();
}

void AIAgent::EnableVisualize(bool e)
{
	m_enableVis=e;
}
bool AIAgent::IsEnableVisualize()
{
	return m_enableVis;
}


void AIAgent::Visualize(IRenderDevice*dev)
{
	if(!m_enableVis)
		return;

	if(m_controller)
		m_controller->Visualize(dev);
	if(m_physicalProp)
		m_physicalProp->Visualize(dev);

	if(m_worldNode)
		m_system->GetNavigationWorld()->GetVisualizer()->DrawNode(dev,m_worldNode);
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Name,AIAgent,string,mT("Name"),EPBT_Basic,mT("Agent's Name"),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(Name,AIAgent,core::string,SetName,GetName,,,false)

}
}

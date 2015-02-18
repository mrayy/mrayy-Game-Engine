#include "stdafx.h"

#include "GenericArchitect.h"
#include "Thinking.h"
#include "SteerController.h"
#include "ActorMemory.h"
#include "VisionSensor.h"
#include "PathFindPlanner.h"
#include "IAIPhysics.h"
#include "AgentObstacle.h"
#include "IDynamicObstacleSolver.h"
#include "AIMovmentTracker.h"

#include "MathUtil.h"
#include <Randomizer.h>

namespace mray{
namespace AI{

GenericArchitect::GenericArchitect(AIActor* owner)
{
	m_owner=owner;

	m_memory=new ActorMemory();
	m_visionSensor=new VisionSensor(owner,m_memory);

	m_brain=new Thinking();
	m_steering=new SteerController(m_owner->GetPhysicalProp());
	m_pathPlanner=new PathFindPlanner(m_owner);
	m_obstacle=new AgentObstacle(owner->GetPhysicalProp());
	m_dynamicObsSolver=0;
	m_movementTracker=new AIMovmentTracker(owner->GetPhysicalProp());

	AddComponent(owner->GetPhysicalProp());
	AddComponent(m_brain);
	AddComponent(m_steering);
	AddComponent(m_obstacle);
	AddComponent(m_memory);
	AddComponent(m_visionSensor);
	AddComponent(m_movementTracker);
	AddComponent(m_pathPlanner);
}
GenericArchitect::~GenericArchitect()
{
	delete m_obstacle;
	delete m_pathPlanner;
	delete m_brain;
	delete m_steering;
	delete m_visionSensor;
	delete m_memory;
	delete m_dynamicObsSolver;
	delete m_movementTracker;
}

AgentObstacle* GenericArchitect::GetObstacle()
{
	return m_obstacle;
}
Thinking* GenericArchitect::GetBrain()
{
	return m_brain;
}
ActorMemory* GenericArchitect::GetMemory()
{
	return m_memory;
}
SteerController* GenericArchitect::GetSteering()
{
	return m_steering;
}

PathFindPlanner* GenericArchitect::GetPathFinder()
{
	return m_pathPlanner;
}
VisionSensor* GenericArchitect::GetVisionSensor()
{
	return m_visionSensor;
}

IDynamicObstacleSolver* GenericArchitect::GetDynamicObstacleSolver()
{
	return m_dynamicObsSolver;
}


void GenericArchitect::SetDynamicObstacleSolver(IDynamicObstacleSolver*dos)
{
	m_dynamicObsSolver=dos;
	AddComponent(m_dynamicObsSolver);
}

bool GenericArchitect::IsAIControlled()
{
	return true;
}

void GenericArchitect::AccumulateLinearForce(const math::vector3d&f)
{
	m_totalLinearForce+=f;
}
void GenericArchitect::AccumulateAngularForce(const math::vector3d&f)
{

	m_totalAngularForce+=f;
}


void GenericArchitect::UpdateInternal(float dt)
{
	float targetSpeed=0;
	math::vector3d targetDir=0;

	math::vector3d force=m_steering->calculate();
	const AIActorList& actors= m_visionSensor->GetActorsList();

	force.y=0;

	targetSpeed=force.Length();
	if(targetSpeed>0)
		targetDir=force/targetSpeed;
	else targetDir=math::vector3d::ZAxis;


	math::matrix4x4 m,lookAt;
	m=math::MathUtil::CreateLookAtMatrix(0,targetDir,math::vector3d::YAxis);
	m.getTransposeMatrix(lookAt);
	math::quaternion resultDir(lookAt);
	//resultDir=resultDir.inverse();//either this or transpose the lookat matrix

	if(m_dynamicObsSolver){
		m_dynamicObsSolver->CalculateAvoidanceForce();
		/*
		math::vector3d forceDir(force);
		math::vector3d avoidDir(avoidance);
		forceDir.Normalize();
		avoidDir.Normalize();
		if(forceDir.dotProduct(avoidDir)>0.7)//nearly the same dir<obstacles are lies in the same line
		{
		float angle=math::Randomizer::rand01()*0.5+0.5;
		angle*=math::Randomizer::rand01()>0.5?-1:+1;
		math::quaternion q(angle,math::vector3d::ZAxis);
		avoidance=q*avoidance;//rotate the avoidance dir
		}*/
		//float len=force.Length();
//		avoidance*=len/m_owner->GetPhysicalProp()->GetMaxSpeed();
		//force=(force+avoidance);
/*
		math::vector3d vec=m_owner->GetPhysicalProp()->GetGlobalOrintation()*m_dynamicObsSolver->GetResultOrientation()*math::vector3d::ZAxis;

		force=((vec+force.Normalize())*0.5)*
			len*m_dynamicObsSolver->GetResultSpeed();
			*/

		resultDir*=m_dynamicObsSolver->GetResultOrientation();
		targetSpeed*=m_dynamicObsSolver->GetResultSpeed();
	}/*
	if(targetSpeed>m_owner->GetPhysicalProp()->GetMaxSpeed())
		targetSpeed=m_owner->GetPhysicalProp()->GetMaxSpeed();*/
	resultDir*=m_owner->GetPhysicalProp()->GetGlobalOrintation().inverse();
	//force.y=0;
	force=m_owner->GetPhysicalProp()->GetGlobalOrintation()*math::vector3d::ZAxis*targetSpeed;

	AccumulateLinearForce(force);
	AccumulateAngularForce(math::vector3d(0,5*math::toDeg(resultDir.getYaw()),0));
}
void GenericArchitect::UpdateMovement(float dt)
{

	//AccumulateLinearForce(math::vector3d(0,-9.8,0));
	float m_brakingRate=0.9;
	math::vector3d accel=(m_totalLinearForce);///m_owner->GetPhysicalProp()->GetMass();

	math::vector3d vel=0;//m_owner->GetPhysicalProp()->GetLinearVelocity();
	if(m_totalLinearForce.x==0 && m_totalLinearForce.z==0){
		vel.x*=m_brakingRate;
		vel.z*=m_brakingRate;
	}

	vel=vel+accel;
	float len=vel.Length();
	math::vector3d velDir=m_totalLinearForce;
	velDir.y=0;
	//velDir.Normalize();

	vel.y+=-9.8;

	math::matrix4x4 VelOriMat,m;
	m=math::MathUtil::CreateLookAtMatrix(0,velDir,math::vector3d::YAxis);
	m.getTransposeMatrix(VelOriMat);
	math::quaternion velOri(VelOriMat);
	//velOri=velOri.inverse();

	math::vector3d ang=0;/*+m_owner->GetPhysicalProp()->GetAngularVelocity()*/

	math::quaternion ori=m_owner->GetPhysicalProp()->GetGlobalOrintation();
	velOri*=ori.inverse();
	velOri.toEulerAngles(ang);
	ang.x=ang.z=0;
	ang+=m_totalAngularForce;


	m_owner->GetPhysicalProp()->SetLinearVelocity(m_owner->GetPhysicalProp()->GetHeading()*len +math::vector3d(0,vel.y,0)/*vel*/);
	m_owner->GetPhysicalProp()->SetAngularVelocity(ang);
}

void GenericArchitect::Update(float dt)
{
	IAIArchitect::Update(dt);
	m_totalLinearForce=0;
	m_totalAngularForce=0;

	UpdateInternal(dt);
	UpdateMovement(dt);
}

bool GenericArchitect::OnMessageTelegram(const AIMessageTelegram&msg)
{
	return m_brain->OnMessageTelegram(msg);
}
void GenericArchitect::Visualize(IRenderDevice*dev)
{
	IAIArchitect::Visualize(dev);
	/*
	m_brain->Visualize(dev);
	m_steering->Visualize(dev);
	m_visionSensor->Visualize(dev);
	if(m_dynamicObsSolver){
		m_dynamicObsSolver->Visualize(dev);
	}*/
}

}
}

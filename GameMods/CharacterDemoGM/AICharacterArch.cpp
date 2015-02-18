
#include "stdafx.h"
#include "AICharacterArch.h"


#include "Thinking.h"
#include "SteerController.h"
#include "ActorMemory.h"
#include "VisionSensor.h"
#include "PathFindPlanner.h"
#include "IAIPhysics.h"
#include "AgentObstacle.h"
#include "IDynamicObstacleSolver.h"
#include "AIMovmentTracker.h"
#include "SensorSet.h"
#include "SensorActuator.h"
#include "FuzzyRaycastDOS.h"

#include "GameEntity.h"
#include "GameEntityManager.h"
#include "GameLevel.h"

namespace mray
{
namespace GameMod
{

AICharacterArch::AICharacterArch(game::GameEntity*entity,animation::IAnimationMixer*mixer,
								 bool animateLegs,bool physicsBased):AIGameArch(entity->GetAIActor(),EAT_Character)
{
	m_entity=entity;
	m_memory=new AI::ActorMemory();
	m_visionSensor=new AI::VisionSensor(m_owner,m_memory);

	m_brain=new AI::Thinking();
	m_steering=new AI::SteerController(m_owner->GetPhysicalProp());
	m_pathPlanner=new AI::PathFindPlanner(m_owner);
	m_obstacle=new AI::AgentObstacle(m_owner->GetPhysicalProp(),0);

	m_sensor=new AI::SensorSet(m_owner->GetNavWorld(),GetObstacle(),m_visionSensor);
	//m_actuator=new AI::SensorActuator(GetObstacle(),m_sensor);
	m_dynamicObsSolver=new AI::FuzzyRaycastDOS(m_obstacle,m_sensor);
	m_movementTracker=new AI::AIMovmentTracker(m_owner->GetPhysicalProp());

	m_dynamicAvoid=true;

	AddComponent(m_owner->GetPhysicalProp());
	AddComponent(m_brain);
	AddComponent(m_steering);
	AddComponent(m_obstacle);
	AddComponent(m_memory);
	AddComponent(m_visionSensor);
	AddComponent(m_movementTracker);
	AddComponent(m_pathPlanner);
	AddComponent(m_dynamicObsSolver);
	AddComponent(m_sensor);

//////////////////////////////////////////////////////////////////////////

	m_charObject=new CharacterObject(this,mixer,animateLegs,physicsBased);
}
AICharacterArch::~AICharacterArch()
{
	delete m_charObject;

	delete m_obstacle;
	delete m_pathPlanner;
	delete m_brain;
	delete m_steering;
	delete m_visionSensor;
	delete m_memory;
	delete m_dynamicObsSolver;
	delete m_movementTracker;
	delete m_sensor;
//	delete m_actuator;

}

AI::Thinking* AICharacterArch::GetBrain()
{
	return m_brain;
}

AI::AgentObstacle* AICharacterArch::GetObstacle()
{
	return m_obstacle;
}
void AICharacterArch::EnableDynamicAvoidance(bool e)
{
	m_dynamicAvoid=e;
}
bool AICharacterArch::IsDynamicAvoidanceEnabled()
{
	return m_dynamicAvoid;
}

math::vector3d AICharacterArch::GetLastLinearVelocity()
{
	return m_lastLinearForce;
}


void AICharacterArch::Update(float dt)
{
	if(m_entity->GetGameEntityManager()->GetGameLevel()->IsEditorMode())
		return;

	IAIArchitect::Update(dt);
	m_totalLinearForce=0;
	m_totalAngularForce=0;

	UpdateInternal(dt);
	UpdateMovement(dt);

	m_charObject->Update(dt);

}


void AICharacterArch::UpdateInternal(float dt)
{
	float targetSpeed=0;
	math::vector3d targetDir=0;

	math::vector3d force=m_steering->calculate();
	const AI::AIActorList& actors= m_visionSensor->GetActorsList();

	force.y=0;

	targetSpeed=force.Length();
	if(targetSpeed==0)
		return;
	if(targetSpeed>0)
		targetDir=force/targetSpeed;
	else targetDir=math::vector3d::ZAxis;


	math::matrix4x4 m,lookAt;
	m.buildCameraLookAtLH(0,targetDir,math::vector3d::YAxis);
	m.getTransposeMatrix(lookAt);
	math::quaternion resultDir(lookAt);
	//resultDir=resultDir.inverse();//either this or transpose the lookat matrix

	if(m_dynamicObsSolver && m_dynamicAvoid){
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
void AICharacterArch::UpdateMovement(float dt)
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
/*
	math::matrix4x4 VelOriMat,m;
	m.buildCameraLookAtLH(0,velDir,math::vector3d::YAxis);
	m.getTransposeMatrix(VelOriMat);
	math::quaternion velOri(VelOriMat);
	//velOri=velOri.inverse();
*/
	math::vector3d ang=0;/*+m_owner->GetPhysicalProp()->GetAngularVelocity()*/
/*
	math::quaternion ori=m_owner->GetPhysicalProp()->GetGlobalOrintation();
	velOri*=ori.inverse();
	velOri.toEulerAngles(ang);
	ang.x=ang.z=0;
	ang+=m_totalAngularForce;
*/
	ang=m_totalAngularForce;

	m_lastLinearForce=m_owner->GetPhysicalProp()->GetHeading()*len +math::vector3d(0,vel.y,0);
	if(len>4)
	{
		MessageBox(0,mT(""),mT(""),0);
	}
	m_owner->GetPhysicalProp()->SetLinearVelocity(m_lastLinearForce);
	m_owner->GetPhysicalProp()->SetAngularVelocity(ang);
}

void AICharacterArch::AccumulateLinearForce(const math::vector3d&f)
{
	m_totalLinearForce+=f;
}
void AICharacterArch::AccumulateAngularForce(const math::vector3d&f)
{

	m_totalAngularForce+=f;
}

bool AICharacterArch::OnMessageTelegram(const AI::AIMessageTelegram& msg)
{
	return m_brain->OnMessageTelegram(msg);
}


}
}



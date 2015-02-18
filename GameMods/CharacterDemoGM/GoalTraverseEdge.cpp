
#include "stdafx.h"
#include "GoalTraverseEdge.h"
#include "GoalsTypeEnum.h"
#include <SteerController.h>
#include <IRenderDevice.h>
#include <DefaultColors.h>

namespace mray{
namespace GameMod{

GoalTraverseEdge::GoalTraverseEdge(AI::SteerController*steer,const AI::PathEdge& e,bool lastEdge)
:m_edge(e)
{
	m_lastEdge=lastEdge;
	m_state=AI::EGS_Inactive;
	m_steer=steer;
	m_stuckCounter=0;
}
GoalTraverseEdge::~GoalTraverseEdge()
{
}
int GoalTraverseEdge::GetGoalType()
{
	return EGoal_TraverseEdge;
}

void GoalTraverseEdge::pause()
{
	m_state=AI::EGS_Paused;
}
void GoalTraverseEdge::resume()
{
	activate();
}
void GoalTraverseEdge::activate()
{
	m_state=AI::EGS_Active;
	if(!m_lastEdge)
		m_steer->seekOn(m_edge.GetEnd());
	else
		m_steer->arriveOn(m_edge.GetEnd());

}
AI::EGoalStatus GoalTraverseEdge::process()
{
	if(m_state==AI::EGS_Inactive)
		activate();
	if(m_steer->getPhysics()->GetGlobalPosition().getDistSQ(m_edge.GetEnd())<3){
		m_state=AI::EGS_Completed;
	}
/*
	if(m_steer->getPhysics()->GetLinearVelocity().LengthSQ()<0.1)
		m_stuckCounter++;
	else
		m_stuckCounter=0;
	*/
	/*if(m_stuckCounter==5)
	{
		m_state=EGS_Failed;
		return m_state;
	}*/
/*
	math::vector3d pos=m_steer->getPhysics()->GetGlobalPosition();
	math::vector3d target=m_edge.GetEnd();//pos+m_steer->getPhysics()->GetLinearVelocity();//m_edge.end;
	pos.y=0;
	target.y=0;

	math::matrix4x4 m,lookAt;
	m.buildCameraLookAtLH(pos,target,math::vector3d::YAxis);
	m.getTransposeMatrix(lookAt);
	math::quaternion q(lookAt);
	math::vector3d angle;
	q*=m_steer->getPhysics()->GetGlobalOrintation().inverse();
	q.toEulerAngles(angle);


	angle.x=angle.z=0;
*/
	//m_steer->getPhysics()->SetAngularVelocity(angle*0.2);

	return m_state;
}
void GoalTraverseEdge::terminate()
{
	if(!m_lastEdge)
		m_steer->seekOff();
	else{
		m_steer->arriveOff();
		m_steer->getPhysics()->SetLinearVelocity(0);
		m_steer->getPhysics()->SetAngularVelocity(0);
	}
}

AI::EGoalStatus GoalTraverseEdge::getStatus()
{
	return m_state;
}

void GoalTraverseEdge::Visualize(AI::IRenderDevice*dev,const core::string&prefix)
{
	math::line3d l(m_steer->getPhysics()->GetGlobalPosition(),m_edge.GetEnd());
	l.pStart.y+=1;
	l.pEnd.y+=1;
	dev->drawLine(l,video::DefaultColors::Green);
	AI::IGoal::Visualize(dev,prefix);
}

}
}
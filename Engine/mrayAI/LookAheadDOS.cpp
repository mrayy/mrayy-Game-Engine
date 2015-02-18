#include "stdafx.h"

#include "LookAheadDOS.h"
#include "IRenderDevice.h"
#include "IAIPhysics.h"
#include <matrix4x4.h>
#include <MathUtil.h>
#include <DefaultColors.h>

#include "AIComponentTypes.h"
#include "IActorQuery.h"

namespace mray
{
namespace AI
{

	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_PROPERTY_TYPE_HEADER(DetectionDistance,LookAheadDOS,float,mT("DetectionDistance"),EPBT_Basic,mT(""),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(DetectionDistance,LookAheadDOS,float,SetDetectionDistance,GetDetectionDistance,core::StringConverter::toString,core::StringConverter::toFloat,false)

	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_PROPERTY_TYPE_HEADER(AvoidanceStrength,LookAheadDOS,float,mT("AvoidanceStrength"),EPBT_Basic,mT(""),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(AvoidanceStrength,LookAheadDOS,float,SetAvoidanceStrength,GetAvoidanceStrength,core::StringConverter::toString,core::StringConverter::toFloat,false)

	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_PROPERTY_TYPE_HEADER(BrakingStrength,LookAheadDOS,float,mT("BrakingStrength"),EPBT_Basic,mT(""),0.1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(BrakingStrength,LookAheadDOS,float,SetBrakingStrength,GetBrakingStrength,core::StringConverter::toString,core::StringConverter::toFloat,false)



LookAheadDOS::LookAheadDOS(AgentObstacle* agent,IActorQuery* query,float detectionDistance) :
	m_agent(agent),m_detectionDistance(detectionDistance),m_brakingStrength(0.1),m_avoidanceStrength(1),m_extendRad(2),m_query(query)
{
	CPropertieDictionary* dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeDetectionDistance::instance);
		dic->addPropertie(&PropertyTypeAvoidanceStrength::instance);
		dic->addPropertie(&PropertyTypeBrakingStrength::instance);
	}
}
LookAheadDOS::~LookAheadDOS()
{
}



void LookAheadDOS::CalculateAvoidanceForce()
{
	math::vector3d force;
	math::vector3d pos=m_agent->GetPhysics()->GetGlobalPosition();
	float radius=m_agent->GetRadius()+m_extendRad;

	float distClosest=math::Infinity;
	AgentObstacle*bestOb=0;
	math::vector3d bestObPos;

	//construct rotation matrix
	math::matrix4x4 orientMat;
	orientMat=math::MathUtil::CreateLookAtMatrix(pos,pos+m_agent->GetPhysics()->GetHeading(),math::vector3d::YAxis);

	const ObstacleList& obstacles=m_query->GetObstaclesList();

	ObstacleList::const_iterator it=obstacles.begin();
	for (;it!=obstacles.end();++it)
	{
		AgentObstacle*ob=*it;
		math::vector3d obPos=ob->GetPhysics()->GetGlobalPosition();
		//float radius=ob->GetVelocity();

		//convert obstacle position from world to agent space
		obPos=orientMat*obPos;
		if(obPos.z<=0 || obPos.z>m_detectionDistance)//behind agent or out of view?
			continue;

		float exbRad=ob->GetRadius()+radius;
		if(fabs(obPos.x)>exbRad)//doesn't overlap with agent's sphere?
			continue;

		//line/circle intersecttion

		float sqrtPart=sqrt(exbRad*exbRad - obPos.x*obPos.x);
		float dist=obPos.z - sqrtPart;
		if(dist <= 0)
			dist=obPos.z + sqrtPart;
		if(dist<distClosest)
		{
			distClosest=dist;
			bestOb=ob;
			bestObPos=obPos;
		}
	}
	if(bestOb)
	{
		float multiplier=/*1+*/(m_detectionDistance-bestObPos.z)/m_detectionDistance;
		force.x=(bestOb->GetRadius()-bestObPos.x)*multiplier*m_avoidanceStrength;
		force.z=(bestOb->GetRadius()-bestObPos.z)*multiplier*m_brakingStrength;
	}
	//orientMat.setTranslation(math::vector3d::Zero);
	force=orientMat.transformNormal(force);
	//return force;
	 
}

void LookAheadDOS::Visualize(IRenderDevice*dev)
{
	math::vector3d pos=m_agent->GetPhysics()->GetGlobalPosition();
	math::vector3d frontVec=m_agent->GetPhysics()->GetHeading();
	//frontVec.Normalize();
	float radius=m_agent->GetRadius()+m_extendRad;
	math::vector3d rightVec=frontVec.crossProduct(math::vector3d::YAxis);

	math::line3d l;
	l.pStart=pos+rightVec*radius;
	l.pEnd=l.pStart+frontVec*m_detectionDistance;
	dev->drawLine(l.pStart,l.pEnd,video::DefaultColors::Blue);

	l.pStart=pos-rightVec*radius;
	l.pEnd=l.pStart+frontVec*m_detectionDistance;
	dev->drawLine(l.pStart,l.pEnd,video::DefaultColors::Blue);
}

}
}

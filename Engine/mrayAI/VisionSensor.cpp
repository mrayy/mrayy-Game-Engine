#include "stdafx.h"


#include "VisionSensor.h"
#include "AISystem.h"
#include "AIActor.h"
#include "mMath.h"
#include "INavWorld.h"
#include "ITimer.h"
#include "ActorMemory.h"
#include "IAIPhysics.h"

#include <quaternion.h>
#include "IRenderDevice.h"

#include "AIComponentTypes.h"
#include "DefaultColors.h"
#include "Engine.h"

namespace mray{
namespace AI{



VisionSensor::VisionSensor(AIActor*owner,ActorMemory*mem)
{
	SetFOV(120);
	m_viewRange=50;

	m_owner=owner;
	m_memory=mem;

	CPropertieDictionary* dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeFoV::instance);
		dic->addPropertie(&PropertyTypeViewRange::instance);
	}
}
VisionSensor::~VisionSensor()
{
}



void VisionSensor::Update(float dt)
{
	math::vector3d pos=m_owner->GetPhysicalProp()->GetGlobalPosition();
	math::vector3d heading=m_owner->GetHeading();
	math::box3d area(pos-m_viewRange,pos+m_viewRange);

	AIActorList actors;
	float distSQ=m_viewRange*m_viewRange;

	m_visibleActors.clear();
	m_ActorObstacles.clear();

	//1st step, make all actors as not visible
	MemoryTicketList& memlist=m_memory->GetMemoryTickets();
	MemoryTicketList::iterator memIt=memlist.begin();
	for(;memIt!=memlist.end();++memIt)
	{
		MemoryTicket*tick=memIt->second;
		if(tick->visionTick)
			tick->visionTick->isVisible=false;
	}

	//get all actors that are in his view area
	m_owner->GetAISystem()->QueryActors(area,actors);
	INavWorld*world=m_owner->GetAISystem()->GetNavigationWorld();

	double time=gEngine.getTimer()->getSeconds();

	AIActorList::iterator it=actors.begin();
	for (;it!=actors.end();++it)
	{
		AIActor*actor=*it;
		if(actor==m_owner || !actor->GetPhysicalProp())
			continue;
		math::vector3d pos2=actor->GetPhysicalProp()->GetGlobalPosition();
		//check if it's inside his view sphere
		if(pos.getDistSQ(pos2)>distSQ)
			continue;	//not in his view distance

		math::vector3d dir=pos2-pos;
		dir.Normalize();

		//check if it's inside his Field of view
		if(dir.dotProduct(heading)<m_cosFov)
			continue;

		//check if the agent can see then other one (Ray casting)
		bool rc=world->lineOfSightTest(pos,m_owner->GetWorldNode(),pos2,actor->GetWorldNode(),0);
		if(rc){
			m_visibleActors.push_back(actor);
			m_ActorObstacles.push_back(actor->GetObstacle());
			//agent can see the other one!
			m_memory->AddVisionTicket(actor,VisionMemTick(time,pos2,actor->GetPhysicalProp()->GetLinearVelocity(),true));
		}
	}
}


bool VisionSensor::SetFOV(const float& f)
{
	m_fov=f;
	m_cosFov=math::cosd(m_fov*0.5);
	return true;
}
const float& VisionSensor::GetFOV()
{
	return m_fov;
}

bool VisionSensor::SetViewRange(const float& f)
{
	m_viewRange=f;
	return true;
}
const float& VisionSensor::GetViewRange()
{
	return m_viewRange;
}


AIActor* VisionSensor::GetOwnerAgent()
{
	return m_owner;
}
const AIActorList & VisionSensor::GetActorsList()
{
	return m_visibleActors;
}

const ObstacleList & VisionSensor::GetObstaclesList()
{
	return m_ActorObstacles;
}
void VisionSensor::Visualize(IRenderDevice*dev)
{
	int numberOfSteps=m_fov/20;
	float stepSize=m_fov/(float)numberOfSteps;
	math::vector3d pos=m_owner->GetPhysicalProp()->GetGlobalPosition();
	math::quaternion relOri=math::quaternion(-m_fov*0.5,math::vector3d::YAxis);
	math::quaternion oriStep=math::quaternion(stepSize,math::vector3d::YAxis);
	math::quaternion ori (m_owner->GetPhysicalProp()->GetGlobalOrintation());
	math::quaternion lastOri=ori*relOri;

	math::vector3d p1=pos+lastOri*math::vector3d::ZAxis*m_viewRange;
	math::vector3d p2;//=pos+ori*relOri.unitIUnverse()*math::vector3d::ZAxis*m_viewRange;

	math::line3d l1(pos,p1);
	dev->drawLine(l1.pStart,l1.pEnd,video::DefaultColors::White);

	for (int i=0;i<numberOfSteps;++i)
	{
		lastOri*=oriStep;
		p2=pos+lastOri*math::vector3d::ZAxis*m_viewRange;
		dev->drawLine(p1,p2,video::DefaultColors::White);
		p1=p2;
	}
	dev->drawLine(pos,p2,video::DefaultColors::White);
	
	AIActorList::iterator it=m_visibleActors.begin();
	for (;it!=m_visibleActors.end();++it)
	{
		AIActor*a=*it;
		math::line3d l1(a->GetPhysicalProp()->GetGlobalPosition(),math::vector3d(0,10,0));
		l1.pEnd+=l1.pStart;
		dev->drawLine(l1.pStart,l1.pEnd,video::DefaultColors::White);
	}

// 	math::line3d l1(pos,pos+m_owner->GetHeading()*m_viewRange);
// 	dev->drawLine(l1,255);
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(FoV,VisionSensor,float,mT("FoV"),EPBT_Basic,mT(""),120);
IMPLEMENT_PROPERTY_TYPE_GENERIC(FoV,VisionSensor,float,SetFOV,GetFOV,core::StringConverter::toString,core::StringConverter::toFloat,false)

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(ViewRange,VisionSensor,float,mT("ViewRange"),EPBT_Basic,mT(""),50);
IMPLEMENT_PROPERTY_TYPE_GENERIC(ViewRange,VisionSensor,float,SetViewRange,GetViewRange,core::StringConverter::toString,core::StringConverter::toFloat,false)

}
}


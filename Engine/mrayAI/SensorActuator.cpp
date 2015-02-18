#include "stdafx.h"

#include "SensorActuator.h"
#include "AgentWorldSensor.h"
#include "AIComponentTypes.h"
#include "IAIPhysics.h"
#include "AgentObstacle.h"
#include "IActorQuery.h"
#include "SensorSet.h"

namespace mray
{
namespace AI
{

SensorActuator::SensorActuator(AgentObstacle* agent,SensorSet*sensors)
:m_sensors(sensors),m_agent(agent)
{
}
SensorActuator::~SensorActuator()
{
}

void SensorActuator::Update(float dt)
{
	m_resultLinearForce=math::vector3d::Zero;
	m_resultAngularForce=math::vector3d::Zero;
	ActuatorList::iterator it=m_actuators.begin();
	for(;it!=m_actuators.end();++it)
	{
		ActuatorInfo& i=(*it);
		if(i.sensor->IsCollide())
		{
			float d=1-i.sensor->GetCollisionDistanceNorm();
			m_resultLinearForce+=i.linearForce*d;
			m_resultAngularForce+=i.angularForce*d;
		}
	}
	m_resultLinearForce=m_agent->GetPhysics()->GetGlobalOrintation()*m_resultLinearForce;
	m_resultAngularForce=/*m_agent->GetPhysics()->GetGlobalOrintation()**/m_resultAngularForce;
}

void SensorActuator::AddActuator(const core::string&name,const math::vector3d &linearForce,const math::vector3d &angularForce)
{
	AgentWorldSensor *sens= m_sensors->GetSensor(name);
	if(!sens)
		return;

	ActuatorInfo i;
	i.name=name;
	i.sensor=sens;
	i.angularForce=angularForce;
	i.linearForce=linearForce;
	m_actuators.push_back(i);
}


const math::vector3d& SensorActuator::GetResultLinearForce()
{
	return m_resultLinearForce;
}
const math::vector3d& SensorActuator::GetResultAngularForce()
{
	return m_resultAngularForce;
}



xml::XMLElement* SensorActuator::loadXMLSettings(xml::XMLElement* elem)
{
	IAIComponent::loadFromXML(elem);
	xml::xmlSubElementsMapIT it= elem->getElementsBegin();
	xml::xmlSubElementsMapIT end= elem->getElementsEnd();

	xml::XMLAttribute*attr;

	for (;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("Actuator")))
		{
			math::vector3d linearForce;
			math::vector3d angularForce;
			core::string name;
			bool senseDyn=false;
			attr= e->getAttribute(mT("Name"));
			if(attr)
			{
				name=attr->value;
			}
			attr= e->getAttribute(mT("LinearForce"));
			if(attr)
			{
				linearForce=core::StringConverter::toVector3d(attr->value);
			}
			attr= e->getAttribute(mT("AngularForce"));
			if(attr)
			{
				angularForce=core::StringConverter::toVector3d(attr->value);
			}
			AddActuator(name,linearForce,angularForce);
		}
	}
	return elem;
}

}
}

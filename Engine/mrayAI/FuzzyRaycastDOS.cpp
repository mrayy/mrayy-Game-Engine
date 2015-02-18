#include "stdafx.h"


#include "FuzzyRaycastDOS.h"
#include "IAIPhysics.h"
#include "AIComponentTypes.h"
#include "AIFuzzyComponent.h"
#include "ILogManager.h"
#include "SensorSet.h"
#include "AgentWorldSensor.h"


namespace mray
{
namespace AI
{


FuzzyRaycastDOS::FuzzyRaycastDOS(AgentObstacle* agent,SensorSet* sensors):
	m_agent(agent),m_sensors(sensors),m_speed(0)
{
	m_fuzzyModule=new AIFuzzyComponent();
}
FuzzyRaycastDOS::~FuzzyRaycastDOS()
{
	delete m_fuzzyModule;
}

void FuzzyRaycastDOS::CalculateAvoidanceForce()
{/*
	AgentObstacle*best=0;
	float bestDist=math::Infinity;
	math::vector3d pos=m_agent->GetPhysics()->GetGlobalPosition();

	ObstacleList::const_iterator it=obstacles.begin();
	for (;it!=obstacles.end();++it)
	{
		AgentObstacle*o=*it;
		float dist=o->GetPhysics()->GetGlobalPosition().getDistSQ(pos);
		if(dist<bestDist){
			best=o;
			bestDist=dist;
		}
	}

	if(!best)
		return math::vector3d::Zero;
*/

	SensorVarList::iterator it=m_sensorVars.begin();
	for(;it!=m_sensorVars.end();++it){
		AgentWorldSensor*sensor= m_sensors->GetSensor(it->sensor);
		if(!sensor)
			continue;
		m_fuzzyModule->Fuzzify(it->variable,sensor->GetCollisionDistanceNorm());
	}

	m_fuzzyModule->ApplyRules();

	m_speed=m_fuzzyModule->Defuzzify(mT("Speed"));
	float rotation=m_fuzzyModule->Defuzzify(mT("Rotation"));

	m_orientaion.fromAngle(rotation,math::vector3d::YAxis);
	//m_orientaion*=m_agent->GetPhysics()->GetGlobalOrintation();
	/*
	q*=m_agent->GetPhysics()->GetGlobalOrintation();
	math::vector3d result=q*math::vector3d::ZAxis*speed*100;



	return result;*/
}


const math::quaternion& FuzzyRaycastDOS::GetResultOrientation()
{
	return m_orientaion;
}
float FuzzyRaycastDOS::GetResultSpeed()
{
	return m_speed;
}

xml::XMLElement* FuzzyRaycastDOS::loadXMLSettings(xml::XMLElement* elem)
{
	IAIComponent::loadXMLSettings(elem);
	xml::XMLAttribute*attr=0;
	xml::XMLElement*e= elem->getSubElement(mT("FuzzyModule"));
	if(!e)
	{
		//gLogManager.log(mT("Cann't Find Fuzzy Expert System tag in Fuzzy Dynamic Obstacles solvers"),ELL_ERROR);
		return elem;
	}
	m_fuzzyModule->loadXMLSettings(e);

	xml::xmlSubElementsMapIT it=elem->getElementsBegin();
	xml::xmlSubElementsMapIT end=elem->getElementsEnd();
	for(;it!=end;++it){

		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("MapSensorVar")))
		{
			SensorVarInfo sv;
			attr=e->getAttribute(mT("sensor"));
			if(!attr)continue;
			sv.sensor=attr->value;

			attr=e->getAttribute(mT("var"));
			if(!attr)continue;

			sv.variable=attr->value;
			m_sensorVars.push_back(sv);
		}
	}
	return elem;
}


}
}


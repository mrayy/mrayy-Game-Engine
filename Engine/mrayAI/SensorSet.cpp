#include "stdafx.h"

#include "SensorSet.h"
#include "AgentWorldSensor.h"
#include "AIComponentTypes.h"
#include "IAIPhysics.h"
#include "AgentObstacle.h"
#include "IActorQuery.h"


namespace mray
{
namespace AI
{

SensorSet::SensorSet(INavWorld*w,AgentObstacle*agent,IActorQuery*query)
:m_world(w),m_agent(agent),m_query(query)
{
}
SensorSet::~SensorSet()
{
	SensorsList::iterator it=m_sensors.begin();
	for(;it!=m_sensors.end();++it)
	{
		delete it->second.sensor;
	}
}


void SensorSet::AddSensor(const core::string&name,AgentWorldSensor* sensor,bool senseDynamicObstacles)
{
	core::string str=name;
	str.make_lower();
	SensorInfo i;
	i.name=name;
	i.sensor=sensor;
	i.senseDynamicObstacles=senseDynamicObstacles;
	m_sensors[str]=i;
}
AgentWorldSensor* SensorSet::GetSensor(const core::string&name)
{
	core::string str=name;
	str.make_lower();
	SensorsList::iterator it= m_sensors.find(str);
	if(it==m_sensors.end())
		return 0;
	return it->second.sensor;
}

void SensorSet::Update(float dt)
{
	SensorsList::iterator it=m_sensors.begin();
	for(;it!=m_sensors.end();++it)
	{
		SensorInfo& i=it->second;
		if(m_query && i.senseDynamicObstacles)
			i.sensor->SenseEnvironmentAndDyn(m_query->GetObstaclesList());
		else 
			i.sensor->SenseEnvironment();
	}
}

xml::XMLElement* SensorSet::loadXMLSettings(xml::XMLElement* elem)
{
	IAIComponent::loadFromXML(elem);
	xml::xmlSubElementsMapIT it= elem->getElementsBegin();
	xml::xmlSubElementsMapIT end= elem->getElementsEnd();

	xml::XMLAttribute*attr;

	for (;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(WorldSensor::getClassType()))
		{
			core::string name;
			bool senseDyn=false;
			AgentWorldSensor*s=new AgentWorldSensor(m_world,m_agent);
			s->loadFromXML(e);
			attr= e->getAttribute(mT("Name"));
			if(attr)
			{
				name=attr->value;
			}
			attr= e->getAttribute(mT("DynamicObstacles"));
			if(attr)
			{
				senseDyn=core::StringConverter::toBool(attr->value);
			}
			AddSensor(name,s,senseDyn);
		}
	}
	return elem;
}
void SensorSet::Visualize(IRenderDevice*dev)
{
	SensorsList::iterator it=m_sensors.begin();
	for(;it!=m_sensors.end();++it)
	{
		it->second.sensor->Visualize(dev);
	}
}


}
}


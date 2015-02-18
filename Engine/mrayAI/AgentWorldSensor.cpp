#include "stdafx.h"

#include "AgentWorldSensor.h"
#include "AgentObstacle.h"
#include "IAIPhysics.h"

#include "AIComponentTypes.h"

namespace mray
{
namespace AI
{
	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_PROPERTY_TYPE_HEADER(Length,AgentWorldSensor,float,mT("Length"),EPBT_Basic,mT("Sensor's Length"),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Length,AgentWorldSensor,float,SetLength,GetLength,core::StringConverter::toString,core::StringConverter::toFloat,false)

	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_PROPERTY_TYPE_HEADER(RelativeAngle,AgentWorldSensor,vector3d,mT("RelativeAngle"),EPBT_Struct,mT("Sensor's Angles"),0);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(RelativeAngle,AgentWorldSensor,math::vector3d,SetRelativeAngle,GetRelativeAngle,core::StringConverter::toString,core::StringConverter::toVector3d,false)



AgentWorldSensor::AgentWorldSensor(INavWorld* world,AgentObstacle* a)
:WorldSensor(world), m_agent(a),m_length(PropertyTypeLength::instance.GetDefaultValue())
{
	CPropertieDictionary* dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeLength::instance);
		dic->addPropertie(&PropertyTypeRelativeAngle::instance);
	}
}
AgentWorldSensor::~AgentWorldSensor()
{
}


float AgentWorldSensor::GetCollisionDistanceNorm()
{
	if(!m_isCollide)
		return 1;
	return GetCollisionDistance()/m_length;
}
void AgentWorldSensor::AttachAgent(AgentObstacle* agent)
{
	m_agent=agent;
}
AgentObstacle* AgentWorldSensor::GetAgent()
{
	return m_agent;
}

bool AgentWorldSensor::SenseEnvironment()
{
	if(!m_agent)
		return false;

	m_absOrination=m_agent->GetPhysics()->GetGlobalOrintation()*m_relativeOrination;
	math::vector3d dir=m_absOrination*math::vector3d::ZAxis;
	m_pos=m_agent->GetPhysics()->GetGlobalPosition()+dir*m_agent->GetRadius()*0.2;
	m_target=m_pos+ dir*m_length;

	return WorldSensor::SenseEnvironment();
}
/*
void AgentWorldSensor::SetRelativeOrination(const math::quaternion& ort)
{
	m_relativeOrination=ort;
}
const math::quaternion& AgentWorldSensor::GetRelativeOrination()
{
	return m_relativeOrination;
}*/

bool AgentWorldSensor::SetLength(const float& len)
{
	m_length=len;
	return true;
}
const float& AgentWorldSensor::GetLength()
{
	return m_length;
}

bool AgentWorldSensor::SetRelativeAngle(const math::vector3d& a)
{
	m_relAngle=a;
	m_relativeOrination.fromEulerAngles(m_relAngle.x,m_relAngle.y,m_relAngle.z);
	return true;
}
const math::vector3d& AgentWorldSensor::GetRelativeAngle()
{
	return m_relAngle;
}

}
}

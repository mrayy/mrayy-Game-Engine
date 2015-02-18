

/********************************************************************
	created:	2010/04/09
	created:	9:4:2010   1:19
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AgentWorldSensor.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AgentWorldSensor
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AgentWorldSensor_h__
#define AgentWorldSensor_h__

#include "WorldSensor.h"
#include <quaternion.h>

namespace mray
{
namespace AI
{
	class AgentObstacle;

class MRAY_AI_DLL AgentWorldSensor:public WorldSensor
{
	DECLARE_RTTI
private:
protected:
	math::quaternion m_relativeOrination;
	math::quaternion m_absOrination;

	float m_length;
	
	math::vector3d m_relAngle;

	AgentObstacle* m_agent;
	/*
	rwProperty<AgentWorldSensor,float> Length;
	rwProperty<AgentWorldSensor,math::vector3d> RelativeAngle;*/

	DECLARE_PROPERTY_TYPE(Length,float,MRAY_AI_DLL);
	DECLARE_PROPERTY_TYPE(RelativeAngle,math::vector3d,MRAY_AI_DLL);

public:
	AgentWorldSensor(INavWorld* world,AgentObstacle* a);
	virtual ~AgentWorldSensor();

	virtual float GetCollisionDistanceNorm();

	void AttachAgent(AgentObstacle* agent);
	AgentObstacle* GetAgent();

	virtual bool SenseEnvironment();
/*
	void SetRelativeOrination(const math::quaternion& ort);
	const math::quaternion& GetRelativeOrination();*/

	bool SetLength(const float& len);
	const float& GetLength();

	bool SetRelativeAngle(const math::vector3d& a);
	const math::vector3d& GetRelativeAngle();

};

}
}

#endif // AgentWorldSensor_h__


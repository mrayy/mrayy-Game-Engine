

/********************************************************************
	created:	2010/05/05
	created:	5:5:2010   8:36
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\FuzzyRaycastDOS.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	FuzzyRaycastDOS
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef FuzzyRaycastDOS_h__
#define FuzzyRaycastDOS_h__

#include "compileConfig.h"
#include "IDynamicObstacleSolver.h"

namespace mray
{
namespace AI
{

	class AIFuzzyComponent;
	class SensorSet;
	class AgentWorldSensor;

class MRAY_AI_DLL FuzzyRaycastDOS:public IDynamicObstacleSolver
{
	DECLARE_RTTI
private:
protected:

	struct SensorVarInfo
	{
		core::string sensor;
		core::string variable;
	};

	typedef std::list<SensorVarInfo> SensorVarList;

	SensorVarList m_sensorVars;

	AIFuzzyComponent* m_fuzzyModule;
	AgentObstacle* m_agent;
	SensorSet* m_sensors;

	math::quaternion m_orientaion;
	float m_speed;
public:
	FuzzyRaycastDOS(AgentObstacle* agent,SensorSet* sensors);
	virtual ~FuzzyRaycastDOS();

	virtual void CalculateAvoidanceForce();

	virtual const math::quaternion& GetResultOrientation();
	virtual float GetResultSpeed();

	xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
};

}
}

#endif // FuzzyRaycastDOS_h__


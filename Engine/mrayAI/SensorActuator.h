

/********************************************************************
	created:	2010/04/20
	created:	20:4:2010   13:39
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\SensorActuator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	SensorActuator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	consist of set of world sensors,depending on their readings,a certain force gonna be generated
				(angular and linear forces)
*********************************************************************/

#ifndef SensorActuator_h__
#define SensorActuator_h__

#include "CompileConfig.h"
#include "IAIComponent.h"
#include <list>

namespace mray
{
namespace AI
{

	class AgentWorldSensor;
	class SensorSet;
	class AgentObstacle;

class MRAY_AI_DLL SensorActuator:public IAIComponent
{
	DECLARE_RTTI
private:
protected:

	struct ActuatorInfo
	{
		core::string name;
		AgentWorldSensor* sensor;
		math::vector3d linearForce;
		math::vector3d angularForce;
		bool senseDynamicObstacles;
	};

	typedef std::list<ActuatorInfo> ActuatorList;
	ActuatorList m_actuators;

	math::vector3d m_resultLinearForce;
	math::vector3d m_resultAngularForce;

	SensorSet* m_sensors;
	AgentObstacle* m_agent;
public:
	SensorActuator(AgentObstacle* agent,SensorSet*sensors);
	virtual ~SensorActuator();

	const math::vector3d& GetResultLinearForce();
	const math::vector3d& GetResultAngularForce();

	virtual void Update(float dt);


	void AddActuator(const core::string&name,const math::vector3d &linearForce,const math::vector3d &angularForce);

	//virtual void Visualize(IRenderDevice*dev);

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
};

}
}


#endif // SensorActuator_h__


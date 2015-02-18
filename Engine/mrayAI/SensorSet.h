

/********************************************************************
	created:	2010/05/06
	created:	6:5:2010   16:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\SensorSet.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	SensorSet
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef SensorSet_h__
#define SensorSet_h__

#include "IAIComponent.h"
#include <map>

namespace mray
{
namespace AI
{
	class AgentWorldSensor;
	class INavWorld;
	class AgentObstacle;
	class IActorQuery;

class MRAY_AI_DLL SensorSet:public IAIComponent
{
	DECLARE_RTTI
private:
protected:
	struct SensorInfo
	{
		SensorInfo():sensor(0),senseDynamicObstacles(false)
		{}
		core::string name;
		AgentWorldSensor* sensor;
		bool senseDynamicObstacles;
	};
	typedef std::map<core::string,SensorInfo> SensorsList;
	SensorsList m_sensors;

	INavWorld* m_world;
	AgentObstacle* m_agent;

	IActorQuery* m_query;
public:
	SensorSet(INavWorld*w,AgentObstacle*agent,IActorQuery*query);
	virtual ~SensorSet();

	void AddSensor(const core::string&name,AgentWorldSensor* sensor,bool senseDynamicObstacles=false);
	AgentWorldSensor* GetSensor(const core::string&name);

	virtual void Update(float dt);

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);

	virtual void Visualize(IRenderDevice*dev);
};

}
}

#endif // SensorSet_h__

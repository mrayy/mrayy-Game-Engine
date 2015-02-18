

/********************************************************************
	created:	2010/04/09
	created:	9:4:2010   1:09
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\WorldSensor.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	WorldSensor
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	it's used to sense the enviroment for collision with walls,*dynamic obstacles*,...
*********************************************************************/

#ifndef WorldSensor_h__
#define WorldSensor_h__

#include "compileConfig.h"
#include "IAIComponent.h"
#include "AgentObstacle.h"
#include <point3d.h>

namespace mray
{
namespace AI
{

	class INavWorld;
	class INavWorldNode;
	class IRenderDevice;

class MRAY_AI_DLL WorldSensor:public IAIComponent
{
	DECLARE_RTTI
private:
protected:
	math::vector3d m_pos;
	math::vector3d m_target;
	math::vector3d m_collision;
	bool m_isCollide;
	float m_distance;

	INavWorldNode* m_beginNode;
	INavWorldNode* m_endNode;

	INavWorld* m_world;

public:
	WorldSensor(INavWorld* world);
	virtual ~WorldSensor();


	virtual bool SenseEnvironment();
	virtual bool SenseEnvironmentAndDyn(const ObstacleList& obstacles);
	const math::vector3d& GetCollisionPoint();

	bool IsCollide();
	float GetCollisionDistance();

	//return collision distance from 0->1
	//0:full collision
	//1:no collision
	virtual float GetCollisionDistanceNorm();

	void SetPosition(const math::vector3d&pos);
	const math::vector3d& GetPosition();

	void SetTarget(const math::vector3d&pos);
	const math::vector3d& GetTarget();

	virtual void Visualize(IRenderDevice*dev);
};

}
}

#endif // WorldSensor_h__

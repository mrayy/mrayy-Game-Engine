
/********************************************************************
	created:	2009/07/17
	created:	17:7:2009   19:38
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AgentObstacle.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AgentObstacle
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___AgentObstacle___
#define ___AgentObstacle___

#include "CompileConfig.h"
#include "IAIComponent.h"
#include "IAIObstacleShape.h"
#include <Point3d.h>
#include <list>

namespace mray{
namespace AI{

	class IAIPhysics;

class MRAY_AI_DLL AgentObstacle:public IAIComponent
{
	DECLARE_RTTI
private:
protected:
	float m_radius;
	IAIPhysics* m_physics;
	//float& _GetRadius();
	//rwProperty<AgentObstacle,float>* Radius;
	IAIObstacleShape* m_shape;

	std::vector<math::vector2d> m_obstaclePoints;

public:
	AgentObstacle(IAIPhysics* ph);
	virtual ~AgentObstacle();

	IAIObstacleShape* GetShape();
	void SetShape(IAIObstacleShape*shape);

	bool IntersectWithLine(const math::line2d& l,math::vector2d*interPoint);

// 
 	void SetRadius(const float& r);
 	float GetRadius()const;

	IAIPhysics* GetPhysics()const;
	void SetPhysics(IAIPhysics*ph);

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);

	virtual void Visualize(IRenderDevice*dev);
};

typedef std::list<AgentObstacle*> ObstacleList;

}
}


#endif //___AgentObstacle___

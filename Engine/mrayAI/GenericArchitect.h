
/********************************************************************
	created:	2010/04/03
	created:	3:4:2010   18:54
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\GenericArchitect.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	GenericArchitect
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	more like an example of creating an arhitect for the agent
*********************************************************************/

#ifndef ___GenericArchitect___
#define ___GenericArchitect___

#include "compileConfig.h"
#include "IAIArchitect.h"
#include "Point3d.h"

namespace mray{
namespace AI{

	class ActorMemory;
	class Thinking;
	class SteerController;
	class VisionSensor;
	class PathFindPlanner;
	class IDynamicObstacleSolver;
	class AIMovmentTracker;

class MRAY_AI_DLL GenericArchitect:public IAIArchitect
{
private:
protected:
	AIActor* m_owner;

	ActorMemory* m_memory;
	
	Thinking* m_brain;
	
	SteerController* m_steering;
	IDynamicObstacleSolver* m_dynamicObsSolver;

	VisionSensor* m_visionSensor;

	PathFindPlanner* m_pathPlanner;

	AgentObstacle* m_obstacle;

	AIMovmentTracker *m_movementTracker;

	math::vector3d m_totalLinearForce;
	math::vector3d m_totalAngularForce;

	virtual void UpdateInternal(float dt);
	void UpdateMovement(float dt);
public:
	GenericArchitect(AIActor* owner);
	virtual~GenericArchitect();

	virtual AgentObstacle* GetObstacle();

	Thinking* GetBrain();
	ActorMemory* GetMemory();
	SteerController* GetSteering();
	PathFindPlanner* GetPathFinder();
	VisionSensor* GetVisionSensor();
	IDynamicObstacleSolver* GetDynamicObstacleSolver();

	void AccumulateLinearForce(const math::vector3d&f);
	void AccumulateAngularForce(const math::vector3d&f);

	void SetDynamicObstacleSolver(IDynamicObstacleSolver*dos);

	virtual bool IsAIControlled();

	void Update(float dt);

	virtual bool OnMessageTelegram(const AIMessageTelegram&msg);

	virtual void Visualize(IRenderDevice*dev);
};

}
}


#endif //___GenericArchitect___

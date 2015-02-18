
/********************************************************************
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AIAgent.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AIAgent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	this class represent general AI Agent,
				it's important to attach to this agent a physical interface,
				so it can gather physical information about the entity it self
*********************************************************************/

#ifndef ___AIAgent___
#define ___AIAgent___

#include <mString.h>
#include <TypedProperty.h>
#include "compileConfig.h"
#include "IAIPhysics.h"
#include "AIActor.h"

namespace mray{
namespace AI{

class MRAY_AI_DLL AIAgent:public AIActor
{
protected:
	core::string m_name;
	
	math::vector3d m_heading;
	math::vector3d m_up;
	/*math::vector3d m_speed;
	math::vector3d m_force;
	
	float m_maxSpeed;
	float m_brakingRate;
*/

	IAIPhysics* m_physicalProp;

	AISystem* m_system;

	INavWorldNode* m_worldNode;

	int m_ID;

/*
	ActorMemory* m_memory;
	GoalThink* m_brain;
	SteerController* m_steering;


	VisionSensor* m_visionSensor;

	PathFindPlanner* m_pathPlanner;
*/
	IAIArchitect* m_controller;
	bool m_enableVis;

	void UpdateMovement(float dt);

	DECLARE_PROPERTY_TYPE(Name,core::string,MRAY_AI_DLL);
public:

	AIAgent(IAIPhysics* phNode,const core::string &name);
	virtual~AIAgent();

	//virtual GoalThink* GetBrain(){return m_brain;}

	virtual bool SetID(uint id);
	virtual uint GetID();

	const core::string& GetName() { return m_name; }
	bool SetName(const core::string &val) { m_name = val; return true;}

	virtual void SetAISystem(AISystem*s);
	virtual AISystem* GetAISystem();
	virtual INavWorld* GetNavWorld();
	virtual AgentObstacle* GetObstacle();

	virtual IAIPhysics* GetPhysicalProp()const{
		return m_physicalProp;
	}

	virtual void AttachController(IAIArchitect*controller);
	virtual IAIArchitect* GetController();

	virtual const math::vector3d& GetUp()const{return m_up;}
	virtual const math::vector3d& GetHeading()const{return m_heading;}
/*
	virtual void SetSpeed(const math::vector3d& s);
	virtual const math::vector3d& GetSpeed()const;

	virtual void SetForce(const math::vector3d& s);
	virtual const math::vector3d& GetForce();

	virtual void SetMaxSpeed(float s);
	virtual float GetMaxSpeed()const;
*/

	virtual bool IsAIControlled();

//	ActorMemory* GetMemory();

	virtual void Update(float dt);

	virtual INavWorldNode* GetWorldNode();

	virtual void UpdateWorldNode();

	//PathFindPlanner* GetPathFinder();

	virtual bool OnMessageTelegram(const AIMessageTelegram&msg);

	virtual void EnableVisualize(bool e);
	virtual bool IsEnableVisualize();

	virtual void Visualize(IRenderDevice*dev);
};


}
}



#endif




/********************************************************************
	created:	2010/06/27
	created:	27:6:2010   17:13
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\AICharacterArch.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	AICharacterArch
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AICharacterArch_h__
#define AICharacterArch_h__

#include "AIGameArch.h"
#include "CharacterObject.h"

namespace mray
{
	namespace game
	{
		class GameEntity;
	}

namespace AI
{
	class ActorMemory;
	class Thinking;
	class SteerController;
	class VisionSensor;
	class PathFindPlanner;
	class IDynamicObstacleSolver;
	class AIMovmentTracker;
	class SensorActuator;
	class SensorSet;
}
namespace GameMod
{

class CHARACTERDEMOGM_API AICharacterArch:public AIGameArch
{
private:
protected:
	AI::ActorMemory* m_memory;
	AI::Thinking* m_brain;
	AI::SteerController* m_steering;
	AI::IDynamicObstacleSolver* m_dynamicObsSolver;
	AI::VisionSensor* m_visionSensor;
	AI::PathFindPlanner* m_pathPlanner;
	AI::AgentObstacle* m_obstacle;
	AI::AIMovmentTracker *m_movementTracker;
	//AI::SensorActuator* m_actuator;
	AI::SensorSet* m_sensor;

	game::GameEntity* m_entity;

	math::vector3d m_totalLinearForce;
	math::vector3d m_totalAngularForce;

	math::vector3d m_lastLinearForce;

	CharacterObject* m_charObject;

	bool m_dynamicAvoid;

	void UpdateInternal(float dt);
	void UpdateMovement(float dt);

	void AccumulateLinearForce(const math::vector3d&f);
	void AccumulateAngularForce(const math::vector3d&f);
public:
	AICharacterArch(game::GameEntity*entity,animation::IAnimationMixer*mixer,bool animateLegs,bool physicsBased);
	virtual ~AICharacterArch();

	void EnableDynamicAvoidance(bool e);
	bool IsDynamicAvoidanceEnabled();

	math::vector3d GetLastLinearVelocity();
	
	AI::Thinking* GetBrain();
	game::GameEntity* GetEntity(){return m_entity;}

	CharacterObject* GetCharacterObject(){return m_charObject;}
	virtual AI::AgentObstacle* GetObstacle();

	virtual void Update(float dt);

	virtual bool OnMessageTelegram(const AI::AIMessageTelegram& msg);
};

}
}


#endif // AICharacterArch_h__

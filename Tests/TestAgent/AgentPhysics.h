
/********************************************************************
	created:	2010/04/03
	created:	3:4:2010   20:40
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent\AgentPhysics.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent
	file base:	AgentPhysics
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___AgentPhysics___
#define ___AgentPhysics___

#include "IAIPhysics.h"

namespace mray{
namespace physics
{
	class ICharacterController;
}
namespace AI{


class AgentPhysics:public IAIPhysics
{
private:
protected:
	physics::ICharacterController* m_phNode;
	math::quaternion m_ori;
	math::vector3d m_angularVeloctity;
	math::quaternion m_oriVel;
public:
	AgentPhysics(physics::ICharacterController*phNode);
	virtual~AgentPhysics();

	virtual float	GetMass();

	virtual void Update(float dt);

	virtual void SetGlobalPosition(const math::vector3d&pos);
	virtual math::vector3d GetGlobalPosition();

	virtual void SetGlobalOrintation(const math::quaternion&or);
	virtual math::quaternion GetGlobalOrintation();

	virtual void SetLinearDamping(float d);
	virtual float GetLinearDamping();

	virtual void SetLinearVelocity(const math::vector3d&v);
	virtual math::vector3d GetLinearVelocity();

	virtual void SetAngularVelocity(const math::vector3d& v);
	virtual math::vector3d GetAngularVelocity();

	virtual void SetMaxAngularVelocity(float m);
	virtual float GetMaxAngularVelocity()const;

	virtual void SetMaxSpeed(float s);
	virtual float GetMaxSpeed()const;

};

}
}

#endif //___AgentPhysics___

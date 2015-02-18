


/********************************************************************
	created:	2010/07/01
	created:	1:7:2010   17:59
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\PhysicalObject.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	PhysicalObject
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef PhysicalObject_h__
#define PhysicalObject_h__

#include "IAIPhysics.h"

namespace mray
{
namespace physics
{
	class IPhysicalNode;
}
namespace GameMod
{

class PhysicalObject:public AI::IAIPhysics
{
private:
protected:
	physics::IPhysicalNode* m_node;
	float m_maxSpeed;
public:
	PhysicalObject(physics::IPhysicalNode* node);
	virtual ~PhysicalObject();

	virtual float	GetMass();

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

#endif // PhysicalObject_h__

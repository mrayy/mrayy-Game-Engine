

/********************************************************************
	created:	2010/07/11
	file base:	PhCharacterPM
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhCharacterPM__
#define __PhCharacterPM__

#include "CharacterGait.h"
#include "IAIPhysics.h"

namespace mray
{
namespace GameMod
{

class PhCharacterPM:public AI::IAIPhysics
{
private:
protected:
	PhysicsBasedCharacter::CharacterGait* m_gait;
	float m_maxSpeed;
	float m_YOffset;
public:
	PhCharacterPM(PhysicsBasedCharacter::CharacterGait* gait,float YOffset);
	virtual~PhCharacterPM();

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

#endif

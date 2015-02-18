

/********************************************************************
	created:	2010/06/27
	created:	27:6:2010   19:12
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\CharControlerPM.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	CharControlerPM
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef CharControlerPM_h__
#define CharControlerPM_h__

#include "IAIPhysics.h"

namespace mray
{
namespace physics
{
	class ICharacterController;
}
namespace GameMod
{

class CharControlerPM:public AI::IAIPhysics
{
private:
protected:
	physics::ICharacterController* m_phNode;
	math::quaternion m_ori;
	math::vector3d m_angularVeloctity;
	math::quaternion m_oriVel;
	float m_maxAngularVel;

	float m_gravity;
	float m_YOffset;
public:
	CharControlerPM(physics::ICharacterController*phNode,float YOffset=0,float gravity=0);
	virtual ~CharControlerPM();

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


#endif // CharControlerPM_h__

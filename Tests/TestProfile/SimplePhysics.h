

/********************************************************************
	created:	2010/07/01
	created:	1:7:2010   13:40
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\SimplePhysics.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	SimplePhysics
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef SimplePhysics_h__
#define SimplePhysics_h__

#include "CompileConfig.h"
#include "IAIPhysics.h"

namespace mray
{
namespace GameMod
{

class SimplePhysics:public AI::IAIPhysics
{
private:
protected:
	math::vector3d m_position;
	math::quaternion m_ori;
public:
	SimplePhysics();
	virtual ~SimplePhysics();

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
#endif // SimplePhysics_h__

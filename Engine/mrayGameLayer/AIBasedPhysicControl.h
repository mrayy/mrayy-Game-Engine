

/********************************************************************
	created:	2010/06/17
	created:	17:6:2010   18:47
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGameLayer\AIBasedPhysicControl.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGameLayer
	file base:	AIBasedPhysicControl
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AIBasedPhysicControl_h__
#define AIBasedPhysicControl_h__

#include "CompileConfig.h"
#include "IPhysicsControl.h"

namespace mray
{
namespace AI
{
	class IAIPhysics;
}
namespace game
{

class MRAY_GAME_DLL AIBasedPhysicControl:public IPhysicsControl
{
private:
protected:
	AI::IAIPhysics* m_physics;
public:
	AIBasedPhysicControl(AI::IAIPhysics* physics);
	virtual ~AIBasedPhysicControl();

	virtual float	GetMass();

	virtual void SetPosition(const math::vector3d&pos);
	virtual math::vector3d GetPosition();

	virtual void SetOrintation(const math::quaternion&or);
	virtual math::quaternion GetOrintation();

	virtual void SetLinearVelocity(const math::vector3d&v);
	virtual math::vector3d GetLinearVelocity();

	virtual void SetAngularVelocity(const math::vector3d& v);
	virtual math::vector3d GetAngularVelocity();

	virtual void SetMaxAngularVelocity(float m);
	virtual float GetMaxAngularVelocity()const;

	virtual void SetMaxSpeed(float s);
	virtual float GetMaxSpeed()const;	

	virtual math::vector3d GetUp();
	virtual math::vector3d GetHeading();
};

}
}

#endif // AIBasedPhysicControl_h__

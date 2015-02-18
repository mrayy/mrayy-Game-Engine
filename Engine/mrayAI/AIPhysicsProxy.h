

/********************************************************************
	created:	2010/06/27
	created:	27:6:2010   19:28
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AIPhysicsProxy.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AIPhysicsProxy
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AIPhysicsProxy_h__
#define AIPhysicsProxy_h__

#include "IAIPhysics.h"

namespace mray
{
namespace AI
{

class MRAY_AI_DLL AIPhysicsProxy:public IAIPhysics
{
private:
protected:
	IAIPhysics* m_physics;
public:
	AIPhysicsProxy(IAIPhysics*ph);
	virtual ~AIPhysicsProxy();

	void SetPhysics(IAIPhysics*ph);
	IAIPhysics* GetPhysics();

	virtual void Update(float dt);
	virtual void Visualize(IRenderDevice*dev);

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

	virtual math::vector3d GetUp();
	virtual math::vector3d GetHeading();
};

}
}

#endif // AIPhysicsProxy_h__
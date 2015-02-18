

/********************************************************************
	created:	2009/02/02
	created:	2:2:2009   23:38
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IPhysicalNode2D.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IPhysicalNode2D
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	interface class for any 2d physics object
*********************************************************************/

#ifndef ___IPhysicalNode2D___
#define ___IPhysicalNode2D___



namespace mray{
namespace physics{

class IPhysicalNode2D
{
public:
	virtual void update(float dt)=0;

	virtual void setMass(float m,const math::vector2d& centerOfMass)=0;

	virtual void SetMassFromShapes()=0;

	virtual math::vector2d getPosition()const=0;
	virtual float getAngle()const=0;

	virtual math::vector2d getLocalCenterOfMass()const=0;
	virtual math::vector2d getWorldCenterOfMass()const=0;

	virtual void setLinearVelocity(const math::vector2d& v)=0;
	virtual math::vector2d getLinearVelocity() const=0;

	virtual void setAngularVelocity(float v)=0;
	virtual float getAngularVelocity() const=0;

	virtual void applyForce(const math::vector2d& force, const math::vector2d& point)=0;
	virtual void applyTorque(float torque)=0;

	virtual void applyImpulse(const math::vector2d& impulse, const math::vector2d& point)=0;

	virtual float getMass() const=0;
	virtual float getInertia() const=0;

	virtual bool isBullet() const=0;

	virtual bool isStatic() const=0;
	virtual bool isDynamic() const=0;
	virtual bool isFrozen() const=0;
	virtual bool isSleeping() const=0;

	virtual void allowSleeping(bool a)=0;

	virtual void wakeUp()=0;

	virtual void putToSleep()=0;

	virtual void setUserData(void*d)=0;
	virtual void* getUserData()const=0;
};

}
}


#endif //___IPhysicalNode2D___

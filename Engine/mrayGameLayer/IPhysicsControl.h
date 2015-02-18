

/********************************************************************
	created:	2010/06/17
	created:	17:6:2010   17:37
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGameLayer\IPhysicsControl.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGameLayer
	file base:	IPhysicsControl
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IPhysicsControl_h__
#define IPhysicsControl_h__

#include "Point3d.h"
#include "quaternion.h"

namespace mray
{
namespace game
{

class IPhysicsControl
{
private:
protected:
public:
	IPhysicsControl(){}
	virtual ~IPhysicsControl(){}

	virtual float	GetMass()=0;

	virtual void SetPosition(const math::vector3d&pos)=0;
	virtual math::vector3d GetPosition()=0;

	virtual void SetOrintation(const math::quaternion&or)=0;
	virtual math::quaternion GetOrintation()=0;

	virtual void SetLinearVelocity(const math::vector3d&v)=0;
	virtual math::vector3d GetLinearVelocity()=0;

	virtual void SetAngularVelocity(const math::vector3d& v)=0;
	virtual math::vector3d GetAngularVelocity()=0;

	virtual void SetMaxAngularVelocity(float m)=0;
	virtual float GetMaxAngularVelocity()const=0;

	virtual void SetMaxSpeed(float s)=0;
	virtual float GetMaxSpeed()const=0;

	virtual math::vector3d GetUp()
	{
		return GetOrintation()*math::vector3d::YAxis;
	}
	virtual math::vector3d GetHeading()
	{
		return GetOrintation()*math::vector3d::ZAxis;
	}
};

}
}

#endif // IPhysicsControl_h__

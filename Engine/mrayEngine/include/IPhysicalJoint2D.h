

/********************************************************************
	created:	2009/02/03
	created:	3:2:2009   0:05
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IPhysicalJoint2D.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IPhysicalJoint2D
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IPhysicalJoint2D___
#define ___IPhysicalJoint2D___

#include "IPhysics2DDef.h"

namespace mray{
namespace physics{

class IPhysicalNode2D;

class IPhysicalJoint2D
{
public:
	virtual ESupported2DJoints getType() const=0;

	/// get the first body attached to this joint.
	virtual IPhysicalNode2D* getBody1()=0;

	/// get the second body attached to this joint.
	virtual IPhysicalNode2D* getBody2()=0;

	/// get the anchor point on body1 in world coordinates.
	virtual math::vector2d getAnchor1() const = 0;

	/// get the anchor point on body2 in world coordinates.
	virtual math::vector2d getAnchor2() const = 0;

	/// get the reaction force on body2 at the joint anchor.
	virtual math::vector2d getReactionForce() const = 0;

	/// get the reaction torque on body2.
	virtual float getReactionTorque() const = 0;

};
//////////////////////////////////////////////////////////////////////////
class IPhysicalJointDistance2D:public IPhysicalJoint2D
{
public:
	virtual ESupported2DJoints getType() const{
		return ESJ2D_DistanceJoint;
	}
public:
};
//////////////////////////////////////////////////////////////////////////
class IPhysicalJointPulley2D:public IPhysicalJoint2D
{
public:
	virtual ESupported2DJoints getType() const{
		return ESJ2D_PulleyJoint;
	}
public:
	/// Get the first ground anchor.
	virtual math::vector2d getGroundAnchor1() const=0;

	/// Get the second ground anchor.
	virtual math::vector2d  getGroundAnchor2() const=0;

	/// Get the current length of the segment attached to body1.
	virtual float  getLength1() const=0;

	/// Get the current length of the segment attached to body2.
	virtual float  getLength2() const=0;

	/// Get the pulley ratio.
	virtual float  getRatio() const=0;
};
//////////////////////////////////////////////////////////////////////////
class IPhysicalJointGear2D:public IPhysicalJoint2D
{
public:
	virtual ESupported2DJoints getType() const{
		return ESJ2D_GearJoint;
	}
public:
	/// get the gear ratio.
	virtual float getRatio() const=0;
};
//////////////////////////////////////////////////////////////////////////
class IPhysicalJointMouse2D:public IPhysicalJoint2D
{
public:
	virtual ESupported2DJoints getType() const{
		return ESJ2D_MouseJoint;
	}
public:
	/// Use this to update the target point.
	virtual void setTarget(const math::vector2d& target)=0;

};
//////////////////////////////////////////////////////////////////////////
class IPhysicalJointPrismatic2D:public IPhysicalJoint2D
{
public:
	virtual ESupported2DJoints getType() const{
		return ESJ2D_PrismaticJoint;
	}
public:
	/// get the current joint translation, usually in meters.
	virtual float getJointTranslation() const=0;

	/// get the current joint translation speed, usually in meters per second.
	virtual float getJointSpeed() const=0;

	/// Is the joint limit enabled?
	virtual bool isLimitEnabled() const=0;

	/// Enable/disable the joint limit.
	virtual void enableLimit(bool flag)=0;

	/// get the lower joint limit, usually in meters.
	virtual float getLowerLimit() const=0;

	/// get the upper joint limit, usually in meters.
	virtual float getUpperLimit() const=0;

	/// set the joint limits, usually in meters.
	virtual void setLimits(float lower, float upper)=0;

	/// Is the joint motor enabled?
	virtual bool isMotorEnabled() const=0;

	/// Enable/disable the joint motor.
	virtual void enableMotor(bool flag)=0;

	/// set the motor speed, usually in meters per second.
	virtual void setMotorSpeed(float speed)=0;

	/// get the motor speed, usually in meters per second.
	virtual float getMotorSpeed() const=0;

	/// set the maximum motor force, usually in N.
	virtual void setMaxMotorForce(float force)=0;

	/// get the current motor force, usually in N.
	virtual float getMotorForce() const=0;

};
//////////////////////////////////////////////////////////////////////////
class IPhysicalJointRevolute2D:public IPhysicalJoint2D
{
public:
	virtual ESupported2DJoints getType() const{
		return ESJ2D_RevoluteJoint;
	}
public:
	/// get the current joint angle in radians.
	virtual float getJointAngle() const=0;

	/// get the current joint angle speed in radians per second.
	virtual float getJointSpeed() const=0;

	/// Is the joint limit enabled?
	virtual bool isLimitEnabled() const=0;

	/// Enable/disable the joint limit.
	virtual void enableLimit(bool flag)=0;

	/// get the lower joint limit in radians.
	virtual float getLowerLimit() const=0;

	/// get the upper joint limit in radians.
	virtual float getUpperLimit() const=0;

	/// set the joint limits in radians.
	virtual void setLimits(float lower, float upper)=0;

	/// Is the joint motor enabled?
	virtual bool isMotorEnabled() const=0;

	/// Enable/disable the joint motor.
	virtual void enableMotor(bool flag)=0;

	/// set the motor speed in radians per second.
	virtual void setMotorSpeed(float speed)=0;

	/// get the motor speed in radians per second.
	virtual float getMotorSpeed() const=0;

	/// set the maximum motor torque, usually in N-m.
	virtual void setMaxMotorTorque(float torque)=0;

	/// get the current motor torque, usually in N-m.
	virtual float getMotorTorque() const=0;


};

}
}


#endif //___IPhysicalJoint2D___

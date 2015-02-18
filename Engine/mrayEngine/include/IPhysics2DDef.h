
/********************************************************************
	created:	2009/02/02
	created:	2:2:2009   23:52
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IPhysics2DDef.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IPhysics2DDef
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IPhysics2DDef___
#define ___IPhysics2DDef___



#include "mTypes.h"


namespace mray{
namespace physics{

	class IPhysicalNode2D;
	class IPhysicalJoint2D;

enum ESupported2DShapes{
	ESS2D_Circle,
	ESS2D_Box,
	ESS2D_Polygon
};

enum ESupported2DJoints{
	ESJ2D_RevoluteJoint,
	ESJ2D_PrismaticJoint,
	ESJ2D_DistanceJoint,
	ESJ2D_PulleyJoint,
	ESJ2D_MouseJoint,
	ESJ2D_GearJoint
};

class IPhysicalNode2DDesc
{
public:
	IPhysicalNode2DDesc(){
		setToDefault();
	}
	virtual void setToDefault(){
		centerOfMass=0;
		mass = 0.0f;
		Inertia = 0.0f;
		userData = 0;
		position=0.0f;
		angle = 0.0f;
		linearDamping = 0.0f;
		angularDamping = 0.0f;
		allowSleep = true;
		isSleeping = false;
		fixedRotation = false;
		isBullet = false;
	}
	float mass;
	/// The position of the shape's centroid relative to the shape's origin.
	math::vector2d centerOfMass;
	/// The rotational inertia of the shape.
	float Inertia;

	/// Use this to store application specific body data.
	void* userData;

	/// The world position of the body. Avoid creating bodies at the origin
	/// since this can lead to many overlapping shapes.
	math::vector2d position;

	/// The world angle of the body in radians.
	float angle;

	/// Linear damping is use to reduce the linear velocity. The damping parameter
	/// can be larger than 1.0f but the damping effect becomes sensitive to the
	/// time step when the damping parameter is large.
	float linearDamping;

	/// Angular damping is use to reduce the angular velocity. The damping parameter
	/// can be larger than 1.0f but the damping effect becomes sensitive to the
	/// time step when the damping parameter is large.
	float angularDamping;

	/// Set this flag to false if this body should never fall asleep. Note that
	/// this increases CPU usage.
	bool allowSleep;

	/// Is this body initially sleeping?
	bool isSleeping;

	/// Should this body be prevented from rotating? Useful for characters.
	bool fixedRotation;

	/// Is this a fast moving body that should be prevented from tunneling through
	/// other moving bodies? Note that all bodies are prevented from tunneling through
	/// static bodies.
	/// @warning You should use this flag sparingly since it increases processing time.
	bool isBullet;
};

//////////////////////////////////////////////////////////////////////////////
class IPhysicalShape2DDef
{
public:
	/// The constructor sets the default shape definition values.
	IPhysicalShape2DDef(){
		setToDefault();
	}
	virtual void setToDefault()
	{
		userData = 0;
		friction = 0.2f;
		restitution = 0.0f;
		density = 0.0f;
		categoryBits = 0x0001;
		maskBits = 0xFFFF;
		groupIndex = 0;
		isSensor = false;
	}

	virtual ~IPhysicalShape2DDef() {}

	virtual ESupported2DShapes getType()=0;

	/// Use this to store application specify shape data.
	void* userData;

	/// The shape's friction coefficient, usually in the range [0,1].
	float friction;

	/// The shape's restitution (elasticity) usually in the range [0,1].
	float restitution;

	/// The shape's density, usually in kg/m^2.
	float density;

	/// A sensor shape collects contact information but never generates a collision
	/// response.
	bool isSensor;

	/// The collision category bits. Normally you would just set one bit.
	ushort categoryBits;

	/// The collision mask bits. This states the categories that this
	/// shape would accept for collision.
	ushort maskBits;

	/// Collision groups allow a certain group of objects to never collide (negative)
	/// or always collide (positive). Zero means no collision group. Non-zero group
	/// filtering always wins against the mask bits.
	ushort groupIndex;
};

//////////////////////////////////////////////////////////////////////////

class PhysicalShape2DCircleDef:public IPhysicalShape2DDef
{
public:
	PhysicalShape2DCircleDef(){
		setToDefault();
	}
	virtual void setToDefault()
	{
		IPhysicalShape2DDef::setToDefault();
		position=0;
		radius=1;
	}
	virtual ESupported2DShapes getType(){
		return ESS2D_Circle;
	}

	math::vector2d position;
	float radius;
};


//////////////////////////////////////////////////////////////////////////

class PhysicalShape2DBoxDef:public IPhysicalShape2DDef
{
public:
	PhysicalShape2DBoxDef(){
		setToDefault();
	}
	virtual void setToDefault()
	{
		IPhysicalShape2DDef::setToDefault();
		position=0;
		extent=1;
		angle=0;
	}
	virtual ESupported2DShapes getType(){
		return ESS2D_Box;
	}

	float angle;
	math::vector2d position;
	math::vector2d extent;
};
//////////////////////////////////////////////////////////////////////////

class PhysicalShape2DPolygonDef:public IPhysicalShape2DDef
{
public:
	PhysicalShape2DPolygonDef(){
		setToDefault();
	}
	virtual void setToDefault()
	{
		IPhysicalShape2DDef::setToDefault();
		m_points.clear();
	}
	virtual ESupported2DShapes getType(){
		return ESS2D_Polygon;
	}

	std::vector<math::vector2d> m_points;
};


//////////////////////////////////////////////////////////////////////////

class IPhysicalJoint2DDesc
{
public:
	IPhysicalJoint2DDesc(){
		setToDefault();
	}
	virtual void setToDefault()
	{
		userData = NULL;
		body1 = NULL;
		body2 = NULL;
		collideConnected = false;
	}

	virtual ESupported2DJoints getType()=0;
	/// Use this to attach application specific data to your joints.
	void* userData;

	/// The first attached body.
	IPhysicalNode2D* body1;

	/// The second attached body.
	IPhysicalNode2D* body2;

	/// Set this flag to true if the attached bodies should collide.
	bool collideConnected;
};

//////////////////////////////////////////////////////////////////////////

class IPhysicalJoint2DPulleyDesc:public IPhysicalJoint2DDesc
{
public:
	IPhysicalJoint2DPulleyDesc(){
		setToDefault();
	}
	virtual void setToDefault()
	{
		IPhysicalJoint2DDesc::setToDefault();
		groundAnchor1=math::vector2d(-1.0f, 1.0f);
		groundAnchor2=math::vector2d(1.0f, 1.0f);
		localAnchor1=math::vector2d(-1.0f, 0.0f);
		localAnchor2=math::vector2d(1.0f, 0.0f);
		length1 = 0.0f;
		maxLength1 = 0.0f;
		length2 = 0.0f;
		maxLength2 = 0.0f;
		ratio = 1.0f;
		collideConnected = true;
	}

	virtual ESupported2DJoints getType(){
		return ESJ2D_PulleyJoint;
	}
	/// The first ground anchor in world coordinates. This point never moves.
	math::vector2d groundAnchor1;

	/// The second ground anchor in world coordinates. This point never moves.
	math::vector2d groundAnchor2;

	/// The local anchor point relative to body1's origin.
	math::vector2d localAnchor1;

	/// The local anchor point relative to body2's origin.
	math::vector2d localAnchor2;

	/// The a reference length for the segment attached to body1.
	float length1;

	/// The maximum length of the segment attached to body1.
	float maxLength1;

	/// The a reference length for the segment attached to body2.
	float length2;

	/// The maximum length of the segment attached to body2.
	float maxLength2;

	/// The pulley ratio, used to simulate a block-and-tackle.
	float ratio;
};

//////////////////////////////////////////////////////////////////////////

class IPhysicalJoint2DMouseDesc:public IPhysicalJoint2DDesc
{
public:
	IPhysicalJoint2DMouseDesc(){
		setToDefault();
	}
	virtual void setToDefault()
	{
		IPhysicalJoint2DDesc::setToDefault();
		target=math::vector2d(0.0f, 0.0f);
		maxForce = 0.0f;
		frequencyHz = 5.0f;
		dampingRatio = 0.7f;
		timeStep = 1.0f / 60.0f;
	}

	virtual ESupported2DJoints getType(){
		return ESJ2D_MouseJoint;
	}
	/// The initial world target point. This is assumed
	/// to coincide with the body anchor initially.
	math::vector2d target;

	/// The maximum constraint force that can be exerted
	/// to move the candidate body. Usually you will express
	/// as some multiple of the weight (multiplier * mass * gravity).
	float maxForce;

	/// The response speed.
	float frequencyHz;

	/// The damping ratio. 0 = no damping, 1 = critical damping.
	float dampingRatio;

	/// The time step used in the simulation.
	float timeStep;
};

//////////////////////////////////////////////////////////////////////////

class IPhysicalJoint2DGearDesc:public IPhysicalJoint2DDesc
{
public:
	IPhysicalJoint2DGearDesc(){
		setToDefault();
	}
	virtual void setToDefault()
	{
		IPhysicalJoint2DDesc::setToDefault();
		joint1 = NULL;
		joint2 = NULL;
		ratio = 1.0f;
	}

	virtual ESupported2DJoints getType(){
		return ESJ2D_GearJoint;
	}

	/// The first revolute/prismatic joint attached to the gear joint.
	IPhysicalJoint2D* joint1;

	/// The second revolute/prismatic joint attached to the gear joint.
	IPhysicalJoint2D* joint2;

	/// The gear ratio.
	/// @see b2GearJoint for explanation.
	float ratio;
};
//////////////////////////////////////////////////////////////////////////

class IPhysicalJoint2DPrismaticDesc:public IPhysicalJoint2DDesc
{
public:
	IPhysicalJoint2DPrismaticDesc(){
		setToDefault();
	}
	virtual void setToDefault()
	{
		IPhysicalJoint2DDesc::setToDefault();
		localAnchor1=0;
		localAnchor2=0;
		localAxis1=math::vector2d(1.0f, 0.0f);
		referenceAngle = 0.0f;
		enableLimit = false;
		lowerTranslation = 0.0f;
		upperTranslation = 0.0f;
		enableMotor = false;
		maxMotorForce = 0.0f;
		motorSpeed = 0.0f;
	}

	virtual ESupported2DJoints getType(){
		return ESJ2D_PrismaticJoint;
	}
	math::vector2d localAnchor1;

	/// The local anchor point relative to body2's origin.
	math::vector2d localAnchor2;

	/// The local translation axis in body1.
	math::vector2d localAxis1;

	/// The constrained angle between the bodies: body2_angle - body1_angle.
	float referenceAngle;

	/// Enable/disable the joint limit.
	bool enableLimit;

	/// The lower translation limit, usually in meters.
	float lowerTranslation;

	/// The upper translation limit, usually in meters.
	float upperTranslation;

	/// Enable/disable the joint motor.
	bool enableMotor;

	/// The maximum motor torque, usually in N-m.
	float maxMotorForce;

	/// The desired motor speed in radians per second.
	float motorSpeed;
};

//////////////////////////////////////////////////////////////////////////

class IPhysicalJoint2DDistanceDesc:public IPhysicalJoint2DDesc
{
public:
	IPhysicalJoint2DDistanceDesc(){
		setToDefault();
	}
	virtual void setToDefault()
	{
		IPhysicalJoint2DDesc::setToDefault();
		localAnchor1=math::vector2d(0.0f, 0.0f);
		localAnchor2=math::vector2d(0.0f, 0.0f);
		length = 1.0f;
		frequencyHz = 0.0f;
		dampingRatio = 0.0f;
	}

	virtual ESupported2DJoints getType(){
		return ESJ2D_DistanceJoint;
	}
	/// The local anchor point relative to body1's origin.
	math::vector2d localAnchor1;

	/// The local anchor point relative to body2's origin.
	math::vector2d localAnchor2;

	/// The equilibrium length between the anchor points.
	float length;

	/// The response speed.
	float frequencyHz;

	/// The damping ratio. 0 = no damping, 1 = critical damping.
	float dampingRatio;
};

//////////////////////////////////////////////////////////////////////////

class IPhysicalJoint2DRevoluteDesc:public IPhysicalJoint2DDesc
{
public:
	IPhysicalJoint2DRevoluteDesc(){
		setToDefault();
	}
	virtual void setToDefault()
	{
		IPhysicalJoint2DDesc::setToDefault();
		localAnchor1=math::vector2d(0.0f, 0.0f);
		localAnchor2=math::vector2d(0.0f, 0.0f);
		referenceAngle = 0.0f;
		lowerAngle = 0.0f;
		upperAngle = 0.0f;
		maxMotorTorque = 0.0f;
		motorSpeed = 0.0f;
		enableLimit = false;
		enableMotor = false;
	}

	virtual ESupported2DJoints getType(){
		return ESJ2D_RevoluteJoint;
	}
	/// The local anchor point relative to body1's origin.
	math::vector2d localAnchor1;

	/// The local anchor point relative to body2's origin.
	math::vector2d localAnchor2;

	/// The body2 angle minus body1 angle in the reference state (radians).
	float referenceAngle;

	/// A flag to enable joint limits.
	bool enableLimit;

	/// The lower angle for the joint limit (radians).
	float lowerAngle;

	/// The upper angle for the joint limit (radians).
	float upperAngle;

	/// A flag to enable the joint motor.
	bool enableMotor;

	/// The desired motor speed. Usually in radians per second.
	float motorSpeed;

	/// The maximum motor torque used to achieve the desired motor speed.
	/// Usually in N-m.
	float maxMotorTorque;
};



}
}


#endif //___IPhysics2DDef___

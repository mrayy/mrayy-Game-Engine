
/********************************************************************
	created:	2009/02/05
	created:	5:2:2009   19:26
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\IPhysicalJoint.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	IPhysicalJoint
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IPhysicalJoint___
#define ___IPhysicalJoint___

#include "IObject.h"
#include "IPhysical3DDef.h"

namespace mray{
namespace physics{
	class IPhysicalNode;
	class IPhysicManager;

	enum EJoint3DState
	{
		EJ3S_Unbound,
		EJ3S_Simulating,
		EJ3S_Broken,
	};


class MRAY_DLL IPhysicalJoint:public IObject
{
protected:
	IPhysicManager* m_creator;
public:

	IObject* userData;

	IPhysicalJoint(IPhysicManager* c):userData(0),m_creator(c)
	{}
	virtual~IPhysicalJoint(){}

	IPhysicManager* GetCreator(){return m_creator;}

	virtual ESupported3DJoints getType()=0;

	virtual IPhysicalNode* getNode1()=0;
	virtual IPhysicalNode* getNode2()=0;

	 //Sets the point where the two nodes are attached, specified in global coordinates.
	virtual void setGlobalAnchor(const math::vector3d &v)=0;


	virtual math::vector3d getGlobalAnchor()=0;

	//Sets the direction of the joint's primary axis, specified in global coordinates.
	virtual void setGlobalAxis(const math::vector3d &v)=0;

	virtual math::vector3d getGlobalAxis()=0;
	
	virtual EJoint3DState getJointState()=0;

	virtual void setBreakable(float maxForce,float maxTorque)=0;

	// Sets the solver extrapolation factor.
	virtual void setSolverExtrapolationFactor(float f)=0;

	virtual float getSolverExtrapolationFactor()=0;

	virtual void setUseAccelerationSpring(bool u)=0;
	virtual bool getUseAccelerationSpring()=0;

	virtual void setLimitPoint(const math::vector3d&p,bool pointOnNode2=true)=0;

	// returns true if point is on node2
	virtual bool getLimitPoint(math::vector3d&p)=0;


	virtual bool addLimitPlane(const math::vector3d& normal, const math::vector3d & pointInPlane, float restitution = 0.0f) = 0;

	virtual void clearLimitPlanes()=0;

	IPhysicalJoint3DDesc* ConvertToDesc();
	virtual bool SaveToDesc(IPhysicalJoint3DDesc* desc)=0;
	virtual bool LoadFromDesc(const IPhysicalJoint3DDesc* desc)=0;
	
};
MakeSharedPtrType(IPhysicalJoint);

//////////////////////////////////////////////////////////////////////////

class ISphericalJoint3D:public IPhysicalJoint
{
public:

	ISphericalJoint3D(IPhysicManager*c):IPhysicalJoint(c){}
	virtual ~ISphericalJoint3D(){}


	virtual ESupported3DJoints getType(){
		return ESJ3D_Spherical;
	}

	virtual void setProjectionMode(EJointProjectionMode m)=0;
	virtual EJointProjectionMode getProjectionMode()=0;

	 bool SaveToDesc(IPhysicalJoint3DDesc* desc)
	{
		IPhysicalJointSphericalDesc*d=dynamic_cast<IPhysicalJointSphericalDesc*>(desc);
		if(!d)
			return false;
		SaveToDesc(d);
		return true;
	}
	 bool LoadFromDesc(const IPhysicalJoint3DDesc* desc)
	{
		const IPhysicalJointSphericalDesc*d=dynamic_cast<const IPhysicalJointSphericalDesc*>(desc);
		if(!d)
			return false;
		LoadFromDesc(d);
		return true;
	}

	virtual void SaveToDesc(IPhysicalJointSphericalDesc* desc)=0;
	virtual void LoadFromDesc(const IPhysicalJointSphericalDesc* desc)=0;
};


//////////////////////////////////////////////////////////////////////////

class IRevoluteJoint3D:public IPhysicalJoint
{
public:

	IRevoluteJoint3D(IPhysicManager*c):IPhysicalJoint(c){}
	virtual ~IRevoluteJoint3D(){}

	virtual ESupported3DJoints getType(){
		return ESJ3D_RevoluteJoint;
	}

	virtual bool getLimit(PhysicalJointLimitPairDesc&d)=0;

	virtual void setMotor(const PhysicalMotor3DDesc&d)=0;
	virtual bool getMotor(PhysicalMotor3DDesc&d)=0;

	virtual void setSpring(const PhysicalSpringDesc&d)=0;
	virtual bool getSpring(PhysicalSpringDesc&d)=0;

	virtual float getAngle()=0;
	virtual float getVelocity()=0;

	virtual void setProjectionMode(EJointProjectionMode m)=0;
	virtual EJointProjectionMode getProjectionMode()=0;

	bool SaveToDesc(IPhysicalJoint3DDesc* desc)
	{
		IPhysicalJointRevoluteDesc*d=dynamic_cast<IPhysicalJointRevoluteDesc*>(desc);
		if(!d)
			return false;
		SaveToDesc(d);
		return true;
	}
	bool LoadFromDesc(const IPhysicalJoint3DDesc* desc)
	{
		const IPhysicalJointRevoluteDesc*d=dynamic_cast<const IPhysicalJointRevoluteDesc*>(desc);
		if(!d)
			return false;
		LoadFromDesc(d);
		return true;
	}
	virtual void SaveToDesc(IPhysicalJointRevoluteDesc* desc)=0;
	virtual void LoadFromDesc(const IPhysicalJointRevoluteDesc* desc)=0;
};


//////////////////////////////////////////////////////////////////////////

class IPulleyJoint3D:public IPhysicalJoint
{
public:

	IPulleyJoint3D(IPhysicManager*c):IPhysicalJoint(c){}
	virtual ~IPulleyJoint3D(){}

	virtual ESupported3DJoints getType(){
		return ESJ3D_PulleyJoint;
	}

	virtual void setMotor(const PhysicalMotor3DDesc&d)=0;
	virtual bool getMotor(PhysicalMotor3DDesc&d)=0;

	bool SaveToDesc(IPhysicalJoint3DDesc* desc)
	{
		IPhysicalJointPulleyDesc*d=dynamic_cast<IPhysicalJointPulleyDesc*>(desc);
		if(!d)
			return false;
		SaveToDesc(d);
		return true;
	}
	bool LoadFromDesc(const IPhysicalJoint3DDesc* desc)
	{
		const IPhysicalJointPulleyDesc*d=dynamic_cast<const IPhysicalJointPulleyDesc*>(desc);
		if(!d)
			return false;
		LoadFromDesc(d);
		return true;
	}
	virtual void SaveToDesc(IPhysicalJointPulleyDesc* desc)=0;
	virtual void LoadFromDesc(const IPhysicalJointPulleyDesc* desc)=0;
};

//////////////////////////////////////////////////////////////////////////

class IPrismaticJoint3D:public IPhysicalJoint
{
public:

	IPrismaticJoint3D(IPhysicManager*c):IPhysicalJoint(c){}
	virtual ~IPrismaticJoint3D(){}


	virtual ESupported3DJoints getType(){
		return ESJ3D_PrismaticJoint;
	}

	bool SaveToDesc(IPhysicalJoint3DDesc* desc)
	{
		IPhysicalJointPrismaticDesc*d=dynamic_cast<IPhysicalJointPrismaticDesc*>(desc);
		if(!d)
			return false;
		SaveToDesc(d);
		return true;
	}
	bool LoadFromDesc(const IPhysicalJoint3DDesc* desc)
	{
		const IPhysicalJointPrismaticDesc*d=dynamic_cast<const IPhysicalJointPrismaticDesc*>(desc);
		if(!d)
			return false;
		LoadFromDesc(d);
		return true;
	}
	virtual void SaveToDesc(IPhysicalJointPrismaticDesc* desc)=0;
	virtual void LoadFromDesc(const IPhysicalJointPrismaticDesc* desc)=0;
};

//////////////////////////////////////////////////////////////////////////

class IDistanceJoint3D:public IPhysicalJoint
{
public:

	IDistanceJoint3D(IPhysicManager*c):IPhysicalJoint(c){}
	virtual ~IDistanceJoint3D(){}


	virtual ESupported3DJoints getType(){
		return ESJ3D_DistanceJoint;
	}

	bool SaveToDesc(IPhysicalJoint3DDesc* desc)
	{
		IPhysicalJointDistanceDesc*d=dynamic_cast<IPhysicalJointDistanceDesc*>(desc);
		if(!d)
			return false;
		SaveToDesc(d);
		return true;
	}
	bool LoadFromDesc(const IPhysicalJoint3DDesc* desc)
	{
		const IPhysicalJointDistanceDesc*d=dynamic_cast<const IPhysicalJointDistanceDesc*>(desc);
		if(!d)
			return false;
		LoadFromDesc(d);
		return true;
	}
	virtual void SaveToDesc(IPhysicalJointDistanceDesc* desc)=0;
	virtual void LoadFromDesc(const IPhysicalJointDistanceDesc* desc)=0;

};

//////////////////////////////////////////////////////////////////////////

class ICylinderJoint3D:public IPhysicalJoint
{
public:

	ICylinderJoint3D(IPhysicManager*c):IPhysicalJoint(c){}
	virtual ~ICylinderJoint3D(){}


	virtual ESupported3DJoints getType(){
		return ESJ3D_Cylindrical;
	}

	bool SaveToDesc(IPhysicalJoint3DDesc* desc)
	{
		IPhysicalJointCylinderDesc*d=dynamic_cast<IPhysicalJointCylinderDesc*>(desc);
		if(!d)
			return false;
		SaveToDesc(d);
		return true;
	}
	bool LoadFromDesc(const IPhysicalJoint3DDesc* desc)
	{
		const IPhysicalJointCylinderDesc*d=dynamic_cast<const IPhysicalJointCylinderDesc*>(desc);
		if(!d)
			return false;
		LoadFromDesc(d);
		return true;
	}

	virtual void SaveToDesc(IPhysicalJointCylinderDesc* desc)=0;
	virtual void LoadFromDesc(const IPhysicalJointCylinderDesc* desc)=0;
};

//////////////////////////////////////////////////////////////////////////

class IDOF6Joint3D:public IPhysicalJoint
{
public:

	IDOF6Joint3D(IPhysicManager*c):IPhysicalJoint(c){}
	virtual ~IDOF6Joint3D(){}


	virtual ESupported3DJoints getType(){
		return ESJ3D_D6Joint;
	}

	virtual void setDrivePosition(const math::vector3d&p)=0;

	virtual void setDriveOrientation(const math::quaternion&p)=0;

	virtual void setDriveLinearVelocity(const math::vector3d&p)=0;

	virtual void setDriveAngularVelocity(const math::vector3d&p)=0;

	virtual void setAngularDriveProperties(PhysicalJointDriveDesc twistDriveDesc,PhysicalJointDriveDesc swingDriveDesc)=0;

	virtual const math::quaternion& getInitialNode2Orintation()=0;
	virtual math::quaternion getCurrentNode2Orintation()=0;

	bool SaveToDesc(IPhysicalJoint3DDesc* desc)
	{
		IPhysicalJointD6Desc*d=dynamic_cast<IPhysicalJointD6Desc*>(desc);
		if(!d)
			return false;
		SaveToDesc(d);
		return true;
	}
	bool LoadFromDesc(const IPhysicalJoint3DDesc* desc)
	{
		const IPhysicalJointD6Desc*d=dynamic_cast<const IPhysicalJointD6Desc*>(desc);
		if(!d)
			return false;
		LoadFromDesc(d);
		return true;
	}
	virtual void SaveToDesc(IPhysicalJointD6Desc* desc)=0;
	virtual void LoadFromDesc(const IPhysicalJointD6Desc* desc)=0;
};


//////////////////////////////////////////////////////////////////////////

class IPointOnLineJoint3D:public IPhysicalJoint
{
public:

	IPointOnLineJoint3D(IPhysicManager*c):IPhysicalJoint(c){}
	virtual ~IPointOnLineJoint3D(){}


	virtual ESupported3DJoints getType(){
		return ESJ3D_PointOnLine;
	}

	bool SaveToDesc(IPhysicalJoint3DDesc* desc)
	{
		IPhysicalJointPointOnLineDesc*d=dynamic_cast<IPhysicalJointPointOnLineDesc*>(desc);
		if(!d)
			return false;
		SaveToDesc(d);
		return true;
	}
	bool LoadFromDesc(const IPhysicalJoint3DDesc* desc)
	{
		const IPhysicalJointPointOnLineDesc*d=dynamic_cast<const IPhysicalJointPointOnLineDesc*>(desc);
		if(!d)
			return false;
		LoadFromDesc(d);
		return true;
	}
	virtual void SaveToDesc(IPhysicalJointPointOnLineDesc* desc)=0;
	virtual void LoadFromDesc(const IPhysicalJointPointOnLineDesc* desc)=0;
};

//////////////////////////////////////////////////////////////////////////

class IPointOnPlaneJoint3D:public IPhysicalJoint
{
public:

	IPointOnPlaneJoint3D(IPhysicManager*c):IPhysicalJoint(c){}
	virtual ~IPointOnPlaneJoint3D(){}


	virtual ESupported3DJoints getType(){
		return ESJ3D_PointOnPlane;
	}

	bool SaveToDesc(IPhysicalJoint3DDesc* desc)
	{
		IPhysicalJointPointOnPlaneDesc*d=dynamic_cast<IPhysicalJointPointOnPlaneDesc*>(desc);
		if(!d)
			return false;
		SaveToDesc(d);
		return true;
	}
	bool LoadFromDesc(const IPhysicalJoint3DDesc* desc)
	{
		const IPhysicalJointPointOnPlaneDesc*d=dynamic_cast<const IPhysicalJointPointOnPlaneDesc*>(desc);
		if(!d)
			return false;
		LoadFromDesc(d);
		return true;
	}
	virtual void SaveToDesc(IPhysicalJointPointOnPlaneDesc* desc)=0;
	virtual void LoadFromDesc(const IPhysicalJointPointOnPlaneDesc* desc)=0;
};

//////////////////////////////////////////////////////////////////////////

class IFixedJoint3D:public IPhysicalJoint
{
public:

	IFixedJoint3D(IPhysicManager*c):IPhysicalJoint(c){}
	virtual ~IFixedJoint3D(){}


	virtual ESupported3DJoints getType(){
		return ESJ3D_FixedJoint;
	}

	bool SaveToDesc(IPhysicalJoint3DDesc* desc)
	{
		IPhysicalJointFixedDesc*d=dynamic_cast<IPhysicalJointFixedDesc*>(desc);
		if(!d)
			return false;
		SaveToDesc(d);
		return true;
	}
	bool LoadFromDesc(const IPhysicalJoint3DDesc* desc)
	{
		const IPhysicalJointFixedDesc*d=dynamic_cast<const IPhysicalJointFixedDesc*>(desc);
		if(!d)
			return false;
		LoadFromDesc(d);
		return true;
	}

	virtual void SaveToDesc(IPhysicalJointFixedDesc* desc)=0;
	virtual void LoadFromDesc(const IPhysicalJointFixedDesc* desc)=0;
};

}
}


#endif //___IPhysicalJoint___

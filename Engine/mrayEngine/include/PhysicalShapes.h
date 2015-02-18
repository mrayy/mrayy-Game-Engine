
/********************************************************************
	created:	2009/01/11
	created:	11:1:2009   0:08
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\PhysicalShapes.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	PhysicalShapes
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PhysicalShapes___
#define ___PhysicalShapes___





#include "IPhysical3DDef.h"

namespace mray{

	namespace scene
	{
		class IMeshBuffer;
	}
	namespace loaders
	{
		class IHeightMapLoader;
	}

namespace physics{

	class IPhysicMaterial;

enum ESupportedShapes{
	ESS_Sphere,
	ESS_Box,
	ESS_Plane,
	ESS_Terrain,
	ESS_Capsule,
	ESS_Convex,
	ESS_TriangleMesh,
	ESS_Wheel
};

class MRAY_DLL IPhysicalShapeDesc
{
public:
	IPhysicalShapeDesc();
	~IPhysicalShapeDesc();
	virtual ESupportedShapes getType()=0;

	virtual void setDefault();
public:

	bool autoDelete;

	core::string name;
	float density;
	
	math::matrix4x4 localSpace;



	//assign material by name
	core::string materialName;
	//assign material by object
	IPhysicMaterial* material;
	//assign material by desc
	PhysicMaterialDesc* materialDesc;

	
	float mass;
	
	GroupID collisionGroup;
	
	CollisionMask groupMask;
	void *userData;

	//EShapeFlag
	uint flags;

	//int materialIndex;

	virtual void SaveToXML(mray::xml::XMLElement *e);
	virtual void LoadFromXML(xml::XMLElement* e);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL SphereShapeDesc:public IPhysicalShapeDesc
{
public:
	SphereShapeDesc();
	SphereShapeDesc(float radius);
	virtual void setDefault();
	virtual ESupportedShapes getType();
public:
	float radius;

	virtual void SaveToXML(mray::xml::XMLElement *e);
	virtual void LoadFromXML(xml::XMLElement* e);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL BoxShapeDesc:public IPhysicalShapeDesc
{
public:
	BoxShapeDesc();
	BoxShapeDesc(const math::vector3d& extent);
	virtual void setDefault();
	virtual ESupportedShapes getType();
public:
	math::vector3d extent;

	virtual void SaveToXML(mray::xml::XMLElement *e);
	virtual void LoadFromXML(xml::XMLElement* e);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL PlaneShapeDesc:public IPhysicalShapeDesc
{
public:
	PlaneShapeDesc();
	PlaneShapeDesc(const math::Plane& p);
	virtual void setDefault();
	virtual ESupportedShapes getType();
public:
	math::Plane plane;

	virtual void SaveToXML(mray::xml::XMLElement *e);
	virtual void LoadFromXML(xml::XMLElement* e);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL TerrainShapeDesc:public IPhysicalShapeDesc
{
protected:
	loaders::IHeightMapLoader* m_heightMapData;
public:
	TerrainShapeDesc();
	TerrainShapeDesc(int cols,int rows,double*data,math::vector3d scale,int skip=0);
	virtual~TerrainShapeDesc();
	virtual void setDefault();
	virtual ESupportedShapes getType();
public:
	core::string heightMapPath;
	int cols;
	int rows;
	int skip;
	double*data;
	math::vector3d scale;

	virtual void SaveToXML(mray::xml::XMLElement *e);
	virtual void LoadFromXML(xml::XMLElement* e);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL CapsuleShapeDesc:public IPhysicalShapeDesc
{
public:
	CapsuleShapeDesc();
	CapsuleShapeDesc(float radius,float height);
	virtual void setDefault();
	virtual ESupportedShapes getType();
public:
	float radius;
	float height;

	virtual void SaveToXML(mray::xml::XMLElement *e);
	virtual void LoadFromXML(xml::XMLElement* e);
};
//////////////////////////////////////////////////////////////////////////

class MRAY_DLL ConvexShapeDesc:public IPhysicalShapeDesc
{
public:
	ConvexShapeDesc();
	ConvexShapeDesc(math::vector3d*points,int count);
	virtual void setDefault();
	virtual ESupportedShapes getType();
public:
	std::vector<math::vector3d> points;
	scene::IMeshBuffer* mesh;
	core::string meshPath;

	bool flipNormals;
	bool computeConvex;
	bool inflanteConvex;

	virtual void SaveToXML(mray::xml::XMLElement *e);
	virtual void LoadFromXML(xml::XMLElement* e);
};

class MRAY_DLL TriangleShapeDesc:public IPhysicalShapeDesc
{
public:
	TriangleShapeDesc();
	TriangleShapeDesc(scene::IMeshBuffer* mesh);
	virtual void setDefault();
	virtual ESupportedShapes getType();
public:
	scene::IMeshBuffer* mesh;
	core::string meshPath;

	bool flipNormals;
	bool hardwareMesh;

	virtual void SaveToXML(mray::xml::XMLElement *e);
	virtual void LoadFromXML(xml::XMLElement* e);
};
//////////////////////////////////////////////////////////////////////////

class WheelContactData
{
public:
	/**
	\brief The point of contact between the wheel shape and the ground.

	*/
	math::vector3d contactPoint;

	/**
	\brief The normal at the point of contact.

	*/
	math::vector3d contactNormal;

	/**
	\brief The direction the wheel is pointing in.
	*/
	math::vector3d longitudalDirection;

	/**
	\brief The sideways direction for the wheel(at right angles to the longitudinal direction).
	*/
	math::vector3d lateralDirection;

	/**
	\brief The magnitude of the force being applied for the contact.
	*/
	float contactForce;

	/**
	\brief What these exactly are depend on NX_WF_INPUT_LAT_SLIPVELOCITY and NX_WF_INPUT_LNG_SLIPVELOCITY flags for the wheel.
	*/
	float longitudalSlip, lateralSlip;

	/**
	\brief the clipped impulses applied at the wheel.
	*/
	float longitudalImpulse, lateralImpulse;

	/**
	\brief The material index of the shape in contact with the wheel.

	@see NxMaterial NxMaterialIndex
	*/
	MaterialID otherShapeMaterialIndex;

	/**
	\brief The distance on the spring travel distance where the wheel would end up if it was resting on the contact point.
	*/
	float contactPosition;
};
class MRAY_DLL WheelShapeDesc:public IPhysicalShapeDesc
{
public:
	WheelShapeDesc();
	virtual void setDefault();
	virtual ESupportedShapes getType();
public:
	/**
	\brief distance from wheel axle to a point on the contact surface.

	<b>Range:</b> (0,inf)<br>
	<b>Default:</b> 1.0

	@see NxWheelShape.getRadius() NxWheelShape.setRadius()
	*/
	float radius;

	/**
	\brief maximum extension distance of suspension along shape's -Y axis.

	The minimum extension is always 0.  

	<b>Range:</b> [0,inf)<br>
	<b>Default:</b> 1.0

	@see NxWheelShape.setSuspensionTravel() NxWheelShape.getSuspensionTravel()
	*/
	float suspensionTravel;

	//(In the old model the capsule height was the sum of the two members above.)
	//^^^ this may be redundant together with suspension.targetValue, not sure yet.

	//simulation constants:

	/**
	\brief data intended for car wheel suspension effects.  

	targetValue must be in [0, 1], which is the rest length of the suspension along the suspensionTravel.  
	0 is the default, which maps to the tip of the ray cast.

	<b>Range:</b> .spring [0,inf)<br>
	<b>Range:</b> .damper [0,inf)<br>
	<b>Range:</b> .targetValue [0,1]<br>
	<b>Default:</b> See #NxSpringDesc


	@see NxSpringDesc NxWheelShape.setSuspension() NxWheelShape.getSuspension()
	*/
	struct SuspensionSpring
	{
		SuspensionSpring()
		{
			setDefault();
		}
		float spring;
		float damper;
		float targetValue;

		void setDefault(){
			spring=0;
			damper=0;
			targetValue=0;
		}
	};
	SuspensionSpring suspension;

	struct TireFunctionDesc
	{
	public:
		TireFunctionDesc()
		{
			SetDefault();
		}
		void SetDefault()
		{
			extremumSlip=1.0f;
			extremumValue=0.02f;

			asymptoteSlip=2.0f;
			asymptoteValue=0.01f;

			stiffnessFactor=1000000.0f;

		}

		float extremumSlip;
		float extremumValue;

		float asymptoteSlip;
		float asymptoteValue;

		float stiffnessFactor;

	};

	TireFunctionDesc lateralTireForceFunction;
	TireFunctionDesc longitudalTireForceFunction;

	/**
	\brief inverse mass of the wheel.

	Determines the wheel velocity that wheel torques can achieve.

	<b>Range:</b> (0,inf)<br>
	<b>Default:</b> 1.0

	@see NxWheelShape.setInverseWheelMass() NxWheelShape.getInverseWheelMass()
	*/
	float	inverseWheelMass;

	//dynamic inputs:

	/**
	\brief Sum engine torque on the wheel axle.

	Positive or negative depending on direction.

	<b>Range:</b> (-inf,inf)<br>
	<b>Default:</b> 0.0

	@see NxWheelShape.setMotorTorque() NxWheelShape.getMotorTorque() brakeTorque
	*/
	float motorTorque;

	/**
	\brief The amount of torque applied for braking.

	Must be nonnegative.  Very large values should lock wheel but should be stable.

	<b>Range:</b> [0,inf)<br>
	<b>Default:</b> 0.0

	@see NxWheelShape.setBrakeTorque() NxWheelShape.getBrakeTorque() motorTorque
	*/
	float brakeTorque;

	/**
	\brief steering angle, around shape Y axis.

	<b>Range:</b> (-PI,PI)<br>
	<b>Default:</b> 0.0

	@see NxWheelShape.setSteerAngle() NxWheelShape.getSteerAngle()
	*/
	float steerAngle;

	virtual void SaveToXML(mray::xml::XMLElement *e);
	virtual void LoadFromXML(xml::XMLElement* e);
};
//////////////////////////////////////////////////////////////////////////

}
}


#endif //___PhysicalShapes___


/********************************************************************
	created:	2009/11/27
	created:	27:11:2009   14:46
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IPhysicalClothDef.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IPhysicalClothDef
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IPhysicalClothDef___
#define ___IPhysicalClothDef___

#include <matrix4x4.h>
#include "mTypes.h"
#include "IPhysical3DDef.h"

namespace mray
{
namespace scene
{
	class IMeshBuffer;
}
namespace physics
{


	enum EClothFlags
	{
		ECF_Pressure,
		ECF_Static,
		ECF_DisableCollision,
		ECF_SelfCollision,
		ECF_Gravity,
		ECF_Bending,
		ECF_BendingOrtho,
		ECF_Damping,
		ECF_CollisionTwoWay,
		ECF_TriangleCollision,
		ECF_Terrable,
		ECF_Hardware,
		ECF_COMDamping,
		ECF_ValidBounds,
		ECF_FluidCollision,
		ECF_DisableDynamicCCD,
		ECF_Adhere,

		ECF_ClothFlags_Count
	};

	enum EClothVertexAttachmentStatus
	{
		ECVAS_None,
		ECVAS_Global,
		ECVAS_Shape
	};
	enum EClothVertexFlags
	{
		ECVF_Attached=1<<0,
		ECVF_Terrable=1<<7
	};
	enum EClothAttachmentFlags
	{
		ECAF_TwoWay=1<<0,
		ECAF_Tearable=1<<1,
	};

	class PhysicalClothConstraintCoeff
	{
	public:
		PhysicalClothConstraintCoeff():maxDistance(0),maxDistanceBias(0),
			collisionSphereRadius(0),collisionSphereDistance(0)
		{
		}
		float maxDistance;
		float maxDistanceBias;
		float collisionSphereRadius;
		float collisionSphereDistance;
	};

class MRAY_DLL PhysicalClothDesc
{
public:
	PhysicalClothDesc();

	virtual void setToDefault();


	bool autoDelete;
	GroupID collisionGroup;

	math::matrix4x4 globalPos;
	
	float thickness;

	float density;

	float bendingStiffness;

	float stretchingStiffness;

	float dampingCoefficient;

	float friction;

	float pressure;

	float tearFactor;

	float collisionResponseCoefficient;

	float attachmentResponseCoefficient;
	
	float attachmentTearFactor;

	float toFluidResponseCoefficient;
	
	float fromFluidResponseCoefficient;

	float minAdhereVelocity;

	uint  solverIterations;

	math::vector3d externalAcceleration;

	math::vector3d windAcceleration;

	float wakeUpCounter;

	float sleepLinearVelocity;

	math::box3d validBounds;

	float relativeGridSpacing;

	std::vector<bool> tearableFlags;

	bool flags[ECF_ClothFlags_Count];

	scene::IMeshBuffer* meshData;
	core::string meshPath;

	IObject* userData;


	void SaveToXML(mray::xml::XMLElement *e);
	void LoadFromXML(xml::XMLElement* e);
};

}
}


#endif //___IPhysicalClothDef___

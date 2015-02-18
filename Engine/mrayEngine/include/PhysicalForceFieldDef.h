

/********************************************************************
	created:	2012/02/15
	created:	15:2:2012   10:30
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\PhysicalForceFieldDef.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	PhysicalForceFieldDef
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhysicalForceFieldDef__
#define __PhysicalForceFieldDef__

#include "IPhysical3DDef.h"

namespace mray
{
namespace scene
{
	class IMeshBuffer;
}
namespace physics
{

	enum EForceFieldCoordinates
	{
		EFFC_Cartesian,
		EFFC_Spherical,
		EFFC_Cylindrical,
		EFFC_Toroidal
	};

	enum EForceFieldType
	{
		EFFT_Gravitational,
		EFFT_Other,
		EFFT_NoInteraction
	};

	enum EForceFieldFlags
	{
		EFFF_VolumetricScalingFluid,
		EFFF_VolumetricScalingCloth,
		EFFF_VolumetricScalingSoftBody,
		EFFF_VolumetricScalingRigidBody,
		EFFF_FlagsCount
	};

	enum EForceFieldShapeType
	{
		EFFShape_Sphere,
		EFFShape_Capsule,
		EFFShape_Box,
		EFFShape_Convex
	};

	typedef unsigned short ForceFieldVariety;

	class IPhysicalForceFieldLinearKernal;
	class IPhysicalForceFieldShapeGroup;
	class IPhysicalForceFieldShapeDesc;

	class PhysicalForceFieldDesc
	{
	public:
		PhysicalForceFieldDesc()
		{
			SetDefault();
		}
		void SetDefault()
		{
			node=0;
			trans.makeIdentity();
			coordinates=EFFC_Cartesian;
			kernal=0;
			forceVarity=0;
			fluidType=EFFT_Other;
			clothType=EFFT_Other;
			softbodyType=EFFT_Other;
			rigidbodyType=EFFT_Other;

			userData=0;
			group=0;

			flags[EFFF_VolumetricScalingCloth]=true;
			flags[EFFF_VolumetricScalingFluid]=true;
			flags[EFFF_VolumetricScalingRigidBody]=true;
			flags[EFFF_VolumetricScalingSoftBody]=true;
		}
		core::string name;

		//global or node relative transformation of the force field
		math::matrix4x4 trans;

		IPhysicalNode* node;

		EForceFieldCoordinates coordinates;

		std::vector<IPhysicalForceFieldShapeDesc*> shapes;

		std::vector<IPhysicalForceFieldShapeGroup*> shapeGroups;

		IPhysicalForceFieldLinearKernal* kernal;

		// Force Field Variety Index, index != 0 has to be created.
		ForceFieldVariety forceVarity;

		EForceFieldType fluidType;

		EForceFieldType clothType;

		EForceFieldType softbodyType;

		EForceFieldType rigidbodyType;

		IObject* userData;

		GroupID group;
		CollisionMask groupMask;

		bool flags[EFFF_FlagsCount];
	};
	//////////////////////////////////////////////////////////////////////////

	class IPhysicalForceFieldShapeDesc
	{
	public:

		IPhysicalForceFieldShapeDesc()
		{
			SetDefault();
		}
		virtual void SetDefault()
		{
			mat.makeIdentity();
			userData=0;
		}
		virtual EForceFieldShapeType getType()const=0;

		math::matrix4x4 mat;
		IObject* userData;
		core::string name;

	};

	class PhsyicalForceFieldSphereShapeDesc:public IPhysicalForceFieldShapeDesc
	{
	public:
		PhsyicalForceFieldSphereShapeDesc()
		{
			SetDefault();
		}

		virtual void SetDefault()
		{
			radius=1;
		}
		virtual EForceFieldShapeType getType()const
		{
			return EFFShape_Sphere;
		}
		float radius;
	};
	class PhsyicalForceFieldCapsuleShapeDesc:public IPhysicalForceFieldShapeDesc
	{
	public:
		PhsyicalForceFieldCapsuleShapeDesc()
		{
			SetDefault();
		}

		virtual void SetDefault()
		{
			radius=1;
			height=1;
		}
		virtual EForceFieldShapeType getType()const
		{
			return EFFShape_Capsule;
		}
		float radius;
		float height;
	};
	class PhsyicalForceFieldBoxShapeDesc:public IPhysicalForceFieldShapeDesc
	{
	public:
		PhsyicalForceFieldBoxShapeDesc()
		{
			SetDefault();
		}

		virtual void SetDefault()
		{
			dimensions=1;
		}
		virtual EForceFieldShapeType getType()const
		{
			return EFFShape_Box;
		}
		math::vector3d dimensions;
	};
	class PhsyicalForceFieldConvexShapeDesc:public IPhysicalForceFieldShapeDesc
	{
	public:
		PhsyicalForceFieldConvexShapeDesc()
		{
			SetDefault();
		}

		virtual void SetDefault()
		{
			mesh=0;
		}
		virtual EForceFieldShapeType getType()const
		{
			return EFFShape_Box;
		}
		scene::IMeshBuffer* mesh;
	};

	//////////////////////////////////////////////////////////////////////////

	class PhysicalForceFieldLinearKernalDesc
	{
	public:
		PhysicalForceFieldLinearKernalDesc()
		{
			SetDefault();
		}


		void SetDefault()
		{
			positionMultiplier.makeIdentity();
			positionMultiplier[0][0]=0;positionMultiplier[1][1]=0;positionMultiplier[2][2]=0;
			velocityMultiplier.makeIdentity();
			velocityMultiplier[0][0]=0;velocityMultiplier[1][1]=0;velocityMultiplier[2][2]=0;
			constant.set(0,0,0);
			positionTarget.set(0,0,0);
			velocityTarget.set(0,0,0);
			falloffLinear.set(0,0,0);
			falloffQuadratic.set(0,0,0);
			noise.set(0,0,0);
			torusRadius=1;
		}

		//Constant part of force field function
		math::vector3d constant;

		//Coefficient of force field function position term
		math::matrix3x3 positionMultiplier;

		//Force field position target.
		math::vector3d positionTarget;

		//Force field function velocity term.
		math::matrix3x3 velocityMultiplier;

		math::vector3d velocityTarget;

		float torusRadius;

		math::vector3d falloffLinear;

		math::vector3d falloffQuadratic;

		math::vector3d noise;

		core::string name;

	};

	class IPhysicalForceFieldLinearKernal
	{
	public:
		virtual math::vector3d getConstant()const=0;
		virtual void setConstant(const math::vector3d& c)=0;

		virtual math::matrix3x3 getPositionMultiplier()const=0;
		virtual void setPositionMultiplier(const math::matrix3x3& c)=0;


		virtual math::matrix3x3 getVelocityMultiplier()const=0;
		virtual void setVelocityMultiplier(const math::matrix3x3& c)=0;


		virtual math::vector3d getPositionTarget()const=0;
		virtual void setPositionTarget(const math::vector3d& c)=0;

		virtual math::vector3d getVelocityTarget()const=0;
		virtual void setVelocityTarget(const math::vector3d& c)=0;

		virtual math::vector3d getFalloffLinear()const=0;
		virtual void setFalloffLinear(const math::vector3d& c)=0;

		virtual math::vector3d getFalloffQuadratic()const=0;
		virtual void setFalloffQuadratic(const math::vector3d& c)=0;

		virtual math::vector3d getNoise()const=0;
		virtual void setNoise(const math::vector3d& c)=0;

		virtual float getTorusRadius()const=0;
		virtual void setTorusRadius(float c)=0;

		virtual const core::string& getName()const=0;
		virtual void setName(const core::string& name)=0;
	};

	//////////////////////////////////////////////////////////////////////////
	class ForceFieldShapeGroupDesc
	{
	public:
		ForceFieldShapeGroupDesc()
		{
			excludeGroup=false;
			userData=0;
			shapes.clear();
		}

		bool excludeGroup;
		core::string name;
		IObject* userData;

		std::vector<IPhysicalForceFieldShapeDesc*> shapes;
	};
}
}

#endif



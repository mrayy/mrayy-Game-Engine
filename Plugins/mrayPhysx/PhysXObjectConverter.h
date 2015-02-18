

/********************************************************************
	created:	2011/12/31
	created:	31:12:2011   0:08
	filename: 	d:\Development\mrayEngine\Engine\mrayPhysx\PhysXObjectConverter.h
	file path:	d:\Development\mrayEngine\Engine\mrayPhysx
	file base:	PhysXObjectConverter
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhysXObjectConverter__
#define __PhysXObjectConverter__

#include <NxUserContactReport.h>

#include <IPhysical3DDef.h>
#include <PhysicalShapes.h>
#include "PhysicalForceFieldDef.h"

class NxBoxShapeDesc;
class NxSphereShapeDesc;
class NxPlaneShapeDesc;
class NxHeightFieldShapeDesc;
class NxCapsuleShapeDesc;
class NxConvexShapeDesc;
class NxWheelShapeDesc;
class NxTriangleMeshShapeDesc;

class NxJointLimitSoftDesc;
class NxJointLimitSoftPairDesc;
class NxSpringDesc;
class NxJointLimitDesc;
class NxJointDriveDesc;
class NxJointLimitPairDesc;
class NxMotorDesc;
class NxTireFunctionDesc;
class NxWheelContactData;
class NxClothDesc;
class NxBodyDesc;

class NxMaterialDesc;

enum NxCombineMode;

namespace mray
{
namespace physics
{
	class BoxShapeDesc;
	class SphereShapeDesc;
	class PlaneShapeDesc;
	class TerrainShapeDesc;
	class CapsuleShapeDesc;
	class ConvexShapeDesc;
	class TriangleShapeDesc;
	class WheelShapeDesc;
	
	class PhysicMaterialDesc;

	class PhysXManager;
	class PhysicalJointLimitSoftDesc;
	class PhysicalJointLimitSoftPairDesc;
	class PhysicalSpringDesc;
	class PhysicalJointLimitDesc;
	class PhysicalJointDriveDesc;
	class PhysicalJointLimitPairDesc;
	class PhysicalMotor3DDesc;

	class PhysicalClothDesc;

	class WheelContactData;
	class PhysicalBodyDesc;

	enum EMaterialCombineMode;

class PhysXObjectConverter
{
private:
protected:
public:
	PhysXObjectConverter(){}
	virtual~PhysXObjectConverter(){}

	static bool isShapeSupported(ESupportedShapes s);
	static bool isJointSupported(ESupported3DJoints s);

	static NxShapeDesc* ConvertShapeDesc(IPhysicalShapeDesc* d,PhysXManager*manager);

	static bool ConvertNodeDesc(PhysicalNodeDesc* desc,NxActorDesc*actorDesc,NxBodyDesc*bodyDesc,PhysXManager*manager);

	static bool ConvertShapeDescCommon(IPhysicalShapeDesc* desc,NxShapeDesc* dstDesc,PhysXManager*manager);

	static bool ConvertShapeDesc(BoxShapeDesc*desc,NxBoxShapeDesc*dstDesc,PhysXManager*manager);
	static bool ConvertShapeDesc(SphereShapeDesc*desc,NxSphereShapeDesc*dstDesc,PhysXManager*manager);
	static bool ConvertShapeDesc(PlaneShapeDesc*desc,NxPlaneShapeDesc*dstDesc,PhysXManager*manager);
	static bool ConvertShapeDesc(TerrainShapeDesc*desc,NxHeightFieldShapeDesc*dstDesc,PhysXManager*manager);
	static bool ConvertShapeDesc(CapsuleShapeDesc*desc,NxCapsuleShapeDesc*dstDesc,PhysXManager*manager);
	static bool ConvertShapeDesc(ConvexShapeDesc*desc,NxConvexShapeDesc*dstDesc,PhysXManager*manager);
	static bool ConvertShapeDesc(TriangleShapeDesc* desc,NxTriangleMeshShapeDesc*dstDesc,PhysXManager*manager);
	static bool ConvertShapeDesc(WheelShapeDesc*desc,NxWheelShapeDesc*dstDesc,PhysXManager*manager);

	static bool ConvertForceFieldDesc(PhysicalForceFieldDesc*desc,NxForceFieldDesc* dstDesc,PhysXManager*manager);
	static bool ConvertForceFieldShapeGroupDesc(ForceFieldShapeGroupDesc*desc,NxForceFieldShapeGroupDesc* dstDesc,PhysXManager*manager);
	static bool ConvertForceFieldShapeDesc(IPhysicalForceFieldShapeDesc*desc,NxForceFieldShapeDesc*dstDesc,PhysXManager*manager);
	static bool ConvertForceFieldLinearKernal(PhysicalForceFieldLinearKernalDesc*desc,NxForceFieldLinearKernelDesc*dstDesc,PhysXManager*manager);

	static bool ConvertClothDesc(PhysicalClothDesc*desc,NxClothDesc*dstDesc,PhysXManager*manager);

	static bool ConvertBodyDesc(PhysicalBodyDesc*desc,NxBodyDesc*dstDesc,PhysXManager*manager);

	static bool ConvertMaterialDesc(PhysicMaterialDesc* desc,NxMaterialDesc* dstDesc);
	static bool ConvertMaterialDesc(NxMaterialDesc* desc,PhysicMaterialDesc* dstDesc);

	static ETriggerStatus ConvertTriggerFlag(NxTriggerFlag f);

	static NxConvexMesh* GenerateConvexMesh(math::vector3d* points,int count,PhysXManager*manager,bool flipNormals,bool inflanteConvex);
	static NxConvexMesh* GenerateConvexMesh(scene::IMeshBuffer* mesh,PhysXManager*manager,bool flipNormals,bool inflanteConvex,bool computeConvex);

	static void copyTireFunctionDesc(NxTireFunctionDesc&dst,const WheelShapeDesc::TireFunctionDesc&src);
	static void copyTireFunctionDesc(WheelShapeDesc::TireFunctionDesc&dst,const NxTireFunctionDesc&src);

	static void copyJointLimitSoftDesc(NxJointLimitSoftDesc&dst,const PhysicalJointLimitSoftDesc&src);
	static void copyJointLimitSoftDesc(PhysicalJointLimitSoftDesc&dst,const NxJointLimitSoftDesc&src);
	//////////////////////////////////////////////////////////////////////////

	static void copyLimitPairDesc(NxJointLimitSoftPairDesc&dst,const PhysicalJointLimitSoftPairDesc&src);
	static void copyLimitPairDesc(PhysicalJointLimitSoftPairDesc&dst,const NxJointLimitSoftPairDesc&src);
	//////////////////////////////////////////////////////////////////////////

	static void copySpringDesc(NxSpringDesc&dst,const PhysicalSpringDesc&src);
	static void copySpringDesc(PhysicalSpringDesc&dst,const NxSpringDesc&src);

	//////////////////////////////////////////////////////////////////////////
	static void copyLimitDesc(NxJointLimitDesc&dst,const PhysicalJointLimitDesc&src);
	static void copyLimitDesc(PhysicalJointLimitDesc&dst,const NxJointLimitDesc&src);
	//////////////////////////////////////////////////////////////////////////

	static void copyJointDriveDesc(NxJointDriveDesc&dst,const PhysicalJointDriveDesc&src);
	static void copyJointDriveDesc(PhysicalJointDriveDesc&dst,const NxJointDriveDesc&src);
	//////////////////////////////////////////////////////////////////////////

	static void copyLimitPairDesc(NxJointLimitPairDesc&dst,const PhysicalJointLimitPairDesc&src);
	static void copyLimitPairDesc(PhysicalJointLimitPairDesc&dst,const NxJointLimitPairDesc&src);

	//////////////////////////////////////////////////////////////////////////

	static void copyMotorDesc(NxMotorDesc&dst,const PhysicalMotor3DDesc&src);
	static void copyMotorDesc(PhysicalMotor3DDesc&dst,const NxMotorDesc&src);

	//////////////////////////////////////////////////////////////////////////
	static void copyWheelContactData(NxWheelContactData&dst,const WheelContactData&src);
	static void copyWheelContactData(WheelContactData&dst,const NxWheelContactData&src);

	static NxContactPairFlag convertContactFlag(EContactReportFlag f);

	static NxCombineMode convertCombineMode(EMaterialCombineMode m);
	static EMaterialCombineMode convertCombineMode(NxCombineMode m);

	static inline NxForceFieldType convertForceFieldType(EForceFieldType t)
	{
		switch (t)
		{
		case EFFT_Gravitational:
			return NX_FF_TYPE_GRAVITATIONAL;
		case EFFT_Other:
			return NX_FF_TYPE_OTHER;
		case EFFT_NoInteraction:
			return NX_FF_TYPE_NO_INTERACTION;
		}
		return NX_FF_TYPE_OTHER;
	}
	static inline EForceFieldType convertForceFieldType(NxForceFieldType t)
	{
		switch (t)
		{
		case NX_FF_TYPE_GRAVITATIONAL:
			return EFFT_Gravitational;
		case NX_FF_TYPE_OTHER:
			return EFFT_Other;
		case NX_FF_TYPE_NO_INTERACTION:
			return EFFT_NoInteraction;
		}
		return EFFT_Other;
	}

	static inline NxForceFieldCoordinates convertForceFieldCoordinates(EForceFieldCoordinates t)
	{
		switch (t)
		{
		case EFFC_Cartesian:
			return NX_FFC_CARTESIAN;
		case EFFC_Spherical:
			return NX_FFC_SPHERICAL;
		case EFFC_Cylindrical:
			return NX_FFC_CYLINDRICAL;
		case EFFC_Toroidal:
			return NX_FFC_TOROIDAL;
		}
		return NX_FFC_CARTESIAN;
	}
	static inline EForceFieldCoordinates convertForceFieldCoordinates(NxForceFieldCoordinates t)
	{
		switch (t)
		{
		case NX_FFC_CARTESIAN:
			return EFFC_Cartesian;
		case NX_FFC_SPHERICAL:
			return EFFC_Spherical;
		case NX_FFC_CYLINDRICAL:
			return EFFC_Cylindrical;
		case NX_FFC_TOROIDAL:
			return EFFC_Toroidal;
		}
		return EFFC_Cartesian;
	}

	static inline NxShapesType convertShapeType(EShapeType t)
	{
		switch (t)
		{
		case EShape_Fixed:return NX_STATIC_SHAPES;
		case EShape_Dynamic:return NX_DYNAMIC_SHAPES;
		case EShape_All:return NX_ALL_SHAPES;
		}
		return NX_STATIC_SHAPES;
	}
	static inline EShapeType convertShapeType(NxShapesType t)
	{
		switch (t)
		{
		case NX_STATIC_SHAPES:return EShape_Fixed;
		case NX_DYNAMIC_SHAPES:return EShape_Dynamic;
		case NX_ALL_SHAPES:return EShape_All;
		}
		return EShape_Fixed;
	}

	static inline NxForceMode convertForceMode(EForceMode m)
	{
		switch(m)
		{
		case EFM_Impulse:return NX_IMPULSE;
		case EFM_VelocityChange:return NX_VELOCITY_CHANGE;
		case EFM_SmoothImpulse:return NX_SMOOTH_IMPULSE;
		case EFM_SmoothVelocityChange:return NX_SMOOTH_VELOCITY_CHANGE;
		case EFM_Acceleration:return NX_ACCELERATION;
		}
		return NX_FORCE;
	}
	static inline EForceMode convertForceMode(NxForceMode m)
	{
		switch(m)
		{
		case NX_IMPULSE:return EFM_Impulse;
		case NX_VELOCITY_CHANGE:return EFM_VelocityChange;
		case NX_SMOOTH_IMPULSE:return EFM_SmoothImpulse;
		case NX_SMOOTH_VELOCITY_CHANGE:return EFM_SmoothVelocityChange;
		case NX_ACCELERATION:return EFM_Acceleration;
		}
		return EFM_Force;
	}
	static inline NxD6JointMotion convertJointMotion(ED6JointMotion m)
	{
		switch(m)
		{
		case ED6Joint_Motion_Free:return NX_D6JOINT_MOTION_FREE;
		case ED6Joint_Motion_Limited:return NX_D6JOINT_MOTION_LIMITED;
		case ED6Joint_Motion_Locked:return NX_D6JOINT_MOTION_LOCKED;
		}
		return NX_D6JOINT_MOTION_LOCKED;
	}
	static inline ED6JointMotion convertJointMotion(NxD6JointMotion m)
	{
		switch(m)
		{
		case NX_D6JOINT_MOTION_FREE :return ED6Joint_Motion_Free;
		case NX_D6JOINT_MOTION_LIMITED:return ED6Joint_Motion_Limited;
		case NX_D6JOINT_MOTION_LOCKED:return ED6Joint_Motion_Locked;
		}
		return ED6Joint_Motion_Locked;
	}

	static inline NxJointProjectionMode convertJoinProjectionMode(EJointProjectionMode m)
	{
		switch(m)
		{
		case EJPM_None:return NX_JPM_NONE;
		case EJPM_PointMindist:return NX_JPM_POINT_MINDIST;
		case EJPM_LinearMindist:return NX_JPM_LINEAR_MINDIST;
		}
		return NX_JPM_NONE;
	}
	static inline EJointProjectionMode convertJoinProjectionMode(NxJointProjectionMode m)
	{
		switch(m)
		{
		case NX_JPM_NONE:return EJPM_None;
		case NX_JPM_POINT_MINDIST:return EJPM_PointMindist;
		case NX_JPM_LINEAR_MINDIST:return EJPM_LinearMindist;
		}
		return EJPM_None;
	}
};


}
}

#endif



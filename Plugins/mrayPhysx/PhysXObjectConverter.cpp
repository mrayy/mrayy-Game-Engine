
#include "stdafx.h"
#include <NxPhysics.h>

#include "PhysXManager.h"
#include "PhysXObjectConverter.h"

#include "PhysicalShapes.h"
#include "IPhysicMaterial.h"
#include "physxcommon.h"
#include "Stream.h"

#include "PhysXForceFieldShapeGroup.h"
#include "PhysXForceFieldLinearKernal.h"
#include "PhysicsMaterialManager.h"

namespace mray
{
namespace physics
{

	bool PhysXObjectConverter::isShapeSupported(ESupportedShapes s){
		switch(s){
		case ESS_Box:
		case ESS_Capsule:
		case ESS_Convex:
		case ESS_Plane:
		case ESS_Sphere:
		case ESS_Terrain:
		case ESS_Wheel:
		case ESS_TriangleMesh:
			return true;
		}
		return false;
	}
	bool PhysXObjectConverter::isJointSupported(ESupported3DJoints s){
		switch(s){
		case ESJ3D_RevoluteJoint:
		case ESJ3D_PrismaticJoint:
		case ESJ3D_DistanceJoint:
		case ESJ3D_PulleyJoint:
		case ESJ3D_FixedJoint:
		case ESJ3D_D6Joint:
		case ESJ3D_Cylindrical:
		case ESJ3D_Spherical:
		case ESJ3D_PointOnLine:
		case ESJ3D_PointOnPlane:
			return true;
		}
		return false;
	}

NxShapeDesc* PhysXObjectConverter::ConvertShapeDesc(IPhysicalShapeDesc* d,PhysXManager*manager)
{

	NxShapeDesc*desc=0;
	if(!isShapeSupported(d->getType()))
		return 0;
	switch(d->getType()){
	case ESS_Box:
		{
			NxBoxShapeDesc* boxDesc=new NxBoxShapeDesc();
			ConvertShapeDesc((BoxShapeDesc*)d,boxDesc,manager);
			desc=boxDesc;
		}
		break;
	case ESS_Capsule:
		{
			NxCapsuleShapeDesc* capDesc=new NxCapsuleShapeDesc();
			ConvertShapeDesc((CapsuleShapeDesc*)d,capDesc,manager);
			desc=capDesc;
		}
		break;
	case ESS_Convex:
		{
			NxConvexShapeDesc* convDesc=new NxConvexShapeDesc();
			ConvertShapeDesc((ConvexShapeDesc*)d,convDesc,manager);
			desc=convDesc;
		}
		break;
	case ESS_Plane:
		{
			NxPlaneShapeDesc *planeDesc=new NxPlaneShapeDesc();
			ConvertShapeDesc((PlaneShapeDesc*)d,planeDesc,manager);
			desc=planeDesc;
		}
		break;
	case ESS_Sphere:
		{
			NxSphereShapeDesc *sphereDesc=new NxSphereShapeDesc();
			ConvertShapeDesc((SphereShapeDesc*)d,sphereDesc,manager);
			desc=sphereDesc;
		}
		break;
	case ESS_Terrain:
		{
			NxHeightFieldShapeDesc* heightFieldShapeDesc=new NxHeightFieldShapeDesc();
			ConvertShapeDesc((TerrainShapeDesc*)d,heightFieldShapeDesc,manager);
			desc=heightFieldShapeDesc;
		}
		break;
	case ESS_Wheel:
		{
			NxWheelShapeDesc *whDesc=new NxWheelShapeDesc();
			ConvertShapeDesc((WheelShapeDesc*)d,whDesc,manager);
			desc=whDesc;
		}
		break;
	case ESS_TriangleMesh:
		{
			NxTriangleMeshShapeDesc *shapeDesc=new NxTriangleMeshShapeDesc();
			ConvertShapeDesc((TriangleShapeDesc*)d,shapeDesc,manager);
			desc=shapeDesc;
		}
		break;
	}
	if(desc){
		core::stringc nameStr;
		core::string_to_char(d->name.c_str(),nameStr);
		desc->name=strdup(nameStr.c_str());
		desc->userData=d->userData;
		if(!desc->isValid())
		{
			gLogManager.log("PhysXManager::ConvertShapeDesc()- shape \""+nameStr+"\" is not valid!",ELL_WARNING);
			delete desc;
			return 0;
		}
	}
	return desc;
}

bool PhysXObjectConverter::ConvertNodeDesc(PhysicalNodeDesc* desc,NxActorDesc*actorDesc,NxBodyDesc*bodyDesc,PhysXManager*manager)
{

	core::stringc nameStr;
	core::string_to_char(desc->name.c_str(),nameStr);
	actorDesc->name=strdup(nameStr.c_str());
	actorDesc->density=desc->density;
	actorDesc->globalPose.setRowMajor44(desc->globalPos.pointer());
	if(desc->bodyDesc){
		PhysXObjectConverter::ConvertBodyDesc(desc->bodyDesc,bodyDesc,manager);
		actorDesc->body=bodyDesc;
	}

	NxActor*actor=0;
	for (int i=0;i<desc->shapes.size();++i)
	{
		IPhysicalShapeDesc*d=desc->shapes[i];
		NxShapeDesc*nxdesc=ConvertShapeDesc(d,manager);
		if(!nxdesc)
			continue;
		actorDesc->shapes.push_back(nxdesc);
	}

	actorDesc->globalPose.setRowMajor44(desc->globalPos.pointer());
	math::vector3d p=desc->globalPos.getTranslation();
	actorDesc->globalPose.t=NxVec3(p.x,p.y,p.z);
	actorDesc->density=desc->density;
	actorDesc->group=desc->actorGroupID;
	actorDesc->contactReportFlags=
		desc->contactFlags[ECRF_StartTouch]?NX_NOTIFY_ON_START_TOUCH:0 |
		desc->contactFlags[ECRF_Touch]?NX_NOTIFY_ON_TOUCH:0 |
		desc->contactFlags[ECRF_EndTouch]?NX_NOTIFY_ON_END_TOUCH:0 |
		desc->contactFlags[ECRF_Impact]?NX_NOTIFY_ON_IMPACT:0 |
		desc->contactFlags[ECRF_Roll]?NX_NOTIFY_ON_ROLL:0 |
		desc->contactFlags[ECRF_Slide]?NX_NOTIFY_ON_SLIDE:0 |
		desc->contactFlags[ECRF_StartTouchForceThreshold]?NX_NOTIFY_ON_START_TOUCH_FORCE_THRESHOLD:0 |
		desc->contactFlags[ECRF_TouchThreshold]?NX_NOTIFY_ON_TOUCH_FORCE_THRESHOLD:0 |
		desc->contactFlags[ECRF_EndTouchThreshold]?NX_NOTIFY_ON_END_TOUCH_FORCE_THRESHOLD:0 |
		desc->contactFlags[ECRF_NotifyForces]?NX_NOTIFY_FORCES:0 |
		desc->contactFlags[ECRF_IgnorePair]?NX_IGNORE_PAIR:0 ;
	//actorDesc.userData=desc->userData;


	//actorDesc.userData=n.pointer();

	if(!actorDesc->isValid())
	{
		NxArray<NxShapeDesc*, NxAllocatorDefault>::Iterator itr=actorDesc->shapes.begin();
		for (;itr!=actorDesc->shapes.end();itr++)
		{
			delete *itr;
		}
		return 0;
	}
	return true;
}
bool PhysXObjectConverter::ConvertShapeDescCommon(IPhysicalShapeDesc* desc,NxShapeDesc* dstDesc,PhysXManager*manager)
{
	dstDesc->density=desc->density;
	dstDesc->localPose.setRowMajor44(desc->localSpace.pointer());

	dstDesc->skinWidth=-1;
	
	dstDesc->mass=desc->mass;
	dstDesc->group=desc->collisionGroup;
	dstDesc->groupsMask.bits0=desc->groupMask.bits[0];
	dstDesc->groupsMask.bits1=desc->groupMask.bits[1];
	dstDesc->groupsMask.bits2=desc->groupMask.bits[2];
	dstDesc->groupsMask.bits3=desc->groupMask.bits[3];
	if(desc->materialName!=mT(""))
	{
		IPhysicMaterial* m= manager->GetMaterialManager()->GetMaterialByName(desc->materialName);
		if(m)
			dstDesc->materialIndex=m->getMaterialIndex();
	}
	if(dstDesc->materialIndex==0 && desc->material)
		dstDesc->materialIndex=desc->material->getMaterialIndex();
	if(dstDesc->materialIndex==0 && desc->materialDesc)
	{
		IPhysicMaterial* m= manager->createMaterial(desc->materialDesc);
		if(m)
			dstDesc->materialIndex=m->getMaterialIndex();
	}

	dstDesc->shapeFlags=NX_SF_VISUALIZATION |
		(desc->flags & ESF_TriggerOnEnter? NX_TRIGGER_ON_ENTER:0) |
		(desc->flags& ESF_TriggerOnLeave? NX_TRIGGER_ON_LEAVE:0) |
		(desc->flags& ESF_TriggerOnStay? NX_TRIGGER_ON_STAY:0) |
		(desc->flags& ESF_TriggerEnabled? NX_TRIGGER_ENABLE:0) |

		(desc->flags& ESF_DisableCollision? NX_SF_DISABLE_COLLISION:0) |
		(desc->flags& ESF_DisableRaycasting? NX_SF_DISABLE_RAYCASTING:0) |

		(desc->flags& ESF_FluidDrain? NX_SF_FLUID_DRAIN:0) |
		(desc->flags& ESF_FluidDisableCollision? NX_SF_FLUID_DISABLE_COLLISION:0) |
		(desc->flags& ESF_FluidTwoWay? NX_SF_FLUID_TWOWAY:0) |

		(desc->flags& ESF_FluidDrain? NX_SF_FLUID_DRAIN:0) |
		(desc->flags& ESF_FluidDisableCollision? NX_SF_FLUID_DISABLE_COLLISION:0) |
		(desc->flags& ESF_FluidTwoWay? NX_SF_FLUID_TWOWAY:0) |

		(desc->flags& ESF_ClothDrain? NX_SF_CLOTH_DRAIN:0) |
		(desc->flags& ESF_ClothDisableCollision? NX_SF_CLOTH_DISABLE_COLLISION:0) |
		(desc->flags& ESF_ClothTwoWay? NX_SF_CLOTH_TWOWAY:0) |

		(desc->flags& ESF_SoftbodyDrain? NX_SF_SOFTBODY_DRAIN:0) |
		(desc->flags& ESF_SoftbodyDisableCollision? NX_SF_SOFTBODY_DISABLE_COLLISION:0) |
		(desc->flags& ESF_SoftbodyTwoWay? NX_SF_SOFTBODY_TWOWAY:0) |

		(desc->flags& ESF_DisableCollisionResponse? NX_SF_DISABLE_RESPONSE:0) |
		(desc->flags& ESF_DynamicDynamicCCD? NX_SF_DYNAMIC_DYNAMIC_CCD:0) |
		(desc->flags& ESF_DisableSceneQueries? NX_SF_DISABLE_SCENE_QUERIES:0) |

		(desc->flags& ESF_ContactForce? NX_SF_POINT_CONTACT_FORCE:0) ;

	//dstDesc->shapeFlags|=NX_SF_DYNAMIC_DYNAMIC_CCD;

	return true;
}
bool PhysXObjectConverter::ConvertShapeDesc(BoxShapeDesc*desc,NxBoxShapeDesc*dstDesc,PhysXManager*manager)
{
	ConvertShapeDescCommon(desc,dstDesc,manager);
	dstDesc->dimensions.set(desc->extent.x,desc->extent.y,desc->extent.z); 
	return true;

}
bool PhysXObjectConverter::ConvertShapeDesc(SphereShapeDesc*desc,NxSphereShapeDesc*dstDesc,PhysXManager*manager)
{
	ConvertShapeDescCommon(desc,dstDesc,manager);

	dstDesc->radius = desc->radius; 
	return true;

}
bool PhysXObjectConverter::ConvertShapeDesc(PlaneShapeDesc*desc,NxPlaneShapeDesc*dstDesc,PhysXManager*manager)
{
	ConvertShapeDescCommon(desc,dstDesc,manager);
	dstDesc->normal=NxVec3(desc->plane.Normal.x,desc->plane.Normal.y,desc->plane.Normal.z);
	dstDesc->d=desc->plane.D;
	return true;
}
bool PhysXObjectConverter::ConvertShapeDesc(TerrainShapeDesc*desc,NxHeightFieldShapeDesc*dstDesc,PhysXManager*manager)
{
	ConvertShapeDescCommon(desc,dstDesc,manager);
	static NxReal sixtyFourKb = 65536.0f;
	static NxReal thirtyTwoKb = 32767.5f;

	NxHeightFieldDesc fieldDesc;


	fieldDesc.nbColumns		= desc->cols/(desc->skip+1);
	fieldDesc.nbRows		= desc->rows/(desc->skip+1);
	fieldDesc.convexEdgeThreshold = 0;

	// allocate storage for samples
	fieldDesc.samples			= new NxU32[fieldDesc.nbColumns*fieldDesc.nbRows];
	fieldDesc.sampleStride	= sizeof(NxU32);

	int skip=desc->skip+1;
	int n=0;
	char* currentByte = (char*)fieldDesc.samples;
	for (NxU32 row = 0; row < fieldDesc.nbRows; row++)
	{
		for (NxU32 column = 0; column < fieldDesc.nbColumns; column++)
		{
			int index=(row*desc->cols+column)*skip;
			NxI16 height = (NxI16)((2*desc->data[index]-1)*thirtyTwoKb);

			NxHeightFieldSample* currentSample = (NxHeightFieldSample*)currentByte;
			currentSample->height = height;

			currentSample->tessFlag = 0;
			currentSample->materialIndex0 = 1;
			currentSample->materialIndex1 = 1;

			currentByte += fieldDesc.sampleStride;
			++n;
		}
	}

	NxHeightField* heightField = manager->GetSDK()->createHeightField(fieldDesc);

	// data has been copied, we can free our buffer
	delete [] fieldDesc.samples;


	dstDesc->heightField	= heightField;
	dstDesc->heightScale	= desc->scale.y / sixtyFourKb ;
	dstDesc->rowScale		= desc->scale.x /(float)fieldDesc.nbRows;
	dstDesc->columnScale	= desc->scale.z /(float)fieldDesc.nbColumns;
	//dstDesc->meshFlags	= NX_MESH_SMOOTH_SPHERE_COLLISIONS;
	//dstDesc->materialIndexHighBits = 0;
	//dstDesc->holeMaterial = 2;

	return true;
}
bool PhysXObjectConverter::ConvertShapeDesc(CapsuleShapeDesc*desc,NxCapsuleShapeDesc*dstDesc,PhysXManager*manager)
{

	ConvertShapeDescCommon(desc,dstDesc,manager);
	dstDesc->radius=desc->radius;
	dstDesc->height=desc->height;
	return true;
}

bool PhysXObjectConverter::ConvertShapeDesc(ConvexShapeDesc*desc,NxConvexShapeDesc*dstDesc,PhysXManager*manager)
{
	ConvertShapeDescCommon(desc,dstDesc,manager);
	if(desc->mesh==0){
		if(desc->points.size()>0)
			dstDesc->meshData=GenerateConvexMesh(&desc->points[0],desc->points.size(),manager,desc->flipNormals,desc->inflanteConvex);
	}else
	{
		dstDesc->meshData=GenerateConvexMesh(desc->mesh,manager,desc->flipNormals,desc->inflanteConvex,desc->computeConvex);
	}


	if(!dstDesc->meshData){
		return 0;
	}
	return true;
}
bool PhysXObjectConverter::ConvertShapeDesc(TriangleShapeDesc* desc,NxTriangleMeshShapeDesc*dstDesc,PhysXManager*manager)
{
	ConvertShapeDescCommon(desc,dstDesc,manager);

	if(!desc->mesh)
		return 0;
	video::IHardwareStreamBuffer* vBuff= desc->mesh->getStream(0,video::EMST_Position);
	if(!vBuff)
		return 0;
	video::IndexData* indexData=desc->mesh->getIndexData();

	if(!indexData->indexBuffer)
		return 0;

	void*vb=vBuff->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);
	void*ib=indexData->indexBuffer->lock(indexData->firstIndex,indexData->indexCount,video::IHardwareBuffer::ELO_ReadOnly);

	NxTriangleMeshDesc meshDesc;
	meshDesc.numVertices=vBuff->length();
	meshDesc.pointStrideBytes=sizeof(math::vector3d);
	meshDesc.points=vb;

	meshDesc.numTriangles=indexData->indexCount/3;
	meshDesc.triangles=ib;
	meshDesc.triangleStrideBytes=3*indexData->indexBuffer->getIndexSize();
	if(indexData->indexBuffer->getIndexType()==video::IHardwareIndexBuffer::EIT_16Bit)
		meshDesc.flags=NX_MF_16_BIT_INDICES;
	else
		meshDesc.flags=0;

	meshDesc.flags|= NX_MF_FLIPNORMALS*desc->flipNormals | 
		NX_MF_HARDWARE_MESH *desc->hardwareMesh;


	MemoryWriteBuffer buf;

	manager->GetCooking()->NxInitCooking();

	bool status = manager->GetCooking()->NxCookTriangleMesh(meshDesc, buf);
	NxTriangleMesh* pMesh;

	if (status)
	{
		pMesh = manager->GetSDK()->createTriangleMesh(MemoryReadBuffer(buf.data));
	}
	else
	{
		assert(false);
		pMesh = NULL;
	}
	manager->GetCooking()->NxCloseCooking();

	if(0)
	{
		NxSimpleTriangleMesh triMesh;
		triMesh.numTriangles=indexData->indexCount/3;
		triMesh.triangles=ib;
		triMesh.triangleStrideBytes=3*indexData->indexBuffer->getIndexSize();

		triMesh.numVertices=vBuff->length();
		triMesh.pointStrideBytes=sizeof(math::vector3d);
		triMesh.points=vb;

		NxCCDSkeleton* sk= manager->getPhysXSDK()->createCCDSkeleton(triMesh);
		dstDesc->ccdSkeleton	=  sk;

	}

	vBuff->unlock();
	indexData->indexBuffer->unlock();

	dstDesc->meshData		= pMesh;
	dstDesc->meshPagingMode = NX_MESH_PAGING_AUTO;

	return true;
}
bool PhysXObjectConverter::ConvertShapeDesc(WheelShapeDesc*desc,NxWheelShapeDesc*dstDesc,PhysXManager*manager)
{

	ConvertShapeDescCommon(desc,dstDesc,manager);
	
	dstDesc->radius=desc->radius;
	dstDesc->suspensionTravel=desc->suspensionTravel;
	dstDesc->inverseWheelMass=desc->inverseWheelMass;
	dstDesc->motorTorque=desc->motorTorque;
	dstDesc->brakeTorque=desc->brakeTorque;
	dstDesc->steerAngle=desc->steerAngle;

	dstDesc->suspension.spring=desc->suspension.spring;
	dstDesc->suspension.damper=desc->suspension.damper;
	dstDesc->suspension.targetValue=desc->suspension.targetValue;

	copyTireFunctionDesc(dstDesc->longitudalTireForceFunction,desc->longitudalTireForceFunction);
	copyTireFunctionDesc(dstDesc->lateralTireForceFunction,desc->lateralTireForceFunction);

	return true;
}


bool PhysXObjectConverter::ConvertForceFieldDesc(PhysicalForceFieldDesc*desc,NxForceFieldDesc* dstDesc,PhysXManager*manager)
{
	if(desc->node)
		dstDesc->actor=((CPhysXNode*)desc->node)->getActor();
	else dstDesc->actor=0;
	dstDesc->clothType=convertForceFieldType(desc->clothType);
	dstDesc->fluidType=convertForceFieldType(desc->fluidType);
	dstDesc->rigidBodyType=convertForceFieldType(desc->rigidbodyType);
	dstDesc->softBodyType=convertForceFieldType(desc->softbodyType);
	dstDesc->coordinates=convertForceFieldCoordinates(desc->coordinates);
	dstDesc->flags=
		(desc->flags[EFFF_VolumetricScalingFluid]?NX_FFF_VOLUMETRIC_SCALING_FLUID:0)|
		(desc->flags[EFFF_VolumetricScalingCloth]?NX_FFF_VOLUMETRIC_SCALING_CLOTH:0)|
		(desc->flags[EFFF_VolumetricScalingSoftBody]?NX_FFF_VOLUMETRIC_SCALING_SOFTBODY:0)|
		(desc->flags[EFFF_VolumetricScalingRigidBody]?NX_FFF_VOLUMETRIC_SCALING_RIGIDBODY:0);
	dstDesc->forceFieldVariety=desc->forceVarity;
	dstDesc->group=desc->group;
	dstDesc->groupsMask.bits0=desc->groupMask.bits[0];
	dstDesc->groupsMask.bits1=desc->groupMask.bits[1];
	dstDesc->groupsMask.bits2=desc->groupMask.bits[2];
	dstDesc->groupsMask.bits3=desc->groupMask.bits[3];
	for(int i=0;i<desc->shapes.size();++i)
	{
		NxForceFieldShapeDesc* sd=0;
		switch (desc->shapes[i]->getType())
		{
		case EFFShape_Sphere:
			sd=new NxSphereForceFieldShapeDesc();
			break;
		case EFFShape_Capsule:
			sd=new NxCapsuleForceFieldShapeDesc();
			break;
		case EFFShape_Box:
			sd=new NxBoxForceFieldShapeDesc();
			break;
		case EFFShape_Convex:
			sd=new NxConvexForceFieldShapeDesc();
			break;
		default:
			continue;
		}
		if(!ConvertForceFieldShapeDesc(desc->shapes[i],sd,manager))
		{
			delete sd;
			continue;
		}
		dstDesc->includeGroupShapes.push_back(sd);
	}
	for(int i=0;i<desc->shapeGroups.size();++i)
	{
		dstDesc->shapeGroups.push_back(((PhysXForceFieldShapeGroup*)desc->shapeGroups[i])->getNxShapeGroup());
	}
	if(desc->kernal)
		dstDesc->kernel=((PhysXForceFieldLinearKernal*)desc->kernal)->getNxKernal();
	else
		dstDesc->kernel=0;

	return true;
}
bool PhysXObjectConverter::ConvertForceFieldShapeGroupDesc(ForceFieldShapeGroupDesc*desc,NxForceFieldShapeGroupDesc* dstDesc,PhysXManager*manager)
{
	dstDesc->flags=desc->excludeGroup ? NX_FFSG_EXCLUDE_GROUP:0 ;
	dstDesc->name=strdup(string_to_char(desc->name).c_str());
	for(int i=0;i<desc->shapes.size();++i)
	{
		NxForceFieldShapeDesc* d=0;
		switch(desc->shapes[i]->getType())
		{
		case EFFShape_Sphere:
			d=new NxSphereForceFieldShapeDesc();
			break;
		case EFFShape_Capsule:
			d=new NxCapsuleForceFieldShapeDesc();
			break;
		case EFFShape_Box:
			d=new NxBoxForceFieldShapeDesc();
			break;
		case EFFShape_Convex:
			d=new NxConvexForceFieldShapeDesc();
			break;
		}
		ConvertForceFieldShapeDesc(desc->shapes[i],d,manager);
		dstDesc->shapes.push_back(d);
	}
	return true;
}

bool PhysXObjectConverter::ConvertForceFieldShapeDesc(IPhysicalForceFieldShapeDesc*desc,NxForceFieldShapeDesc*dstDesc,PhysXManager*manager)
{
	dstDesc->pose.setRowMajor44(desc->mat.mat);
	dstDesc->name=strdup(core::string_to_char(desc->name).c_str());
	switch (desc->getType())
	{
	case EFFShape_Sphere:
		((NxSphereForceFieldShapeDesc*)dstDesc)->radius=((PhsyicalForceFieldSphereShapeDesc*)desc)->radius;
		break;
	case EFFShape_Capsule:
		((NxCapsuleForceFieldShapeDesc*)dstDesc)->radius=((PhsyicalForceFieldCapsuleShapeDesc*)desc)->radius;
		((NxCapsuleForceFieldShapeDesc*)dstDesc)->height=((PhsyicalForceFieldCapsuleShapeDesc*)desc)->height;
		break;
	case EFFShape_Box:
		((NxBoxForceFieldShapeDesc*)dstDesc)->dimensions=ToNxVec3(((PhsyicalForceFieldBoxShapeDesc*)desc)->dimensions);
		break;
	case EFFShape_Convex:
		((NxConvexForceFieldShapeDesc*)dstDesc)->meshData=GenerateConvexMesh(((PhsyicalForceFieldConvexShapeDesc*)desc)->mesh,manager,false,false,true);
	default:
		return false;
	}
	return true;
}
bool PhysXObjectConverter::ConvertForceFieldLinearKernal(PhysicalForceFieldLinearKernalDesc*desc,NxForceFieldLinearKernelDesc*dstDesc,PhysXManager*manager)
{
	dstDesc->constant=ToNxVec3(desc->constant);
	dstDesc->positionTarget=ToNxVec3(desc->positionTarget);
	dstDesc->velocityTarget=ToNxVec3(desc->velocityTarget);
	dstDesc->falloffLinear=ToNxVec3(desc->falloffLinear);
	dstDesc->falloffQuadratic=ToNxVec3(desc->falloffQuadratic);
	dstDesc->noise=ToNxVec3(desc->noise);
	dstDesc->torusRadius=desc->torusRadius;
	dstDesc->positionMultiplier.setRowMajor(desc->positionMultiplier.mat);
	dstDesc->velocityMultiplier.setRowMajor(desc->velocityMultiplier.mat);

	dstDesc->name=strdup(string_to_char(desc->name).c_str());
	return true;
}

bool PhysXObjectConverter::ConvertClothDesc(PhysicalClothDesc*desc,NxClothDesc*dstDesc,PhysXManager*manager)
{

	NxClothMeshDesc mdesc;
	NxBounds3 tmpBounds;
	MemoryWriteBuffer buf;
	NxClothMesh* mClothMesh=0;
	video::IHardwareStreamBuffer* normalStream=0;

	if(!desc->meshData)
		return false;
	video::IHardwareStreamBuffer* posStream= desc->meshData->getStream(0,video::EMST_Position);
	video::IndexData* indexData= desc->meshData->getIndexData();
	if(!posStream || !indexData || !indexData->indexBuffer)
		return false;

	dstDesc->globalPose.setRowMajor44(desc->globalPos.pointer());
	dstDesc->thickness=desc->thickness;
	dstDesc->density=desc->density;
	dstDesc->bendingStiffness=desc->bendingStiffness;
	dstDesc->stretchingStiffness=desc->stretchingStiffness;
	dstDesc->dampingCoefficient=desc->dampingCoefficient;
	dstDesc->friction=desc->friction;
	dstDesc->pressure=desc->pressure;
	dstDesc->tearFactor=desc->tearFactor;
	dstDesc->collisionResponseCoefficient=desc->collisionResponseCoefficient;
	dstDesc->attachmentResponseCoefficient=desc->attachmentResponseCoefficient;
	dstDesc->attachmentTearFactor=desc->attachmentTearFactor;
	dstDesc->toFluidResponseCoefficient=desc->toFluidResponseCoefficient;
	dstDesc->fromFluidResponseCoefficient=desc->fromFluidResponseCoefficient;
	dstDesc->minAdhereVelocity=desc->minAdhereVelocity;
	dstDesc->solverIterations=desc->solverIterations;
	dstDesc->externalAcceleration=ToNxVec3(desc->externalAcceleration);
	dstDesc->windAcceleration=ToNxVec3(desc->windAcceleration);
	dstDesc->wakeUpCounter=desc->wakeUpCounter;
	dstDesc->sleepLinearVelocity=desc->sleepLinearVelocity;
	dstDesc->relativeGridSpacing=desc->relativeGridSpacing;

	tmpBounds.set(ToNxVec3(desc->validBounds.MinP),ToNxVec3(desc->validBounds.MaxP));
	dstDesc->validBounds=tmpBounds;
	dstDesc->collisionGroup=desc->collisionGroup;



	mdesc.numVertices=posStream->length();
	video::IHardwareIndexBuffer::EIndexType indexType= indexData->indexBuffer->getIndexType();
	mdesc.numTriangles=indexData->indexCount/3;
	mdesc.pointStrideBytes=sizeof(math::vector3d);
	mdesc.triangleStrideBytes=3*indexData->indexBuffer->getIndexSize();
	mdesc.weldingDistance=0.0001f;
	mdesc.flags=NX_CLOTH_MESH_WELD_VERTICES;


	void*vb=posStream->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);
	void*ib=indexData->indexBuffer->lock(indexData->firstIndex,indexData->indexCount,video::IHardwareBuffer::ELO_ReadOnly);

	mdesc.points=vb;
	mdesc.triangles=ib;
	if(indexData->indexBuffer->getIndexType()==video::IHardwareIndexBuffer::EIT_16Bit)
		mdesc.flags|=NX_MF_16_BIT_INDICES;

	if(desc->flags[ECF_Terrable])
		mdesc.flags|=NX_CLOTH_MESH_TEARABLE;

	mdesc.vertexFlagStrideBytes =sizeof(NxU32);
	if(desc->tearableFlags.size()==mdesc.numVertices)
	{
		mdesc.vertexFlags=malloc(sizeof(NxU32)*mdesc.numVertices); 
		NxU32* flags = (NxU32*)mdesc.vertexFlags;
		for(int i=0;i<mdesc.numVertices;++i)
		{
			flags[i]=desc->tearableFlags[i]?NX_CLOTH_VERTEX_TEARABLE:0;
		}
	}


	manager->GetCooking()->NxInitCooking();
	if(manager->GetCooking()->NxCookClothMesh(mdesc, buf)){
		mClothMesh = manager->getPhysXSDK()->createClothMesh(MemoryReadBuffer(buf.data));
	}
	manager->GetCooking()->NxCloseCooking();


	posStream->unlock();
	indexData->indexBuffer->unlock();

	if(!mClothMesh)
		return false;

	int maxVertices=3*posStream->length();
	int maxIndices=indexData->indexCount;
	video::IHardwareIndexBuffer::EIndexType resultIndexType;
	if(maxIndices>=65535)
		resultIndexType=video::IHardwareIndexBuffer::EIT_32Bit;
	else resultIndexType=video::IHardwareIndexBuffer::EIT_16Bit;


	dstDesc->clothMesh=mClothMesh;

	dstDesc->flags =0;
	dstDesc->flags |= desc->flags[ECF_Pressure]?NX_CLF_PRESSURE:0;
	dstDesc->flags |= desc->flags[ECF_Static]?NX_CLF_STATIC:0;
	dstDesc->flags |= desc->flags[ECF_DisableCollision]?NX_CLF_DISABLE_COLLISION:0;
	dstDesc->flags |= desc->flags[ECF_SelfCollision]?NX_CLF_SELFCOLLISION:0;
	dstDesc->flags |= desc->flags[ECF_Gravity]?NX_CLF_GRAVITY:0;
	dstDesc->flags |= desc->flags[ECF_Bending]?NX_CLF_BENDING:0;
	dstDesc->flags |= desc->flags[ECF_BendingOrtho]?NX_CLF_BENDING_ORTHO:0;
	dstDesc->flags |= desc->flags[ECF_Damping]?NX_CLF_DAMPING:0;
	dstDesc->flags |= desc->flags[ECF_CollisionTwoWay]?NX_CLF_COLLISION_TWOWAY:0;
	dstDesc->flags |= desc->flags[ECF_TriangleCollision]?NX_CLF_TRIANGLE_COLLISION:0;
	dstDesc->flags |= desc->flags[ECF_Terrable]?NX_CLF_TEARABLE:0;
	dstDesc->flags |= desc->flags[ECF_Hardware]?NX_CLF_HARDWARE:0;
	dstDesc->flags |= desc->flags[ECF_COMDamping]?NX_CLF_COMDAMPING:0;
	dstDesc->flags |= desc->flags[ECF_ValidBounds]?NX_CLF_VALIDBOUNDS:0;
	dstDesc->flags |= desc->flags[ECF_FluidCollision]?NX_CLF_FLUID_COLLISION:0;
	dstDesc->flags |= desc->flags[ECF_DisableDynamicCCD]?NX_CLF_DISABLE_DYNAMIC_CCD:0;
	dstDesc->flags |= desc->flags[ECF_Adhere]?NX_CLF_ADHERE:0;
	return true;
}

bool PhysXObjectConverter::ConvertBodyDesc(PhysicalBodyDesc*desc,NxBodyDesc*dstDesc,PhysXManager*manager)
{

	dstDesc->angularDamping=desc->angularDamping;
	dstDesc->angularVelocity=ToNxVec3(desc->angularVelocity);
	dstDesc->CCDMotionThreshold=desc->CCDMotionThreshold;
	dstDesc->linearDamping=desc->linearDamping;
	dstDesc->linearVelocity=ToNxVec3(desc->linearVelocity);
	dstDesc->mass=desc->mass;
	dstDesc->massLocalPose.setRowMajor44(desc->massLocalPos.pointer());
	dstDesc->massSpaceInertia=ToNxVec3(desc->massSpaceInertia);
	dstDesc->maxAngularVelocity=desc->maxAngularVelocity;
	dstDesc->sleepAngularVelocity=desc->sleepAngularVelocity;
	dstDesc->sleepDamping=desc->sleepDamping;
	dstDesc->sleepEnergyThreshold=desc->sleepEnergyThreshold;
	dstDesc->sleepLinearVelocity=desc->sleepLinearVelocity;
	dstDesc->solverIterationCount=desc->solverIterationCount;
	dstDesc->wakeUpCounter=desc->wakeupCounter;
	dstDesc->contactReportThreshold=desc->contactReportThreshold;

	dstDesc->flags |= (desc->kinematic?NX_BF_KINEMATIC:0) | (desc->disableGravity?NX_BF_DISABLE_GRAVITY:0) ;

	return true;
}
NxConvexMesh* PhysXObjectConverter::GenerateConvexMesh(math::vector3d* points,int count,PhysXManager*manager
													   ,bool flipNormals,bool inflanteConvex)
{

	NxConvexMesh* resMesh=0;
	NxConvexMeshDesc convMesh;
	convMesh.numVertices = count;    
	convMesh.pointStrideBytes= sizeof(math::vector3d);    
	convMesh.points= points;    
	convMesh.flags= NX_CF_COMPUTE_CONVEX ;

	convMesh.flags|= NX_MF_FLIPNORMALS*flipNormals | 
		NX_CF_INFLATE_CONVEX *inflanteConvex;

	manager->GetCooking()->NxInitCooking();
	MemoryWriteBuffer buf;
	if(manager->GetCooking()->NxCookConvexMesh(convMesh, buf)){
		resMesh = manager->GetSDK()->createConvexMesh(MemoryReadBuffer(buf.data));
	}
	manager->GetCooking()->NxCloseCooking();
	return resMesh;
}
NxConvexMesh* PhysXObjectConverter::GenerateConvexMesh(scene::IMeshBuffer* mesh,PhysXManager*manager,bool flipNormals,bool inflanteConvex,bool computeConvex)
{
	if(!mesh)
		return 0;
	video::IHardwareStreamBuffer* vBuff;
	video::IndexData* indexData;
	NxConvexMesh* resMesh=0;


	NxConvexMeshDesc convMesh;

	vBuff= mesh->getStream(0,video::EMST_Position);
	if(!vBuff)
		return 0;
	indexData=mesh->getIndexData();

	if(!indexData->indexBuffer)
		return 0;

	void*vb=vBuff->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);
	void*ib=indexData->indexBuffer->lock(indexData->firstIndex,indexData->indexCount,video::IHardwareBuffer::ELO_ReadOnly);

	convMesh.numVertices=vBuff->length();
	convMesh.pointStrideBytes=sizeof(math::vector3d);

	convMesh.points=vb;

	if(!computeConvex)  
	{

		convMesh.numTriangles=indexData->indexCount/3;
		convMesh.triangles=ib;
		convMesh.triangleStrideBytes=3*indexData->indexBuffer->getIndexSize();
		if(indexData->indexBuffer->getIndexType()==video::IHardwareIndexBuffer::EIT_16Bit)
			convMesh.flags=NX_MF_16_BIT_INDICES;
		else
			convMesh.flags=0;
	}else
		convMesh.flags = NX_CF_COMPUTE_CONVEX ;


	convMesh.flags|= NX_MF_FLIPNORMALS*flipNormals | 
		NX_CF_INFLATE_CONVEX *inflanteConvex;

	manager->GetCooking()->NxInitCooking();
	MemoryWriteBuffer buf;
	if(manager->GetCooking()->NxCookConvexMesh(convMesh, buf)){
		resMesh = manager->GetSDK()->createConvexMesh(MemoryReadBuffer(buf.data));
	}
	manager->GetCooking()->NxCloseCooking();

	if(vBuff)
		vBuff->unlock();
	if(indexData)
		indexData->indexBuffer->unlock();
	return resMesh;

}
bool PhysXObjectConverter::ConvertMaterialDesc(PhysicMaterialDesc* desc,NxMaterialDesc* dstDesc)
{
	dstDesc->dynamicFriction=desc->dynamicFriction;
	dstDesc->dynamicFrictionV=desc->dynamicFrictionV;

	dstDesc->staticFriction=desc->staticFriction;
	dstDesc->staticFrictionV=desc->staticFrictionV;

	dstDesc->restitution=desc->restitution;
	dstDesc->dirOfAnisotropy=NxVec3(desc->dirOfAnisotropy.x,desc->dirOfAnisotropy.y,desc->dirOfAnisotropy.z);
	dstDesc->flags= desc->flags[EMF_Anisotropic]? NX_MF_ANISOTROPIC :0|
					desc->flags[EMF_DisableFriction]? NX_MF_DISABLE_FRICTION:0|
					desc->flags[EMF_DisableStrongFriction]? NX_MF_DISABLE_STRONG_FRICTION:0;
	return true;

}
bool PhysXObjectConverter::ConvertMaterialDesc(NxMaterialDesc* desc,PhysicMaterialDesc* dstDesc)
{
	dstDesc->dynamicFriction=desc->dynamicFriction;
	dstDesc->dynamicFrictionV=desc->dynamicFrictionV;

	dstDesc->staticFriction=desc->staticFriction;
	dstDesc->staticFrictionV=desc->staticFrictionV;

	dstDesc->restitution=desc->restitution;
	dstDesc->dirOfAnisotropy.set(desc->dirOfAnisotropy.x,desc->dirOfAnisotropy.y,desc->dirOfAnisotropy.z);
	dstDesc->flags[EMF_Anisotropic]= (desc->flags & NX_MF_ANISOTROPIC )!=0;
	dstDesc->flags[EMF_DisableFriction]= (desc->flags & NX_MF_DISABLE_FRICTION )!=0;
	dstDesc->flags[EMF_DisableStrongFriction]= (desc->flags & NX_MF_DISABLE_STRONG_FRICTION )!=0;
	return true;
}
ETriggerStatus PhysXObjectConverter::ConvertTriggerFlag(NxTriggerFlag f)
{
	return (EShapeFlag)
		(
		f&NX_TRIGGER_ON_ENTER ? ESF_TriggerOnEnter:0 |
		f&NX_TRIGGER_ON_LEAVE ? ESF_TriggerOnLeave:0 |
		f&NX_TRIGGER_ON_STAY?  ESF_TriggerOnStay:0 |
		f&NX_TRIGGER_ENABLE ? ESF_TriggerEnabled:0 |

		f&NX_SF_DISABLE_COLLISION ? ESF_DisableCollision:0 |
		f&NX_SF_DISABLE_RAYCASTING ? ESF_DisableRaycasting:0 |

		f&NX_SF_FLUID_DRAIN ? ESF_FluidDrain:0 |
		f&NX_SF_FLUID_DISABLE_COLLISION ? ESF_FluidDisableCollision:0 |
		f&NX_SF_FLUID_TWOWAY ? ESF_FluidTwoWay:0 |

		f&NX_SF_FLUID_DRAIN ? ESF_FluidDrain:0 |
		f&NX_SF_FLUID_DISABLE_COLLISION ? ESF_FluidDisableCollision:0 |
		f&NX_SF_FLUID_TWOWAY ? ESF_FluidTwoWay:0 |

		f&NX_SF_CLOTH_DRAIN ? ESF_ClothDrain:0 |
		f&NX_SF_CLOTH_DISABLE_COLLISION ? ESF_ClothDisableCollision:0 |
		f&NX_SF_CLOTH_TWOWAY ? ESF_ClothTwoWay:0 |

		f&NX_SF_SOFTBODY_DRAIN ? ESF_SoftbodyDrain:0 |
		f&NX_SF_SOFTBODY_DISABLE_COLLISION ? ESF_SoftbodyDisableCollision:0 |
		f&NX_SF_SOFTBODY_TWOWAY ? ESF_SoftbodyTwoWay:0 |

		f&NX_SF_DISABLE_RESPONSE ? ESF_DisableCollisionResponse:0 |
		f&NX_SF_DYNAMIC_DYNAMIC_CCD ? ESF_DynamicDynamicCCD:0 |
		f&NX_SF_DISABLE_SCENE_QUERIES ? ESF_DisableSceneQueries:0 );
}

void PhysXObjectConverter::copyTireFunctionDesc(NxTireFunctionDesc&dst,const WheelShapeDesc::TireFunctionDesc&src)
{
	dst.extremumSlip=src.extremumSlip;
	dst.extremumValue=src.extremumValue;
	dst.asymptoteSlip=src.asymptoteSlip;
	dst.asymptoteValue=src.asymptoteValue;
	dst.stiffnessFactor=src.stiffnessFactor;
}
void PhysXObjectConverter::copyTireFunctionDesc(WheelShapeDesc::TireFunctionDesc&dst,const NxTireFunctionDesc&src)
{
	dst.extremumSlip=src.extremumSlip;
	dst.extremumValue=src.extremumValue;
	dst.asymptoteSlip=src.asymptoteSlip;
	dst.asymptoteValue=src.asymptoteValue;
	dst.stiffnessFactor=src.stiffnessFactor;
}

void PhysXObjectConverter::copyJointLimitSoftDesc(NxJointLimitSoftDesc&dst,const PhysicalJointLimitSoftDesc&src){
	dst.value		=math::toRad(src.value);
	dst.restitution	=src.restitution;
	dst.spring	=src.spring;
	dst.damping	=src.damping;
}
void PhysXObjectConverter::copyJointLimitSoftDesc(PhysicalJointLimitSoftDesc&dst,const NxJointLimitSoftDesc&src){
	dst.value		=math::toDeg(src.value);
	dst.restitution	=src.restitution;
	dst.spring	=src.spring;
	dst.damping	=src.damping;
}
//////////////////////////////////////////////////////////////////////////

void PhysXObjectConverter::copyLimitPairDesc(NxJointLimitSoftPairDesc&dst,const PhysicalJointLimitSoftPairDesc&src){
	copyJointLimitSoftDesc(dst.low,src.low);
	copyJointLimitSoftDesc(dst.high,src.high);
}
void PhysXObjectConverter::copyLimitPairDesc(PhysicalJointLimitSoftPairDesc&dst,const NxJointLimitSoftPairDesc&src){
	copyJointLimitSoftDesc(dst.low,src.low);
	copyJointLimitSoftDesc(dst.high,src.high);
}

//////////////////////////////////////////////////////////////////////////

void PhysXObjectConverter::copySpringDesc(NxSpringDesc&dst,const PhysicalSpringDesc&src){
	dst.damper		=src.damper;
	dst.spring	=src.spring;
	dst.targetValue	=src.targetValue;
}
void PhysXObjectConverter::copySpringDesc(PhysicalSpringDesc&dst,const NxSpringDesc&src){
	dst.damper		=src.damper;
	dst.spring	=src.spring;
	dst.targetValue	=src.targetValue;
}

//////////////////////////////////////////////////////////////////////////

void PhysXObjectConverter::copyLimitDesc(NxJointLimitDesc&dst,const PhysicalJointLimitDesc&src){
	dst.value		=math::toRad(src.value);
	dst.restitution	=src.restitution;
	dst.hardness	=src.hardness;
}
void PhysXObjectConverter::copyLimitDesc(PhysicalJointLimitDesc&dst,const NxJointLimitDesc&src){
	dst.value		=math::toDeg(src.value);
	dst.restitution	=src.restitution;
	dst.hardness	=src.hardness;
}
//////////////////////////////////////////////////////////////////////////

void PhysXObjectConverter::copyJointDriveDesc(NxJointDriveDesc&dst,const PhysicalJointDriveDesc&src){
	dst.spring		=src.spring;
	dst.damping	=src.damping;
	dst.forceLimit	=src.forceLimit;
	dst.driveType = 0;
	if (src.drivePosition)
	{
		dst.driveType = dst.driveType | NX_D6JOINT_DRIVE_POSITION;
	}
	if (src.driveVelocity)
	{
		dst.driveType = dst.driveType | NX_D6JOINT_DRIVE_VELOCITY;
	}
}
void PhysXObjectConverter::copyJointDriveDesc(PhysicalJointDriveDesc&dst,const NxJointDriveDesc&src){
	dst.spring		=src.spring;
	dst.damping	=src.damping;
	dst.forceLimit	=src.forceLimit;
	if ((src.driveType.bitField & NX_D6JOINT_DRIVE_POSITION)==NX_D6JOINT_DRIVE_POSITION)
		dst.drivePosition = true;
	else
		dst.drivePosition = false;
	if ((src.driveType.bitField & NX_D6JOINT_DRIVE_VELOCITY)==NX_D6JOINT_DRIVE_VELOCITY)
		dst.driveVelocity = true;
	else
		dst.driveVelocity = false;
}
//////////////////////////////////////////////////////////////////////////

void PhysXObjectConverter::copyLimitPairDesc(NxJointLimitPairDesc&dst,const PhysicalJointLimitPairDesc&src){
	copyLimitDesc(dst.low,src.low);
	copyLimitDesc(dst.high,src.high);
}
void PhysXObjectConverter::copyLimitPairDesc(PhysicalJointLimitPairDesc&dst,const NxJointLimitPairDesc&src){
	copyLimitDesc(dst.low,src.low);
	copyLimitDesc(dst.high,src.high);
}

//////////////////////////////////////////////////////////////////////////

void PhysXObjectConverter::copyMotorDesc(NxMotorDesc&dst,const PhysicalMotor3DDesc&src){
	dst.freeSpin=src.freeSpin;
	dst.maxForce=src.maxForce;
	dst.velTarget=src.velTarget;
}
void PhysXObjectConverter::copyMotorDesc(PhysicalMotor3DDesc&dst,const NxMotorDesc&src){
	dst.freeSpin=src.freeSpin;
	dst.maxForce=src.maxForce;
	dst.velTarget=src.velTarget;
}

//////////////////////////////////////////////////////////////////////////

void PhysXObjectConverter::copyWheelContactData(NxWheelContactData&dst,const WheelContactData&src)
{
	dst.contactPoint=ToNxVec3(src.contactPoint);
	dst.contactNormal=ToNxVec3(src.contactNormal);
	dst.longitudalDirection=ToNxVec3(src.longitudalDirection);
	dst.lateralDirection=ToNxVec3(src.lateralDirection);
	dst.contactForce=src.contactForce;
	dst.longitudalSlip=src.longitudalSlip;
	dst.lateralSlip=src.lateralSlip;
	dst.longitudalImpulse=src.longitudalImpulse;
	dst.lateralImpulse=src.lateralImpulse;
	dst.otherShapeMaterialIndex=src.otherShapeMaterialIndex;
	dst.contactPosition=src.contactPosition;
}
void PhysXObjectConverter::copyWheelContactData(WheelContactData&dst,const NxWheelContactData&src)
{
	dst.contactPoint=ToVec3(src.contactPoint);
	dst.contactNormal=ToVec3(src.contactNormal);
	dst.longitudalDirection=ToVec3(src.longitudalDirection);
	dst.lateralDirection=ToVec3(src.lateralDirection);
	dst.contactForce=src.contactForce;
	dst.longitudalSlip=src.longitudalSlip;
	dst.lateralSlip=src.lateralSlip;
	dst.longitudalImpulse=src.longitudalImpulse;
	dst.lateralImpulse=src.lateralImpulse;
	dst.otherShapeMaterialIndex=src.otherShapeMaterialIndex;
	dst.contactPosition=src.contactPosition;
}

NxContactPairFlag PhysXObjectConverter::convertContactFlag(EContactReportFlag f)
{
	switch(f)
	{
	case ECRF_StartTouch:
		return NX_NOTIFY_ON_START_TOUCH;
	case ECRF_Touch:
		return NX_NOTIFY_ON_TOUCH;
	case ECRF_EndTouch:
		return NX_NOTIFY_ON_END_TOUCH;
	case ECRF_Impact:
		return NX_NOTIFY_ON_IMPACT;
	case ECRF_Roll:
		return NX_NOTIFY_ON_ROLL;
	case ECRF_Slide:
		return NX_NOTIFY_ON_SLIDE;
	case ECRF_StartTouchForceThreshold:
		return NX_NOTIFY_ON_START_TOUCH_FORCE_THRESHOLD;
	case ECRF_TouchThreshold:
		return NX_NOTIFY_ON_TOUCH_FORCE_THRESHOLD;
	case ECRF_EndTouchThreshold:
		return NX_NOTIFY_ON_END_TOUCH_FORCE_THRESHOLD;
	case ECRF_IgnorePair:
		return NX_IGNORE_PAIR;
	case ECRF_NotifyForces:
		return NX_NOTIFY_FORCES;

	}
	return NX_IGNORE_PAIR;
}

NxCombineMode PhysXObjectConverter::convertCombineMode(EMaterialCombineMode m)
{
	switch(m)
	{
	case ECM_Average: return NX_CM_AVERAGE;
	case ECM_Min: return NX_CM_MIN;
	case ECM_Multiply: return NX_CM_MULTIPLY;
	case ECM_Max: return NX_CM_MAX;
	}
	return NX_CM_AVERAGE;
}
EMaterialCombineMode PhysXObjectConverter::convertCombineMode(NxCombineMode m)
{
	switch(m)
	{
	case NX_CM_AVERAGE: return ECM_Average;
	case NX_CM_MIN: return ECM_Min;
	case NX_CM_MULTIPLY: return ECM_Multiply;
	case NX_CM_MAX: return ECM_Max;
	}
	return ECM_Average;
}

}
}



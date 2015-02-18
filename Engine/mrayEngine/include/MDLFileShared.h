


/********************************************************************
	created:	2012/02/11
	created:	11:2:2012   16:41
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\MDLFileShared.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	MDLFileShared
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __MDLFileShared__
#define __MDLFileShared__

#include "IHardwareIndexBuffer.h"

namespace mray
{
namespace loaders
{

namespace MDLLoaderV0
{
	struct MDLHeader
	{
		int magic;
		int version;
		int objectsCount;
		bool hasSkeleton;
		int totalCycles;
		int totalLocators;
	};

	struct MeshHeader
	{
//		char objectName[64];
// 		math::vector3d position;
// 		math::vector3d rotation;
// 		math::vector3d scale;
//		int vertCount;
//		int uvCount;
//		int triCount;
		int streamsCount;
		bool hasSkin;
		math::box3d boundingBox;
		video::EMeshRenderType renderType;
	};
	struct IndexStreamHeader
	{
		video::IHardwareIndexBuffer::EIndexType indexType;
		uint indexSize;
		uint numIndicies;

		int firstIndex;
		int indexCount;

		video::IHardwareBuffer::EUsageType usage;
		bool useVirtualBuffer;
		bool software;
	};
	struct StreamHeader
	{
		int streamIndex;
		video::EMeshStreamType streamType;
		video::EStreamDataType dataType;
		uint length;
		video::IHardwareBuffer::EUsageType usage;
		bool useVirtualBuffer;
		bool software;
	};

	struct CycleHeader
	{
		math::box3d boundingBox;
		int start;
		int end;
	};
	struct SkeletonHeader
	{
	//	char name[64];
		int rootsCount;
	};
	struct SkinClusterHeader
	{
		int bonesCount;
	};
	struct SkinClusterBoneHeader
	{
		int index;
		int vertices;
	};

	struct JoinHeader
	{
	//	char name[64];
		int index;

		math::vector3d position;
		math::quaternion rotation;
		math::vector3d scale;

		math::vector3d basePosition;
		math::quaternion baseRotation;
		math::vector3d baseScale;
		int childCount;
	};
}

}
}

#endif

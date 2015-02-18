
#include "stdafx.h"
#include "mdlLoaderV0.h"
#include "IStream.h"
#include "SMesh.h"

#include "MeshBufferData.h"
#include "BoneNode.h"

#include "MDLFileShared.h"
#include "IFileSystem.h"
#include "ILogManager.h"
#include "StreamReader.h"


namespace mray
{
namespace loaders
{


const core::string mdlLoaderV0::m_extension(mT("mdl"));
const core::string mdlLoaderV0::m_description(mT("MRAY MDL File Format"));


mdlLoaderV0::mdlLoaderV0()
{
}
mdlLoaderV0::~mdlLoaderV0()
{
}

scene::BoneNode* mdlLoaderV0::LoadBone(scene::Skeleton*skeleton,scene::BoneNode*parent,OS::IStream* stream)
{
	core::string tmpStr;
	OS::StreamReader reader(stream);
	MDLLoaderV0::JoinHeader jointHeader;
	core::stringc str=reader.binReadStringC();
	stream->read(&jointHeader,sizeof(jointHeader));
	core::char_to_string(str.c_str(),tmpStr);
	scene::BoneNodePtr bone=skeleton->createBone(tmpStr,parent);
	bone->setID(jointHeader.index);
	bone->setBasePosition(jointHeader.basePosition);
	bone->setBaseOrintation(jointHeader.baseRotation);
	//bone->setBasePosition(jointHeader.baseScale);
	bone->setPosition(jointHeader.position);
	bone->setOrintation(jointHeader.rotation);
	bone->setScale(jointHeader.scale);
	for(int i=0;i<jointHeader.childCount;++i)
	{
		LoadBone(skeleton,bone,stream);
	}
	return bone;
}
scene::Skeleton* mdlLoaderV0::LoadSkeleton(OS::IStream* stream)
{
	core::string tmpStr;
	OS::StreamReader reader(stream);
	MDLLoaderV0::SkeletonHeader skHeader;
	core::stringc str=reader.binReadStringC();
	stream->read(&skHeader,sizeof(skHeader));
	core::char_to_string(str.c_str(),tmpStr);
	scene::Skeleton* skeleton=new scene::Skeleton(tmpStr);

	for(int i=0;i<skHeader.rootsCount;++i)
	{
		LoadBone(skeleton,0,stream);
	}
	skeleton->refreshRoots();
	return skeleton;
}
scene::SMesh* mdlLoaderV0::load(const  core::string& fileName)
{
	if(!canLoad(fileName))return 0;
	OS::IStreamPtr file=gFileSystem.openFile(fileName,OS::BIN_READ);
	if(!file)return 0;
	return load(file);
}
scene::SMesh* mdlLoaderV0::load(OS::IStream* stream)
{
	if(!stream)
		return 0;
	core::string tmpStr;
	OS::StreamReader reader(stream);

	MDLLoaderV0::MDLHeader header;
	MDLLoaderV0::MeshHeader meshHeader;
	MDLLoaderV0::SkinClusterHeader skinHeader;
	MDLLoaderV0::SkinClusterBoneHeader skinBoneHeader;
	MDLLoaderV0::StreamHeader streamHeader;
	MDLLoaderV0::IndexStreamHeader indexStreamHeader;

	stream->read(&header,sizeof(header));
	if(header.magic!=0x1A1B)
	{
		gLogManager.log(mT("mdlLoaderV0::load() - Incorrect file format"),ELL_WARNING);
		return 0;
	}
	if(header.version!=0x10)
	{
		gLogManager.log(mT("mdlLoaderV0::load() - Incorrect file version"),ELL_WARNING);
		return 0;
	}

	scene::SMesh* mesh=new scene::SMesh();
	if(header.hasSkeleton)
	{
		mesh->setSkeleton(LoadSkeleton(stream));
	}
	for(int buffId=0;buffId<header.objectsCount;++buffId)
	{
		scene::MeshBufferData* buffData=mesh->addNewBuffer();
		scene::IMeshBuffer* buffer=buffData->getMeshBuffer();

		core::stringc str=reader.binReadStringC();
		stream->read(&meshHeader,sizeof(meshHeader));
		core::char_to_string(str.c_str(),tmpStr);
		buffer->setMeshName(tmpStr);
		buffer->setBoundingBox(meshHeader.boundingBox);
		buffer->setRenderType(meshHeader.renderType);

		stream->read(&indexStreamHeader,sizeof(indexStreamHeader));
		indexStreamHeader.software=false;
		buffer->createIndexBuffer(indexStreamHeader.indexType,indexStreamHeader.numIndicies,indexStreamHeader.usage,indexStreamHeader.useVirtualBuffer,indexStreamHeader.software);
		buffer->getIndexData()->firstIndex=indexStreamHeader.firstIndex;
		buffer->getIndexData()->indexCount=indexStreamHeader.indexCount;
		video::IHardwareIndexBuffer* indexBuffer=buffer->getIndexData()->indexBuffer.pointer();
		void*data= indexBuffer->lock(0,0,video::IHardwareBuffer::ELO_Discard);
		stream->read(data,indexBuffer->getSizeInBytes());
		indexBuffer->unlock();

		for(int i=0;i<meshHeader.streamsCount;++i)
		{
			stream->read(&streamHeader,sizeof(streamHeader));
			streamHeader.software=false;
			video::IHardwareStreamBuffer* streamBuff=buffer->createStream(streamHeader.streamIndex,streamHeader.streamType,streamHeader.dataType,streamHeader.length,streamHeader.usage,streamHeader.useVirtualBuffer,streamHeader.software);

			void* data=streamBuff->lock(0,0,video::IHardwareBuffer::ELO_Discard);
			stream->read(data,streamBuff->getSizeInBytes());
			streamBuff->unlock();
		}
		if(meshHeader.hasSkin)
		{
			stream->read(&skinHeader,sizeof(skinHeader));

			scene::ISkinCluster* cluster= buffer->getSkinCluster();
			if(!cluster)
			{
				cluster=new scene::ISkinCluster(mesh->getSkeleton());
				buffer->setSkinCluster(cluster);
			}

			for(int i=0;i<skinHeader.bonesCount;++i)
			{
				stream->read(&skinBoneHeader,sizeof(skinBoneHeader));
				scene::ISkinCluster::SBoneWeights* weights= cluster->getBoneWeights(skinBoneHeader.index);
				weights->vertices.resize(skinBoneHeader.vertices);
				if(skinBoneHeader.vertices>0)
					stream->read(&weights->vertices[0],sizeof(float)*skinBoneHeader.vertices);
			}
		}

	}
	stream->close();
	mesh->calcCurrBoundingBox();
	return mesh;
}
bool mdlLoaderV0::canLoad(const  core::string& fileName)
{
	int i=fileName.findlast('.');
	if(i==-1)
		return false;
	core::string s= fileName.substr(i+1,m_extension.length());
	return s.equals_ignore_case(m_extension);
}
const core::string& mdlLoaderV0::getExtension()
{
	return m_extension;
}


}
}
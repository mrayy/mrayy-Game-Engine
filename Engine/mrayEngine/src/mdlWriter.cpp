
#include "stdafx.h"
#include "mdlWriter.h"
#include "IStream.h"
#include "SMesh.h"

#include "MeshBufferData.h"
#include "BoneNode.h"

#include "MDLFileShared.h"
#include "StreamWriter.h"

namespace mray
{
namespace loaders
{


const core::string mdlWriter::m_extension(mT("mdl"));
const core::string mdlWriter::m_description(mT("MRAY MDL File Format"));


mdlWriter::mdlWriter()
{
}
mdlWriter::~mdlWriter()
{
}

bool mdlWriter::write(scene::SMesh*m,OS::IStream* stream)
{
	if(!stream || !m)
		return false;
	core::stringc tmpStr;
	OS::StreamWriter writer(stream);
	MDLLoaderV0::MDLHeader header;
	MDLLoaderV0::MeshHeader meshHeader;
	MDLLoaderV0::SkinClusterHeader skinHeader;
	MDLLoaderV0::SkinClusterBoneHeader skinBoneHeader;
	MDLLoaderV0::StreamHeader streamHeader;
	MDLLoaderV0::IndexStreamHeader indexStreamHeader;
	header.magic=0x1A1B;
	header.version=0x10;
	header.objectsCount=m->getBuffersCount();
	header.hasSkeleton=!m->getSkeleton().isNull();
	header.totalCycles=0;
	header.totalLocators=0;
	stream->write(&header,sizeof(header));


	//write the skeleton if exists
	if(m->getSkeleton())
	{
		WriteSkeleton(m->getSkeleton().pointer(),stream);
	}

	//write all buffers
	for(int buffId=0;buffId<m->getBuffersCount();++buffId)
	{
		scene::MeshBufferData* buffData= m->getBufferData(buffId);
		scene::IMeshBuffer* buffer= buffData->getMeshBuffer();
		core::string_to_char(buffer->getMeshName(),tmpStr);

		//int len=math::Min<uint>(sizeof(meshHeader.objectName),tmpStr.length());
		//memcpy(meshHeader.objectName,tmpStr.c_str(),len);
		//meshHeader.objectName[len]=0;
		writer.binWriteString(tmpStr);
		video::StreamsDataCRef streams= buffer->getStreamsData();
		video::StreamsMap::const_iterator streamIt=streams->streams.begin();
		meshHeader.streamsCount=streams->streams.size();

		meshHeader.hasSkin=!buffer->getSkinCluster().isNull();
		meshHeader.boundingBox=buffer->getBoundingBox();
		meshHeader.renderType= buffer->getRenderType();

		//write mesh buffer header
		stream->write(&meshHeader,sizeof(meshHeader));

		video::IHardwareIndexBuffer* indexBuffer=buffer->getIndexData()->indexBuffer.pointer();

		indexStreamHeader.firstIndex=buffer->getIndexData()->firstIndex;
		indexStreamHeader.indexCount=buffer->getIndexData()->indexCount;
		indexStreamHeader.indexType=indexBuffer->getIndexType();
		indexStreamHeader.indexSize=indexBuffer->getIndexSize();
		indexStreamHeader.numIndicies=indexBuffer->getIndexCount();
		indexStreamHeader.usage=indexBuffer->getUsageType();
		indexStreamHeader.useVirtualBuffer=indexBuffer->hasVirtualBuffer();
		indexStreamHeader.software=!indexBuffer->isInGPU();

		//write index header
		stream->write(&indexStreamHeader,sizeof(indexStreamHeader));
		
		//write indicies
		void* indicies=indexBuffer->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);
		stream->write(indicies,indexBuffer->getSizeInBytes());
		indexBuffer->unlock();


		for(;streamIt!=streams->streams.end();++streamIt)
		{
			video::IHardwareStreamBuffer* streamBuff=streamIt->second.pointer();
			streamHeader.streamIndex=streamIt->first.index;
			streamHeader.streamType=streamIt->first.type;
			streamHeader.dataType=streamBuff->getElementData();
			streamHeader.length=streamBuff->length();
			streamHeader.usage=streamBuff->getUsageType();
			streamHeader.useVirtualBuffer=streamBuff->hasVirtualBuffer();
			streamHeader.software=!streamBuff->isInGPU();
			//write stream header
			stream->write(&streamHeader,sizeof(streamHeader));

			//write stream data
			void* data=streamBuff->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);
			stream->write(data,streamBuff->getSizeInBytes());
			streamBuff->unlock();
		}

		if(meshHeader.hasSkin)
		{
			scene::ISkinCluster* cluster= buffer->getSkinCluster();
			const scene::ISkinCluster::BoneWeightList& weights=cluster->getBoneWeightList();

			skinHeader.bonesCount=weights.size();
			stream->write(&skinHeader,sizeof(skinHeader));

			scene::ISkinCluster::BoneWeightList::const_iterator it=weights.begin();
			for(;it!=weights.end();++it)
			{
				const scene::ISkinCluster::SBoneWeights& bweights=*it;
				skinBoneHeader.index= bweights.boneIndex;
				skinBoneHeader.vertices= bweights.vertices.size();
				stream->write(&skinBoneHeader,sizeof(skinBoneHeader));
				if(skinBoneHeader.vertices>0)
					stream->write(&bweights.vertices[0],sizeof(float)*skinBoneHeader.vertices);
			}
		}
	}

	stream->close();
	
	return true;
}

void mdlWriter::WriteSkeleton(scene::Skeleton*skeleton,OS::IStream* stream)
{
	core::stringc tmpStr;

	OS::StreamWriter writer(stream);
	MDLLoaderV0::SkeletonHeader skHeader;

	core::string_to_char(skeleton->getResourceName(),tmpStr);
	writer.binWriteString(tmpStr);
	skHeader.rootsCount=skeleton->getRootBonesCount();

	stream->write(&skHeader,sizeof(skHeader));

	for(int i=0;i<skHeader.rootsCount;++i)
	{
		scene::BoneNode* bone=skeleton->getRootBone(i);
		WriteBone(bone,stream);
	}
}
void mdlWriter::WriteBone(scene::BoneNode* bone,OS::IStream* stream)
{
	core::stringc tmpStr;
	OS::StreamWriter writer(stream);

	MDLLoaderV0::JoinHeader jointHeader;

	core::string_to_char(bone->getNodeName(),tmpStr);
	jointHeader.index=bone->getID();
	
	jointHeader.position=bone->getPosition();
	jointHeader.rotation=bone->getOrintation();
	jointHeader.scale=bone->getScale();

	jointHeader.basePosition=bone->getBasePosition();
	jointHeader.baseRotation=bone->getBaseOrintation();
	jointHeader.baseScale=bone->getScale();

	jointHeader.childCount=bone->getSubBones().size();
	writer.binWriteString(tmpStr);
	stream->write(&jointHeader,sizeof(jointHeader));

	const scene::BonesList&bones= bone->getSubBones();
	scene::BonesList::const_iterator it=bones.begin();

	for(;it!=bones.end();++it)
	{
		WriteBone(*it,stream);
	}
}

bool mdlWriter::canWrite(const core::string& fileName)
{
	int i=fileName.findlast('.');
	if(i==-1)
		return false;
	core::string s= fileName.substr(i+1,m_extension.length());
	return s.equals_ignore_case(m_extension);
}
const core::string& mdlWriter::getExt()
{
	return m_extension;
}

const core::string& mdlWriter::getDescription()
{
	return m_description;
}




}
}







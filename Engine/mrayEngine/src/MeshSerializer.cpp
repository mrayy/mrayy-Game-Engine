#include "stdafx.h"

#include "MeshSerializer.h"
#include "SMesh.h"
#include "IStream.h"
#include "BoneNode.h"
#include "SMorphMeshBuffer.h"

#include "StreamWriter.h"
#include "StreamReader.h"

#include "ILogManager.h"
#include "MeshBufferData.h"

namespace mray
{
namespace scene
{

struct __SMeshHeader
{
	char magicNumber[4];
	int version;

	int submeshCount;
	bool hasSkeleton;
};

struct __MeshBufferHeader
{
	int streamsCount;
	video::EMeshRenderType renderType;
	int morphTargetsCount;
	bool skinCluster;

	math::box3d boundingBox;

	int indexCount;
	video::IHardwareIndexBuffer::EIndexType indexType;
	video::IHardwareBuffer::EUsageType indexUsage;
	bool indexUseVirtualBuffer;
	bool indexSoftware;
};

struct __StreamHeader
{
	int index;
	video::EMeshStreamType type;
	video::EStreamDataType dataType;
	int length;
	video::IHardwareBuffer::EUsageType usage;
	bool useVirtualBuffer;
	bool software;
 
};

const char MeshSerializer::HeaderMagicNumber[4]={'M','R','A','Y'};
const int MeshSerializer::CurrentMeshVersion=0x100;

MeshSerializer::MeshSerializer()
{
}
MeshSerializer::~MeshSerializer()
{
}

SMesh* MeshSerializer::Deserialize(OS::IStream*stream)
{
	OS::StreamReader reader(stream);
	__SMeshHeader header;
	reader.read(&header,sizeof(header));
	if(memcmp(header.magicNumber,HeaderMagicNumber,sizeof(HeaderMagicNumber))!=0){
		gLogManager.log(mT("MeshSerializer::Deserialize() - unrecognized file: ")+stream->getStreamName(),ELL_WARNING);
		return 0;
	}
	if(header.version!=CurrentMeshVersion){
		gLogManager.log(mT("MeshSerializer::Deserialize() - not supported version, file: ")+stream->getStreamName(),ELL_WARNING);
		return 0;
	}
	SMesh*mesh=new SMesh();
	if(header.hasSkeleton)
	{
		mesh->setSkeleton(_readSkeleton(&reader));
	}

	for (int i=0;i<header.submeshCount;++i)
	{
		MeshBufferData*data=mesh->addNewBuffer();
		_readBufferData(mesh,data,&reader);
	}
	return mesh;
}
void MeshSerializer::Serialize(SMesh*mesh,OS::IStream*stream)
{
	OS::StreamWriter writer(stream);

	__SMeshHeader header;
	memcpy(header.magicNumber,HeaderMagicNumber,4*sizeof(char));
	header.version=CurrentMeshVersion;
	header.submeshCount=mesh->getBuffersCount();
	header.hasSkeleton=mesh->getSkeleton();

	writer.write(&header,sizeof(header));

	if(header.hasSkeleton)
	{
		_writeSkeleton(mesh->getSkeleton(),&writer);
	}

	for (int i=0;i<mesh->getBuffersCount();++i)
	{
		_writeBufferData(mesh,mesh->getBufferData(i),&writer);
	}
}

//////////////////////////////////////////////////////////////////////////

BoneNode* MeshSerializer::_readBone(Skeleton*sk,OS::StreamReader*reader)
{
	int tmpI=reader->binReadInt();
	BoneNode*bone= new BoneNode(reader->readString(),0,0,0);
	sk->addBone(bone);

	math::vector3d pos;
	math::quaternion ori;
	reader->read(&pos,sizeof(pos));
	reader->read(&ori,sizeof(ori));


	bone->setBasePosition(pos);
	bone->setBaseOrintation(ori);
	bone->setID(tmpI);

	tmpI=reader->binReadInt();
	for(int i=0;i<tmpI;++i)
	{
		bone->addBone(_readBone(sk,reader));
	}

	return bone;
}
Skeleton* MeshSerializer::_readSkeleton(OS::StreamReader*reader)
{
	core::string name=reader->readString();

	Skeleton* ret=new Skeleton(name);
	int bonesCnt=reader->binReadInt();
	for(int i=0;i<bonesCnt;++i)
	{
		_readBone(ret,reader);
	}
	ret->refreshRoots();
	return ret;
}




ISkinCluster* MeshSerializer::_readSkinCluster(SMesh*mesh,OS::StreamReader*reader)
{
	Skeleton*sk= mesh->getSkeleton();
	ISkinCluster*ret=new ISkinCluster(sk);
	int cnt=sk->getBonesCount();
	for (int i=0;i<cnt;++i)
	{
		int index=reader->binReadInt();
		int cnt=reader->binReadInt();

		ISkinCluster::SBoneWeights*weights= ret->getBoneWeights(index);

		weights->vertices.resize(cnt);

		reader->read(&weights->vertices[0],weights->vertices.size()*sizeof(float));
	}
	return ret;
}
void MeshSerializer::_readBufferData(SMesh*mesh,MeshBufferData*buffData,OS::StreamReader*reader)
{
	__MeshBufferHeader header;
	IMeshBuffer*buff=buffData->getMeshBuffer();
	video::IndexData*indicies= buff->getIndexData();


	buff->setMeshName(reader->readString());
	reader->read(&header,sizeof(header));
	

	buff->setBoundingBox(header.boundingBox);
	buff->setRenderType(header.renderType);

	for(int i=0;i<header.streamsCount;++i)
	{
		_readStream(buff,reader);
	}

	video::IHardwareIndexBuffer*indexBuffer=buff->createIndexBuffer(header.indexType,header.indexCount,header.indexUsage,header.indexUseVirtualBuffer,header.indexSoftware);

	void*data=indexBuffer->lock(indicies->firstIndex,0,video::IHardwareBuffer::ELO_Discard);
	reader->read(data,indicies->indexCount*indicies->indexBuffer->getIndexSize());
	indexBuffer->unlock();

	for(int i=0;i<header.morphTargetsCount;++i)
	{
		int streamsCnt;
		reader->read(&streamsCnt,sizeof(streamsCnt));

		for (int i=0;i<streamsCnt;++i)
		{
			SMorphMeshBuffer*morph=new SMorphMeshBuffer(buff);
			_readStream(morph,reader);
		}
	}

	if(header.skinCluster)
	{
		buff->setSkinCluster(_readSkinCluster(mesh,reader));
	}
}
void MeshSerializer::_readStream(IMeshBuffer*buff,OS::StreamReader*reader)
{
	__StreamHeader header;
	reader->read(&header,sizeof(header));

	video::IHardwareStreamBuffer*stream=buff->createStream(header.index,header.type,header.dataType,header.length,header.usage,header.useVirtualBuffer,header.software);

	void*data=stream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	reader->read(data,stream->getSizeInBytes());
	stream->unlock();
}
//////////////////////////////////////////////////////////////////////////
void MeshSerializer::_writeBone(BoneNode*bone,OS::StreamWriter*writer)
{
	writer->binWriteInt(bone->getID());
	writer->writeString(bone->getNodeName());

	math::vector3d pos= bone->getBasePosition();
	math::quaternion ori= bone->getBaseOrintation();
	writer->write(&pos,sizeof(pos));
	writer->write(&ori,sizeof(ori));


	const BonesList &subBones=bone->getSubBones();
	BonesList::const_iterator it=subBones.begin();
	for(;it!=subBones.end();++it)
	{
		_writeBone(*it,writer);
	}
}
void MeshSerializer::_writeSkeleton(Skeleton*sk,OS::StreamWriter*writer)
{
	int bonesCnt=sk->getRootBonesCount();
	writer->writeString(sk->getResourceName());
	writer->binWriteInt(bonesCnt);
	for(int i=0;i<bonesCnt;++i)
	{
		BoneNode*bone= sk->getBone(i);
		_writeBone(bone,writer);
	}
}
void MeshSerializer::_writeBufferData(SMesh*mesh,MeshBufferData*buffData,OS::StreamWriter*writer)
{
	__MeshBufferHeader header;
	IMeshBuffer*buff=buffData->getMeshBuffer();
	video::IndexData*indicies= buff->getIndexData();

	header.renderType=buff->getRenderType();
	header.morphTargetsCount=buff->getMorphTargetCount();
	header.streamsCount=buff->getStreamsCount();
	header.skinCluster=(buff->getSkinCluster().pointer()!=0);
	header.indexCount=indicies->indexCount;
	header.indexType=indicies->indexBuffer->getIndexType();
	header.indexUsage=indicies->indexBuffer->getUsageType();
	header.indexSoftware=(indicies->indexBuffer->isInGPU()==0);
	header.indexUseVirtualBuffer=indicies->indexBuffer->hasVirtualBuffer();
	header.boundingBox=buff->getBoundingBox();

	writer->writeString(buff->getMeshName());
	writer->write(&header,sizeof(header));

	video::StreamsData* streams=buff->getStreamsData();
	video::StreamsMap::iterator streamsIt=streams->streams.begin();

	for (;streamsIt!=streams->streams.end();++streamsIt)
	{
		_writeStream(&(streamsIt->first),streamsIt->second,writer);
	}

	video::IHardwareIndexBuffer*indexBuffer=indicies->indexBuffer;
	void*data=indexBuffer->lock(indicies->firstIndex,0,video::IHardwareBuffer::ELO_ReadOnly);
	writer->write(data,indicies->indexCount*indicies->indexBuffer->getIndexSize());
	indexBuffer->unlock();

	for(int i=0;i<header.morphTargetsCount;++i)
	{
		IMeshBuffer*morph=buff->getMorphTarget(i);
		int streamsCnt=morph->getStreamsCount();
		writer->write(&streamsCnt,sizeof(streamsCnt));

		streams=morph->getStreamsData();
		streamsIt=streams->streams.begin();
		for (;streamsIt!=streams->streams.end();++streamsIt)
		{
			_writeStream(&(streamsIt->first),streamsIt->second,writer);
		}
	}

	if(header.skinCluster)
	{
		_writeSkinCluster(mesh,buff->getSkinCluster(),writer);
	}
}
void MeshSerializer::_writeStream(const video::MeshStreamKey*key,video::IHardwareStreamBuffer*stream,OS::StreamWriter*writer)
{
	__StreamHeader header;
	header.index=key->index;
	header.type=key->type;
	header.dataType=stream->getElementData();
	header.length=stream->length();
	header.software=!stream->isInGPU();
	header.usage=stream->getUsageType();
	header.useVirtualBuffer=stream->hasVirtualBuffer();
	writer->write(&header,sizeof(header));

	void*data=stream->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);
	writer->write(data,stream->getSizeInBytes());
	stream->unlock();
}


void MeshSerializer::_writeSkinCluster(SMesh*mesh,ISkinCluster*c,OS::StreamWriter*writer)
{
	Skeleton*sk= mesh->getSkeleton();
	int cnt=sk->getBonesCount();
	for (int i=0;i<cnt;++i)
	{
		BoneNode*b= sk->getBone(i);
		ISkinCluster::SBoneWeights*weights= c->getBoneWeights(b->getID());
		if(weights){
			writer->binWriteInt(weights->boneIndex);
			writer->binWriteInt(weights->vertices.size());
			writer->write(&weights->vertices[0],weights->vertices.size()*sizeof(float));
		}
	}
}

}
}



#include "stdafx.h"

#include "SMorphMeshBuffer.h"
#include "IHardwareBufferManager.h"
#include "SoftwareStreamBuffer.h"
#include "SoftwareIndexBuffer.h"



namespace mray
{
namespace scene
{

SMorphMeshBuffer::SMorphMeshBuffer(IMeshBuffer* buffer):m_buffer(buffer)
{
	m_streams=new video::StreamsData;
}
SMorphMeshBuffer::~SMorphMeshBuffer()
{
}

uint SMorphMeshBuffer::calcSizeInternal()
{
	uint size=0;

	video::StreamsMap::iterator it= m_streams->streams.begin();
	for(;it!=m_streams->streams.end();++it){
		size+=it->second->getSizeInBytes();
	}
	return size;
}

void SMorphMeshBuffer::setMeshName(const core::string&name)
{
	m_buffer->setMeshName(name);
}
const core::string& SMorphMeshBuffer::getMeshName()
{
	return m_buffer->getMeshName();
}

bool SMorphMeshBuffer::addStream(int index,video::EMeshStreamType type,const video::IHardwareStreamBufferPtr& stream)
{
	video::MeshStreamKey key(index,type);
	video::StreamsMap::iterator it= m_streams->streams.find(key);
	if(it!=m_streams->streams.end())
		return 0; //already exist
	m_streams->streams.insert(video::StreamsMap::value_type(key,stream));
	return true;
}
const video::IHardwareStreamBufferPtr&  SMorphMeshBuffer::createStream(int streamIndex,video::EMeshStreamType type,
																   video::EStreamDataType elemType,int length,video::IHardwareBuffer::EUsageType usage,bool useVirtualBuffer,bool software)
{	
	video::MeshStreamKey key(streamIndex,type);
	video::StreamsMap::iterator it= m_streams->streams.find(key);
	if(it!=m_streams->streams.end()){
		it->second->setElementType(elemType);
		it->second->resize(length);
		return it->second; //already exist
	}
	GCPtr<video::IHardwareStreamBuffer>stream;
	if(software)
		stream=video::IHardwareBufferManager::getInstance().createSoftwareStreamBuffer(elemType,length,usage);
	else
		stream=video::IHardwareBufferManager::getInstance().createHardwareStreamBuffer(elemType,length,usage,useVirtualBuffer);

	m_streams->streams[key]=stream;

	if(m_streams->count==0 || m_streams->count>length)
		m_streams->count=length;

	return m_streams->streams[key];
}
void SMorphMeshBuffer::removeStream(int index,video::EMeshStreamType type)
{
	video::MeshStreamKey key(index,type);
	m_streams->streams.erase(key);
}
const video::IHardwareStreamBufferPtr&  SMorphMeshBuffer::getStream(int index,video::EMeshStreamType type)
{
	video::MeshStreamKey key(index,type);
	video::StreamsMap::iterator it= m_streams->streams.find(key);
	if(it!=m_streams->streams.end())
		return it->second;
	return video::IHardwareStreamBufferPtr::Null;
}
int SMorphMeshBuffer::getStreamsCount()const
{
	return m_streams->streams.size();
}
const video::StreamsDataPtr&  SMorphMeshBuffer::getStreamsData()
{
	return m_streams;
}
/*

void SMorphMeshBuffer::setOrginalUVCount(int cnt)
{
	m_buffer->setOrginalUVCount(cnt);
}
math::vector2d* SMorphMeshBuffer::getOrginalUV()
{
	return m_buffer->getOrginalUV();
}
int SMorphMeshBuffer::getOrginalUVCount()
{
	return m_buffer->getOrginalUVCount();
}
*/

const video::IHardwareIndexBufferPtr&   SMorphMeshBuffer::createIndexBuffer(video::IHardwareIndexBuffer::EIndexType type,
	uint numIndicies,video::IHardwareIndexBuffer::EUsageType usage,bool useVirtualBuffer,bool software)
{
	return m_buffer->createIndexBuffer(type,numIndicies,usage,useVirtualBuffer,software);
}
const video::IndexDataPtr&  SMorphMeshBuffer::getIndexData()
{
	return m_buffer->getIndexData();
}

video::EMeshRenderType SMorphMeshBuffer::getRenderType()
{
	return m_buffer->getRenderType();
}
void SMorphMeshBuffer::setRenderType(video::EMeshRenderType mr)
{
	m_buffer->setRenderType(mr);
}

void SMorphMeshBuffer::setBoundingBox(const math::box3d& b)
{
	BoundingBox=b;
}
const math::box3d& SMorphMeshBuffer::getBoundingBox()const
{
	return  BoundingBox;
}
void SMorphMeshBuffer::recalculateBoundingBox()
{
	video::IHardwareStreamBuffer* stream=getStream(0,video::EMST_Position);
	if(!stream)
		return;
	int count=stream->length();
	if(count==0)
		BoundingBox.reset(0,0,0);
	else
	{
		math::vector3d*verts=(math::vector3d*)stream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);
		BoundingBox.reset(verts[0]);
		for(uint i=0;i<count;++i)
			BoundingBox.AddPoint(verts[i]);
		stream->unlock();
	}
} 

void SMorphMeshBuffer::addMorphTarget(const IMeshBufferPtr& target)
{
}
const IMeshBufferPtr&  SMorphMeshBuffer::getMorphTarget(int i)
{
	return IMeshBufferPtr::Null;
}
int SMorphMeshBuffer::getMorphTargetCount()
{
	return 0;
}

void SMorphMeshBuffer::setSkeletonName(const core::string&name)
{
	m_buffer->setSkeletonName(name);
}
const core::string& SMorphMeshBuffer::getSkeletonName()
{
	return m_buffer->getSkeletonName();
}

void SMorphMeshBuffer::setSkinCluster(const ISkinClusterPtr& cluster)
{
	m_buffer->setSkinCluster(cluster);
}
const ISkinClusterPtr&  SMorphMeshBuffer::getSkinCluster()
{
	return m_buffer->getSkinCluster();
}

IMeshBuffer* SMorphMeshBuffer::getDuplicate()
{
	SMorphMeshBuffer*buffer=new SMorphMeshBuffer(m_buffer);

	buffer->m_streams=m_streams->duplicate();

	return buffer;
}
IMeshBuffer* SMorphMeshBuffer::duplicateContext()
{
	SMorphMeshBuffer*buffer=new SMorphMeshBuffer(m_buffer);

	buffer->m_streams=m_streams;
	return buffer;
}


}
}

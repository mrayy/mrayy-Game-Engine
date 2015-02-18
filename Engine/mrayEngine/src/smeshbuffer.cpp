#include "stdafx.h"

#include "SMeshBuffer.h"
#include "ISkinCluster.h"
#include "IHardwareBufferManager.h"
#include "SoftwareStreamBuffer.h"
#include "SoftwareIndexBuffer.h"
//#include "ShaderResourceManager.h"

namespace mray{
namespace scene{

//////////////////////////////////////////////////////////////////////////

SMeshBuffer::SMeshBuffer()
{
	renderType=video::MR_TRIANGLES;
	/*scale=1;
	position=0;
	rotate=0;
	if(_parent)
		_parent->addChild(this);
*/

	m_indexData=new video::IndexData;
	m_streams=new video::StreamsData;
}


SMeshBuffer::~SMeshBuffer()
{
//	Frames.clear();
// 	orginalUVCoords.clear();
// 	UVTriangles.clear();

	m_streams=0;

	m_indexData=0;
};
void SMeshBuffer::setMeshName(const core::string&name){
	Name=name;
}
const core::string& SMeshBuffer::getMeshName(){
	return Name;
}
const video::IHardwareIndexBufferPtr& SMeshBuffer::createIndexBuffer(video::IHardwareIndexBuffer::EIndexType type,uint numIndicies,
																  video::IHardwareIndexBuffer::EUsageType usage,bool useVirtualBuffer,bool software)
{
	if(software)
		m_indexData->indexBuffer=video::IHardwareBufferManager::getInstance().createSoftwareIndexBuffer(type,numIndicies,usage);
	else
		m_indexData->indexBuffer=video::IHardwareBufferManager::getInstance().createHardwareIndexBuffer(type,numIndicies,usage,useVirtualBuffer);
	m_indexData->indexCount=numIndicies;
	return m_indexData->indexBuffer;
}

const video::IndexDataPtr& SMeshBuffer::getIndexData(){
	return m_indexData;
}

bool SMeshBuffer::addStream(int index,video::EMeshStreamType type,const video::IHardwareStreamBufferPtr& stream){
	video::MeshStreamKey key(index,type);
	video::StreamsMap::iterator it= m_streams->streams.find(key);
	if(it!=m_streams->streams.end())
		return 0; //already exist
	m_streams->streams.insert(video::StreamsMap::value_type(key,stream));

	if(type==video::EMST_Position)
		m_invertedIndex.resize(stream->length());
	return true;
}
const video::IHardwareStreamBufferPtr& SMeshBuffer::createStream(int streamIndex,video::EMeshStreamType type,video::EStreamDataType elemType,
															  int length,video::IHardwareBuffer::EUsageType usage,bool useVirtualBuffer,bool software){
	video::MeshStreamKey key(streamIndex,type);
	video::StreamsMap::iterator it= m_streams->streams.find(key);
	if(it!=m_streams->streams.end()){
		it->second->setElementType(elemType);
		it->second->SetLength(length);
		return it->second; //already exist
	}
	video::IHardwareStreamBufferPtr stream;
	if(software)
		stream=video::IHardwareBufferManager::getInstance().createSoftwareStreamBuffer(elemType,length,usage);
	else
		stream=video::IHardwareBufferManager::getInstance().createHardwareStreamBuffer(elemType,length,usage,useVirtualBuffer);
	
	m_streams->streams[key]=stream;

	if(m_streams->count==0 || m_streams->count>length)
		m_streams->count=length;

	if(type==video::EMST_Position)
		m_invertedIndex.resize(length);

	return m_streams->streams[key];
}
void SMeshBuffer::removeStream(int index,video::EMeshStreamType type){
	video::MeshStreamKey key(index,type);
	m_streams->streams.erase(key);

}
const video::IHardwareStreamBufferPtr& SMeshBuffer::getStream(int index,video::EMeshStreamType type){
	video::MeshStreamKey key(index,type);
	video::StreamsMap::iterator it= m_streams->streams.find(key);
	if(it!=m_streams->streams.end())
		return it->second;
	return video::IHardwareStreamBufferPtr::Null;
}
int SMeshBuffer::getStreamsCount()const{
	return m_streams->streams.size();
}/*

void SMeshBuffer::setOrginalUVCount(int cnt){
	orginalUVCoords.resize(cnt);
}

math::vector2d* SMeshBuffer::getOrginalUV(){
	return orginalUVCoords.pointer();
}
int SMeshBuffer::getOrginalUVCount(){
	return orginalUVCoords.size();
}*/

int* SMeshBuffer::getInvertedIndexPtr()
{
	return &m_invertedIndex[0];
}
int SMeshBuffer::getInvertedIndex(int i)
{
	if(i>=m_invertedIndex.size())
		return i;
	return m_invertedIndex[i];
}

uint SMeshBuffer::calcSizeInternal(){
	uint size=0;

/*
		for(int i=0;i<Frames.size();++i){
			size+=Frames[i].Vertices.size()*sizeof(SFrame::SKey);
		}*/
	

	video::StreamsMap::iterator it= m_streams->streams.begin();
	for(;it!=m_streams->streams.end();++it){
		size+=it->second->getSizeInBytes();
	}
//	size+=orginalUVCoords.size()*sizeof(math::Point2df);
	//size+=UVTriangles.size()*sizeof(math::Point3di);
	if(m_indexData->indexBuffer)
		size+=m_indexData->indexBuffer->getSizeInBytes();

	return size;
}
	

void SMeshBuffer::resetMesh(){
/*	if(Frames.size()==0)
		return ;
	GCPtr<video::IHardwareStreamBuffer> stream= getStream(0,video::EMST_Position);
	if(!stream ||  Frames[0].Vertices.size()!=stream->length() || stream->getElementData()!=video::ESDT_Point3f)
		return;
	math::vector3d *v=(math::vector3d *)stream->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);
	for(int i=0;i<stream->length();++i){
		v[i]=Frames[0].Vertices[i].pos;
	}*/
}


void SMeshBuffer::addMorphTarget(const IMeshBufferPtr& target)
{
	m_morphTargets.push_back(target);
}
const IMeshBufferPtr& SMeshBuffer::getMorphTarget(int i)
{
	if(i>=m_morphTargets.size())
		return IMeshBufferPtr::Null;
	return m_morphTargets[i];
}
int SMeshBuffer::getMorphTargetCount()
{
	return m_morphTargets.size();
}


/*

void SMeshBuffer::setFramesCount(int c){
	Frames.resize(c);
}

void SMeshBuffer::addFrame(const SFrame& frame){
	Frames.push_back(frame);
}
SFrame& SMeshBuffer::getFrame(int idx){
	return Frames[idx];
}
int SMeshBuffer::getFramesCount(){
	return Frames.size();
}*/


math::vector3d SMeshBuffer::getVertexPos(int vertex)
{

	video::IHardwareStreamBuffer* vStream= getStream(0,video::EMST_Position);
	if(vStream && vStream->getElementData()!=video::ESDT_Point3f)
		return 0;
	
	math::vector3d vec=((math::vector3d*)vStream->lock(vertex*sizeof(math::vector3d),sizeof(math::vector3d),video::IHardwareBuffer::ELO_ReadOnly))[0];

	vStream->unlock();
	return vec;
}


void SMeshBuffer::recalculateBoundingBox()
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


IMeshBuffer* SMeshBuffer::getDuplicate(){
	SMeshBuffer *buffer=new SMeshBuffer();

	buffer->m_sCluster=m_sCluster;
	buffer->Name=Name;
	buffer->BoundingBox=BoundingBox;
	/*
	buffer->Frames.resize(Frames.size());
	for(int i=0;i<Frames.size();++i){
		buffer->Frames[i]=Frames[i];
	}*/

	buffer->m_streams=m_streams->duplicate();
// 	video::StreamsMap::iterator it= m_streams.begin();
// 	for(;it!=m_streams.end();++it){
// 		GCPtr<video::IHardwareStreamBuffer> stream= buffer->createStream(it->second->getStreamIndex(),it->second->getType(),
// 			it->second->getElementData(),it->second->length(),it->second->getUsageType(),it->second->hasVirtualBuffer());
// 		stream->copyData(stream,0,0,it->second->getSizeInBytes());
// 	}

	buffer->m_indexData=m_indexData->duplicate();

	//buffer->orginalUVCoords.resize(orginalUVCoords.size());
	//mraySystem::memCopy(buffer->orginalUVCoords.pointer(),orginalUVCoords.pointer(),orginalUVCoords.size()*sizeof(math::Point2df));


	//buffer->UVTriangles.resize(UVTriangles.size());
	//mraySystem::memCopy(buffer->UVTriangles.pointer(),UVTriangles.pointer(),UVTriangles.size()*sizeof(math::Point3di));


	buffer->setSkinCluster(getSkinCluster());

	buffer->renderType=renderType;

	buffer->m_morphTargets.resize(m_morphTargets.size());
	for (int i=0;i<m_morphTargets.size();++i)
	{
		buffer->m_morphTargets[i]=m_morphTargets[i];
	}

	return buffer;
}


IMeshBuffer *SMeshBuffer::duplicateContext(){
	SMeshBuffer *buffer=new SMeshBuffer();

	buffer->Name=Name;
	buffer->m_streams=m_streams;
	buffer->m_indexData=m_indexData;
	buffer->m_sCluster=m_sCluster;
	buffer->BoundingBox=BoundingBox;

	buffer->renderType=renderType;

	buffer->m_morphTargets.resize(m_morphTargets.size());
	for (int i=0;i<m_morphTargets.size();++i)
	{
		buffer->m_morphTargets[i]=m_morphTargets[i];
	}

	return buffer;
}

void SMeshBuffer::setSkinCluster(const ISkinClusterPtr& cluster){
	m_sCluster=cluster;
}
const ISkinClusterPtr& SMeshBuffer::getSkinCluster(){
	return m_sCluster;
}

}
}

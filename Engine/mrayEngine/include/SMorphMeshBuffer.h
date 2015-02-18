

/********************************************************************
	created:	2010/06/04
	created:	4:6:2010   14:15
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\SMorphMeshBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	SMorphMeshBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	contains mesh morph data(vertex,normals), and its attached to the orginal mesh buffer which contains the indicies
*********************************************************************/

#ifndef SMorphMeshBuffer_h__
#define SMorphMeshBuffer_h__

#include "IMeshBuffer.h"

namespace mray
{
namespace scene
{

class SMorphMeshBuffer:public IMeshBuffer
{
private:
protected:
	IMeshBuffer* m_buffer;
	GCPtr<video::StreamsData> m_streams;
	math::box3d BoundingBox;
public:
	SMorphMeshBuffer(IMeshBuffer* buffer);
	virtual ~SMorphMeshBuffer();

	virtual uint calcSizeInternal();

	virtual void setMeshName(const core::string&name);
	virtual const core::string& getMeshName();

	virtual bool addStream(int index,video::EMeshStreamType type,const video::IHardwareStreamBufferPtr& stream);
	virtual const video::IHardwareStreamBufferPtr&  createStream(int streamIndex,video::EMeshStreamType type,
		video::EStreamDataType elemType,int length,video::IHardwareBuffer::EUsageType usage,bool useVirtualBuffer,bool software=false);
	virtual void removeStream(int index,video::EMeshStreamType type);
	virtual const video::IHardwareStreamBufferPtr&  getStream(int index,video::EMeshStreamType type);
	virtual int getStreamsCount()const;
	virtual const video::StreamsDataPtr&  getStreamsData();

// 	virtual void setOrginalUVCount(int cnt);
// 	virtual math::vector2d* getOrginalUV();
// 	virtual int getOrginalUVCount();

	virtual const video::IHardwareIndexBufferPtr&   createIndexBuffer(video::IHardwareIndexBuffer::EIndexType type,
		uint numIndicies,video::IHardwareIndexBuffer::EUsageType usage,bool useVirtualBuffer,bool software=false);
	virtual const video::IndexDataPtr&  getIndexData();

	virtual video::EMeshRenderType getRenderType();
	virtual void setRenderType(video::EMeshRenderType mr);

	virtual void setBoundingBox(const math::box3d& b);
	virtual const math::box3d& getBoundingBox()const;
	virtual void recalculateBoundingBox();

	virtual void addMorphTarget(const IMeshBufferPtr&  target);
	virtual const IMeshBufferPtr&  getMorphTarget(int i);
	virtual int getMorphTargetCount();

	virtual void setSkeletonName(const core::string&name);
	virtual const core::string& getSkeletonName();

	virtual void setSkinCluster(const ISkinClusterPtr& cluster);
	virtual const ISkinClusterPtr&  getSkinCluster();

	virtual IMeshBuffer* getDuplicate();
	virtual IMeshBuffer* duplicateContext();
};

}
}

#endif // SMorphMeshBuffer_h__

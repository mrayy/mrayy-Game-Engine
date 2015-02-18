
/********************************************************************
	created:	2009/02/08
	created:	8:2:2009   18:53
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IMeshBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IMeshBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IMeshBuffer___
#define ___IMeshBuffer___

#include "IHardwareStreamBuffer.h"
#include "IHardwareIndexBuffer.h"

#include "ISkinCluster.h"



#include "videoCommon.h"



namespace mray{
namespace scene{

	MakeSharedPtrType(IMeshBuffer)

class IMeshBuffer
{
public:


	IMeshBuffer(){}
	virtual~IMeshBuffer(){}

	virtual uint calcSizeInternal()=0;

	virtual void setMeshName(const core::string&name)=0;
	virtual const core::string& getMeshName()=0;

	virtual bool addStream(int index,video::EMeshStreamType type,const video::IHardwareStreamBufferPtr& stream)=0;

	virtual const video::IHardwareStreamBufferPtr& createStream(int streamIndex,video::EMeshStreamType type,
		video::EStreamDataType elemType,int length,video::IHardwareBuffer::EUsageType usage,bool useVirtualBuffer,bool software=false)=0;

	virtual void removeStream(int index,video::EMeshStreamType type)=0;

	virtual const video::IHardwareStreamBufferPtr& getStream(int index,video::EMeshStreamType type)=0;
	virtual int getStreamsCount()const=0;
	virtual const video::StreamsDataPtr& getStreamsData()=0;
// 
// 	virtual void setOrginalUVCount(int cnt)=0;
// 	virtual math::vector2d* getOrginalUV()=0;
// 	virtual int getOrginalUVCount()=0;

	virtual const video::IHardwareIndexBufferPtr&  createIndexBuffer(video::IHardwareIndexBuffer::EIndexType type,
		uint numIndicies,video::IHardwareIndexBuffer::EUsageType usage,bool useVirtualBuffer,bool software=false)=0;
	virtual const video::IndexDataPtr& getIndexData()=0;

	virtual video::EMeshRenderType getRenderType()=0;
	virtual void setRenderType(video::EMeshRenderType mr)=0;

	virtual void setBoundingBox(const math::box3d& b)=0;
	virtual const math::box3d& getBoundingBox()const=0;
	virtual void recalculateBoundingBox()=0;

	virtual void addMorphTarget(const IMeshBufferPtr& target)=0;
	virtual const IMeshBufferPtr& getMorphTarget(int i)=0;
	virtual int getMorphTargetCount()=0;

	virtual int* getInvertedIndexPtr(){return 0;}
	virtual int getInvertedIndex(int i){return i;}

/*


	virtual void setFramesCount(int c)=0;
	virtual void addFrame(const SFrame& frame)=0;
	virtual SFrame& getFrame(int idx)=0;
	virtual int getFramesCount()=0;
*/

	virtual void setSkeletonName(const core::string&name)=0;
	virtual const core::string& getSkeletonName()=0;

	virtual void setSkinCluster(const ISkinClusterPtr& cluster)=0;
	virtual const ISkinClusterPtr& getSkinCluster()=0;

	virtual IMeshBuffer* getDuplicate()=0;
	virtual IMeshBuffer* duplicateContext()=0;

};


}
}


#endif //___IMeshBuffer___

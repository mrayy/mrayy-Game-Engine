

#ifndef ___SMESHBUFFER___
#define ___SMESHBUFFER___


#include "mString.h"



#include "macros.h"
//#include "GPUShader.h"
#include "IMeshBuffer.h"

#include "Skeleton.h"

namespace mray{
namespace scene{






class MRAY_DLL SMeshBuffer:public IMeshBuffer
{
protected:
	void resetMesh();

	uint calcSizeInternal();

	std::vector<IMeshBufferPtr> m_morphTargets;

	GCPtr<ISkinCluster> m_sCluster;

	core::string m_skeletonName;
	core::string Name;

	std::vector<int> m_invertedIndex;

	GCPtr<video::IndexData> m_indexData;
	
	GCPtr<video::StreamsData> m_streams;

	video::EMeshRenderType renderType;


public:

	math::box3d	 BoundingBox;

	/*std::vector<math::Point2df>	 orginalUVCoords;*/
	//std::vector<math::Point3di>	 UVTriangles;
public:
	SMeshBuffer();
	virtual~SMeshBuffer();


	virtual void setMeshName(const core::string&name);
	virtual const core::string& getMeshName();

	virtual void setSkeletonName(const core::string&name){m_skeletonName=name;}
	virtual const core::string& getSkeletonName(){return m_skeletonName;}

	bool addStream(int index,video::EMeshStreamType type,const video::IHardwareStreamBufferPtr& stream);
	const video::IHardwareStreamBufferPtr& createStream(int streamIndex,video::EMeshStreamType type,
		video::EStreamDataType elemType,int length,video::IHardwareBuffer::EUsageType usage,bool useVirtualBuffer,bool software=false);
	void removeStream(int index,video::EMeshStreamType type);
	const video::IHardwareStreamBufferPtr& getStream(int index,video::EMeshStreamType type);

	virtual const video::StreamsDataPtr& getStreamsData(){return m_streams;}

	int getStreamsCount()const;
// 
// 	virtual void setOrginalUVCount(int cnt);
// 	virtual math::vector2d* getOrginalUV();
// 	virtual int getOrginalUVCount();

	virtual int* getInvertedIndexPtr();
	virtual int getInvertedIndex(int i);

	virtual const video::IHardwareIndexBufferPtr& createIndexBuffer(video::IHardwareIndexBuffer::EIndexType type,uint numIndicies,
		video::IHardwareIndexBuffer::EUsageType usage,bool useVirtualBuffer,bool software=false);
	virtual const video::IndexDataPtr& getIndexData();
/*

	virtual void setFramesCount(int c);
	virtual void addFrame(const SFrame& frame);
	virtual SFrame& getFrame(int idx);
	virtual int getFramesCount();*/



	video::EMeshRenderType getRenderType(){return renderType;}
	void setRenderType(video::EMeshRenderType mr){renderType=mr;}
 
	const math::box3d& getBoundingBox()const{return BoundingBox;}
	void setBoundingBox(const math::box3d& b){BoundingBox=b;}


	//this function will calc The Bounding Box for curr frame
	//it's good for some operations at each time..but it's expensive to call it
	//much
	void recalculateBoundingBox();

	math::vector3d getVertexPos(int vertex);
	

	virtual void addMorphTarget(const IMeshBufferPtr& target);
	virtual const IMeshBufferPtr& getMorphTarget(int i);
	virtual int getMorphTargetCount();



	virtual void setSkinCluster(const ISkinClusterPtr& cluster);
	virtual const ISkinClusterPtr& getSkinCluster();


	IMeshBuffer* getDuplicate();
	IMeshBuffer* duplicateContext();
};

};//scene
};//mray

#endif














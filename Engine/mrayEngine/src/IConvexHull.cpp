#include "stdafx.h"


#include "IConvexHull.h"
#include "SMeshBuffer.h"



namespace mray{
namespace scene{


void IConvexHull::copyMesh(GCPtr<scene::SMesh>mesh){
	convexMesh=new  SMeshBuffer();
	int vtxSizeCnt=0;
	for(int b=0;b<mesh->getBuffersCount();b++){
		GCPtr<SMeshBuffer>buff=mesh->getBuffer(b);
		GCPtr<video::IHardwareStreamBuffer>v1Stream=buff->getStream(0,video::EMST_Position);
		if(!v1Stream)
			continue;
		vtxSizeCnt+=v1Stream->getElementSize();
	}
	GCPtr<video::IHardwareStreamBuffer>vStream=convexMesh->createStream(0,video::EMST_Position,video::ESDT_Point3f,vtxSizeCnt,
		video::IHardwareBuffer::EUT_Static,true);

	int currV=0;

	math::vector3d*vtx2=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	for(int b=0;b<mesh->getBuffersCount();b++){
		GCPtr<SMeshBuffer>buff=mesh->getBuffer(b);
		GCPtr<video::IHardwareStreamBuffer>v1Stream=buff->getStream(0,video::EMST_Position);
		if(!v1Stream)
			continue;
		int vtxCnt=v1Stream->length();
		math::vector3d*vtx=(math::vector3d*)v1Stream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);
		for(int i=0;i<vtxCnt;++i){
			vtx2[currV++]=vtx[i];
		}
		v1Stream->unlock();
	}
	vStream->unlock();
}

}
}



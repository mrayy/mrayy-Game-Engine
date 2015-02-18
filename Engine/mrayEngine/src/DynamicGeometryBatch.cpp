#include "stdafx.h"

#include "DynamicGeometryBatch.h"
#include "MeshRenderableNode.h"
#include "ISceneNode.h"
#include "MeshBufferData.h"


namespace mray{
namespace scene{

DynamicGeometryBatch::DynamicGeometryBatch(){
	m_mesh=new SMesh();
	m_iCnt=0;
	m_vCnt=0;

	g_max16bit=0xFFFF;
	g_max32bit=0xFFFFFFFF;
}

DynamicGeometryBatch::~DynamicGeometryBatch(){
}


bool DynamicGeometryBatch::addNode(MeshRenderableNode* node){
	SMesh* m= node->getMesh();
	return addNode(m,node->getOwner());
}

bool DynamicGeometryBatch::addNode(SMesh* meshBuffer,IMovable* node)
{
	for(int i=0;i<meshBuffer->getBuffersCount();i++){
		IMeshBuffer* buff=meshBuffer->getBuffer(i);
		uint iCnt=buff->getIndexData()->indexCount;
		if((long)(m_iCnt+iCnt)>g_max32bit)
			return false;
		m_vCnt+=buff->getStreamsData()->count;
		m_iCnt+=iCnt;
	}

	return IGeometryBatch::addNode(meshBuffer,node);;
}



uint DynamicGeometryBatch::build(){
	if(!m_vCnt || !m_iCnt)
		return 0;

	video::IHardwareIndexBuffer::EIndexType iType;
	//check for 16/32 bit index buffer
	bool b16=0;
	if(m_iCnt>=g_max16bit)
		iType=video::IHardwareIndexBuffer::EIT_32Bit;
	else {
		iType=video::IHardwareIndexBuffer::EIT_16Bit;
		b16=true;
	}

	m_lastTrans.clear();
	m_mesh->clear();

	MeshBufferData*bdata= m_mesh->addNewBuffer();
	IMeshBuffer* buffer=bdata->getMeshBuffer();

	buffer->createStream(0,video::EMST_Position,video::ESDT_Point3f,m_vCnt,video::IHardwareBuffer::EUT_DynamicWriteOnly,0,0);
	buffer->createStream(0,video::EMST_Normal,video::ESDT_Point3f,m_vCnt,video::IHardwareBuffer::EUT_DynamicWriteOnly,0,0);
	buffer->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,m_vCnt,video::IHardwareBuffer::EUT_DynamicWriteOnly,0,0);
	buffer->createIndexBuffer(iType,m_iCnt,video::IHardwareIndexBuffer::EUT_DynamicWriteOnly,0);

	union{
		ushort*i16;
		uint*i32;
	}index,sindex;
	video::IHardwareIndexBuffer* idxBuff=buffer->getIndexData()->indexBuffer;
	void*idx=idxBuff->lock(0,0,video::IHardwareBuffer::ELO_Discard);

	if(b16)index.i16=(ushort*)idx;
	else index.i32=(uint*)idx;
	int offset=0;
	int m=0;

	std::list<InstanceData>::iterator it=m_instances.begin();
	for (;it!=m_instances.end();++it)
	{

		m_lastTrans.push_back(math::matrix4x4::Identity);
		SMesh* mesh=it->meshBuffer;
		for(int i=0;i<mesh->getBuffersCount();i++){
			MeshBufferData* bd= mesh->getBufferData(i);
			IMeshAnimator* anim=bd->getAnimator();
			IMeshBuffer* buffer;
			if(anim){
				buffer=anim->getMeshBuffer();
			}else{
				buffer=bd->getMeshBuffer();
			}

			video::IHardwareIndexBuffer* sidxBuff=buffer->getIndexData()->indexBuffer;
			void*sidx=sidxBuff->lock(0,0,video::IHardwareBuffer::ELO_Discard);
			uint svCnt=buffer->getStreamsData()->count;

			bool sb16=0;
			if(sidxBuff->getIndexType()== video::IHardwareIndexBuffer::EIT_16Bit)
			{
				sb16=true;
				sindex.i16=(ushort*)sidx;
			}else sindex.i32=(uint*)sidx;

			uint sIStart=buffer->getIndexData()->firstIndex;
			uint sICnt=buffer->getIndexData()->indexCount;
			for(int k=sIStart;k<sICnt;k++){
				if(b16){
					if(sb16)
						index.i16[m]=sindex.i16[k]+offset;
					else
						index.i16[m]=sindex.i32[k]+offset;
				}else{
					if(sb16)
						index.i32[m]=sindex.i16[k]+offset;
					else
						index.i32[m]=sindex.i32[k]+offset;
				}
				m++;
			}
			offset+=svCnt;
			sidxBuff->unlock();
		}
	}


	idxBuff->unlock();
	return 1;
}

void DynamicGeometryBatch::buildMesh(IMeshBuffer* b){
}

void DynamicGeometryBatch::update(){


	IMeshBuffer* buffer=m_mesh->getBuffer(0);
	if(!buffer)
		return;

	video::IHardwareStreamBuffer* vstream=buffer->getStream(0,video::EMST_Position);
	video::IHardwareStreamBuffer* nstream=buffer->getStream(0,video::EMST_Normal);

	math::vector3d*vtx=(math::vector3d*)vstream->lock(0,0,video::IHardwareBuffer::ELO_Normal);
	math::vector3d*norm=(math::vector3d*)nstream->lock(0,0,video::IHardwareBuffer::ELO_Normal);

	int n=0;
	math::box3d bbox;
	bool first=true;

	std::list<math::matrix4x4>::iterator tIt=m_lastTrans.begin();
	std::list<InstanceData>::iterator it=m_instances.begin();
	for (;it!=m_instances.end();++it,++tIt)
	{
		SMesh* mesh=it->meshBuffer;
		const math::matrix4x4&worldMat=it->node->getAbsoluteTransformation();
		bool changed=false;
		const float*m1=worldMat.getMatPointer();
		const float*m2=(*tIt).getMatPointer();
		for(int i=0;i<16;++i){
			if(m1[i]!=m2[i])
			{
				changed=true;
				break;
			}
		}
		if(!changed)
			continue;
		*tIt=worldMat;


		for(int i=0;i<mesh->getBuffersCount();i++){
			MeshBufferData* bd= mesh->getBufferData(i);
			IMeshAnimator* anim=bd->getAnimator();
			IMeshBuffer* buffer;
			if(anim){
				buffer=anim->getMeshBuffer();
			}else{
				buffer=bd->getMeshBuffer();
			}

			video::IHardwareStreamBuffer* svstream=buffer->getStream(0,video::EMST_Position);
			math::vector3d*svtx=(math::vector3d*)svstream->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);

			video::IHardwareStreamBuffer* snstream=buffer->getStream(0,video::EMST_Normal);
			math::vector3d*snorm=(math::vector3d*)snstream->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);


			uint svStart=buffer->getStreamsData()->start;
			uint svCnt=buffer->getStreamsData()->count;

			for(int k=svStart;k<svCnt;k++){

				vtx[n]=worldMat*(svtx[k]);
				norm[n]=worldMat.transformNormal(snorm[k]);

				if(first){
					first=false;
					bbox.reset(vtx[n]);
				}else
					bbox.AddPoint(vtx[n]);
				n++;
			}

			svstream->unlock();
			snstream->unlock();
		}
	}

	vstream->unlock();
	nstream->unlock();

	buffer->setBoundingBox(bbox);
	m_mesh->calcCurrBoundingBox();

}

}
}

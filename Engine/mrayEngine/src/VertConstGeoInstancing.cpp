#include "stdafx.h"

#include "VertConstGeoInstancing.h"
#include "MeshRenderableNode.h"
#include "ISceneNode.h"
#include "MeshBufferData.h"


namespace mray{
namespace scene{

VertConstGeoInstancing::VertConstGeoInstancing(int streamIndex,bool limitTo16Bit){
	m_mesh=new SMesh();
	m_streamIndex=streamIndex;
	m_lastIDAssigned=0;

	g_max16bit=0xFFFF;
	g_max32bit=0xFFFFFFFF;

	m_limitTo16Bit=limitTo16Bit;
}

VertConstGeoInstancing::~VertConstGeoInstancing()
{
	delete m_mesh;
}


bool VertConstGeoInstancing::addNode(MeshRenderableNode*node){
	if( !node)
		return false;
	SMesh* m= node->getMesh();
	return addNode(m,node->getOwner());
}

bool VertConstGeoInstancing::addNode(SMesh* meshBuffer,IMovable* node)
{
	if(!meshBuffer || !node)
		return false;
	for(int i=0;i<meshBuffer->getBuffersCount();i++){
		GeometryInstance inst;
		MeshBufferData* data=meshBuffer->getBufferData(i);
		video::RenderMaterial*mtrl= data->getMaterial();
		DynamicInstanceList::iterator it= m_internalInstances.find(mtrl);
		GeometryCollection* collection=0;
		if(it==m_internalInstances.end())
		{
			m_internalInstances[mtrl].mtrl=mtrl;
			collection=&m_internalInstances[mtrl];
		}else
		{
			collection=&(it->second);
		}
		IMeshBuffer* buff=data->getMeshBuffer();

		uint iCnt=buff->getIndexData()->indexCount;
		if(m_limitTo16Bit && collection->m_iCnt+iCnt>g_max16bit)
			continue;
		if((long)(collection->m_iCnt+iCnt)>g_max32bit)
			continue;
		collection->m_vCnt+=buff->getStreamsData()->count;
		collection->m_iCnt+=iCnt;
		inst.buffer=buff;
		inst.worldMat=node->getAbsoluteTransformation();
		inst.id=m_lastIDAssigned++;
		collection->instances.push_back(inst);
	}

	return IGeometryBatch::addNode(meshBuffer,node);
}


uint VertConstGeoInstancing::build(){
	DynamicInstanceList::iterator it= m_internalInstances.begin();
	for (;it!=m_internalInstances.end();++it)
	{
		GeometryCollection* geometry=&(it->second);
		video::IHardwareIndexBuffer::EIndexType iType;
		//check for 16/32 bit index buffer
		bool b16=0;
		if(geometry->m_iCnt>=g_max16bit)
			iType=video::IHardwareIndexBuffer::EIT_32Bit;
		else {
			iType=video::IHardwareIndexBuffer::EIT_16Bit;
			b16=true;
		}

		MeshBufferData*bdata= m_mesh->addNewBuffer();
		bdata->setMaterial(it->first);
		IMeshBuffer* buffer=bdata->getMeshBuffer();
		geometry->buff=buffer;

		buffer->createStream(0,video::EMST_Position,video::ESDT_Point3f,geometry->m_vCnt,video::IHardwareBuffer::EUT_StaticWriteOnly,0,0);
		buffer->createStream(0,video::EMST_Normal,video::ESDT_Point3f,geometry->m_vCnt,video::IHardwareBuffer::EUT_StaticWriteOnly,0,0);
		buffer->createStream(0,video::EMST_Texcoord,video::ESDT_Point3f,geometry->m_vCnt,video::IHardwareBuffer::EUT_StaticWriteOnly,0,0);
		//buffer->createStream(m_streamIndex,video::EMST_Texcoord,video::ESDT_Point1f,geometry->m_vCnt,video::IHardwareBuffer::EUT_StaticWriteOnly,0,0);
		buffer->createIndexBuffer(iType,geometry->m_iCnt,video::IHardwareIndexBuffer::EUT_DynamicWriteOnly,0);

		video::IHardwareStreamBuffer* vstream=buffer->getStream(0,video::EMST_Position);
		video::IHardwareStreamBuffer* nstream=buffer->getStream(0,video::EMST_Normal);
		video::IHardwareStreamBuffer* tcstream=buffer->getStream(0,video::EMST_Texcoord);
		//video::IHardwareStreamBuffer* idstream=buffer->getStream(m_streamIndex,video::EMST_Texcoord);

		math::vector3d*vtx=(math::vector3d*)vstream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
		math::vector3d*norm=(math::vector3d*)nstream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
		math::vector3d*tc=(math::vector3d*)tcstream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
		//float*ids=(float*)idstream->lock(0,0,video::IHardwareBuffer::ELO_Discard);

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
		int n=0;
		int obj=0;
		math::box3d bbox;
		bool first=true;

		m_mesh->GetWorldMatricies().resize(m_instances.size());

		InstanceList::iterator it=m_instances.begin();
		for (;it!=m_instances.end();++it,++obj)
		{
			IMovable* node=it->node;
			SMesh* mesh=it->meshBuffer;
			const math::matrix4x4&worldMat=node->getAbsoluteTransformation();

			m_mesh->GetWorldMatricies()[obj]=worldMat;

			for(int i=0;i<mesh->getBuffersCount();i++){
				MeshBufferData* bd= mesh->getBufferData(i);
				IMeshBuffer* buffer;
				buffer=bd->getMeshBuffer();

				video::IHardwareStreamBuffer* svstream=buffer->getStream(0,video::EMST_Position);
				math::vector3d*svtx=(math::vector3d*)svstream->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);

				video::IHardwareStreamBuffer* snstream=buffer->getStream(0,video::EMST_Normal);
				math::vector3d*snorm=(math::vector3d*)snstream->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);

				video::IHardwareStreamBuffer* stcstream=buffer->getStream(0,video::EMST_Texcoord);
				math::vector2d*stc=(math::vector2d*)stcstream->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);

				video::IHardwareIndexBuffer* sidxBuff=buffer->getIndexData()->indexBuffer;
				void*sidx=sidxBuff->lock(0,0,video::IHardwareBuffer::ELO_Discard);

				bool sb16=0;
				if(sidxBuff->getIndexType()== video::IHardwareIndexBuffer::EIT_16Bit)
				{
					sb16=true;
					sindex.i16=(ushort*)sidx;
				}else sindex.i32=(uint*)sidx;

				uint sIStart=buffer->getIndexData()->firstIndex;
				uint sICnt=buffer->getIndexData()->indexCount;
				uint svStart=buffer->getStreamsData()->start;
				uint svCnt=buffer->getStreamsData()->count;

				for(int k=svStart;k<svCnt;k++){

					vtx[n]=svtx[k];
					norm[n]=snorm[k];
					tc[n].set(stc[k].x,stc[k].y,obj);
					//ids[n]=obj;

					if(first){
						first=false;
						bbox.reset(vtx[n]);
					}else
						bbox.AddPoint(vtx[n]);

					n++;
				}


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
				svstream->unlock();
				snstream->unlock();
				stcstream->unlock();
			}
		}

		buffer->setBoundingBox(bbox);
		vstream->unlock();
		nstream->unlock();
		tcstream->unlock();
	//	idstream->unlock();
		idxBuff->unlock();
	}
	return 1;
}

std::vector<math::matrix4x4> &VertConstGeoInstancing::getMatricies(){return m_mesh->GetWorldMatricies();}
void VertConstGeoInstancing::update()
{

	{
		InstanceList::iterator it=m_instances.begin();
		for (int obj=0;it!=m_instances.end();++it,++obj)
		{
			IMovable* node=it->node;
			///SMesh* mesh=node->getMesh();
			const math::matrix4x4&worldMat=node->getAbsoluteTransformation();

			m_mesh->GetWorldMatricies()[obj]=worldMat;
		}
	}
	{
		DynamicInstanceList::iterator it= m_internalInstances.begin();
		for(;it!= m_internalInstances.end();++it)
		{
			std::list<GeometryInstance>::iterator instIt= it->second.instances.begin();
			std::list<GeometryInstance>::iterator instEnd= it->second.instances.end();
			bool first=true;
			math::box3d bbox;
			for(;instIt!=instEnd;++instIt)
			{
				math::box3d b=instIt->buffer->getBoundingBox();
				m_mesh->GetWorldMatricies()[instIt->id].transformBox(b);
				if(first)
				{
					bbox=b;
					first=false;
				}else
				{
					bbox.AddBox(b);
				}
			}
			it->second.buff->setBoundingBox(bbox);
		}
		m_mesh->calcCurrBoundingBox();
	}

}

}
}

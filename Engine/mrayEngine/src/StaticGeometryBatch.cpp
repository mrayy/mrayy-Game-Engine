#include "stdafx.h"

#include "StaticGeometryBatch.h"
#include "MeshRenderableNode.h"
#include "ISceneNode.h"
#include "MeshBufferData.h"


namespace mray{
namespace scene{


StaticGeometryBatch::StaticGeometryBatch(bool limitTo16Bit){
	g_max16bit=0xFFFF;
	g_max32bit=0xFFFFFFFF;

	m_limitTo16Bit=limitTo16Bit;

	m_mesh=new SMesh();

	m_streamsToCopy.push_back(MeshStreamInfo(video::EMST_Position));
	m_streamsToCopy.push_back(MeshStreamInfo(video::EMST_Normal));
	m_streamsToCopy.push_back(MeshStreamInfo(video::EMST_Texcoord,0));
}
StaticGeometryBatch::StaticGeometryBatch(const std::list<MeshStreamInfo>&streamsToCopy,bool limitTo16Bit){
	g_max16bit=0xFFFF;
	g_max32bit=0xFFFFFFFF;

	m_limitTo16Bit=limitTo16Bit;

	m_mesh=new SMesh();

	m_streamsToCopy=streamsToCopy;
}
StaticGeometryBatch::~StaticGeometryBatch()
{
	delete m_mesh;
}

scene::SMesh* StaticGeometryBatch::getMesh()
{
	return m_mesh;
}

bool StaticGeometryBatch::addNode(MeshRenderableNode* node){
	SMesh* m= node->getMesh();
	return addNode(m,node->getOwner());
}

bool StaticGeometryBatch::addNode(SMesh* meshBuffer,IMovable* node)
{
	return addNode(meshBuffer,node->getAbsoluteTransformation());
}

bool StaticGeometryBatch::addNode(SMesh* meshBuffer,const math::matrix4x4& transform)
{
	SMesh* m= meshBuffer;
	for(int i=0;i<m->getBuffersCount();i++){
		MeshBufferData* data=m->getBufferData(i);
		video::RenderMaterial*mtrl= data->getMaterial();
		addNode(data->getMeshBuffer(),mtrl,transform);
	}

	return true;
}

bool StaticGeometryBatch::addNode(IMeshBuffer* buff ,video::RenderMaterial*mtrl,const math::matrix4x4& transform)
{

	StaticInstanceList::iterator it= m_internalInstances.find(mtrl);
	GeometryCollection* collection=0;
	if(it==m_internalInstances.end())
	{
		m_internalInstances[mtrl].mtrl=mtrl;
		collection=&m_internalInstances[mtrl];
	}else
	{
		collection=&(it->second);
	}

	uint iCnt=buff->getIndexData()->indexCount;
	if(m_limitTo16Bit && collection->m_iCnt+iCnt>g_max16bit)
		return false;
	if((long)(collection->m_iCnt+iCnt)>g_max32bit)
		return false;
	collection->m_vCnt+=buff->getStreamsData()->count;
	collection->m_iCnt+=iCnt;

	GeometryInstance inst;
	inst.buffer=buff;
	inst.worldMat=transform;
	collection->instances.push_back(inst);
	return true;
}
void StaticGeometryBatch::clearAll()
{
	m_mesh->clear();
	m_internalInstances.clear();
	IGeometryBatch::clearAll();
}

uint StaticGeometryBatch::build(){

	m_mesh->clear();
	StaticInstanceList::iterator mit= m_internalInstances.begin();
	for (;mit!=m_internalInstances.end();++mit)
	{
		video::IHardwareIndexBuffer::EIndexType iType;
		union{
			ushort*i16;
			uint*i32;
		}index,sindex;

		GeometryCollection* geometry=&(mit->second);

		//check for 16/32 bit index buffer
		bool b16=0;
		if(geometry->m_iCnt>=g_max16bit)
			iType=video::IHardwareIndexBuffer::EIT_32Bit;
		else {
			iType=video::IHardwareIndexBuffer::EIT_16Bit;
			b16=true;
		}


		MeshBufferData*bdata= m_mesh->addNewBuffer();
		bdata->setMaterial(mit->first);
		IMeshBuffer* buffer=bdata->getMeshBuffer();



		std::list<GeometryInstance>::iterator it=geometry->instances.begin();
		if(it==geometry->instances.end())
			continue;

		std::list<MeshStreamInfo>::iterator sit=m_streamsToCopy.begin();
		for(;sit!=m_streamsToCopy.end();++sit)
		{
			video::IHardwareStreamBuffer*stream= it->buffer->getStream(sit->index,sit->stream);
			sit->__usedStream=0;
			sit->__dataPtr=0;
			if(!stream)
				continue;
			sit->__usedStream=buffer->createStream(sit->index,sit->stream,stream->getElementData(),
				geometry->m_vCnt,video::IHardwareBuffer::EUT_Static,0,0);
			sit->__dataPtr=sit->__usedStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
			if(sit->stream==video::EMST_Position || sit->stream==video::EMST_Normal)
				sit->__onlyCopy=false;
		}
/*
		math::vector3d*vtx=(math::vector3d*)vstream->lock(0,0,video::IHardwareBuffer::ELO_Discard);

		video::IHardwareStreamBuffer* nstream=buffer->createStream(0,video::EMST_Normal,video::ESDT_Point3f,geometry->m_vCnt,video::IHardwareBuffer::EUT_Static,0,0);
		math::vector3d*norm=(math::vector3d*)nstream->lock(0,0,video::IHardwareBuffer::ELO_Discard);

		video::IHardwareStreamBuffer* tcstream=buffer->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,geometry->m_vCnt,video::IHardwareBuffer::EUT_Static,0,0);
		math::vector2d*tc=(math::vector2d*)tcstream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
*/
		video::IHardwareIndexBuffer* idxBuff=buffer->createIndexBuffer(iType,geometry->m_iCnt,video::IHardwareIndexBuffer::EUT_StaticWriteOnly,0);
		void*idx=idxBuff->lock(0,0,video::IHardwareBuffer::ELO_Discard);


		if(b16)index.i16=(ushort*)idx;
		else index.i32=(uint*)idx;

		int m=0;
		int n=0;
		int offset=0;

		math::box3d bbox;
		bool first=true;
		for (;it!=geometry->instances.end();++it)
		{
			GeometryInstance&inst=*it;
			sit=m_streamsToCopy.begin();
			bool needTransform=false;
			for(;sit!=m_streamsToCopy.end();++sit)
			{
				sit->__srcStream=inst.buffer->getStream(sit->index,sit->stream);
				sit->__srcPtr=0;
				if(!sit->__srcStream)
					continue;
				sit->__srcPtr=sit->__srcStream->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);
				if(sit->__onlyCopy==false)
					needTransform=true;
				sit->__dataSize=sit->__srcStream->getElementSize();
			}
			/*
			video::IHardwareStreamBuffer* svstream=inst.buffer->getStream(0,video::EMST_Position);
			math::vector3d*svtx=(math::vector3d*)svstream->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);

			video::IHardwareStreamBuffer* snstream=inst.buffer->getStream(0,video::EMST_Normal);
			math::vector3d*snorm=(math::vector3d*)snstream->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);

			video::IHardwareStreamBuffer* stcstream=inst.buffer->getStream(0,video::EMST_Texcoord);
			math::vector2d*stc=(math::vector2d*)stcstream->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);
*/
			video::IHardwareIndexBuffer* sidxBuff=inst.buffer->getIndexData()->indexBuffer;
			void*sidx=sidxBuff->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);

			bool sb16=0;
			if(sidxBuff->getIndexType()== video::IHardwareIndexBuffer::EIT_16Bit)
			{
				sb16=true;
				sindex.i16=(ushort*)sidx;
			}else sindex.i32=(uint*)sidx;

			uint svStart=inst.buffer->getStreamsData()->start;
			uint svCnt=inst.buffer->getStreamsData()->count;
			uint sIStart=inst.buffer->getIndexData()->firstIndex;
			uint sICnt=inst.buffer->getIndexData()->indexCount;

			if(needTransform){
				for(int k=svStart;k<svCnt;k++){

					sit=m_streamsToCopy.begin();
					for(;sit!=m_streamsToCopy.end();++sit)
					{
						MeshStreamInfo&ifo=*sit;
						if(!ifo.__onlyCopy){
							if(ifo.stream==video::EMST_Position){
								math::vector3d*vtx=((math::vector3d*)ifo.__dataPtr);
								vtx[n]=inst.worldMat*(((math::vector3d*)ifo.__srcPtr)[k]);
								if(first){
									first=false;
									bbox.reset(vtx[n]);
								}else
									bbox.AddPoint(vtx[n]);
							}else //if(ifo.stream==video::EMST_Normal)
							{
								((math::vector3d*)ifo.__dataPtr)[n]=inst.worldMat.transformNormal(((math::vector3d*)ifo.__srcPtr)[k]);
							}
						}else
						{
							memcpy(((uchar*)ifo.__dataPtr)+ifo.__dataSize*n,((uchar*)ifo.__srcPtr)+ifo.__dataSize*k,ifo.__dataSize);
						}
					}
					n++;
				}
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

			sit=m_streamsToCopy.begin();
			for(;sit!=m_streamsToCopy.end();++sit)
			{
				if(sit->__srcStream)
					sit->__srcStream->unlock();
			}
			sidxBuff->unlock();
		}
		sit=m_streamsToCopy.begin();
		for(;sit!=m_streamsToCopy.end();++sit)
		{
			if(sit->__usedStream)
				sit->__usedStream->unlock();
		}
		idxBuff->unlock();

		buffer->setBoundingBox(bbox);
	}
	m_mesh->calcCurrBoundingBox();

	return true;
}

void StaticGeometryBatch::update(){
}

}
}



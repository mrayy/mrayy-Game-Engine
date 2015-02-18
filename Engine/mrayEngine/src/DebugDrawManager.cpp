#include "stdafx.h"

#include "DebugDrawManager.h"
#include "IVideoDevice.h"
#include "SMesh.h"
#include "ThreadBarrier.h"
#include "JobPool.h"
#include "mraySystem.h"
#include "IMutex.h"
#include "IThreadManager.h"
#include "DefaultColors.h"
#include "FPSCalc.h"
#include "MeshGenerator.h"
#include "MeshBufferData.h"
#include "MutexLocks.h"
#include "ThreadJobManager.h"
#include "IJobPiece.h"

namespace mray
{
namespace scene
{

	class DebugDrawManagerBuildJob:public IJobPiece
	{
		DebugDrawManager* m_manager;
	public:
		DebugDrawManagerBuildJob(DebugDrawManager*mngr):m_manager(mngr)
		{
		}
		bool Execute()
		{
			m_manager->_BuildDrawingLists();

			return true;
		}
	};

DebugDrawManager::DebugDrawManager(video::IVideoDevice* device):m_device(device),m_material(0)
{
	m_camera=0;
	m_mesh=new SMesh();
	for(int i=0;i<EBT_Count;++i)
	{
		m_buffers[i].vertixCount=0;
		m_buffers[i].indexCount=0;
		m_buffers[i].buffer=m_mesh->addNewBuffer()->getMeshBuffer();

		m_buffers[i].positionBuffer=m_buffers[i].buffer->createStream(0,video::EMST_Position,video::ESDT_Point3f,0,video::IHardwareBuffer::EUT_DynamicWriteOnlyDiscardable,false,true);
		m_buffers[i].colorBuffer=m_buffers[i].buffer->createStream(0,video::EMST_Color,video::ESDT_Point3f,0,video::IHardwareBuffer::EUT_DynamicWriteOnlyDiscardable,false,true);

		if(i==EBT_Textured)
		{
			m_buffers[i].texBuffer=m_buffers[i].buffer->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,0,video::IHardwareBuffer::EUT_DynamicWriteOnlyDiscardable,false,true);
		}else
			m_buffers[i].texBuffer=0;

		m_buffers[i].buffer->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,0,video::IHardwareBuffer::EUT_DynamicWriteOnlyDiscardable,false,true);
		m_buffers[i].indexData=m_buffers[i].buffer->getIndexData();

		
	}
	m_buffers[EBT_WireFrame].buffer->setRenderType(video::MR_LINES);
	m_buffers[EBT_Textured].buffer->setRenderType(video::MR_TRIANGLES);
	m_buffers[EBT_Solid].buffer->setRenderType(video::MR_TRIANGLES);

	m_buildPrimitivesJob=new DebugDrawManagerBuildJob(this);
	m_buildPrimitivesCompleted= newClassDelegate2<DebugDrawManager,void,JobOrder*,bool>(mT(""),this,&DebugDrawManager::OnBuildPrimitivesCompleted);
//	m_buildPrimitivesJobBarrier=new OS::ThreadBarrier();

	m_threadManager=new ThreadJobManager();
	m_threadManager->AddPiece(m_buildPrimitivesJob);

//	m_primitivesMutex=OS::IThreadManager::getInstance().createMutex();

//	m_buildPrimitivesJob->SetTargetQueue(EDefaultJobQ_Draw);

	m_material.setRenderState(video::RS_Lighting,video::ES_DontUse);
	m_material.SetThickness(3);
//	m_material.SetAlphaReferenceValue(0.3);
//	m_material.SetAlphaFunction(video::EAF_LessEqual);
}
DebugDrawManager::~DebugDrawManager()
{
	delete m_mesh;
	delete m_buildPrimitivesJob;
	delete m_buildPrimitivesCompleted;
//	delete m_buildPrimitivesJobBarrier;
//	delete m_primitivesMutex;
	delete m_threadManager;


	PrimitiveList::iterator it= m_deadPrimitives.begin();
	PrimitiveList::iterator end= m_deadPrimitives.end();
	for (;it!=end;++it)
	{
		delete *it;
	}
	m_deadPrimitives.clear();
}

DebugDrawManager::PrimitiveObject* DebugDrawManager::CreatePrimitive()
{
	if(m_deadPrimitives.size()==0)
		return new PrimitiveObject();

//	OS::ScopedLock locker(m_primitivesMutex);
	PrimitiveObject*res= *m_deadPrimitives.begin();
	m_deadPrimitives.erase(m_deadPrimitives.begin());
	return res;
}
void DebugDrawManager::RemovePrimitive(PrimitiveObject* p)
{
//	OS::ScopedLock locker(m_primitivesMutex);
	m_deadPrimitives.push_back(p);
}
void DebugDrawManager::AddPrimitiveToDraw(PrimitiveObject* p,DebugDrawManager::EBufferType buffer)
{
//	OS::ScopedLock locker(m_primitivesMutex);
	m_buffers[buffer].primitives[p->texture].push_back(p);
	if(p->type<EPT_Count)
	{
		m_buffers[buffer].vertixCount+=m_standardPrimitives[p->type].vertices.size();
		m_buffers[buffer].indexCount+=m_standardPrimitives[p->type].indices.size();
	}else
	{
		m_buffers[buffer].vertixCount+=p->vertices.size();
		m_buffers[buffer].indexCount+=p->indices.size();
	}
}

void DebugDrawManager::AddLine(const math::vector3d& start,const math::vector3d& end,const video::SColor& clr,float lineWidth,float duration,bool depthTest )
{
	PrimitiveObject* p=CreatePrimitive();
	p->type=EPT_Custom;
	p->color=clr;
	p->duration=duration;
	p->texture=0;
	p->thickness = lineWidth;

	p->uv.resize(0);
	p->vertices.resize(2);
	p->vertices[0]=start;
	p->vertices[1]=end;

	p->indices.resize(2);
	p->indices[0]=0;
	p->indices[1]=1;
	AddPrimitiveToDraw(p,EBT_WireFrame);
}
void DebugDrawManager::AddArrow(const math::vector3d& start,const math::vector3d& end,const video::SColor& clr,float lineWidth,float duration,bool depthTest )
{

	PrimitiveObject* p=CreatePrimitive();
	p->type=EPT_Custom;
	p->color=clr;
	p->duration=duration;
	p->texture=0;
	p->thickness = lineWidth;

	p->uv.resize(0);
	p->vertices.resize(2);
	p->indices.resize(2);
	p->vertices[0]=start;
	p->vertices[1]=end;

	p->indices[0]=0;
	p->indices[1]=1;
	AddPrimitiveToDraw(p,EBT_WireFrame);
}

void DebugDrawManager::AddCross(const math::vector3d& pos,float size,const video::SColor& clr,float duration,bool depthTest )
{
	PrimitiveObject* p=CreatePrimitive();
	p->type=EPT_Custom;
	p->color=clr;
	p->duration=duration;
	p->texture=0;

	p->uv.resize(0);
	p->vertices.resize(6);
	p->indices.resize(6);

	float hsz=size*0.5;

	math::vector3d *vptr=&p->vertices[0];
	ushort *iptr=&p->indices[0];
	vptr[0]=pos;
	vptr[1]=pos;
	vptr[2]=pos;
	vptr[3]=pos;
	vptr[4]=pos;
	vptr[5]=pos;

	vptr[0].x-=hsz;
	vptr[1].x+=hsz;
	vptr[2].y-=hsz;
	vptr[3].y+=hsz;
	vptr[4].z-=hsz;
	vptr[5].z+=hsz;

	iptr[0]=0;	iptr[1]=1;
	iptr[2]=2;	iptr[3]=3;
	iptr[4]=4;	iptr[5]=5;


	AddPrimitiveToDraw(p,EBT_WireFrame);
}
void DebugDrawManager::AddSphere(const math::vector3d& pos,float radius,const video::SColor& clr,bool wireframe,float duration,bool depthTest )
{
	PrimitiveObject* p=CreatePrimitive();
	p->type=EPT_Sphere;
	p->color=clr;
	p->duration=duration;
	p->texture=0;

	p->pos=pos;
	p->scale=radius;
	AddPrimitiveToDraw(p,wireframe? EBT_WireFrame:EBT_Solid);
}
void DebugDrawManager::AddCircle(const math::vector3d& pos,float radius,const math::vector3d& dir,const video::SColor& clr,float duration,bool depthTest )
{
	PrimitiveObject* p=CreatePrimitive();
	p->type=EPT_Custom;
	p->color=clr;
	p->duration=duration;
	p->texture=0;

	int n=16;

	p->uv.resize(0);
	p->vertices.resize(n);
	p->indices.resize(n+1);

	math::Plane plane(pos,dir);

	float step=math::PI32/(float)n;
	math::vector3d tmp;
	for (int i=0;i<n;++i)
	{
		//tmp=pos;
		float angle=step*i;
		p->vertices[i]=pos+math::vector3d(radius*cos(angle),radius*sin(angle),0);
		
	}

	AddPrimitiveToDraw(p,EBT_WireFrame);
}
void DebugDrawManager::AddAxis(const math::matrix4x4& transform,float size,float duration,bool depthTest )
{
	math::vector3d p0,p1;
	p0=transform.getTranslation()*size;
	p1=(transform*math::vector3d::XAxis)*size;
	AddArrow(p0,p1,video::DefaultColors::Red,1,duration,depthTest);

	p1=(transform*math::vector3d::YAxis)*size;
	AddArrow(p0,p1,video::DefaultColors::Green,1,duration,depthTest);

	p1=(transform*math::vector3d::ZAxis)*size;
	AddArrow(p0,p1,video::DefaultColors::Blue,1,duration,depthTest);
}
void DebugDrawManager::AddMesh(const math::vector3d* verts,const ushort* indicies,int vCnt,int iCnt,const video::SColor& clr,bool wireframe,float duration,bool depthTest )
{
	PrimitiveObject* p=CreatePrimitive();
	p->type=EPT_Custom;
	p->color=clr;
	p->duration=duration;
	p->texture=0;

	p->uv.resize(0);
	p->vertices.resize(vCnt);
	p->indices.resize(iCnt);

	mraySystem::memCopy(&p->vertices[0],verts,vCnt*sizeof(math::vector3d));
	mraySystem::memCopy(&p->indices[0],indicies,iCnt*sizeof(ushort));

	AddPrimitiveToDraw(p,wireframe? EBT_WireFrame:EBT_Solid);
}
void DebugDrawManager::AddAABB(const math::box3d& aabb,const video::SColor& clr,float duration,bool depthTest )
{
	PrimitiveObject* p=CreatePrimitive();
	p->texture=0;
	p->color=clr;
	p->duration=duration;

	p->uv.resize(0);
	p->vertices.resize(8);
	p->indices.resize(24);

	math::vector3d *vptr=&p->vertices[0];
	ushort *iptr=&p->indices[0];
	aabb.getEdges(vptr);

	iptr[0]=0;	iptr[1]=1;
	iptr[2]=1;	iptr[3]=2;
	iptr[4]=2;	iptr[5]=3;
	iptr[6]=3;	iptr[7]=0;

	iptr[8]=4;	iptr[9]=5;
	iptr[10]=5;	iptr[11]=6;
	iptr[12]=6;	iptr[13]=7;
	iptr[14]=7;	iptr[15]=4;

	iptr[16]=0;	iptr[17]=4;
	iptr[18]=1;	iptr[19]=5;
	iptr[20]=2;	iptr[21]=6;
	iptr[22]=3;	iptr[23]=7;

	AddPrimitiveToDraw(p,EBT_WireFrame);

}
void DebugDrawManager::AddOBB(const math::matrix4x4& transform,const math::vector3d& scale,const video::SColor& clr,float duration,bool depthTest )
{
	PrimitiveObject* p=CreatePrimitive();
	p->type=EPT_Custom;
	p->texture=0;
	p->color=clr;
	p->duration=duration;

	p->uv.resize(0);
	p->vertices.resize(8);
	p->indices.resize(24);

	math::box3d box;
	box.MinP=-scale*0.5;
	box.MaxP=scale*0.5;
	math::vector3d edges[8];
	box.getEdges(edges);

	math::vector3d *vptr=&p->vertices[0];
	ushort *iptr=&p->indices[0];

	for (int i=0;i<8;++i)
	{
		vptr[i]=transform*edges[i];
	}
	iptr[0]=0;	iptr[1]=1;
	iptr[2]=1;	iptr[3]=2;
	iptr[4]=2;	iptr[5]=3;
	iptr[6]=3;	iptr[7]=0;

	iptr[8]=4;	iptr[9]=5;
	iptr[10]=5;	iptr[11]=6;
	iptr[12]=6;	iptr[13]=7;
	iptr[14]=7;	iptr[15]=4;

	iptr[16]=0;	iptr[17]=4;
	iptr[18]=1;	iptr[19]=5;
	iptr[20]=2;	iptr[21]=6;
	iptr[22]=3;	iptr[23]=7;

	AddPrimitiveToDraw(p,EBT_WireFrame);
}

void DebugDrawManager::AddString(const math::vector3d& pos,const core::string& txt,const video::SColor& clr,float duration,bool depthTest )
{
}
void DebugDrawManager::AddBillboard(const math::vector3d& pos,const math::vector2d& size,video::ITexture* tex,const video::SColor& clr,float zrot/* =0 */,float duration/* =0 */,bool depthTest/* =true */ )
{
	PrimitiveObject* p=CreatePrimitive();
	p->type=EPT_Billboard;
	p->texture=tex;
	p->duration=duration;
	p->scale.x=size.x;
	p->scale.y=size.y;
	p->scale.z=zrot;
	p->pos=pos;

	p->color=clr;

	p->vertices.resize(4);
	p->indices.resize(6);
	p->uv.resize(4);

	p->indices[0]=0;
	p->indices[1]=2;
	p->indices[2]=1;
	p->indices[3]=0;
	p->indices[4]=3;
	p->indices[5]=2;

	p->uv[0].set(0,0);
	p->uv[1].set(0,1);
	p->uv[2].set(1,1);
	p->uv[3].set(1,0);

	AddPrimitiveToDraw(p,EBT_Textured);
}

void DebugDrawManager::Clear()
{
//	OS::ScopedLock locker(m_primitivesMutex);
	for(int i=0;i<EBT_Count;++i)
	{
		TexturedPrimitives::iterator tit= m_buffers[i].primitives.begin();
		TexturedPrimitives::iterator tend= m_buffers[i].primitives.end();

		for(;tit!=tend;++tit)
		{
			PrimitiveList::iterator it= tit->second.begin();
			PrimitiveList::iterator end= tit->second.end();
			for (;it!=end;++it)
			{
				m_deadPrimitives.push_back(*it);
			}
		}
		m_buffers[i].primitives.clear();
	}
}

void DebugDrawManager::StartDraw(IViewNode* cam)
{
	m_camera=cam;
	//m_buildPrimitivesJobBarrier->Lock();
	//JobPool::getInstance().AddRequest(m_buildPrimitivesJob,m_buildPrimitivesCompleted);
	m_threadManager->BeginProcessing();
}
void DebugDrawManager::EndDraw()
{/*
	if(m_buildPrimitivesJobBarrier->GetLocksCount()>0)
		m_buildPrimitivesJobBarrier->Wait();*/

	m_threadManager->EndProcessing();

	//OS::ScopedLock locker(m_primitivesMutex);

//	m_device->set3DMode();
	m_device->useRenderPass(&m_material);
	m_device->setTransformationState(video::TS_WORLD,math::matrix4x4::Identity);
	for(int i=0;i<EBT_Count;++i)
	{
		BufferData* data=&m_buffers[i];
		for(int j=0;j<data->texObjects.size();++j)
		{
			m_buffers[i].indexData->firstIndex=data->texObjects[j].startIndex;
			m_buffers[i].indexData->indexCount=data->texObjects[j].count;
			m_material.setTexture(data->texObjects[j].tex,0);
			m_device->useTexture(0,m_material.getTextureUnit(0));
			m_device->drawSingleMesh(m_buffers[i].buffer);
		}
	}
}
void DebugDrawManager::Update(float dt)
{
	for(int i=0;i<EBT_Count;++i)
	{
		BufferData* data=&m_buffers[i];
		TexturedPrimitives::iterator tit= data->primitives.begin();
		TexturedPrimitives::iterator tend= data->primitives.end();

		for(;tit!=tend;++tit)
		{
			PrimitiveList::iterator it= tit->second.begin();
			PrimitiveList::iterator end= tit->second.end();

			for (;it!=end;)
			{
				PrimitiveObject* p=*it;
				p->duration-=dt;
				if(p->duration<=0)
				{
					data->vertixCount-=p->vertices.size();
					data->indexCount-=p->indices.size();
					m_deadPrimitives.push_back(p);
					PrimitiveList::iterator it2=it;
					++it;
					tit->second.erase(it2);				
				}else
					++it;
			}
		}
	}
}

void DebugDrawManager::_BuildDrawingLists()
{
	//OS::ScopedLock locker(m_primitivesMutex);
	for(int i=0;i<EBT_Count;++i)
	{
		BufferData* data=&m_buffers[i];
		if(data->positionBuffer->length()<data->vertixCount)
		{
			data->positionBuffer->resize(data->vertixCount*sizeof(math::vector3d));
			data->colorBuffer->resize(data->vertixCount*sizeof(math::vector3d));
			if(i==EBT_Textured)
			{
				data->texBuffer->resize(data->vertixCount*sizeof(math::vector2d));
			}
		}
		if(data->indexData->indexBuffer->getIndexCount()<data->indexCount)
		{
			data->indexData->indexBuffer->resize(data->indexCount*sizeof(ushort));
		}
		data->indexData->indexCount=data->indexCount;

		math::vector3d *vPtr=(math::vector3d*)data->positionBuffer->lock(0,0,video::IHardwareBuffer::ELO_Discard);
		math::vector3d *cPtr=(math::vector3d*)data->colorBuffer->lock(0,0,video::IHardwareBuffer::ELO_Discard);
		math::vector2d *uvPtr=0;
		if(i==EBT_Textured)
		{
			uvPtr=(math::vector2d*)data->texBuffer->lock(0,0,video::IHardwareBuffer::ELO_Discard);
		}

		ushort *iPtr=(ushort*)data->indexData->indexBuffer->lock(0,0,video::IHardwareBuffer::ELO_Discard);

		int currentIndex=0;
		uint indexCtr=0;
		TexturedPrimitives::iterator tit= data->primitives.begin();
		TexturedPrimitives::iterator tend= data->primitives.end();
		data->texObjects.clear();

		for(;tit!=tend;++tit)
		{
			PrimitiveList::iterator it= tit->second.begin();
			PrimitiveList::iterator end= tit->second.end();
			TexturedObject obj;
			obj.tex=tit->first;
			obj.startIndex=indexCtr;
			for (;it!=end;++it)
			{
				PrimitiveObject* p=*it;
				PrimitiveObject* p2=p;
				if(p->type<EPT_Count)
				{
					p2=&m_standardPrimitives[p->type];
				}else if(p->type==EPT_Billboard)
				{
					MeshGenerator::getInstance().generateBillboard(m_camera,p->pos,math::vector2d(p->scale.x,p->scale.y),p->scale.z,true,true,&p->vertices[0]);
				}

				int cnt=p2->vertices.size();
				math::vector3d *srcVPtr=&p2->vertices[0];
				math::vector3d *startVPtr=vPtr;

				for(int vCtr=0;vCtr<cnt;++vCtr){
					*vPtr++=*srcVPtr++;
					(*cPtr++).set(p->color.R,p->color.G,p->color.B);
				}
				if(i==EBT_Textured)
				{
					math::vector2d *srcUVPtr=&p2->uv[0];
					for(int vCtr=0;vCtr<cnt;++vCtr){
						*uvPtr++=*srcUVPtr++;
					}
				}

				switch(p->type)
				{
				case EPT_Sphere:
					for (int vCtr=0;vCtr<cnt;++vCtr)
					{
						(*startVPtr)=(*startVPtr)*p->scale+p->pos;
						++startVPtr;
					}
					break;
				}
				cnt=p->indices.size();
				ushort* srcIPtr=&p2->indices[0];
				for(int iCtr=0;iCtr<cnt;++iCtr)
				{
					*iPtr++=currentIndex+*srcIPtr++;
				}
				indexCtr+=cnt;
				currentIndex+=p2->vertices.size();
			}
			obj.count=indexCtr-obj.startIndex;

			data->texObjects.push_back(obj);
		}
		data->positionBuffer->unlock();
		data->colorBuffer->unlock();
		data->indexData->indexBuffer->unlock();
		if(i==EBT_Textured)
		{
			data->texBuffer->unlock();
		}

	}
}



//////////////////////////////////////////////////////////////////////////

void DebugDrawManager::OnBuildPrimitivesCompleted(JobOrder* o,bool s)
{
	//m_buildPrimitivesJobBarrier->Unlock();
}

}
}

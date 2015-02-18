#include "stdafx.h"


#include "GUIBatchRenderer.h"

#include "IGUIRenderOperation.h"

#include "IVideoDevice.h"
#include "SMeshBuffer.h"

namespace mray
{
namespace GUI
{

GUIBatchRenderer::GUIBatchRenderer()
{
	m_batchRender=false;
	m_device=0;
	m_lastTexture=0;
	int maxQuads=500;
	m_maxBatchSize=6*maxQuads;
	m_mesh=new scene::SMeshBuffer();
	m_mesh->createStream(0,video::EMST_Position,video::ESDT_Point2f,4*maxQuads,video::IHardwareBuffer::EUT_DynamicWriteOnly,false,true);
	m_mesh->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,4*maxQuads,video::IHardwareBuffer::EUT_DynamicWriteOnly,false,true);
	m_mesh->createStream(0,video::EMST_Color,video::ESDT_Point4f,4*maxQuads,video::IHardwareBuffer::EUT_DynamicWriteOnly,false,true);

	m_mesh->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,m_maxBatchSize,video::IHardwareBuffer::EUT_StaticWriteOnly,false,false);

	m_mesh->setRenderType(video::MR_TRIANGLES);

	m_posStream=m_mesh->getStream(0,video::EMST_Position);
	m_tcStream=m_mesh->getStream(0,video::EMST_Texcoord);
	m_clrStream=m_mesh->getStream(0,video::EMST_Color);

	m_indicies=m_mesh->getIndexData();
	
	FATAL_ERROR(!m_indicies,mT("Index Buffer"));
	FATAL_ERROR(!m_indicies->indexBuffer.pointer(),mT("Index Buffer"));
	

	ushort* ind= (ushort*)m_indicies->indexBuffer-> lock(0,0,video::IHardwareBuffer::ELO_Discard);
	for(ushort i=0,j=0;i<m_maxBatchSize;i+=6,j+=4)
	{
		ind[i+0]=j+0;
		ind[i+1]=j+1;
		ind[i+2]=j+2;
		ind[i+3]=j+0;
		ind[i+4]=j+2;
		ind[i+5]=j+3;
	}
	m_indicies->indexBuffer->unlock();

	m_locked=false;

	m_posPtr=0;
	m_tcPtr=0;
	m_clrPtr=0;

}
GUIBatchRenderer::~GUIBatchRenderer()
{
	delete m_mesh;
}

void GUIBatchRenderer::SetRenderMode(bool batched)
{
	m_batchRender=batched;
}

void GUIBatchRenderer::_lockBuffers()
{
	if(m_locked)return;
	m_locked=true;
	m_indicies->indexCount=0;

	m_posPtr=(math::vector2d*)m_posStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	m_tcPtr=(math::vector2d*)m_tcStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	m_clrPtr=(video::SColor*)m_clrStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
}

void GUIBatchRenderer::_unlockBuffers()
{
	if(!m_locked)return;
	m_locked=false;
	m_posStream->unlock();
	m_tcStream->unlock();
	m_clrStream->unlock();

	m_posPtr=0;
	m_tcPtr=0;
	m_clrPtr=0;
}
/*
bool GUIBatchRenderer::_ProcessSingleOperation(IGUIRenderOperation*op)
{
	ushort used;
	bool renderMore=false;
	ushort limit=m_maxBatchSize-m_indicies->indexCount;
	bool rendered=true;
	op->PrepareToRender();
	do
	{
		rendered=false;;
		used=0;
		renderMore=!op->AddToBuffer(m_posPtr,m_tcPtr,m_clrPtr,limit,used);
		limit-=used;
		m_indicies->indexCount+=used;
		if(limit==0)
		{
			if(!renderMore)
				rendered=true;
			_unlockBuffers();
			m_device->drawSingleMesh(m_mesh);
			m_indicies->indexCount=0;
			limit=m_maxBatchSize;
			_lockBuffers();
		}
	}while(renderMore);

	return rendered;
}

void GUIBatchRenderer::_RenderGroup(const OperationList&grp)
{

	_lockBuffers();
	OperationList::const_iterator it=grp.begin();
	ushort limit=m_maxBatchSize;
	bool rendered=true;
	for(;it!=grp.end();++it)
	{
		IGUIRenderOperation*op=*it;
		rendered=_ProcessSingleOperation(op);
	}
	_unlockBuffers();
	if(!rendered)
		m_device->drawSingleMesh(m_mesh);
}


void GUIBatchRenderer::AddRenderOperation(const IGUIRenderOperationPtr& op,bool forceImediate)
{
	if(!m_batchRender || forceImediate)
	{
		m_device->useTexture(0,op->GetTexture());
		_lockBuffers();
		bool res=_ProcessSingleOperation(op);
		_unlockBuffers();
		if(!res)
			m_device->drawSingleMesh(m_mesh);

	}else
		m_operations[op->GetTexture()].push_back(op);
}
*/

void GUIBatchRenderer::Prepare()
{
	m_lastTexture=0;
	_lockBuffers();
}

void GUIBatchRenderer::AddQuad(video::TextureUnit*tex,const math::rectf&pos,const math::rectf&tc,const video::SColor*clr,const math::rectf* clip,float angle)
{
	if(!pos.isValid())
		return;
	if(!m_locked)_lockBuffers();

	if(tex!=m_lastTexture)
	{
		_unlockBuffers();
		m_device->useTexture(0,m_lastTexture);
		m_device->drawSingleMesh(m_mesh);
		_lockBuffers();
		m_lastTexture=tex;
	}

	m_posPtr[0].x=pos.ULPoint.x;
	m_posPtr[0].y=pos.ULPoint.y;
	m_tcPtr[0].x=tc.ULPoint.x;
	m_tcPtr[0].y=tc.ULPoint.y;
	m_clrPtr[0]=clr[0];

	m_posPtr[1].x=pos.BRPoint.x;
	m_posPtr[1].y=pos.ULPoint.y;
	m_tcPtr[1].x=tc.BRPoint.x;
	m_tcPtr[1].y=tc.ULPoint.y;
	m_clrPtr[1]=clr[1];

	m_posPtr[2].x=pos.BRPoint.x;
	m_posPtr[2].y=pos.BRPoint.y;
	m_tcPtr[2].x=tc.BRPoint.x;
	m_tcPtr[2].y=tc.BRPoint.y;
	m_clrPtr[2]=clr[2];

	m_posPtr[3].x=pos.ULPoint.x;
	m_posPtr[3].y=pos.BRPoint.y;
	m_tcPtr[3].x=tc.ULPoint.x;
	m_tcPtr[3].y=tc.BRPoint.y;
	m_clrPtr[3]=clr[3];

	if(angle!=0)
	{
		float cs=math::cosd(angle);
		float sn=math::sind(angle);
		float x=0;
		math::vector2d hsz = pos.ULPoint+ pos.getSize() / 2;
		for(int i=0;i<4;++i)
		{
			m_posPtr[i] -= hsz;
			x=m_posPtr[i].x;
			m_posPtr[i].x = cs*x - sn*m_posPtr[i].y;
			m_posPtr[i].y = sn*x + cs*m_posPtr[i].y;
			m_posPtr[i] += hsz;
		}
	}

	m_posPtr+=4;
	m_tcPtr+=4;
	m_clrPtr+=4;

	m_indicies->indexCount+=6;
	if(m_indicies->indexCount>=m_maxBatchSize)
	{
		_unlockBuffers();
		m_device->useTexture(0,m_lastTexture);
		m_device->drawSingleMesh(m_mesh);
		_lockBuffers();
	}

}

void GUIBatchRenderer::AddQuad(video::TextureUnit*tex,const math::rectf&pos,const math::rectf&tc,const video::SColor&clr,const math::rectf* clip,float angle)
{
	video::SColor carr[4]={clr,clr,clr,clr};
	AddQuad(tex,pos,tc,carr,clip,angle);
}

void GUIBatchRenderer::Flush()
{
	_unlockBuffers();

	m_device->useTexture(0,m_lastTexture);
	m_device->drawSingleMesh(m_mesh);
	m_lastTexture=0;
	_lockBuffers();

	/*
	OperationMap::iterator it= m_operations.begin();
	for(;it!=m_operations.end();++it)
	{
		m_device->useTexture(0,it->first);
		_RenderGroup(it->second);
	}*/
}/*
void GUIBatchRenderer::Clear()
{
	m_operations.clear();
}
*/
}
}


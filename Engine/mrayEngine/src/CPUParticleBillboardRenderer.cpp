#include "stdafx.h"

#include "CPUParticleBillboardRenderer.h"
#include "IParticle.h"
#include "SMeshBuffer.h"

#include "IViewNode.h"

namespace mray
{
namespace scene
{

CPUParticleBillboardRenderer::CPUParticleBillboardRenderer(ushort maxParticles)
{
	m_maxParticles=maxParticles;
	m_usedParticles=0;
	m_locked=0;

	m_rotatable=true;

	// setup vertices
	int vCnt=4*maxParticles;
	m_meshBuffer=new SMeshBuffer();

	video::IHardwareStreamBuffer* vStream= m_meshBuffer->createStream(0,video::EMST_Position,video::ESDT_Point3f,vCnt,
		video::IHardwareBuffer::EUT_DynamicWriteOnly,0);
	video::IHardwareStreamBuffer* tc0Stream= m_meshBuffer->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,vCnt,
		video::IHardwareBuffer::EUT_StaticWriteOnly,0);
	video::IHardwareStreamBuffer* colorStream= m_meshBuffer->createStream(0,video::EMST_Color,video::ESDT_Point4f,vCnt,
		video::IHardwareBuffer::EUT_DynamicWriteOnly,0);


	math::vector2d*tc0=(math::vector2d*)tc0Stream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	for(int i=0;i<vCnt;i+=4){

		tc0[i+0].set(1,0);
		tc0[i+1].set(1,1);
		tc0[i+2].set(0,1);
		tc0[i+3].set(0,0);
	}
	tc0Stream->unlock();

	// setup indicies
	int icnt=4*maxParticles;
	m_meshBuffer->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,icnt,
		video::IHardwareBuffer::EUT_StaticWriteOnly,0);

	m_indecies=m_meshBuffer->getIndexData();

	m_indecies->firstIndex=0;
	m_indecies->indexCount=0;

	ushort*idx=(ushort*) m_indecies->indexBuffer->lock(0,0,video::IHardwareBuffer::ELO_Discard);

	int n=0;
	for (int i=0;i<icnt;i+=4,n+=4)
	{
		idx[i+0]=0+n;
		idx[i+1]=1+n;
		idx[i+2]=2+n;
		idx[i+3]=3+n;
		/*
		idx[i+3]=0+n;
		idx[i+4]=2+n;
		idx[i+5]=3+n;*/
	}
	m_indecies->indexBuffer->unlock();
	//	m_meshBuffer->setRenderType(video::MR_TRIANGLES);
	m_meshBuffer->setRenderType(video::MR_QUADS);

	m_vnode=0;
}

CPUParticleBillboardRenderer::~CPUParticleBillboardRenderer()
{
	delete m_meshBuffer;
}


void CPUParticleBillboardRenderer::reset(IViewNode*vnode)
{
	m_vnode=vnode;


	m_particles.resize(0);
}

void CPUParticleBillboardRenderer::addParticle(IParticle* p)
{
	m_particles.push_back(ParticleInfo(p->position.getDistSQ(m_vnode->getViewPos()),p));
}

void CPUParticleBillboardRenderer::addParticle(const math::vector3d&pos,float angle,const math::vector2d&scale,const video::SColor&clr)
{
	if(!m_locked)
		return;
	if(m_usedParticles>=m_maxParticles)
		return;
	m_usedParticles++;
	m_clrPtr[0]=clr;
	m_clrPtr[1]=clr;
	m_clrPtr[2]=clr;
	m_clrPtr[3]=clr;

	math::vector3d X,Y;


	if(m_rotatable){
		float ca=math::cosd(angle);
		float sn=math::sind(angle);
		X=( m_ver*ca+m_hor*sn)*scale.x*0.5;
		Y=(-m_ver*sn+m_hor*ca)*scale.y*0.5;
	}
	else
	{
		X=m_ver*scale.x*0.5;
		Y=m_hor*scale.y*0.5;
	}

	m_posPtr[0]=pos+X+Y;
	m_posPtr[1]=pos-X+Y;
	m_posPtr[2]=pos-X-Y;
	m_posPtr[3]=pos+X-Y;

	m_clrPtr+=4;
	m_posPtr+=4;
}

void CPUParticleBillboardRenderer::done()
{
	m_indecies->indexCount=0;
	m_usedParticles=0;
	m_locked=1;
	m_ver=m_vnode->getViewMatrix().getAxisVector(0);
	m_hor=m_vnode->getViewMatrix().getAxisVector(1);

	m_posPtr=(math::vector3d*)m_meshBuffer->getStream(0,video::EMST_Position)->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	m_clrPtr=(video::SColor*)m_meshBuffer->getStream(0,video::EMST_Color)->lock(0,0,video::IHardwareBuffer::ELO_Discard);

//	m_particles.sort();
	std::sort(m_particles.begin(),m_particles.end());
	for(int i=m_particles.size()-1;i>=0;--i){
		IParticle*p=m_particles[i].part;
		addParticle(p->position,p->AngleZ,math::vector2d(p->scale.x,p->scale.y),p->color);
	}
	m_locked=false;
	m_indecies->indexCount=m_usedParticles*4;
	m_meshBuffer->getStream(0,video::EMST_Position)->unlock();
	m_meshBuffer->getStream(0,video::EMST_Color)->unlock();
}

IMeshBuffer* CPUParticleBillboardRenderer::getMeshBuffer()
{
	return m_meshBuffer;
}


}
}

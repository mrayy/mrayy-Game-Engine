#include "stdafx.h"

#include "GPUParticleBillboardRenderer.h"
#include "IHardwareBufferManager.h"
#include "SMeshBuffer.h"
#include "IParticle.h"
#include "IViewNode.h"

namespace mray{
namespace scene{


GPUParticleBillboardRenderer::GPUParticleBillboardRenderer(ushort maxParticles)
{
	m_vnode=0;

	m_maxParticles=maxParticles;
	m_usedParticles=0;
	m_locked=0;
	m_tc1Ptr=0;
	m_tc2Ptr=0;
	m_tc3Ptr=0;

	// setup vertices
	int vCnt=4*maxParticles;
	m_meshBuffer=new SMeshBuffer();

	video::IHardwareStreamBuffer* vStream= m_meshBuffer->createStream(0,video::EMST_Position,video::ESDT_Point3f,vCnt,
		video::IHardwareBuffer::EUT_StaticWriteOnly,0);
	video::IHardwareStreamBuffer* tc0Stream= m_meshBuffer->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,vCnt,
		video::IHardwareBuffer::EUT_StaticWriteOnly,0);

	m_meshBuffer->createStream(1,video::EMST_Texcoord,video::ESDT_Point4f,vCnt,
		video::IHardwareBuffer::EUT_Dynamic,0);
	m_meshBuffer->createStream(2,video::EMST_Texcoord,video::ESDT_Point4f,vCnt,
		video::IHardwareBuffer::EUT_Dynamic,0);
	m_meshBuffer->createStream(3,video::EMST_Texcoord,video::ESDT_Point4f,vCnt,
		video::IHardwareBuffer::EUT_Dynamic,0);

	math::vector3d*vtx=(math::vector3d*)vStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	math::vector2d*tc0=(math::vector2d*)tc0Stream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	for(int i=0;i<vCnt;i+=4){
		vtx[i+0].set( 1,0, 1);
		vtx[i+1].set(-1,0, 1);
		vtx[i+2].set(-1,0,-1);
		vtx[i+3].set( 1,0,-1);

		tc0[i+0].set(1,0);
		tc0[i+1].set(1,1);
		tc0[i+2].set(0,1);
		tc0[i+3].set(0,0);
	}
	vStream->unlock();
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
}
GPUParticleBillboardRenderer::~GPUParticleBillboardRenderer()
{
	delete m_meshBuffer;
}

void GPUParticleBillboardRenderer::reset(IViewNode*vnode)
{
	m_vnode=vnode;
}
void GPUParticleBillboardRenderer::addParticle(IParticle* p){

	m_particles.push_back(ParticleInfo(p->position.getDistSQ(m_vnode->getViewPos()),p));
	//addParticle(p->position,p->AngleZ,math::vector2d(p->scale.x,p->scale.y),p->color);
}

void GPUParticleBillboardRenderer::addParticle(const math::vector3d&pos,float angle,
										const math::vector2d&scale,const video::SColor&color)
{
	if(!m_locked)
		return;
	if(m_usedParticles>=m_maxParticles)
		return;
	m_usedParticles++;
	m_tc1Ptr[0].set(pos.x,pos.y,pos.z,0);
	m_tc2Ptr[0].set(math::sind(angle),math::cosd(angle),scale.x,scale.y);
	m_tc3Ptr[0].set(color.R,color.G,color.B,color.A);

	m_tc1Ptr[3]=m_tc1Ptr[2]=m_tc1Ptr[1]=m_tc1Ptr[0];
	m_tc2Ptr[3]=m_tc2Ptr[2]=m_tc2Ptr[1]=m_tc2Ptr[0];
	m_tc3Ptr[3]=m_tc3Ptr[2]=m_tc3Ptr[1]=m_tc3Ptr[0];

	m_tc1Ptr+=4;
	m_tc2Ptr+=4;
	m_tc3Ptr+=4;
}
void GPUParticleBillboardRenderer::done(){
	if(m_locked)
		return;

	m_indecies->indexCount=0;
	m_usedParticles=0;
	m_locked=true;

	m_tc1Ptr=(math::vector4d*)m_meshBuffer->getStream(1,video::EMST_Texcoord)->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	m_tc2Ptr=(math::vector4d*)m_meshBuffer->getStream(2,video::EMST_Texcoord)->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	m_tc3Ptr=(math::vector4d*)m_meshBuffer->getStream(3,video::EMST_Texcoord)->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	//	m_particles.sort();
	std::sort(m_particles.begin(),m_particles.end());
	for(int i=m_particles.size()-1;i>=0;--i){
		IParticle*p=m_particles[i].part;
		addParticle(p->position,p->AngleZ,math::vector2d(p->scale.x,p->scale.y),p->color);
	}
	m_indecies->indexCount=m_usedParticles*4;

	m_meshBuffer->getStream(1,video::EMST_Texcoord)->unlock();
	m_meshBuffer->getStream(2,video::EMST_Texcoord)->unlock();
	m_meshBuffer->getStream(3,video::EMST_Texcoord)->unlock();

	m_tc1Ptr=0;
	m_tc2Ptr=0;
	m_tc3Ptr=0;
	m_locked=false;
}



IMeshBuffer* GPUParticleBillboardRenderer::getMeshBuffer(){
	return m_meshBuffer;
}
video::IndexData* GPUParticleBillboardRenderer::getIndecies(){
	return m_indecies;
}

}
}

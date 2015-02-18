
#include "stdafx.h"
#include "AIRenderDevice.h"

#include "mraySystem.h"

namespace mray
{
namespace AI
{


AIRenderDevice::AIRenderDevice(video::IVideoDevice*dev,GUI::IFont*font)
{
	m_device=dev;
	m_font=font;

	m_mesh=new scene::SMeshBuffer();
	m_mesh->createStream(0,video::EMST_Position,video::ESDT_Point3f,0,video::IHardwareBuffer::EUT_Dynamic,false,true);
	//m_mesh->createStream(0,video::EMST_Color,video::ESDT_Color4b,0,video::IHardwareBuffer::EUT_Dynamic,false,true);
	m_mesh->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,0,video::IHardwareBuffer::EUT_Dynamic,false,true);
}
AIRenderDevice:: ~AIRenderDevice()
{
}


const core::array<core::string> & AIRenderDevice::getTextLines()
{
	return m_txtLines;
}

void AIRenderDevice::drawLine(const math::line3d&l,const video::SColor&c)
{
	m_device->set3DMode();
	m_device->draw3DLine(l.pStart,l.pEnd,c);
}
void AIRenderDevice::drawText(const math::vector2d&p,const core::string&text,const video::SColor&c)
{
	m_device->set2DMode();
	GUI::FontAttributes attr;
	attr.fontColor=c;
	attr.fontAligment=GUI::EFA_MiddleLeft;
	attr.fontSize=14;
	attr.hasShadow=true;
	attr.shadowColor.Set(0,0,0,255);
	attr.shadowOffset=math::vector2d(2);
	attr.spacing=2;
	attr.wrap=0;
	attr.RightToLeft=0;
	m_font->print(math::rectf(p,p),&attr,0,text);
}
void AIRenderDevice::clearTextLines()
{
	m_txtLines.resize(0);
}

void AIRenderDevice::addTextLine(const core::string&text)
{
	m_txtLines.push_back(text);
}

void AIRenderDevice::renderLines(const math::vector3d*points,int ptCnt,const ushort*idx,int idxCnt,const video::SColor&c)
{
	m_device->set3DMode();
	m_device->setTransformationState(video::TS_WORLD,math::matrix4x4::Identity);

	GCPtr<video::IHardwareStreamBuffer>vS=m_mesh->getStream(0,video::EMST_Position);
	GCPtr<video::IHardwareIndexBuffer> iB=m_mesh->getIndexData()->indexBuffer;
	vS->resize(ptCnt*sizeof(math::vector3d));
	m_mesh->getStreamsData()->count=ptCnt;
	iB->resize(idxCnt*sizeof(ushort));
	m_mesh->getIndexData()->indexCount=idxCnt;

	math::vector3d* v=(math::vector3d* )vS->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	ushort*ind=(ushort*)iB->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	
	mraySystem::memCopy(v,points,ptCnt*sizeof(math::vector3d));
	mraySystem::memCopy(ind,idx,idxCnt*sizeof(ushort));

	vS->unlock();
	iB->unlock();
	m_mesh->setRenderType(video::MR_LINES);
	m_device->drawSingleMesh(m_mesh);
}

void AIRenderDevice::renderTriangles(const math::vector3d*points,int ptCnt,const ushort*idx,int idxCnt,const video::SColor&c)
{
	m_device->set3DMode();
	m_device->setTransformationState(video::TS_WORLD,math::matrix4x4::Identity);

	GCPtr<video::IHardwareStreamBuffer>vS=m_mesh->getStream(0,video::EMST_Position);
	GCPtr<video::IHardwareIndexBuffer> iB=m_mesh->getIndexData()->indexBuffer;
	vS->resize(ptCnt*sizeof(math::vector3d));
	m_mesh->getStreamsData()->count=ptCnt;
	iB->resize(idxCnt*sizeof(ushort));
	m_mesh->getIndexData()->indexCount=idxCnt;

	math::vector3d* v=(math::vector3d* )vS->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	ushort*ind=(ushort*)iB->lock(0,0,video::IHardwareBuffer::ELO_Discard);

	mraySystem::memCopy(v,points,ptCnt*sizeof(math::vector3d));
	mraySystem::memCopy(ind,idx,idxCnt*sizeof(ushort));	


	vS->unlock();
	iB->unlock();
	m_mesh->setRenderType(video::MR_TRIANGLES);
	m_device->drawSingleMesh(m_mesh);
}


}
}
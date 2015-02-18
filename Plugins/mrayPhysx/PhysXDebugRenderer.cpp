#include "stdafx.h"

#include <NxDebugRenderable.h>
#include <SoftwareIndexBuffer.h>
#include <SoftwareStreamBuffer.h>
#include <NxDebugRenderable.h>
#include "PhysXDebugRenderer.h"
#include <IVideoDevice.h>
#include <SMeshBuffer.h>
#include <Engine.h>


namespace mray{
namespace physics{

PhysXDebugRenderer::PhysXDebugRenderer(){
	m_mesh=new scene::SMeshBuffer();
	m_mesh->createStream(0,video::EMST_Position,video::ESDT_Point3f,0,video::IHardwareBuffer::EUT_Dynamic,false,true);
	m_mesh->createStream(0,video::EMST_Color,video::ESDT_Point4f,0,video::IHardwareBuffer::EUT_Dynamic,false,true);

	m_mesh->createIndexBuffer(video::IHardwareIndexBuffer::EIT_32Bit,0,video::IHardwareBuffer::EUT_Dynamic,false,true);
}

void PhysXDebugRenderer::renderData(const NxDebugRenderable& data) {

	video::IVideoDevice*device=Engine::getInstance().getDevice();

	device->setTransformationState(video::TS_WORLD,math::matrix4x4::Identity);
	//device->useMaterial(0);

	GCPtr<video::IHardwareIndexBuffer> iB=m_mesh->getIndexData()->indexBuffer;
	unsigned int NbPoints = data.getNbPoints();
	if(NbPoints)
	{
		video::IHardwareStreamBuffer*vS=m_mesh->getStream(0,video::EMST_Position);
		video::IHardwareStreamBuffer*cS=m_mesh->getStream(0,video::EMST_Color);
		vS->resize(NbPoints);
		cS->resize(NbPoints);
		m_mesh->getStreamsData()->count=NbPoints;


		math::vector3d* v=(math::vector3d* )vS->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
		video::SColor* c=(video::SColor* )cS->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

    	int vertIndex = 0;
    	int colorIndex = 0;
		const NxDebugPoint* Points = data.getPoints();
		while(NbPoints--)
		{
			v[vertIndex] =math::vector3d(Points->p.x,Points->p.y,Points->p.z);
			c[vertIndex] =video::SColor((Points->color>>16)&0xff,(Points->color>>8)&0xff,Points->color&0xff,255);
			vertIndex++;
	      	Points++;
		}
		vS->unlock();
		cS->unlock();
		m_mesh->setRenderType(video::MR_POINTS);
		device->drawSingleMesh(m_mesh);

	}

	unsigned int NbLines = data.getNbLines();
	if(NbLines)
	{
		video::IHardwareStreamBuffer*vS=m_mesh->getStream(0,video::EMST_Position);
		video::IHardwareStreamBuffer*cS=m_mesh->getStream(0,video::EMST_Color);
		vS->resize(NbLines*2);
		cS->resize(NbLines*2);
		m_mesh->getStreamsData()->count=NbLines*2;
		iB->resize(NbLines*2);
		m_mesh->getIndexData()->indexCount=NbLines*2;

		math::vector3d* v=(math::vector3d* )vS->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
		video::SColor* c=(video::SColor* )cS->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

    	int vertIndex = 0;
    	int colorIndex = 0;
    	int lineIndex = 0;
		const NxDebugLine* Lines = data.getLines();
		uint*ind=(uint*)iB->lock(0,0,video::IHardwareBuffer::ELO_Discard);
		while(NbLines--)
		{
			v[vertIndex] =math::vector3d(Lines->p0.x,Lines->p0.y,Lines->p0.z);
			c[vertIndex].setColorRGBA(Lines->color);// =video::SColor((Lines->color>>16)&0xff*math::i255,(Lines->color>>8)&0xff,Lines->color&0xff,255);
			ind[lineIndex]=vertIndex;
			lineIndex++;

			vertIndex++;
			v[vertIndex] =math::vector3d(Lines->p1.x,Lines->p1.y,Lines->p1.z);
			c[vertIndex].setColorRGBA(Lines->color);// =video::SColor((Lines->color>>16)&0xff,(Lines->color>>8)&0xff,Lines->color&0xff,255);
			
			ind[lineIndex]=vertIndex;
			lineIndex++;

			vertIndex++;
	      	Lines++;
		}
		vS->unlock();
		cS->unlock();
		iB->unlock();
		m_mesh->setRenderType(video::MR_LINES);
		device->drawSingleMesh(m_mesh);

	}

	unsigned int NbTris = data.getNbTriangles();
	if(NbTris && false)
	{
		video::IHardwareStreamBuffer*vS=m_mesh->getStream(0,video::EMST_Position);
		video::IHardwareStreamBuffer*cS=m_mesh->getStream(0,video::EMST_Color);
		vS->resize(NbTris*3);
		cS->resize(NbTris*3);
		m_mesh->getStreamsData()->count=NbTris*3;
		iB->resize(NbTris*3);
		m_mesh->getIndexData()->indexCount=NbTris*3;

		math::vector3d* v=(math::vector3d* )vS->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
		video::SColor* c=(video::SColor* )cS->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

    	int vertIndex = 0;
    	int colorIndex = 0;
    	int TriIndex = 0;
		const NxDebugTriangle* Triangles = data.getTriangles();
		uint*ind=(uint*)iB->lock(0,0,video::IHardwareBuffer::ELO_Discard);
		while(NbPoints--)
		{
			v[vertIndex] =math::vector3d(Triangles->p0.x,Triangles->p0.y,Triangles->p0.z);
			c[vertIndex] =video::SColor((Triangles->color>>16)&0xff,(Triangles->color>>8)&0xff,Triangles->color&0xff,255);
			ind[TriIndex]=vertIndex;
			TriIndex++;

			vertIndex++;
			v[vertIndex] =math::vector3d(Triangles->p1.x,Triangles->p1.y,Triangles->p1.z);
			c[vertIndex] =video::SColor((Triangles->color>>16)&0xff,(Triangles->color>>8)&0xff,Triangles->color&0xff,255);
			
			ind[TriIndex]=vertIndex;
			TriIndex++;

			vertIndex++;
			v[vertIndex] =math::vector3d(Triangles->p2.x,Triangles->p2.y,Triangles->p2.z);
			c[vertIndex] =video::SColor((Triangles->color>>16)&0xff,(Triangles->color>>8)&0xff,Triangles->color&0xff,255);
			
			ind[TriIndex]=vertIndex;
			TriIndex++;

			vertIndex++;
	      	Triangles++;
		}
		vS->unlock();
		cS->unlock();
		iB->unlock();
		m_mesh->setRenderType(video::MR_TRIANGLES);
		device->drawSingleMesh(m_mesh);
	}

}


}
}





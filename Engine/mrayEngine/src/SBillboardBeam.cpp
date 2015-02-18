#include "stdafx.h"

#include "SBillboardBeam.h"
#include "IViewNode.h"
#include "ISceneManager.h"
#include "IVideoDevice.h"
#include "MeshGenerator.h"
#include "MeshBufferData.h"
#include "Engine.h"

#include "RenderTechnique.h"
#include "RenderPass.h"

namespace mray{
namespace scene{

SBillboardBeam::SBillboardBeam():MeshRenderableNode(0),m_Width(1)
{
	_init();
}
SBillboardBeam::SBillboardBeam(const math::vector3d& start,const math::vector3d& end,float width):
MeshRenderableNode(0),m_Width(width)
{
	m_StartPoint=start;
	m_EndPoint=end;
	_init();
}

void SBillboardBeam::_init()
{
	m_customRender=true;

	m_useWorldSpace=false;

	
	MeshBufferData *meshD=m_mesh->addNewBuffer();
	IMeshBuffer *mesh=meshD->getMeshBuffer();

	video::IHardwareStreamBuffer* vStream=mesh->createStream(0,video::EMST_Position,
		video::ESDT_Point3f,4,video::IHardwareStreamBuffer::EUT_Dynamic,true);
	video::IHardwareStreamBuffer* uvStream=mesh->createStream(0,video::EMST_Texcoord,
		video::ESDT_Point2f,4,video::IHardwareStreamBuffer::EUT_Static,true);
	math::vector2df *tc=(math::vector2df *)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

	ushort ind[6];

	m_dirty=true;
	ind[0]=0;
	ind[1]=1;
	ind[2]=2;
	ind[3]=1;
	ind[4]=3;
	ind[5]=2;

	const video::IHardwareIndexBufferPtr&indBuff=mesh->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,6,video::IHardwareIndexBuffer::EUT_StaticWriteOnly,false);
	indBuff->writeData(0,indBuff->getSizeInBytes(),ind);

	tc[0]=math::vector2d(1,1);
	tc[1]=math::vector2d(0,1);
	tc[2]=math::vector2d(1,0);
	tc[3]=math::vector2d(0,0);


	uvStream->unlock();

	mesh->setRenderType(video::MR_TRIANGLES);

	
}
SBillboardBeam::~SBillboardBeam(){
}

void SBillboardBeam::SetUseWorldSpace(bool u)
{
	m_useWorldSpace=u;
}
bool SBillboardBeam::GetUseWorldSpace()
{
	return m_useWorldSpace;
}

void SBillboardBeam::setWidth(float width){
	m_Width=width;
	m_dirty=true;
}
float SBillboardBeam::getWidth(){
	return m_Width;
}

void SBillboardBeam::setStartEndPoints(const math::vector3d& start,const math::vector3d& end){
	m_StartPoint=start;
	m_EndPoint=end;
	m_dirty=true;
}

void SBillboardBeam::setStartPoint(const math::vector3d& start){
	m_StartPoint=start;
	m_dirty=true;
}

void SBillboardBeam::setEndPoint(const math::vector3d& end){
	m_EndPoint=end;
	m_dirty=true;
}

const math::vector3d& SBillboardBeam::getStartPoint(){
	return m_StartPoint;
}
const math::vector3d& SBillboardBeam::getEndPoint(){
	return m_EndPoint;
}


bool SBillboardBeam::render(IRenderPass*pass){
	
	video::IVideoDevice*dev=Engine::getInstance().getDevice();
	IViewNode* cam=pass->getViewNode();
	if(!cam || ! dev)return true;

	math::matrix4x4* m=0;
	if(m_useWorldSpace && m_owner)
	{
		m=&m_owner->getAbsoluteTransformation();
	}
	//if(m_dirty)
	{
	//	m_dirty=false;
		MeshGenerator::getInstance().generateBillboardBeam(cam,m_Width,m_StartPoint,
			m_EndPoint,m_mesh->getBuffer(0),m);
	}
	
	//if(m_useWorldSpace)
	{
		dev->setTransformationState(video::TS_WORLD,math::matrix4x4::Identity);
	}
	/*else if(m_owner)
	{
		dev->setTransformationState(video::TS_WORLD,m_owner->getAbsoluteTransformation());

	}*/

	int c=getMesh()->getBuffersCount();
	for(int i=0;i<c;++i)
	{
		MeshBufferData*data=getMesh()->getBufferData(i);
		const video::RenderPassList& lst= data->getMaterial()->GetActiveTechnique()->GetPasses();
		for(int j=0;j<lst.size();++j)
		{
			dev->useRenderPass(lst[j]);
			dev->drawSingleMesh(data->getMeshBuffer());
		}
	}
	return true;
}


}
}

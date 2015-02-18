

#include "stdafx.h"
#include "GUIElement3D.h"
#include "IGUIElementRT.h"
#include "MeshBufferData.h"
#include "RenderPass.h"
#include "RenderTechnique.h"
#include "ISceneNode.h"

namespace mray
{
namespace scene
{

GUIElement3D::GUIElement3D()
	:MeshRenderableNode(0)
{
	m_element=0;
	

	math::vector3d vpos[]={
		math::vector3d(-0.5,0,-0.5),
		math::vector3d( 0.5,0,-0.5),
		math::vector3d( 0.5,0, 0.5),
		math::vector3d(-0.5,0, 0.5)
	};
	math::vector2d vtc[]={
		math::vector2d(1,0),
		math::vector2d( 0,0),
		math::vector2d( 0, 1),
		math::vector2d(1, 1)
	};

	ushort ind[]={0,1,2,0,2,3};
	scene::IMeshBuffer* buf= getMesh()->addNewBuffer()->getMeshBuffer();
	video::IHardwareStreamBuffer* vStream=buf->createStream(0,video::EMST_Position,video::ESDT_Point3f,4,video::IHardwareStreamBuffer::EUT_Static,true);
	video::IHardwareStreamBuffer* uvStream= buf->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,4,video::IHardwareStreamBuffer::EUT_Static,true);
	vStream->writeData(0,vStream->getSizeInBytes(),vpos);
	uvStream->writeData(0,uvStream->getSizeInBytes(),vtc);


	video::IHardwareIndexBuffer*indBuff=buf->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,6,video::IHardwareIndexBuffer::EUT_StaticWriteOnly,false);
	indBuff->writeData(0,indBuff->getSizeInBytes(),ind);
	math::box3d b;
	b.MinP.set(-0.5,0,-0.5);
	b.MaxP.set(0.5,0,0.5);
	buf->setBoundingBox(b);

	buf->setRenderType(video::MR_TRIANGLES);


	video::RenderMaterial* mtrl=new video::RenderMaterial();
	m_rendePass= mtrl->CreateTechnique("")->CreatePass("");
	m_rendePass->setRenderState(video::RS_Lighting,video::ES_DontUse);
	m_rendePass->setRenderState(video::RS_CullFace,video::ES_DontUse);

	setMaterial(mtrl,0);
}

GUIElement3D::~GUIElement3D()
{

}
void GUIElement3D::setOwner(ISceneNode*o)
{
	MeshRenderableNode::setOwner(o);
	if(m_element)
		m_element->SetOwner(o);
}

void GUIElement3D::SetElement(IGUIElementRT* e)
{
	m_element=e;
	if(m_element)
	{
		m_rendePass->setTexture(m_element->GetRenderTexture(),0);
		m_element->SetOwner(getOwner());
		m_element->SetManualRender(true);
	}
	else
		m_rendePass->setTexture(0,0);
}
bool GUIElement3D::preRender(IRenderPass*pass)
{
	bool ret=MeshRenderableNode::preRender(pass);
	if(m_element)
	{
		m_element->DrawElement();
	}
	return ret;
}

}
}
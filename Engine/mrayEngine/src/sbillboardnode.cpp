#include "stdafx.h"


#include "SBillboardNode.h"

#include "IVideoDevice.h"
#include "ISceneManager.h"
#include "IViewNode.h"
#include "MeshGenerator.h"
#include "MeshBufferData.h"
#include "Engine.h"
#include "RenderTechnique.h"
#include "RenderPass.h"

namespace mray{
namespace scene{



SBillboardNode::SBillboardNode(const math::vector2d& size):
MeshRenderableNode(0)
//AngleZ(EPT_BOOLEAN,mT("AngleZ"),mT("zAngle of the billboard)"))
{
	m_angleZ=0;
	StartUp();
	setSize(size);
	m_customRender=true;
}
SBillboardNode::~SBillboardNode()
{
}


void SBillboardNode::StartUp()
{

	MeshBufferData* buffD=getMesh()->addNewBuffer();

	IMeshBuffer* buff=buffD->getMeshBuffer();
//	buffD->getMaterial()->shader=0;
//	buff->currFrame.resize(4);
	//buff->createStream(0,video::EMST_Position,video::ESDT_Point3f,4,video::IHardwareStreamBuffer::EUT_Static,true);

	video::IHardwareStreamBuffer* uvStream=buff->createStream(0,video::EMST_Texcoord,
		video::ESDT_Point2f,4,video::IHardwareStreamBuffer::EUT_Static,true);
	math::vector2df *tc=(math::vector2df *)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);


	tc[0]=math::vector2d(1,0);
	tc[1]=math::vector2d(1,1);
	tc[2]=math::vector2d(0,1);
	tc[3]=math::vector2d(0,0);

	ushort ind[6];

	ind[0]=0;
	ind[1]=1;
	ind[2]=2;
	ind[3]=0;
	ind[4]=2;
	ind[5]=3;

	video::IHardwareIndexBuffer* indBuff=buff->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,6,video::IHardwareIndexBuffer::EUT_StaticWriteOnly,false);
	indBuff->writeData(0,indBuff->getSizeInBytes(),ind);


	buff->setRenderType(video::MR_TRIANGLES);
	buffD->getMaterial()->GetActiveTechnique()->GetPassAt(0)->setRenderState(video::RS_CullFace,video::ES_DontUse);

	uvStream->unlock();

	enableXRotation=enableYRotation=1;

}

void SBillboardNode::setupMesh(){
	return;
//	CameraNode* cam=sceneMngr->getActiveCamera();
// 	if(!cam)return;
// 	float rotZ=m_angleZ;//getOrintation().getRoll();
// 	MeshGenerator::getInstance().generateBillboard(cam,0,1,rotZ,enableYRotation,enableXRotation,billMesh);
}

bool SBillboardNode::preRender(IRenderPass*pass)
{

	setupMesh();

	return MeshRenderableNode::preRender(pass);

}

bool SBillboardNode::render(IRenderPass*pass)
{
	video::IVideoDevice*dev=Engine::getInstance().getDevice();
	
	IViewNode* cam=pass->getViewNode();
	math::vector3d pos=m_owner->getAbsolutePosition();
	if(!cam || ! dev)return true; 
//	float rotZ=getOrintation().getRoll();
	MeshGenerator::getInstance().generateBillboard(cam,pos,nodeSize,m_angleZ,
		enableYRotation,enableXRotation,getMesh()->getBuffer(0));

	dev->setTransformationState(video::TS_WORLD,math::matrix4x4::Identity);
	int c=getMesh()->getBuffersCount();
	for(int i=0;i<c;++i)
	{
		MeshBufferData*data=getMesh()->getBufferData(i);
		const video::RenderPassList& lst= data->getMaterial()->GetActiveTechnique()->GetPasses();
		for(int j=0;j<lst.size();++i)
		{
			dev->useRenderPass(lst[i]);
			dev->drawSingleMesh(data->getMeshBuffer());
		}
	}
	return true; 

}
void SBillboardNode::update(float dt)
{
	MeshRenderableNode::update(dt);
}
void SBillboardNode::setSize(math::vector2d size)
{
	nodeSize=size;
	if(size.x==0)nodeSize.x=1;
	if(size.y==0)nodeSize.y=1;

	float bnd=(size.x+size.y)/5.0f;
	math::box3d b;

	b.MinP.set(-bnd,-bnd,-bnd);
	b.MaxP.set(bnd,bnd,bnd);
	getMesh()->getBuffer(0)->setBoundingBox(b);
}




void SBillboardNode::setTexture(video::ITexturePtr  tex){
	getMesh()->getBufferData(0)->getMaterial()->GetActiveTechnique()->GetPassAt(0)->setTexture(tex,0);
}




xml::XMLElement* SBillboardNode::loadXMLSettings(xml::XMLElement* e)
{
	xml::XMLElement* elem=MeshRenderableNode::loadXMLSettings(e);
	if(!elem)return 0;
	xml::XMLAttribute*attr=elem->getAttribute(mT("RotationX"));
	if(attr)
	{
		enableXRotation=core::StringConverter::toBool(attr->value);
	}
	attr=elem->getAttribute(mT("RotationY"));
	if(attr)
	{
		enableYRotation=core::StringConverter::toBool(attr->value);
	}
	attr=elem->getAttribute(mT("AngleZ"));
	if(attr)
	{
		m_angleZ=core::StringConverter::toFloat(attr->value);
	}
	attr=elem->getAttribute(mT("Size"));
	if(attr)
	{
		nodeSize=core::StringConverter::toVector2d(attr->value);
	}
	return elem;
}

xml::XMLElement* SBillboardNode::exportXMLSettings(xml::XMLElement*elem)
{
	xml::XMLElement*e=MeshRenderableNode::exportXMLSettings(elem);
	e->addAttribute(mT("RotationX"),core::StringConverter::toString(enableXRotation,true));
	e->addAttribute(mT("RotationY"),core::StringConverter::toString(enableYRotation,true));
	e->addAttribute(mT("AngleZ"),core::StringConverter::toString(m_angleZ));
	e->addAttribute(mT("Size"),core::StringConverter::toString(nodeSize));
	return e;
}








}
}

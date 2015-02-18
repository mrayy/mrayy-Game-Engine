#include "stdafx.h"


#include "SSkyBoxNode.h"
#include "ISceneManager.h"
#include "CameraNode.h"
#include "ITexture.h"
#include "IVideoDevice.h"
#include "MeshGenerator.h"

#include "RenderGroup.h"
#include "SMesh.h"
#include "MeshBufferData.h"
#include "RenderTechnique.h"
#include "RenderPass.h"
#include "RenderTechnique.h"
#include "RenderPass.h"

namespace mray{
namespace scene{

SSkyBoxNode::SSkyBoxNode():
ISkyShape()
{
	MeshGenerator::getInstance().generateSkyBox(getMesh());
	/*
	for(int i=0;i<getMesh()->getBuffersCount();++i)
	{
		getMesh()->getBufferData(i)->getMaterial()->GetActiveTechnique()->GetPassAt(0)->getTextureUnit(0)->setTextureClamp(video::ETW_WrapS,video::ETC_CLAMP_TO_EDGE);
		getMesh()->getBufferData(i)->getMaterial()->GetActiveTechnique()->GetPassAt(0)->getTextureUnit(0)->setTextureClamp(video::ETW_WrapT,video::ETC_CLAMP_TO_EDGE);
		getMesh()->getBufferData(i)->getMaterial()->GetActiveTechnique()->GetPassAt(0)->getTextureUnit(0)->setTextureClamp(video::ETW_WrapR,video::ETC_CLAMP_TO_EDGE);
	}*/
	m_customRender=true;

}
 


SSkyBoxNode::~SSkyBoxNode()
{
}

void SSkyBoxNode::SetTexture(video::ITextureCRef skyTex)
{
	video::RenderMaterial* m= getMesh()->getBufferData(0)->getMaterial();
	if(m)
	{
		m->GetActiveTechnique()->GetPassAt(0)->setTexture(skyTex,0);
	}
}


/*

bool SSkyBoxNode::preRender()
{
// 	MeshRenderableNode::preRender();
	// 	return;
	RenderOperation ROP;
	ROP.node=this;
	SMesh* mesh=getMesh();
	for (int i=0;i<mesh->getBuffersCount();++i)
	{
		//video::RenderMaterial*mr=getMaterial(i);
		ROP.mesh=mesh->getBufferData(i);
		m_owner->getSceneManager()->getRenderManager()->addOperation(RGH_Skies,ROP);
	}
	//getSceneManager()->addNodeToRender(this,NRT_SOLID);

	if(Visible)
	{
		sceneMngr->addNodeToRender(this,NRT_SKY);
		ISceneNode::preRender();
	}
}
*/


bool SSkyBoxNode::render(IRenderPass*pass)
{
	video::IVideoDevice*dev=m_owner->getSceneManager()->getDevice();
	if(!dev )return false;

//	return true;
//	dev->pushMatrix();
//	dev->setTranslation(position);
//	dev->set3DMode();
	math::matrix4x4 mat;
	mat.setTranslation(pass->getViewNode()->getViewPos());
	dev->setTransformationState(video::TS_WORLD,mat);

	for(int i=0;i<getMesh()->getBuffersCount();++i)
	{
		scene::MeshBufferData* bd= getMesh()->getBufferData(i);
		if(pass->useMaterial(bd->getMaterial()->GetActiveTechnique()->GetPassAt(0)))
			dev->useRenderPass(bd->getMaterial()->GetActiveTechnique()->GetPassAt(0));
		bd->getShaderCallback()->setConstants(pass->getMaterialShader(bd->getMaterial()->GetActiveTechnique()->GetPassAt(0)),video::IShaderConstantsCallback::ESUT_Both);
		dev->drawSingleMesh(bd->getMeshBuffer());
	}
	m_owner->SetDrown(true);
//	dev->popMatrix();
	return true;
}


}
}

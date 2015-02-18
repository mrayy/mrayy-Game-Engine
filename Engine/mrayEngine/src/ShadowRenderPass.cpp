#include "stdafx.h"


#include "ShadowRenderPass.h"
#include "ShaderResourceManager.h"
#include "Engine.h"
#include "VideoValues.h"
#include "LightNode.h"
#include "CameraNode.h"
#include "ShaderSemanticTable.h"
#include "RenderPass.h"
#include "RenderTechnique.h"


namespace mray{
namespace scene{

ShadowRenderPass::ShadowRenderPass(LightNode*light)
{
	m_camera=0;
	m_light=light;
	m_defaultDepthShader=new video::GPUShader();
	m_defaultDepthShader->SetVertexShader(gShaderResourceManager.loadShaderFromFile(mT("depthShader.cg"), video::EShader_VertexProgram, "main_vp", video::ShaderPredefList(), mT("cg")));
	m_defaultDepthShader->SetFragmentShader(gShaderResourceManager.loadShaderFromFile(mT("depthShader.cg"), video::EShader_FragmentProgram, "main_fp", video::ShaderPredefList(), mT("cg")));
	m_material=new video::RenderPass(0);
	m_material->SetCullFaceDirection(video::ECFM_CW);
}
ShadowRenderPass::~ShadowRenderPass()
{
}

void ShadowRenderPass::beginPass(ISceneManager*smgr,ViewPort*vp)
{
	m_light->prepareForShadowPass(m_camera,this);
	Engine::getInstance().getDevice()->setRenderTarget(
		m_light->getShadowMap(),true,true,1);

//	video::ShaderSemanticTable::getInstance().setViewNode(m_light);
	video::ShaderSemanticTable::getInstance().addLight(m_light);
	smgr->setActiveViewnode(m_light);
	//smgr->setActiveFrustrum(m_camera->getViewFrustrum());

	//use default shadow depth shader
	Engine::getInstance().getDevice()->useShader(m_defaultDepthShader);
}
void ShadowRenderPass::endPass()
{
	Engine::getInstance().getDevice()->setRenderTarget(0,true,true,0);
	video::ShaderSemanticTable::getInstance().disableLights();
}
/*
bool ShadowRenderPass::canRenderType(NODE_RENDER_TYPE t)
{
	if(t==NRT_LIGHT || t==NRT_SKY)
		return false;
	return true;
}*/

void ShadowRenderPass::setActiveCamera(CameraNode* cam)
{
	m_camera=cam;
}
IViewNode* ShadowRenderPass::getViewNode()
{
	return m_light;
}

video::GPUShader* ShadowRenderPass::getMaterialShader(video::RenderPass*m)
{
	if(m && m->GetOwnerTechnique() && m->GetOwnerTechnique()->GetDepthShader()){
		return m->GetOwnerTechnique()->GetDepthShader();
	}
	//couldn't find custom shadow shader,return the default one..
	return m_defaultDepthShader;
}

bool ShadowRenderPass::useMaterial(video::RenderPass*m)
{
	m_material->setRenderShader(getMaterialShader(m));
	Engine::getInstance().getDevice()->useShader(getMaterialShader(m));
	//Engine::getInstance().getDevice()->useMaterial(m_material);
	return false;
}
bool ShadowRenderPass::canRenderObject(IRenderable*r)
{
	if(r->isShadowCaster())
		return true;
	return false;
}

}
}

#include "stdafx.h"

#include "NormalRenderPass.h"
#include "Engine.h"
#include "ISceneManager.h"
#include "CameraNode.h"
#include "ShaderSemanticTable.h"
#include "ViewPort.h"
#include "RenderPass.h"


namespace mray{
namespace scene{

NormalRenderPass::NormalRenderPass()
{
	m_viewport=0;
	m_camera=0;
}
NormalRenderPass::~NormalRenderPass()
{
}

void NormalRenderPass::beginPass(ISceneManager*smgr,ViewPort*vp)
{
	if(m_camera){
		video::ShaderSemanticTable::getInstance().setViewNode(m_camera);
		smgr->setActiveViewnode(m_camera);
		m_camera->render(0);
	}
	m_viewport=vp;
	if(m_viewport)
		m_viewport->onRenderBegin();
}
void NormalRenderPass::endPass()
{
	if(m_viewport)
		m_viewport->onRenderDone();
}
/*
bool NormalRenderPass::canRenderType(NODE_RENDER_TYPE t)
{
	return true;
}*/
void NormalRenderPass::setActiveCamera(CameraNode* cam)
{
	m_camera=cam;
}
IViewNode* NormalRenderPass::getViewNode()
{
	return m_camera;
}

video::GPUShader* NormalRenderPass::getMaterialShader(video::RenderPass*m)
{
	if(!m)
		return 0;
	return m->getRenderShader();
}

bool NormalRenderPass::useMaterial(video::RenderPass*m)
{
	return true;
}
bool NormalRenderPass::canRenderObject(IRenderable*r)
{
	return true;
}

}
}
#include "stdafx.h"

#include "ReflectionRenderPass.h"
#include "Engine.h"
#include "CameraNode.h"
#include "ShaderSemanticTable.h"
#include "RenderPass.h"



namespace mray
{
namespace scene
{


ReflectionRenderPass::ReflectionRenderPass(CameraNode*camera)
{
	m_camera=camera;
}

ReflectionRenderPass::~ReflectionRenderPass()
{
}


void ReflectionRenderPass::SetRenderTarget(GCPtr<video::IRenderTarget> rt)
{
	m_reflectionRT=rt;
}

video::IRenderTarget* ReflectionRenderPass::GetRenderTarget()
{
	return m_reflectionRT;
}

void ReflectionRenderPass::SetReflectionPlane(const math::Plane&p)
{
	m_reflectionPlane=p;
}
const math::Plane& ReflectionRenderPass::GetReflectionPlane()
{
	return m_reflectionPlane;
}
/*
bool ReflectionRenderPass::canRenderType(NODE_RENDER_TYPE t)
{
	return true;
}*/


void ReflectionRenderPass::beginPass(ISceneManager*smgr,ViewPort*vp)
{
	m_camera->enableReflection(m_reflectionPlane);
	video::ShaderSemanticTable::getInstance().setViewNode(m_camera);
	smgr->setActiveViewnode(m_camera);
	m_camera->render(0);
	if(m_reflectionRT)
		Engine::getInstance().getDevice()->setRenderTarget(
			m_reflectionRT,true,true,1);


}

void ReflectionRenderPass::endPass()
{
	Engine::getInstance().getDevice()->setRenderTarget(0,0,0,video::SColor(0,0,0,1));
	m_camera->disableReflection();
}


void ReflectionRenderPass::setActiveCamera(CameraNode* cam)
{
	m_camera=cam;
}

IViewNode* ReflectionRenderPass::getViewNode()
{
	return m_camera;
}


bool ReflectionRenderPass::useMaterial(video::RenderPass*m)
{
	//return true;
	if(!m){
		//Engine::getInstance().getDevice()->setRenderState()
		return true;
	}else{
		video::ECullFaceMode tmp= m->GetCullFaceDirection();
		m->SetCullFaceDirection((tmp==video::ECFM_CW)?video::ECFM_CCW:video::ECFM_CW);
		Engine::getInstance().getDevice()->useRenderPass(m);
		m->SetCullFaceDirection(tmp);
	}
	return false;
}

bool ReflectionRenderPass::canRenderObject(IRenderable*r)
{
	//IRenderable should have flag (Reflectable) to enable/disable them to be reflected
	return true;
}

video::GPUShader* ReflectionRenderPass::getMaterialShader(video::RenderPass*m)
{
	if(!m)
		return 0;
	return m->getRenderShader();
}


}
}





#include "stdafx.h"
#include "CUBEMapRenderNode.h"
#include "Engine.h"
#include "IVideoDevice.h"



namespace mray
{
namespace scene
{

CUBEMapRenderNode::CUBEMapRenderNode(const  core::string&name,int id,ISceneManager* sceneMngr)
:ISceneNode(name,id,sceneMngr)
{
}
CUBEMapRenderNode::~CUBEMapRenderNode()
{
}

void CUBEMapRenderNode::CreateCubeMap(const math::vector2d& sz,video::EPixelFormat fmt)
{
	video::IVideoDevice *dev= Engine::getInstance().getDevice();

	for(int i=0;i<6;++i)
	{
		video::ITexturePtr t=dev->createTexture2D(sz,fmt,true);
		m_faces[i]=Engine::getInstance().getDevice()->createRenderTarget(mT(""),t,0,0,0);
	}
}
void CUBEMapRenderNode::SetCubeMap(const video::IRenderTargetPtr *faces)
{
	for(int i=0;i<6;++i)
	{
		m_faces[i]=faces[i];
	}
}

void CUBEMapRenderNode::preRender()
{
}
void CUBEMapRenderNode::render(IRenderPass*pass)
{
}
void CUBEMapRenderNode::update(float dt)
{
}


}
}

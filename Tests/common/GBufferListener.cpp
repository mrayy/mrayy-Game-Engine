 
#include "stdafx.h"

#include "GBufferListener.h"
#include "ViewPort.h"
#include "IVideoDevice.h"
#include "ShaderResourceManager.h"
#include "DefaultColors.h"

namespace mray{
namespace scene{


GBufferListener::GBufferListener(video::IVideoDevice*dev,scene::ViewPort*vp)
{
	m_device=dev;
	m_vp=vp;

	math::vector2d size=m_vp->getAbsViewPort().getSize();

	m_colorRT=m_device->createTexture2D(size,video::EPixel_Float16_RGBA,true);
	m_normalRT=m_device->createTexture2D(size,video::EPixel_Float16_RGBA,true);
	//m_depthRT=m_device->createTexture2D(size,video::EPixel_Float32_R);

	m_GBuffer=m_device->createRenderTarget(mT("RenderTarget"),m_colorRT,0,0,0);
	m_GBuffer->attachRenderTarget(m_normalRT,1);
	//m_GBuffer->attachRenderTarget(m_depthRT,2);

	m_vp->setAutoClearRT(false);

	m_vp->setRenderTarget(m_GBuffer);

	m_clearRTShader=new video::GPUShader();
	m_clearRTShader->SetVertexShader(gShaderResourceManager.loadShader(mT("clearGeoBuffer.cg"),video::EShader_VertexProgram,"main_vp",mT("cg")));
	m_clearRTShader->SetFragmentShader(gShaderResourceManager.loadShader(mT("clearGeoBuffer.cg"),video::EShader_FragmentProgram,"main_fp",mT("cg")));
}
GBufferListener::~GBufferListener()
{
}

void GBufferListener::onRenderBegin()
{
	m_device->set2DMode();
	//Engine::getInstance().getDevice()->set2DMode();
	m_device->useShader(m_clearRTShader);
	m_device->setRenderTarget(m_GBuffer,0,0);
	//m_callback.setConstants(m_clearRTShader);
	m_device->draw2DImage(math::rectf(0,0,1,1),video::DefaultColors::White);
	m_device->setRenderTarget(0);
	m_device->unuseShader();
	m_device->set3DMode();
}
void GBufferListener::onRenderDone()
{
}

video::IRenderTargetCRef GBufferListener::getGuffer()
{
	return m_GBuffer;
}

video::ITextureCRef GBufferListener::getColorRT()
{
	return m_colorRT;
}

video::ITextureCRef GBufferListener::getNormalRT(){
	return m_normalRT;
}

video::ITextureCRef GBufferListener::getDepthRT(){
	return m_depthRT;
}

}
}

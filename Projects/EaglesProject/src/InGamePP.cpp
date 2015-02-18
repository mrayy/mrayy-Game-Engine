
#include "InGamePP.h"
#include <IRenderTarget.h>
#include <ShaderResourceManager.h>
#include <TextureResourceManager.h>


namespace mray{
namespace gameMod{

InGamePP::InGamePP(video::IVideoDevice*dev,const math::vector2d &size):IPostProcessing(dev)
{
	noiseFactor=0;
	m_finalRT=dev->createRenderTarget(mT("InGamePP_FinalRT"),size,video::EPixelFormat::EPixel_R8G8B8,0);
	m_callback=new PostprocessingShaderConstants();
	m_final=gShaderResourceManager.loadShader("InGamePP.cg","InGamePP.cg",mT("cg"));
	m_noiseTex=gTextureResourceManager.loadTexture2D(mT("Noise128x128.tga"));
}
InGamePP::~InGamePP(){
}

GCPtr<video::IRenderTarget> InGamePP::render(GCPtr<video::IRenderTarget> input){
	if(!isEnabled())return input;

	m_callback->tex0=input->getColorTexture();
	m_callback->tex1=m_noiseTex;

	device->setRenderTarget(m_finalRT);

	device->useShader(m_final);
	m_callback->setConstants(m_final);
	m_final->setPixelShaderConstant("noiseFactor",&noiseFactor,1);
	device->draw2DImage(math::rectf(0,0,1,1),input->getColorTexture(),video::SColor(255));


	device->unuseShader();
	device->setRenderTarget(0);

	return m_finalRT;
}
GCPtr<video::IRenderTarget> InGamePP::getOutput(){
	return m_finalRT;
}


}
}



#include "BloomPP.h"
#include <IRenderTarget.h>
#include <ShaderResourceManager.h>


namespace mray{
namespace gameMod{

BloomPP::BloomPP(video::IVideoDevice*dev,const math::vector2d &size):IPostProcessing(dev)
{
	m_finalRT=dev->createRenderTarget(mT("BloomPP_FinalRT"),size,video::EPixelFormat::EPixel_Float16_RGB,0);
	m_blurRT=dev->createRenderTarget(mT("BloomPP_BlurRT"),size/4,video::EPixelFormat::EPixel_Float16_RGB,0);

	m_callback=new PostprocessingShaderConstants();

	m_uBlur=gShaderResourceManager.loadShader("HBlur.cg","HBlur.cg",mT("cg"));
	m_vBlur=gShaderResourceManager.loadShader("VBlur.cg","VBlur.cg",mT("cg"));
	m_final=gShaderResourceManager.loadShader("BFinal.cg","BFinal.cg",mT("cg"));
}
BloomPP::~BloomPP(){
}

GCPtr<video::IRenderTarget> BloomPP::render(GCPtr<video::IRenderTarget> input){
	if(!isEnabled())return input;

	device->setRenderTarget(m_blurRT);

	//device->set2DMode();
	m_callback->tex0=input->getColorTexture();
	m_callback->setConstants(m_uBlur);
	m_callback->setConstants(m_vBlur);

	for(int i=0;i<1;++i){
		device->useShader(m_uBlur);
		device->draw2DImage(math::rectf(0,0,1,1),input->getColorTexture(),video::SColor(255));

		m_callback->tex0=m_blurRT->getColorTexture();

		device->useShader(m_vBlur);
		device->draw2DImage(math::rectf(0,0,1,1),input->getColorTexture(),video::SColor(255));
	}

	m_callback->tex1=input->getColorTexture();

	device->setRenderTarget(m_finalRT);

	device->useShader(m_final);
	m_callback->setConstants(m_final);
	device->draw2DImage(math::rectf(0,0,1,1),input->getColorTexture(),video::SColor(255));


	device->unuseShader();
	device->setRenderTarget(0);

	return m_finalRT;
}
GCPtr<video::IRenderTarget> BloomPP::getOutput(){
	return m_finalRT;
}


}
}



#include "BloomPP.h"
#include <IRenderTarget.h>
#include <ShaderResourceManager.h>


namespace mray{

BloomPP::BloomPP(video::IVideoDevice*dev,const math::vector2d &size):device(dev)
{
	m_blurTU1=new video::TextureUnit();
	m_blurTU2=new video::TextureUnit();
	m_finalUT=new video::TextureUnit();
	m_renderUT=new video::TextureUnit();

	m_blurTU1->SetTexture(dev->createTexture2D(size,video::EPixelFormat::EPixel_Float16_RGB));
	m_blurTU2->SetTexture(dev->createTexture2D(size,video::EPixelFormat::EPixel_Float16_RGB));
	m_finalUT->SetTexture(dev->createTexture2D(size,video::EPixelFormat::EPixel_Float16_RGB));


	m_finalRT=dev->createRenderTarget(mT("BloomPP_FinalRT"),m_blurTU1->GetTexture(),0,0,0);
	m_blurRT1=dev->createRenderTarget(mT("BloomPP_BlurRT1"),m_blurTU2->GetTexture(),0,0,0);
	m_blurRT2=dev->createRenderTarget(mT("BloomPP_BlurRT2"),m_finalUT->GetTexture(),0,0,0);

	m_callback=new PostprocessingShaderConstants();

	m_uBlur=gShaderResourceManager.loadShader("HBlur.cg","HBlur.cg",mT("cg"));
	m_vBlur=gShaderResourceManager.loadShader("VBlur.cg","VBlur.cg",mT("cg"));
	m_final=gShaderResourceManager.loadShader("BFinal.cg","BFinal.cg",mT("cg"));

}
BloomPP::~BloomPP(){
}

video::IRenderTarget* BloomPP::render(video::IRenderTarget* input){
	if(!isEnabled())return input;


	//device->set2DMode();
	m_callback->tex0->SetTexture(input->getColorTexture());

	m_renderUT->SetTexture(input->getColorTexture());
	for(int i=0;i<1;++i){
		device->setRenderTarget(m_blurRT1);
		m_callback->setConstants(m_uBlur);
		device->useShader(m_uBlur);
		device->draw2DImage(math::rectf(0,0,1,1),m_renderUT,video::SColor(255));

		device->setRenderTarget(m_blurRT2);

		m_callback->tex0->SetTexture(m_blurRT1->getColorTexture());
		m_callback->setConstants(m_vBlur);
		device->useShader(m_vBlur);
		device->draw2DImage(math::rectf(0,0,1,1),m_renderUT,video::SColor(255));
		m_callback->tex0->SetTexture(m_blurRT2->getColorTexture());
	}

	m_callback->tex1->SetTexture(input->getColorTexture());

	device->setRenderTarget(m_finalRT);
	m_callback->tex0->SetTexture(m_blurRT2->getColorTexture());

	device->useShader(m_final);
	m_callback->setConstants(m_final);
	device->draw2DImage(math::rectf(0,0,1,1),m_renderUT,video::SColor(255));


	device->unuseShader();
	device->setRenderTarget(0);

	return m_finalRT;
}
video::IRenderTarget* BloomPP::getOutput(){
	return m_finalRT;
}


}


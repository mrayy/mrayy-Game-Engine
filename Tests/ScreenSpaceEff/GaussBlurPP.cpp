
#include "ShaderResourceManager.h"
#include "GaussBlurPP.h"

namespace mray{
namespace video{


GaussBlurPP::GaussBlurPP(IVideoDevice*dev,const math::vector2d &size){

	device=dev;

	m_blurShader[0]=gShaderResourceManager.getResource(mT("PostProcess/HBlur"));
	m_blurShader[1]=gShaderResourceManager.getResource(mT("PostProcess/VBlur"));

	m_blurTU[0]=new video::TextureUnit();
	m_blurTU[0]->SetTexture(device->createTexture2D(size,video::EPixel_Float16_RGB));
	m_blurRT[0]=device->createRenderTarget(mT("BrightFilterPP_RT"),m_blurTU[0]->GetTexture(),0,0,0);

	m_blurTU[1]=new video::TextureUnit();
	m_blurTU[1]->SetTexture(device->createTexture2D(size,video::EPixel_Float16_RGB));
	m_blurRT[1]=device->createRenderTarget(mT("BrightFilterPP_RT"),m_blurTU[1]->GetTexture(),0,0,0);
}

GaussBlurPP::~GaussBlurPP(){

}


video::IRenderTarget* GaussBlurPP::render(video::IRenderTarget* input){

	m_callback.tex0->SetTexture(input->getColorTexture(0));
	for (int i=0;i<2;++i)
	{
		device->useShader(m_blurShader[i]);
		m_callback.setConstants(m_blurShader[i]);

		device->setRenderTarget(m_blurRT[i]);
		device->draw2DImage(math::rectf(0,0,1,1),0,255);

		m_callback.tex0->SetTexture(m_blurRT[i]->getColorTexture(0));
	}
	device->setRenderTarget(0);
	return m_blurRT[1];
}

video::IRenderTarget* GaussBlurPP::getOutput(){
	return m_blurRT[1];
}

}
}
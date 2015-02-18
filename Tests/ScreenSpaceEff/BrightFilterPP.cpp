

#include "BrightFilterPP.h"
#include "ShaderResourceManager.h"


namespace mray{
namespace video{

BrightFilterPP::BrightFilterPP(IVideoDevice*dev,const math::vector2d &size){

	exposure=0.6;
	threshold=0.7;

	device=dev;

	m_brightFilter=gShaderResourceManager.getResource(mT("PostProcess/BrightFilter"));

	m_tu=new video::TextureUnit();
	m_tu->SetTexture(device->createTexture2D(size,video::EPixel_Float16_RGB));
	m_rt=device->createRenderTarget(mT("BrightFilterPP_RT"),m_tu->GetTexture(),0,0,0);

}
BrightFilterPP::~BrightFilterPP(){
}


video::IRenderTarget* BrightFilterPP::render(video::IRenderTarget* input){


	device->useShader(m_brightFilter);


	m_callback.tex0->SetTexture(input->getColorTexture(0));
	m_callback.tex1->SetTexture(m_luminance);
	m_callback.setConstants(m_brightFilter);


	m_brightFilter->setPixelShaderConstant("Exposure",&exposure,1);
	m_brightFilter->setPixelShaderConstant("Threshold",&threshold,1);

	device->setRenderTarget(m_rt);
	device->draw2DImage(math::rectf(0,0,1,1),0,255);

	device->setRenderTarget(0);
	return m_rt;
}
video::IRenderTarget* BrightFilterPP::getOutput(){
	return m_rt;
}

}
}




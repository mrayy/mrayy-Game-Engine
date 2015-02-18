
#include "ToneMappingPP.h"
#include "ShaderResourceManager.h"


namespace mray{
namespace video{

ToneMappingPP::ToneMappingPP(IVideoDevice*dev,const math::vector2d &size){

	exposure=0.6;

	device=dev;

	m_toneMapping=gShaderResourceManager.getResource(mT("PostProcess/ToneMapping"));

	m_tu=new video::TextureUnit();
	m_tu->SetTexture(device->createTexture2D(size,video::EPixel_Float16_RGB));
	m_rt=device->createRenderTarget(mT("ToneMappingPP_RT"),m_tu->GetTexture(),0,0,0);

}
ToneMappingPP::~ToneMappingPP(){
}


IRenderTarget* ToneMappingPP::render(video::IRenderTarget* input){

	device->useShader(m_toneMapping);


	m_callback.tex0->SetTexture(input->getColorTexture(0));
	m_callback.tex1->SetTexture(m_luminance);
	m_callback.setConstants(m_toneMapping);

	m_toneMapping->setPixelShaderConstant("Exposure",&exposure,1);

	device->setRenderTarget(m_rt);
	device->draw2DImage(math::rectf(0,0,1,1),0,255);

	device->setRenderTarget(0);
	return m_rt;
}
IRenderTarget* ToneMappingPP::getOutput(){
	return m_rt;
}

}
}





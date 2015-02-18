
#include "AdaptiveLuminancePP.h"
#include "ShaderResourceManager.h"


namespace mray{
namespace video{

AdaptiveLuminancePP::AdaptiveLuminancePP(IVideoDevice*dev){


	device=dev;

	m_shader=gShaderResourceManager.getResource(mT("PostProcess/AdaptiveLum"));
	m_copyShader=gShaderResourceManager.getResource(mT("PostProcess/Copy"));

	m_oldLuminance=device->createTexture2D(1,video::EPixel_Float16_RGB);

	m_Lum=new video::TextureUnit();
	m_Lum->SetTexture(device->createTexture2D(1,video::EPixel_Float16_RGB));
	m_rt=device->createRenderTarget(mT("AdaptiveLumPP_RT"),m_Lum->GetTexture(),0,0,0);

	m_copyRt=device->createRenderTarget(mT("AdaptiveLumCopyPP_RT"),m_oldLuminance,0,0,0);

	m_first=true;
}
AdaptiveLuminancePP::~AdaptiveLuminancePP(){
}


IRenderTarget* AdaptiveLuminancePP::render(IRenderTarget* input){

	device->useShader(m_shader);


	m_callback.tex0->SetTexture(m_oldLuminance);

	m_callback.tex1->SetTexture(input->getColorTexture(0));
	m_first=false;

	m_callback.setConstants(m_shader);

	device->setRenderTarget(m_rt);
	device->draw2DImage(math::rectf(0,0,1,1),0,255);

	device->setRenderTarget(0);

	device->useShader(m_copyShader);

	m_callback.tex0->SetTexture(m_Lum->GetTexture());
	m_callback.setConstants(m_copyShader);

	device->setRenderTarget(m_copyRt);
	device->draw2DImage(math::rectf(0,0,1,1),0,255);

	device->setRenderTarget(0);

	return m_rt;
}
IRenderTarget* AdaptiveLuminancePP::getOutput(){
	return m_rt;
}

}
}



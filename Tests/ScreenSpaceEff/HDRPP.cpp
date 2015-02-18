
#include "HDRPP.h"
#include "ShaderResourceManager.h"

#include "CFPS.h"


namespace mray{
namespace video{

HDRPP::HDRPP(IVideoDevice*dev,const math::vector2d &size){

	device=dev;

	m_brightFilter=new BrightFilterPP(dev,size);
	m_toneMapping=new ToneMappingPP(dev,size);
	m_luminance=new LuminancePP(dev,size);
	m_adaptiveLum=new AdaptiveLuminancePP(dev);

	m_gaussBlur=new GaussBlurPP(dev,size/16);

	m_finalShader=gShaderResourceManager.getResource(mT("PostProcess/Sum"));

	m_finalTU=new video::TextureUnit();
	m_finalTU->SetTexture(device->createTexture2D(size,video::EPixel_Float16_RGBA));
	m_finalRT=device->createRenderTarget(mT("HDRFinal_RT"),m_finalTU->GetTexture(),0,0,0);

	exposureAdaptSpeed=0.02;
	m_exposure=0.7;
}
HDRPP::~HDRPP(){
}

IRenderTarget* HDRPP::render(IRenderTarget* input){

	m_luminance->render(input);
	//m_adaptiveLum->render(m_luminance->getOutput());
	//m_toneMapping->m_luminance= m_brightFilter->m_luminance=m_adaptiveLum->m_Lum->texture;


	video::IHardwarePixelBuffer*surf=m_luminance->getOutput()->getColorTexture(0)->getSurface(0);

	video::LockedPixelBox lockedBox= surf->lock(math::box3d(0,math::vector3d(1,1,0)),video::IHardwarePixelBuffer::ELO_ReadOnly);

	float average=((float*)lockedBox.data)[0];
	surf->unlock();

	average=math::Max(average,0.2f);

	m_exposure=math::lerp<float>(m_exposure,0.5/average,exposureAdaptSpeed*gFPS.dt());

	//m_exposure=math::Min(0.7f,m_exposure);

	m_toneMapping->exposure=m_exposure;
	m_brightFilter->exposure=m_exposure;
	m_toneMapping->m_luminance= m_brightFilter->m_luminance=m_luminance->getOutput()->getColorTexture(0);

	m_toneMapping->render(input);
	
	return m_brightFilter->render(m_toneMapping->getOutput());
	m_gaussBlur->render(m_brightFilter->getOutput());


	device->useShader(m_finalShader);


	m_callback.tex0->SetTexture(m_toneMapping->getOutput()->getColorTexture());
	m_callback.tex1->SetTexture(m_gaussBlur->getOutput()->getColorTexture());
	m_callback.setConstants(m_finalShader);

	device->setRenderTarget(m_finalRT);
	device->draw2DImage(math::rectf(0,0,1,1),0,255);

	device->setRenderTarget(0);
	return m_finalRT;
}
IRenderTarget* HDRPP::getOutput(){
	return m_brightFilter->getOutput();
	return m_finalRT;
}


void HDRPP::SetExposure(float e){
	m_exposure=e;
}
float HDRPP::GetExposure()
{
	return m_exposure;
}
void HDRPP::SetThreshold(float t){
	m_brightFilter->threshold=t;
}

}
}

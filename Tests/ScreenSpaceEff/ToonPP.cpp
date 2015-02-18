
#include "ToonPP.h"

#include <IRenderTarget.h>
#include <ShaderResourceManager.h>


namespace mray{
namespace video{

ToonPP::ToonPP(IVideoDevice*dev,const math::vector2d &size){
	m_device=dev;

	m_toonShader=gShaderResourceManager.getResource(mT("Toon/Outline"));

	m_toonTex=dev->createTexture2D(size,video::EPixel_X8R8G8B8);

	m_toonRT=dev->createRenderTarget(mT("ToonPP_FinalRT"),m_toonTex,0,0,0);
}
ToonPP::~ToonPP(){
}

IRenderTarget* ToonPP::render(IRenderTarget* input){
	if(!isEnabled())return input;


	//device->set2DMode();
	m_callback.tex0->SetTexture(m_normalRT);
	m_callback.tex1->SetTexture(m_colorRT);
	m_callback.setConstants(m_toonShader);
	m_device->setRenderTarget(m_toonRT);
	m_device->useShader(m_toonShader);
	m_device->draw2DImage(math::rectf(0,0,1,1),0,video::SColor(255));
	m_device->setRenderTarget(0);
	return m_toonRT;

}
IRenderTarget* ToonPP::getOutput(){
	return m_toonRT;
}

void ToonPP::setColorTexture(GCPtr<video::ITexture> t){
	m_colorRT=t;
}
void ToonPP::setNormalTexture(GCPtr<video::ITexture> t){
	m_normalRT=t;
}
void ToonPP::setDepthTexture(GCPtr<video::ITexture> t){
	m_depthRT=t;
}

}
}
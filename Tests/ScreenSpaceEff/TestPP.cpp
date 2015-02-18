
#include "TestPP.h"
#include "ShaderResourceManager.h"


namespace mray{

TestPP::TestPP(video::IVideoDevice*dev,const math::vector2d &size){

	device=dev;

	m_TU=new video::TextureUnit();
	m_TU->SetTexture(device->createTexture2D(size,video::EPixel_Float16_GR));
	m_TU=device->createRenderTarget(mT("TestPP_RT"),m_TU->GetTexture(),0,0,0);

	m_shader=gShaderResourceManager.getResource(mT("PostProcess/TestPP"));
}
TestPP::~TestPP(){
}



video::IRenderTarget* TestPP::render(video::IRenderTarget* input){

	device->set2DMode();
	device->useShader(m_shader);

	m_callback.tex0->SetTexture(input->getColorTexture(0));
	m_callback.setConstants(m_shader);

	math::vector3d sz=m_callback.tex0->GetTexture()->getSize();

	m_shader->setPixelShaderConstant("TexDim",&sz.x,2);

	device->setRenderTarget(m_RT);
	device->draw2DImage(math::rectf(0,0,1,1),0,255);

	device->setRenderTarget(0);
	device->unuseShader();

	return m_RT;
}
video::IRenderTarget*  TestPP::getOutput(){
	return m_RT;
}


}



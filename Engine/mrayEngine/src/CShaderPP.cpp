#include "stdafx.h"



#include "CShaderPP.h"
#include "IRenderTarget.h"
#include "IVideoDevice.h"


namespace mray{
namespace video{

CShaderPP::CShaderPP(const GPUShaderPtr& shader,video::IVideoDevice*dev,const math::vector2d&size,bool depthBuff):
	device(dev),m_renderTarget(0),m_shader(0),m_targetSize(size)
{
	if(shader==GPUShaderPtr::Null)
		return;

	m_texUnit=new video::TextureUnit();
	//create renderTarget equals to viewport size
	m_texUnit->SetTexture(device->createTexture2D(m_targetSize,video::EPixel_R8G8B8A8,true));
	m_renderTarget=device->createRenderTarget(shader->GetName()+mT("_PP"),m_texUnit->GetTexture(),0,0,depthBuff);
	m_shader=shader;

	m_texUnit->setTextureClamp(video::ETW_WrapR,ETC_CLAMP_TO_EDGE);
	m_texUnit->setTextureClamp(video::ETW_WrapS,ETC_CLAMP_TO_EDGE);
	m_texUnit->setTextureClamp(video::ETW_WrapT,ETC_CLAMP_TO_EDGE);
}

CShaderPP::~CShaderPP(){
	m_renderTarget=0;
	m_shader=0;
}

void CShaderPP::Setup(const math::rectf& targetVP)
{

}

IRenderTarget* CShaderPP::render(IRenderTarget* input)
{
	if(!m_shader || !m_renderTarget || !isEnabled())
		return input;

	math::recti origVP;
  	origVP.ULPoint.x=-1;
  	origVP.ULPoint.y=-1;
  	origVP.BRPoint.x=1;
  	origVP.BRPoint.y=1;
	
	device->setRenderTarget(m_renderTarget,false,false);

	if(m_shader->GetFragmentShader())
	{
		m_texUnit->SetTexture(input->GetColorTexture());
		m_shader->GetFragmentShader()->setTexture(mT("rtColor"),m_texUnit);
	}
	math::vector2d sz=input->GetSize();
	if(m_shader->GetVertexShader())
		m_shader->GetVertexShader()->setConstant(mT("viewPortSize"),&sz.x,2);
	m_shader->use();
		m_texUnit->SetTexture(input->GetColorTexture());
		device->useTexture(0,m_texUnit);
		device->draw2DImage(origVP,SColor(255));
		m_texUnit->SetTexture(0);
	m_shader->unuse();

	device->setRenderTarget(0);

	return m_renderTarget;
}


IRenderTarget*  CShaderPP::getOutput(){
	return m_renderTarget;
}


}
}





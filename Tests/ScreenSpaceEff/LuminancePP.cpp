
#include "LuminancePP.h"
#include "ShaderResourceManager.h"


namespace mray{
namespace video{

LuminancePP::LuminancePP(IVideoDevice*dev,const math::vector2d &size){

	device=dev;

	generateLuminance(size);

	m_lum2x2=gShaderResourceManager.getResource(mT("PostProcess/Luminance2x2"));
	m_lum3x3=gShaderResourceManager.getResource(mT("PostProcess/Luminance3x3"));
}
LuminancePP::~LuminancePP(){
}


void LuminancePP::generateLuminance(const math::vector2d &size){
	int chainLength=1;
	int startSize=math::Min(size.x,size.y);

	int length=1;

	for (length=1;length<startSize;length*=3)
	{
		++chainLength;
	}
	chainLength--;
	m_lumRT=new GCPtr<IRenderTarget>[chainLength];
	m_lumTU=new GCPtr<TextureUnit>[chainLength];

	for (int i=0;i<chainLength;++i)
	{
		m_lumTU[i]=new video::TextureUnit();
		video::EPixelFormat fmt=video::EPixel_Float16_GR;
		if(i==chainLength-1)
			fmt=video::EPixel_Float32_GR;

		m_lumTU[i]->SetTexture(device->createTexture2D(length,fmt));
		m_lumRT[i]=device->createRenderTarget(mT("LumPP_RT"+i),m_lumTU[i]->GetTexture(),0,0,0);
		length/=3;
	}

	m_lumCount=chainLength;
}

video::IRenderTarget* LuminancePP::render(video::IRenderTarget* input){
	
	device->useShader(m_lum2x2);


	m_callback.tex0->SetTexture(input->getColorTexture(0));
	m_callback.setConstants(m_lum2x2);

	math::vector3d sz=m_callback.tex0->GetTexture()->getSize();

	m_lum2x2->setPixelShaderConstant("TexDim",&sz.x,2);

	device->setRenderTarget(m_lumRT[0]);
	device->draw2DImage(math::rectf(0,0,1,1),0,255);

	device->useShader(m_lum3x3);
	for (int i=1;i<m_lumCount;++i)
	{
		device->setRenderTarget(m_lumRT[i]);

		m_callback.tex0->SetTexture(m_lumRT[i-1]->getColorTexture(0));
		sz=m_callback.tex0->GetTexture()->getSize();
		m_callback.setConstants(m_lum3x3);
		m_lum2x2->setPixelShaderConstant("TexDim",&sz.x,2);

		device->draw2DImage(math::rectf(0,0,1,1),0,255);
	}
	device->setRenderTarget(0);
	return m_lumRT[m_lumCount-1];
}
video::IRenderTarget* LuminancePP::getOutput(){
	return m_lumRT[m_lumCount-1];
}

}
}


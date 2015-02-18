
#include "GlarePP.h"
#include <IRenderTarget.h>
#include <ShaderResourceManager.h>


namespace mray{

GlarePP::GlarePP(video::IVideoDevice*dev,const math::vector2d &size):IPostProcessing(dev)
{
	m_finalRT=dev->createRenderTarget(mT("GlarePP_FinalRT"),size,video::EPixel_R8G8B8A8,0);

	for(int i=0;i<maxLuminance;i++){
		int sz=1<<(maxLuminance-i-1);
		m_lumRT[i]=dev->createRenderTarget(mT("GlarePP_LumRT"+i),sz,video::EPixel_LUMINANCE8,0);
	}

	m_blurRT[0]=dev->createRenderTarget(mT("GlarePP_BlurRT"),size/8,video::EPixel_R8G8B8A8,0);
	m_blurRT[1]=dev->createRenderTarget(mT("GlarePP_BlurRT"),size/8,video::EPixel_R8G8B8A8,0);

	m_callback=new PostprocessingShaderConstants();

	m_uBlur=gShaderResourceManager.loadShader("HBlur.cg","HBlur.cg",mT("cg"));
	m_vBlur=gShaderResourceManager.loadShader("VBlur.cg","VBlur.cg",mT("cg"));
	m_lum=gShaderResourceManager.loadShader("Luminance.cg","Luminance.cg",mT("cg"));
	m_final=gShaderResourceManager.loadShader("GlareFinal.cg","GlareFinal.cg",mT("cg"));
}
GlarePP::~GlarePP(){
}

GCPtr<video::IRenderTarget> GlarePP::render(GCPtr<video::IRenderTarget> input){
	if(!isEnabled())return input;


	device->useShader(m_lum);
	m_callback->tex0=input->getColorTexture();
	//Luminance
	for(int i=0;i<maxLuminance;i++){
		device->setRenderTarget(m_lumRT[i]);
		if(i>0){
			m_callback->tex0=m_lumRT[i-1]->getColorTexture();
		}
		m_callback->setConstants(m_lum);
		device->draw2DImage(math::rectf(0,0,1,1),m_callback->tex0,video::SColor(255));
	}


	//UBlur
	device->setRenderTarget(m_blurRT[0]);
	m_callback->tex0=input->getColorTexture();
	m_callback->setConstants(m_uBlur);
	device->useShader(m_uBlur);
	device->draw2DImage(math::rectf(0,0,1,1),input->getColorTexture(),video::SColor(255));

	//VBlur
	device->setRenderTarget(m_blurRT[1]);
	m_callback->tex0=m_blurRT[0]->getColorTexture();
	m_callback->setConstants(m_vBlur);
	device->useShader(m_vBlur);
	device->draw2DImage(math::rectf(0,0,1,1),input->getColorTexture(),video::SColor(255));

	//Final
	m_callback->tex0=input->getColorTexture();
	m_callback->tex1=m_blurRT[1]->getColorTexture();
	m_callback->tex2=m_lumRT[maxLuminance-1]->getColorTexture();
	device->setRenderTarget(m_finalRT);

	device->useShader(m_final);
	m_callback->setConstants(m_final);
	device->draw2DImage(math::rectf(0,0,1,1),input->getColorTexture(),video::SColor(255));


	device->unuseShader();
	device->setRenderTarget(0);

	return m_finalRT;
}
GCPtr<video::IRenderTarget> GlarePP::getOutput(){
	return m_finalRT;
}


}



#include "GlarePP.h"
#include <IRenderTarget.h>
#include <ShaderResourceManager.h>


namespace mray{

GlarePP::GlarePP(video::IVideoDevice*dev,const math::vector2d &size):device(dev)
{
	m_renderUT=new video::TextureUnit();

	m_finalTU=new video::TextureUnit();
	m_finalTU->SetTexture(dev->createTexture2D(size,video::EPixel_Float16_RGB));
	m_finalRT=dev->createRenderTarget(mT("GlarePP_FinalRT"),m_finalTU->GetTexture(),0,0,0);

	for(int i=0;i<maxLuminance;i++){
		int sz=1<<(maxLuminance-i-1);

		m_lumTU[i]=new video::TextureUnit();
		m_lumTU[i]->SetTexture(dev->createTexture2D(sz,video::EPixel_Float16_RGB));
		m_lumRT[i]=dev->createRenderTarget(mT("GlarePP_LumRT"+i),m_lumTU[i]->GetTexture(),0,0,0);
	}

	m_blurTU[0]=new video::TextureUnit();
	m_blurTU[0]->SetTexture(dev->createTexture2D(size/16,video::EPixel_Float16_RGB));
	m_blurRT[0]=dev->createRenderTarget(mT("GlarePP_BlurRT"),m_blurTU[0]->GetTexture(),0,0,0);
	m_blurTU[1]=new video::TextureUnit();
	m_blurTU[1]->SetTexture(dev->createTexture2D(size/16,video::EPixel_Float16_RGB));
	m_blurRT[1]=dev->createRenderTarget(mT("GlarePP_BlurRT"),m_blurTU[1]->GetTexture(),0,0,0);

	m_callback=new PostprocessingShaderConstants();

	m_uBlur=gShaderResourceManager.loadShader("HBlur.cg","HBlur.cg",mT("cg"));
	m_vBlur=gShaderResourceManager.loadShader("VBlur.cg","VBlur.cg",mT("cg"));
	m_lum=gShaderResourceManager.loadShader("Luminance.cg","Luminance.cg",mT("cg"));
	m_final=gShaderResourceManager.loadShader("GlareFinal.cg","GlareFinal.cg",mT("cg"));
}
GlarePP::~GlarePP(){
}

video::IRenderTarget* GlarePP::render(video::IRenderTarget* input){
	if(!isEnabled())return input;


	device->useShader(m_lum);
	m_callback->tex0->SetTexture(input->getColorTexture());
	//Luminance
	for(int i=0;i<maxLuminance;i++){
		device->setRenderTarget(m_lumRT[i]);
		if(i>0){
			m_callback->tex0->SetTexture(m_lumRT[i-1]->getColorTexture());
		}
		m_callback->setConstants(m_lum);
		device->draw2DImage(math::rectf(0,0,1,1),m_callback->tex0,video::SColor(255));
	}


	m_renderUT->SetTexture(input->getColorTexture());
	//UBlur
	device->setRenderTarget(m_blurRT[0]);
	m_callback->tex0->SetTexture(input->getColorTexture());
	m_callback->setConstants(m_uBlur);
	device->useShader(m_uBlur);
	device->draw2DImage(math::rectf(0,0,1,1),m_renderUT,video::SColor(255));

	//VBlur
	device->setRenderTarget(m_blurRT[1]);
	m_callback->tex0->SetTexture(m_blurRT[0]->getColorTexture());
	m_callback->setConstants(m_vBlur);
	device->useShader(m_vBlur);
	device->draw2DImage(math::rectf(0,0,1,1),m_renderUT,video::SColor(255));

	//Final
	m_callback->tex0->SetTexture(input->getColorTexture());
	m_callback->tex1->SetTexture(m_blurRT[1]->getColorTexture());
	m_callback->tex2->SetTexture(m_lumRT[maxLuminance-1]->getColorTexture());
	device->setRenderTarget(m_finalRT);

	device->useShader(m_final);
	m_callback->setConstants(m_final);
	device->draw2DImage(math::rectf(0,0,1,1),m_renderUT,video::SColor(255));


	device->unuseShader();
	device->setRenderTarget(0);

	return m_finalRT;
}
video::IRenderTarget* GlarePP::getOutput(){
	return m_finalRT;
}


}


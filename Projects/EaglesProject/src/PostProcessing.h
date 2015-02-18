
/********************************************************************
	created:	2008/11/18
	created:	18:11:2008   1:20
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\Engine\PostProcessing.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\Engine
	file base:	PostProcessing
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PostProcessing___
#define ___PostProcessing___

#include <IPostProcessing.h>
#include <LogManager.h>
#include <IRenderTarget.h>
#include <ShaderResourceManager.h>
#include "ShaderCallback.h"

namespace mray{

class GrayscalePP:public video::IPostProcessing
{
	GCPtr<video::IRenderTarget> m_renderTarget;
	GCPtr<video::IGPUShaderProgram>m_Shader;
	GCPtr<PostprocessingShaderConstants> m_callback;
public:
	GrayscalePP(video::IVideoDevice*dev):IPostProcessing(dev)
	{
		//create renderTarget equals to viewport size
		m_renderTarget=device->createRenderTarget(mT("GrayscaleRT"),256,video::EPixel_R8G8B8,false);
		m_callback=new PostprocessingShaderConstants();
		m_Shader=gShaderResourceManager.loadShader("grayscale.cg","grayscale.cg",mT("cg"));

		if(!m_Shader){
			gLogManager.log(mT("couldn't create Grayscale shader!!"),ELL_WARNING);
		}else{
			gLogManager.log(mT("Grayscale shader created"),ELL_SUCCESS);
		}

	}

	virtual GCPtr<video::IRenderTarget> render(GCPtr<video::IRenderTarget> input){

		if(!m_renderTarget || !isEnabled())return input;


		device->setRenderTarget(m_renderTarget);

		//device->set2DMode();
		m_callback->tex0=input->getColorTexture();
		device->useShader(m_Shader);
		m_callback->setConstants(m_Shader);
		device->draw2DImage(math::rectf(0,0,1,1),input->getColorTexture(),video::SColor(255));

		device->unuseShader();
		device->setRenderTarget(0);

		return m_renderTarget;
	}
	virtual GCPtr<video::IRenderTarget> getOutput(){

		return m_renderTarget;
	}
};

}


#endif //___PostProcessing___

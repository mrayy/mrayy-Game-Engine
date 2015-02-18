
/********************************************************************
	created:	2009/09/17
	created:	17:9:2009   4:43
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff\ToonPP.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff
	file base:	ToonPP
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ToonPP___
#define ___ToonPP___

#include <IPostProcessing.h>
#include <IVideoDevice.h>
#include "ShaderCallback.h"

namespace mray{
namespace video{

class ToonPP:public IPostProcessing
{
private:
protected:
	PostprocessingShaderConstants m_callback;
	IVideoDevice*m_device;

	GCPtr<video::IGPUShaderProgram> m_toonShader;

	GCPtr<video::ITexture> m_colorRT;
	GCPtr<video::ITexture> m_normalRT;
	GCPtr<video::ITexture> m_depthRT;
public:
	GCPtr<video::IRenderTarget> m_toonRT;
	GCPtr<video::ITexture> m_toonTex;

	ToonPP(IVideoDevice*dev,const math::vector2d &size);
	virtual~ToonPP();

	virtual IRenderTarget* render(IRenderTarget* input);
	virtual IRenderTarget* getOutput();

	void setColorTexture(GCPtr<video::ITexture> t);
	void setNormalTexture(GCPtr<video::ITexture> t);
	void setDepthTexture(GCPtr<video::ITexture> t);
};

}
}

#endif //___ToonPP___

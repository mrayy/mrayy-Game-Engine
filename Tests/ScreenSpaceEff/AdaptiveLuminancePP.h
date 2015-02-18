/********************************************************************
	created:	2009/09/06
	created:	6:9:2009   22:33
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff\AdaptiveLuminancePP.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff
	file base:	AdaptiveLuminancePP
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___AdaptiveLuminancePP___
#define ___AdaptiveLuminancePP___


#include <IPostProcessing.h>
#include <IVideoDevice.h>
#include "ShaderCallback.h"

namespace mray{
namespace video{

class AdaptiveLuminancePP:public IPostProcessing
{
private:
protected:
	IVideoDevice*device;

	GCPtr<IRenderTarget> m_rt;
	GCPtr<IRenderTarget> m_copyRt;

	GCPtr<IGPUShaderProgram> m_shader;
	GCPtr<IGPUShaderProgram> m_copyShader;

	PostprocessingShaderConstants m_callback;

	bool m_first;

public:
	GCPtr<ITexture> m_oldLuminance;
	GCPtr<TextureUnit> m_Lum;

	AdaptiveLuminancePP(IVideoDevice*dev);
	virtual~AdaptiveLuminancePP();

	virtual IRenderTarget* render(IRenderTarget* input);
	virtual IRenderTarget* getOutput();
};

}
}

#endif //___AdaptiveLuminancePP___


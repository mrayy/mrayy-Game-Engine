/********************************************************************
	created:	2009/09/06
	created:	6:9:2009   23:18
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff\GaussBlurPP.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff
	file base:	GaussBlurPP
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GaussBlurPP___
#define ___GaussBlurPP___


#include <IPostProcessing.h>
#include <IVideoDevice.h>
#include "ShaderCallback.h"

namespace mray{
namespace video{

class GaussBlurPP:public IPostProcessing
{
protected:
	IVideoDevice*device;

	GCPtr<TextureUnit> m_blurTU[2];
	GCPtr<IRenderTarget> m_blurRT[2];

	GCPtr<IGPUShaderProgram> m_blurShader[2];

	PostprocessingShaderConstants m_callback;
public:

	GaussBlurPP(IVideoDevice*dev,const math::vector2d &size);
	virtual~GaussBlurPP();

	virtual IRenderTarget* render(IRenderTarget* input);
	virtual IRenderTarget* getOutput();

};

}
}

#endif //___GaussBlurPP___


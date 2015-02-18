
/********************************************************************
	created:	2009/09/06
	created:	6:9:2009   23:08
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff\BrightFilterPP.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff
	file base:	BrightFilterPP
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___BrightFilterPP___
#define ___BrightFilterPP___

#include <IPostProcessing.h>
#include <IVideoDevice.h>
#include "ShaderCallback.h"

namespace mray{
namespace video{

class BrightFilterPP:public IPostProcessing
{
protected:
	IVideoDevice*device;
	GCPtr<IRenderTarget> m_rt;


	GCPtr<IGPUShaderProgram> m_brightFilter;

	PostprocessingShaderConstants m_callback;
public:
	GCPtr<ITexture> m_luminance;
	GCPtr<TextureUnit> m_tu;

	float exposure;
	float threshold;

	BrightFilterPP(IVideoDevice*dev,const math::vector2d &size);
	virtual~BrightFilterPP();

	virtual IRenderTarget* render(IRenderTarget* input);
	virtual IRenderTarget* getOutput();
};

}
}


#endif //___BrightFilterPP___



/********************************************************************
	created:	2009/09/07
	created:	7:9:2009   0:29
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff\ToneMappingPP.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff
	file base:	ToneMappingPP
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ToneMappingPP___
#define ___ToneMappingPP___

#include <IPostProcessing.h>
#include <IVideoDevice.h>
#include "ShaderCallback.h"

namespace mray{
namespace video{

class ToneMappingPP:public IPostProcessing
{
protected:
	IVideoDevice*device;
	GCPtr<IRenderTarget> m_rt;

	GCPtr<IGPUShaderProgram> m_toneMapping;

	PostprocessingShaderConstants m_callback;
public:
	GCPtr<ITexture> m_luminance;

	GCPtr<TextureUnit> m_tu;

	float exposure;

	ToneMappingPP(IVideoDevice*dev,const math::vector2d &size);
	virtual~ToneMappingPP();

	virtual IRenderTarget* render(IRenderTarget* input);
	virtual IRenderTarget* getOutput();

};

}
}

#endif //___ToneMappingPP___

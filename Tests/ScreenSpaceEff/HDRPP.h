
/********************************************************************
	created:	2009/04/22
	created:	22:4:2009   19:35
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\HDRPP.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	HDRPP
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___HDRPP___
#define ___HDRPP___

#include <IPostProcessing.h>
#include "ShaderCallback.h"

#include "GaussBlurPP.h"
#include "BrightFilterPP.h"
#include "ToneMappingPP.h"
#include "LuminancePP.h"
#include "AdaptiveLuminancePP.h"

namespace mray{
namespace video{

class HDRPP:public IPostProcessing
{
private:

	IVideoDevice*device;

	float m_exposure;
public:

	GCPtr<IRenderTarget> m_finalRT;

	GCPtr<IGPUShaderProgram> m_finalShader;

	PostprocessingShaderConstants m_callback;
public:

	float exposureAdaptSpeed;

	GCPtr<LuminancePP> m_luminance;
	GCPtr<BrightFilterPP> m_brightFilter;
	GCPtr<GaussBlurPP> m_gaussBlur;
	GCPtr<ToneMappingPP> m_toneMapping;
	GCPtr<AdaptiveLuminancePP> m_adaptiveLum;


	GCPtr<TextureUnit> m_finalTU;

	HDRPP(IVideoDevice*dev,const math::vector2d &size);
	virtual~HDRPP();

	virtual IRenderTarget* render(IRenderTarget* input);
	virtual IRenderTarget* getOutput();

	void SetExposure(float e);
	float GetExposure();
	void SetThreshold(float t);
};

}
}

#endif //___HDRPP___

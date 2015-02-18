/********************************************************************
	created:	2009/09/06
	created:	6:9:2009   22:33
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff\LuminancePP.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff
	file base:	LuminancePP
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___LuminancePP___
#define ___LuminancePP___


#include <IPostProcessing.h>
#include <IVideoDevice.h>
#include "ShaderCallback.h"

namespace mray{
namespace video{

class LuminancePP:public IPostProcessing
{
private:
protected:
	IVideoDevice*device;


	GCPtr<IGPUShaderProgram> m_lum2x2;
	GCPtr<IGPUShaderProgram> m_lum3x3;

	PostprocessingShaderConstants m_callback;

	void generateLuminance(const math::vector2d &size);
public:

	int m_lumCount;
	GCPtr<TextureUnit> *m_lumTU;
	GCPtr<IRenderTarget> *m_lumRT;

	LuminancePP(IVideoDevice*dev,const math::vector2d &size);
	virtual~LuminancePP();

	virtual IRenderTarget* render(IRenderTarget* input);
	virtual IRenderTarget* getOutput();
};

}
}

#endif //___LuminancePP___


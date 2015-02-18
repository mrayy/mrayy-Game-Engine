
/********************************************************************
	created:	2009/09/12
	created:	12:9:2009   23:02
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff\DepthOfFieldPP.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff
	file base:	DepthOfFieldPP
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___DepthOfFieldPP___
#define ___DepthOfFieldPP___

#include <IPostProcessing.h>
#include "ShaderCallback.h"

namespace mray{
namespace video{

class DepthOfFieldPP:public IPostProcessing
{
private:
protected:
public:
	DepthOfFieldPP(IVideoDevice*dev,const math::vector2d &size);
	virtual~DepthOfFieldPP();

	virtual GCPtr<IRenderTarget> render(GCPtr<IRenderTarget> input);
	virtual GCPtr<IRenderTarget> getOutput();

	void SetExposure(float e);
	void SetThreshold(float t);

};

}
}


#endif //___DepthOfFieldPP___

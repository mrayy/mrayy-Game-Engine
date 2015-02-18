
/********************************************************************
	created:	2009/04/22
	created:	22:4:2009   19:35
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\InGamePP.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	InGamePP
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___InGamePP___
#define ___InGamePP___

#include <IPostProcessing.h>
#include "ShaderCallback.h"

namespace mray{
namespace gameMod{

class InGamePP:public video::IPostProcessing
{
private:
protected:
	GCPtr<video::IRenderTarget> m_finalRT;
	GCPtr<video::IGPUShaderProgram> m_final;
	GCPtr<PostprocessingShaderConstants> m_callback;
	GCPtr<video::ITexture> m_noiseTex;
public:
	float noiseFactor;
public:

	InGamePP(video::IVideoDevice*dev,const math::vector2d &size);
	virtual~InGamePP();

	virtual GCPtr<video::IRenderTarget> render(GCPtr<video::IRenderTarget> input);
	virtual GCPtr<video::IRenderTarget> getOutput();
};

}
}


#endif //___InGamePP___

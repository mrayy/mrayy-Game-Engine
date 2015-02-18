
/********************************************************************
	created:	2009/04/22
	created:	22:4:2009   19:35
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\BloomPP.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	BloomPP
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___BloomPP___
#define ___BloomPP___

#include <IPostProcessing.h>
#include "ShaderCallback.h"

namespace mray{
namespace gameMod{

class BloomPP:public video::IPostProcessing
{
private:
protected:
	GCPtr<video::IRenderTarget> m_blurRT;
	GCPtr<video::IRenderTarget> m_finalRT;
	GCPtr<video::IGPUShaderProgram> m_uBlur;
	GCPtr<video::IGPUShaderProgram> m_vBlur;
	GCPtr<video::IGPUShaderProgram> m_final;
	GCPtr<PostprocessingShaderConstants> m_callback;
public:
	BloomPP(video::IVideoDevice*dev,const math::vector2d &size);
	virtual~BloomPP();

	virtual GCPtr<video::IRenderTarget> render(GCPtr<video::IRenderTarget> input);
	virtual GCPtr<video::IRenderTarget> getOutput();
};

}
}


#endif //___BloomPP___

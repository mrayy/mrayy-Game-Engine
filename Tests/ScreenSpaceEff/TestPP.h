
/********************************************************************
	created:	2010/02/13
	created:	13:2:2010   16:30
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff\TestPP.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff
	file base:	TestPP
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___TestPP___
#define ___TestPP___

#include <IPostProcessing.h>
#include <IVideoDevice.h>
#include "ShaderCallback.h"

namespace mray{

class TestPP:public video::IPostProcessing
{
private:
protected:
	video::IVideoDevice*device;

	GCPtr<video::TextureUnit> m_TU;
	GCPtr<video::IRenderTarget> m_RT;
	GCPtr<video::IGPUShaderProgram> m_shader;
	PostprocessingShaderConstants m_callback;


public:
	TestPP(video::IVideoDevice*dev,const math::vector2d &size);
	virtual~TestPP();

	virtual video::IRenderTarget* render(video::IRenderTarget* input);
	virtual video::IRenderTarget* getOutput();
};

}


#endif //___TestPP___

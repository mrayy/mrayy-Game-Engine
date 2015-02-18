
/********************************************************************
	created:	2009/08/13
	created:	13:8:2009   4:07
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGLDevice\GLRenderBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGLDevice
	file base:	GLRenderBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GLRenderBuffer___
#define ___GLRenderBuffer___

#include "GLHardwarePixelBuffer.h"

namespace mray{
namespace video{

class MRAY_GL_DLL GLRenderBuffer:public GLHardwarePixelBuffer
{
private:
protected:
	uint m_renderBufferID;
	uint m_numSamples;
public:
	GLRenderBuffer(EPixelFormat format,uint width,uint height,uint numSamples);
	virtual~GLRenderBuffer();


	virtual bool isRenderBuffer(){
		return true;
	}

	void Resize(uint w,uint h,uint d=1);

	uint getRenderBufferID(){return m_renderBufferID;}

	virtual void bindToFrameBuffer(GLenum attachment,uint zoffset);
	virtual bool isInGPU(){
		return true;
	}
};

}
}

#endif //___GLRenderBuffer___


/********************************************************************
	created:	2009/08/12
	created:	12:8:2009   12:13
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGLDevice\GLHardwarePixelBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGLDevice
	file base:	GLHardwarePixelBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GLHardwarePixelBuffer___
#define ___GLHardwarePixelBuffer___

#include <IHardwarePixelBuffer.h>
#include "compileConfig.h"

#   include <GL/glew.h>

namespace mray{
namespace video{

class MRAY_GL_DLL GLHardwarePixelBuffer:public IHardwarePixelBuffer
{
private:
protected:

	GLenum m_internalFormat;
	ELockOptions m_currentLO;
	LockedPixelBox m_buffer;

	virtual LockedPixelBox inner_lock(const math::box3d&lockBox,ELockOptions options);
	virtual void inner_unlock();

	virtual void allocBuffer();
	virtual void deallocBuffer();

	virtual void download(const LockedPixelBox&b);
	virtual void upload(const LockedPixelBox&b);
public:
	GLHardwarePixelBuffer(uint width,uint height,uint depth,ETextureType texType,EPixelFormat format,
		EUsageType usage,uint id);
	virtual~GLHardwarePixelBuffer();

	virtual void Resize(uint w,uint h,uint d);

	virtual void bindToFrameBuffer(GLenum attachment,uint zoffset);

	virtual void blitFromMemory(const LockedPixelBox&b);
	virtual const core::string& getDeviceType();
};

}
}


#endif //___GLHardwarePixelBuffer___

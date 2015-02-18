

/********************************************************************
	created:	2009/08/13
	created:	13:8:2009   1:01
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGLDevice\GLTextureBuffer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGLDevice
	file base:	GLTextureBuffer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GLTextureBuffer___
#define ___GLTextureBuffer___

#include "GLHardwarePixelBuffer.h"

namespace mray{
namespace video{

class GLTextureBuffer:public GLHardwarePixelBuffer
{
private:
protected:
	GLenum m_target;
	GLenum m_glFormat;
	uint m_texID;
	int m_face;
	int m_level;
	GLenum m_targetFace;

	bool m_softwareMipmaps;
public:
	GLTextureBuffer(ETextureType texType,uint texId,int face,int level,EUsageType usage,bool softwareMipmaps);
	virtual~GLTextureBuffer();

	virtual void download(const LockedPixelBox&b);
	virtual void upload(const LockedPixelBox&b);

	void bindToFramebuffer(GLenum attachment, uint zoffset);
	void copyFromFramebuffer(uint zoffset);

	uint getTextureID(){return m_texID;}

	virtual bool isInGPU(){return true;}

	virtual bool isRenderBuffer(){
		return false;
	}
};

}
}


#endif //___GLTextureBuffer___

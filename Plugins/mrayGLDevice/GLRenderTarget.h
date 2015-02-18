
/********************************************************************
	created:	2009/01/23
	created:	23:1:2009   12:51
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGLDevice\GLRenderTarget.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGLDevice
	file base:	GLRenderTarget
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GLRenderTarget___
#define ___GLRenderTarget___

#include <IRenderTarget.h>
#include "GLTexture.h"
#include "marray.h"

namespace mray{
namespace video{
	class GLDev;

class GLRenderTarget:public IRenderTarget
{
	uint m_multiSampleFrameBuff;

	std::vector<GCPtr<RenderTextureBuffer>> m_ColorTex;
	GCPtr<RenderTextureBuffer> m_DepthTex;
	core::string m_name;

	GLDev* m_device;

	math::vector2di m_size;
	int m_numSamples;

	bool checkFrameBufferStatus();
public:
	GLRenderTarget(const core::string&name,GLDev *device,math::vector2di size,int numSamples,
		EPixelFormat format,bool depthBuffer);
	virtual~GLRenderTarget();

	virtual void recreate(math::vector2di newSize,EPixelFormat format,bool depthBuffer);
	virtual math::vector2di getSize();

	virtual video::mrayDev*getDevice();
	virtual void setDevice(video::mrayDev* dev);

	virtual void clear(const video::SColor&c,bool clearBackbuffer,bool clearDepthBuffer);

	virtual void bind();
	virtual void unbind();


	virtual GCPtr<ITexture> getColorTexture(int i=0);
	virtual GCPtr<ITexture> getDepthTexture();

};

}
}


#endif //___GLRenderTarget___

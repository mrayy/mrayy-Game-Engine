
/********************************************************************
	created:	2009/07/18
	created:	18:7:2009   1:17
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGLDevice\GLMRenderTarget.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGLDevice
	file base:	GLMRenderTarget
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GLMRenderTarget___
#define ___GLMRenderTarget___

#include "GLPBuffer.h"
#include "GCPtr.h"
#include "marray.h"
#include "GLPBufferTexture.h"
#include "IRenderTarget.h"

namespace mray{
namespace video{

	class GLDev;

class MRAY_GL_DLL GLMRenderTarget:public IRenderTarget
{
private:
protected:
	GLDev* m_device;

	GCPtr<GLPBuffer> m_pbuffer;
	std::vector<GCPtr<ITexture>> m_textures;
public:
	GLMRenderTarget(video::GLDev* dev,bool floatingPoint,int auxBuffers,int numComponents=4,int bitPerChannel=8);
	virtual~GLMRenderTarget();

	virtual void initialize(const math::vector2di&s,bool shareContexts,bool shareObjects);

	virtual video::mrayDev*getDevice();
	virtual void setDevice(video::mrayDev* dev);

	virtual math::vector2di GetSize();

	virtual void clear(const video::SColor&c,bool clearBackbuffer,bool clearDepthBuffer);

	virtual void attachRenderTarget(const ITexturePtr&tex,uint index=0);
	virtual void deattachRenderTarget(const ITexturePtr&tex,uint index=0);

	virtual void bind();
	virtual void unbind();

	virtual const ITexturePtr& GetColorTexture(int idx);
	virtual const IHardwarePixelBufferPtr& GetDepthBuffer();
	virtual const IHardwarePixelBufferPtr& GetStencilBuffer();

	int GetColorTextureCount(){return m_textures.size();}

	void Resize(int x,int y);

	virtual void GetParameter(const core::string& name,void* param);
};

}
}


#endif //___GLMRenderTarget___


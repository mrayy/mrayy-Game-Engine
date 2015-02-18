/********************************************************************
	created:	2009/03/24
	created:	24:3:2009   10:20
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGLDevice\GLFBORenderTarget.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGLDevice
	file base:	GLFBORenderTarget
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GLFBORenderTarget___
#define ___GLFBORenderTarget___

#include "IRenderTarget.h"
#include "GLTexture.h"
#include "marray.h"

namespace mray{
namespace video{

class MRAY_GL_DLL GLFBORenderTarget:public IRenderTarget
{
	uint m_FrameBuffer;

	bool m_sharedDepthBuffer,m_sharedStencilBuffer;

	core::string m_name;

	math::vector2di m_size;

	int m_usedTarget;
	typedef std::vector<ITexturePtr> TextureList;
	TextureList m_targets;

	GCPtr<GLTexture> m_colorBuffer;

	IHardwarePixelBufferPtr m_depthBuffer;
	IHardwarePixelBufferPtr m_stencilBuffer;

	bool checkFrameBufferStatus();

	GCPtr<ITextureListener> m_listener;



public:

	void OnTextureChanged(ITexture* tex);
public:

	GLFBORenderTarget(const core::string&name);
	~GLFBORenderTarget();
	
	bool initialize(const ITexturePtr& colorBuffer,
		const IHardwarePixelBufferPtr& depthBuffer,const IHardwarePixelBufferPtr& stencilBuffer,bool depthOnly);


	math::vector2di GetSize();

	void bind();
	void unbind();

	void clear(const video::SColor&c,bool clearBackbuffer,bool clearDepthBuffer);

	void refreshAttachment(uint index);

	void attachRenderTarget(const ITexturePtr&tex,uint index=0);
	void deattachRenderTarget(const ITexturePtr&tex,uint index=0);


	const ITexturePtr& GetColorTexture(int idx=0);
	const IHardwarePixelBufferPtr& GetDepthBuffer();
	const IHardwarePixelBufferPtr& GetStencilBuffer();
	int GetColorTextureCount(){return m_targets.size();}

	void Resize(int x,int y);
};

}
}


#endif //___GLFBORenderTarget___


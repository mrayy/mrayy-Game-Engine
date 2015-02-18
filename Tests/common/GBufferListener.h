
/********************************************************************
	created:	2009/09/07
	created:	7:9:2009   22:57
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\common\GBufferListener.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\common
	file base:	GBufferListener
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GBufferListener___
#define ___GBufferListener___

#include "ViewportListener.h"
#include "ViewPort.h"

#include "IShaderConstantsCallback.h"
#include "IRenderTarget.h"

namespace mray{
namespace scene{

class GBufferListener:public IViewportListener
{
private:
protected:
	ViewPort*m_vp;
	video::IVideoDevice*m_device;

	GCPtr<video::IRenderTarget> m_GBuffer;
	GCPtr<video::GPUShader> m_clearRTShader;

	video::IShaderConstantsCallback m_callback;

	GCPtr<video::ITexture> m_colorRT;
	GCPtr<video::ITexture> m_normalRT;
	GCPtr<video::ITexture> m_depthRT;
public:
	GBufferListener(video::IVideoDevice*dev,scene::ViewPort*vp);

	virtual~GBufferListener();

	virtual void onRenderBegin();
	virtual void onRenderDone();

	video::ITextureCRef getColorRT();
	video::ITextureCRef getNormalRT();
	video::ITextureCRef getDepthRT();

	video::IRenderTargetCRef getGuffer();
};

}
}


#endif //___GBufferListener___

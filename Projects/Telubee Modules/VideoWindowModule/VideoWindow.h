

#ifndef __VideoWINDOW__
#define __VideoWINDOW__

#include "OptionContainer.h"
#include "VideoGrabberTexture.h"
#include "ViewportListener.h"
#include "ParsedShaderPP.h"
#include "GstNetworkVideoPlayer.h"
#include "RenderWindow.h"

namespace mray
{

namespace TBee
{
	class TBeeServiceContext;

class VideoWindow:public scene::IViewportListener,public video::IRenderWindowListener
{
protected:

	scene::ViewPort* m_VideoViewPort;
	video::VideoGrabberTexture* m_VideoGrabber;
	int m_VideoMonitor;
	video::RenderWindow* m_VideoWnd;
	GCPtr<video::ParsedShaderPP> m_I420ToRGB;
	video::GstNetworkVideoPlayer* m_player;
	GCPtr<InputManager> m_inputManager;

	math::rectf m_projectionRect;

	TBeeServiceContext* m_context;
public:
	VideoWindow();
	virtual ~VideoWindow();

	void Parse(const OptionContainer& opt);

	bool OnInit(TBeeServiceContext* context);
	void OnClose();
	void OnEnable();
	void OnDisable();
	bool IsActive(){ return m_VideoWnd != 0; }

	void OnConnected(const core::string &ipaddr, int VideoPort, bool rtcp);
	void OnDisconnected();

	void OnUpdate(float dt);
	void onRenderDone(scene::ViewPort*vp);
	virtual void WindowPostRender(video::RenderWindow* wnd);

	video::RenderWindow* GetVideoWindow(){
		return m_VideoWnd;
	}
};

}
}

#endif

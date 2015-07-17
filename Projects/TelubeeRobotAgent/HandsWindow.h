

#ifndef __HANDSWINDOW__
#define __HANDSWINDOW__

#include "OptionContainer.h"
#include "VideoGrabberTexture.h"
#include "ViewportListener.h"
#include "ParsedShaderPP.h"
#include "GstNetworkVideoPlayer.h"

namespace mray
{

	class TRApplication;

class HandsWindow:public scene::IViewportListener
{
protected:

	scene::ViewPort* m_handsViewPort;
	video::VideoGrabberTexture* m_handsGrabber;
	int m_handsMonitor;
	video::RenderWindow* m_handsWnd;
	GCPtr<video::ParsedShaderPP> m_undistortShader;
	GCPtr<video::ParsedShaderPP> m_I420ToRGB;
	video::GstNetworkVideoPlayer* m_player;

	TRApplication* m_app;

	math::rectf m_projectionRect;
public:
	HandsWindow();
	virtual ~HandsWindow();

	void Parse(const OptionContainer& opt);

	bool OnInit(TRApplication* app);
	void OnClose();
	void OnEnable();
	void OnDisable();
	bool IsActive(){ return m_handsWnd != 0; }

	void OnConnected(const core::string &ipaddr, int handsPort, bool rtcp);
	void OnDisconnected();

	void OnUpdate(float dt);
	void onRenderDone(scene::ViewPort*vp);
};

}

#endif






/********************************************************************
	created:	2013/12/05
	created:	5:12:2013   20:26
	filename: 	C:\Development\mrayEngine\Projects\TelubeeARStreamer\ARApplication
	file base:	ARApplication
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ARApplication_h__
#define ARApplication_h__

#include "CMRayApplication.h"
#include "GUIBatchRenderer.h"
#include "ICameraVideoGrabber.h"
#include "VideoGrabberTexture.h"
#include "ViewPort.h"


namespace mray
{

class RobotCommunicator;
class GstVideoGrabberImpl;
class IRobotCommunicatorListener;
class IMessageSink;
class ARApplication :public CMRayApplication, public scene::IViewportListener
{
protected:
	scene::ViewPort* m_viewPort;

	GCPtr<GUI::GUIBatchRenderer> m_guiRender;

	GCPtr<video::ICameraVideoGrabber> m_cameras[2];

	video::VideoGrabberTexture m_cameraTextures[3];
	video::VideoGrabberTexture* m_playerGrabber;

	video::ITexturePtr m_rtTexture;
	video::IRenderTargetPtr m_renderTarget;;

	network::NetAddress m_remoteAddr;

	bool m_debugging;

	void _InitResources();

public:
	ARApplication();
	virtual~ARApplication();

	virtual void onEvent(Event* event);

	virtual void init(const OptionContainer &extraOptions);
	virtual void draw(scene::ViewPort* vp);
	virtual void WindowPostRender(video::RenderWindow* wnd);
	virtual void update(float dt);
	virtual void onDone();
	virtual void onRenderDone(scene::ViewPort*vp);

};

}

#endif // ARApplication_h__



/********************************************************************
	created:	2014/02/16
	created:	16:2:2014   12:32
	filename: 	C:\Development\mrayEngine\AugTelWhiteBoard\Application.h
	file path:	C:\Development\mrayEngine\AugTelWhiteBoard
	file base:	Application
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __Application__
#define __Application__



#include "CMRayApplication.h"
#include "GUIBatchRenderer.h"
#include "GUIManager.h"
#include "ViewPort.h"
#include "LineDrawer.h"
#include "DirectShowVideoGrabber.h"
#include "VideoGrabberTexture.h"

namespace mray
{
	namespace video
	{
		class CVCalibration;
	}

	class OptiTrackDataSource;
	class CVData;

class Application :public CMRayApplication, public scene::IViewportListener,public ISingleton<Application>
{
protected:
	scene::ViewPort* m_mainVP;
	ILineDrawer *m_lineDrawer;

	OptiTrackDataSource* m_optiProvider;

	video::IRenderTargetPtr m_rt;

	CVData* m_cvData;
	
	GCPtr<video::DirectShowVideoGrabber> m_cam;

	GCPtr<video::VideoGrabberTexture> m_videoGrabber;

public:
	Application();
	virtual~Application();
	virtual void onEvent(Event* event);

	OptiTrackDataSource* GetOptiProvider(){
		return m_optiProvider;
	}

	virtual void init(const OptionContainer &extraOptions);

	virtual void draw(scene::ViewPort* vp);
	virtual void WindowPostRender(video::RenderWindow* wnd);
	virtual void update(float dt);
	virtual void onDone();

	virtual void onRenderDone(scene::ViewPort*vp);
};

}


#endif

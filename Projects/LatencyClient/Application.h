

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
#include "IUDPClient.h"

namespace mray
{
	class UDPReceivingThread;

class Application :public CMRayApplication, public scene::IViewportListener,public ISingleton<Application>
{
protected:
	scene::ViewPort* m_mainVP;
	video::IRenderTargetPtr m_rt;


	OS::IMutex* m_mutex;
	network::IUDPClient* m_rcvClient;
	UDPReceivingThread* m_rcvThreadFunc;
	OS::IThread* m_rcvThread;

	bool m_showColor;

	GUI::GUIBatchRendererPtr m_guiRenderer;

	void StartReciver(int port);
public:
	Application();
	virtual~Application();
	virtual void onEvent(Event* event);


	virtual void init(const OptionContainer &extraOptions);

	virtual void draw(scene::ViewPort* vp);
	virtual void WindowPostRender(video::RenderWindow* wnd);
	virtual void update(float dt);
	virtual void onDone();

	virtual void onRenderDone(scene::ViewPort*vp);
	virtual void _ReceiveData(OS::IStream* stream);
};

}


#endif

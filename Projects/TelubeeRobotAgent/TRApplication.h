

/********************************************************************
	created:	2013/12/01
	created:	1:12:2013   23:23
	filename: 	C:\Development\mrayEngine\Projects\TelubeeRobotAgent\TRApplication.h
	file path:	C:\Development\mrayEngine\Projects\TelubeeRobotAgent
	file base:	TRApplication
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __TRApplication__
#define __TRApplication__



#include "CMRayApplication.h"
#include "GUIBatchRenderer.h"
#include "ViewPort.h"
#include "IRobotController.h"

#if USE_OPENNI
#include "OpenNIHandler.h"
#include "GeomDepthRect.h"
#include "OpenNIManager.h"
#endif

#include "IUDPClient.h"
#include "CameraProfile.h"
#include "GstPlayerBin.h"
#include "RenderWindow.h"
#include "ParsedShaderPP.h"
#include "VideoGrabberTexture.h"
#include "IServiceModule.h"

#if USE_HANDS
#include "HandsWindow.h"
#endif

#include "IStreamListener.h"
#include "CommunicationMessages.h"
#include "TBeeServiceContext.h"
#include "RobotCommunicator.h"

namespace mray
{

class GstVideoGrabberImpl;


class TRApplication :public CMRayApplication, public scene::IViewportListener,public video::IGStreamerStreamerListener,public TBee::IRobotCommunicatorListener
{
protected:

	enum class EController
	{
		XBox,
		Logicool
	};

	TBee::TBeeServiceContext m_serviceContext;
	TBee::TbeeServiceRenderContext m_renderContext;

	bool m_robotInited;

	EController m_controller;
	scene::ViewPort* m_viewPort;

	GCPtr<GUI::GUIBatchRenderer> m_guiRender;


	std::vector<TBee::IServiceModule*> m_services;

	TBee::RobotCommunicator* m_robotCommunicator;

#if USE_OPENNI
	TBee::OpenNIHandler* m_openNi;
	TBee::GeomDepthRect m_depthRect;
	GCPtr<OpenNIManager> m_openNIMngr;
	bool m_depthSend;
#endif



	bool m_isStarted;

	bool m_debugging;
	bool m_enablePlayers;
	bool m_enableStream;



	struct DebugData
	{
		DebugData()
		{
			userConnected = false;
		}
		bool userConnected;
		network::NetAddress userAddress;
		bool debug;
	}m_debugData;

	void _InitResources();

	bool m_startVideo;

	bool m_isDone;

public:
	TRApplication();
	virtual~TRApplication();


	virtual void onEvent(Event* event);
	virtual void onClose();

	virtual void init(const OptionContainer &extraOptions);

	virtual void draw(scene::ViewPort* vp);
	virtual void WindowPostRender(video::RenderWindow* wnd);
	virtual void update(float dt);
	virtual void onDone();

	virtual void onRenderDone(scene::ViewPort*vp);

	virtual void OnUserConnected(TBee::RobotCommunicator* sender, const TBee::UserConnectionData& data);
	void OnRobotStatus(TBee::RobotCommunicator* sender, const RobotStatus& status);
	void OnUserDisconnected(TBee::RobotCommunicator* sender, const network::NetAddress& address);
	void OnUserMessage(network::NetAddress* addr, const core::string& msg, const core::string& value);


#if USE_PLAYERS
	//GCPtr<video::GstPlayerBin> GetPlayers(){ return m_players; }
#endif


	void OnStreamerReady(video::IGStreamerStreamer* s);
	void OnStreamerStarted(video::IGStreamerStreamer* s);
	void OnStreamerStopped(video::IGStreamerStreamer* s);


};

}


#endif

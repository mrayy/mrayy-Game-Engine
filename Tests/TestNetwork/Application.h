

#ifndef __Application__
#define __Application__

/********************************************************************
	created:	2011/04/18
	created:	18:4:2011   12:04
	filename: 	d:\Development\mrayEngine\Tests\TestNetwork\Application.h
	file path:	d:\Development\mrayEngine\Tests\TestNetwork
	file base:	Application
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#include "CMRayApplication.h"

#include "GUIBatchRenderer.h"
#include "RemoteDebugger.h"
#include "RemoteDebugClient.h"

namespace mray
{

class Application:public CMRayApplication
{
private:
protected:
	scene::CameraNode* m_camera;
	scene::ViewPort* m_viewPort;
	GUI::GUIBatchRenderer* m_guiRender;

	debug::RemoteDebugger* m_remoteDebugger;
	debug::RemoteDebugClient* m_remoteClient;

	GCPtr<network::INetwork> m_network;

	network::ISocket* m_socket;

public:
	Application();
	virtual~Application();

	virtual void init(core::array<SOptionElement> *extraOptions);
	virtual void update(float dt);
	virtual void draw();

};


}

#endif
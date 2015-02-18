


/********************************************************************
	created:	2012/06/26
	created:	26:6:2012   14:53
	filename: 	d:\Development\mrayEngine\Tests\PhysicsTest\Application.h
	file path:	d:\Development\mrayEngine\Tests\PhysicsTest
	file base:	Application
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __Application__
#define __Application__

#include "IDelegateContainer.h"
#include "CMRayApplication.h"
#include "GUIBatchRenderer.h"
#include "GUIManager.h"
#include "ViewPort.h"
#include "SParticleSystem.h"
#include "GUIManagerRT.h"


namespace mray
{
class Application:public CMRayApplication,public IDelegateContainer
{
private:
protected:
	GCPtr<scene::LightNode> m_light;
	GCPtr<scene::CameraNode> m_camera;
	GCPtr<GUI::GUIManager> m_guiManager;
	scene::ViewPort* m_viewPort;
	scene::ViewPort* m_guiViewPort;	
	GCPtr<scene::GUIManagerRT> m_guiRtManager;

	//overlay1 m_overlayElement;

	GCPtr<scene::SParticleSystem> m_particles;

	DECLARE_OBJECT_DELEGATE(PrintHello);
public:
	GCPtr<GUI::GUIBatchRenderer> m_guiRender;

	Application();
	virtual~Application();

	virtual void onEvent(Event* event);

	virtual void init(const OptionContainer &extraOptions);

	virtual void draw(scene::ViewPort* vp);
	virtual void WindowPostRender(video::RenderWindow* wnd);
	virtual void update(float dt);
	virtual void onDone();

};


}

#endif

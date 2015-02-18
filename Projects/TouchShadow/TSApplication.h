

/********************************************************************
	created:	2013/08/16
	created:	16:8:2013   11:38
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\TSApplication.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	TSApplication
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __TSApplication__
#define __TSApplication__



#include "CMRayApplication.h"
#include "GUIBatchRenderer.h"
#include "GameEntityManager.h"
#include "DeviceController.h"
#include "IDebugDrawManager.h"
#include "sparticlesystem.h"
#include "AudioSoundInterface.h"
#include "ViewportListener.h"
#include "ParsedShaderPP.h"



namespace mray
{

	class OpenNIManager;
	class OpenNICaptureDevice;
	class InteractionDemo;
	class EffectManager;

class TSApplication:public CMRayApplication,public scene::IViewportListener
{
protected:
	void _InitResources();
	void CreatePhysicsSystem();
	GCPtr<GUI::GUIBatchRenderer> m_guiRender;
	GCPtr<physics::IPhysicManager>		m_phManager;
	GCPtr<game::GameEntityManager> m_gameManager;
	GCPtr<scene::IDebugDrawManager> m_dbgMngr;

	std::vector<InteractionDemo*> m_demos;

	OpenNIManager* m_openNiManager;

	GCPtr<DeviceController> m_deviceController;

	GCPtr<scene::SParticleSystem> m_rainPS;

	video::ParsedShaderPP::PassValue* m_rgbShiftValue;

	video::RenderWindow* m_previewWnd;
	scene::ViewPort* m_vp;
	scene::ViewPort* m_previewVP;
	scene::CameraNode* m_camera;

	GCPtr<EffectManager> m_effects;

	video::ITexturePtr m_logo;
	video::ITexturePtr m_title;

	video::IRenderTargetPtr m_finalRT;


	void LoadDemos(const core::string& path);

	void DrawDebug(scene::ViewPort* vp);
public:
	static TSApplication* Instance;

	static TSApplication* getInstance(){return Instance;}

	TSApplication();
	virtual~TSApplication();


	void SetActiveDemo(InteractionDemo* d);

	virtual void onEvent(Event* event);

	virtual void init(const OptionContainer &extraOptions);

	virtual void WindowPreViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp);
	virtual void WindowPostViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp);
	virtual void update(float dt);
	virtual void onDone();

	virtual void onRenderDone(scene::ViewPort*vp);


	scene::CameraNode* getCamera(){return m_camera;}
	game::GameEntityManager* GetGameManager(){return m_gameManager;}
};

}


#endif
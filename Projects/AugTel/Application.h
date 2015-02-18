

/********************************************************************
created:	2012/10/09
created:	9:10:2012   22:07
filename: 	d:\Development\mrayEngine\Projects\AugTel\Application.h
file path:	d:\Development\mrayEngine\Projects\AugTel\
file base:	Application
file ext:	h
author:		MHD YAMEN SARAIJI

purpose:
*********************************************************************/
#ifndef ___Application___
#define ___Application___



#include "CMRayApplication.h"

#include "GUIBatchRenderer.h"
#include "GUIManager.h"
#include "GameEntityManager.h"
#include "ViewPort.h"

#include <ISoundManager.h>
#include <IVideoClipManager.h>

#include "IRenderingState.h"
#include "RenderBlender.h"
#include "RenderingStateManager.h"

#include "ApplicationStateManager.h"
#include "OpenNIManager.h"
#include "TBeeRenderer.h"
#include "WiimoteManager.h"

namespace mray
{
	namespace TBee
	{
		class RenderingStateManager;
	}
	using namespace TBee;

namespace AugTel
{

class ApplicationOculusData;

class Application :public CMRayApplication, public scene::IViewportListener, public ITBeeRendererListener
{
protected:
	scene::ViewPort* m_mainVP;

	GCPtr<GUI::GUIBatchRenderer> m_guiRenderer;
	GCPtr<sound::ISoundManager> m_soundManager;
	GCPtr<video::IVideoClipManager> m_videoManager;
	GCPtr<OpenNIManager> m_openNIMngr;

	TBeeRenderer* m_tbRenderer;


	GCPtr<TBee::ApplicationStateManager> m_appStateManager;
	TBee::RenderingStateManager* m_renderingState;
	GCPtr<controllers::WiimoteManager> m_wiiManager;

	GCPtr<GUI::IGUIManager> m_previewGUI;

	int m_cam1;
	int m_cam2;

	bool m_drawUI;

	bool m_remoteCamera;
	video::ITexturePtr m_screenShot;
	bool m_screenShotEnabled;
	float m_screenShotTimer;

	video::RenderWindow* m_previewWnd;
	video::IRenderTargetPtr m_previewRT;
	void _InitResources();
	void RenderUI(const math::rectf& rc);

	void LoadSettingsXML(const core::string &path);

	void _initStates();
public:
	Application();
	virtual~Application();

	virtual void onEvent(Event* event);

	virtual void init(const OptionContainer &extraOptions);

	virtual void draw(scene::ViewPort* vp);
	virtual void WindowPostRender(video::RenderWindow* wnd);
	virtual void update(float dt);
	virtual void onDone();

	int GetCamera(int c)
	{
		return c == 0 ? m_cam1 : m_cam2;
	}

	virtual void onRenderDone(scene::ViewPort*vp);

	GCPtr<video::IVideoClipManager> GetVideoManager(){ return m_videoManager; }

	virtual void OnRendererDraw(TBeeRenderer* r, const math::rectf& rc, video::IRenderTarget* rt, ETargetEye eye);
	video::RenderWindow* GetPreviewWindow(){ return m_previewWnd; }
	GCPtr<GUI::IGUIManager> GetPreviewGUIManager(){ return m_previewGUI; }

};

}
}
#endif

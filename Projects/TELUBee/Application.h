

/********************************************************************
	created:	2012/10/09
	created:	9:10:2012   22:07
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\Application.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
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
#include "TBeeRenderer.h"
#include "WiimoteManager.h"

namespace mray
{
namespace TBee
{
	class RenderingStateManager;

	class ApplicationOculusData;

class Application:public CMRayApplication,public scene::IViewportListener,public ITBeeRendererListener
{
protected:
	scene::ViewPort* m_mainVP;

	GCPtr<GUI::GUIBatchRenderer> m_guiRender;
	GCPtr<sound::ISoundManager> m_soundManager;
	GCPtr<video::IVideoClipManager> m_videoManager;

	video::ITexturePtr m_screenShot;

	GCPtr<TBee::ApplicationStateManager> m_appStateManager;
	TBee::RenderingStateManager* m_renderingState;


	TBeeRenderer* m_tbRenderer;

	GCPtr<controllers::WiimoteManager> m_wiimoteManager;

	bool m_drawUI;

	int m_cam1;
	int m_cam2;


	void _InitResources();
	void RenderUI(const math::rectf& rc);

	void LoadSettingsXML(const core::string &path);
	void _RenderVP(int i);

	void _initStates();

	void _createViewports();
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

	virtual void OnRendererDraw(TBeeRenderer* r, const math::rectf& vp, video::IRenderTarget* rt, ETargetEye eye);


	GCPtr<video::IVideoClipManager> GetVideoManager(){return m_videoManager;}
};

}
}
#endif

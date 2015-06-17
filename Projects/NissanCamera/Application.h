

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

#include "IRenderingState.h"
#include "RenderBlender.h"
#include "RenderingStateManager.h"

#include "ApplicationStateManager.h"
#include "TBeeRenderer.h"
#include "LocalCameraVideoSource.h"

namespace mray
{
	using namespace  TBee;
namespace NCam
{
class RenderingStateManager;
class ConsoleLogDevice;

class ApplicationOculusData;

class Application :public CMRayApplication, public scene::IViewportListener, public ITBeeRendererListener
{
protected:
	scene::ViewPort* m_mainVP;

	GCPtr<GUI::GUIBatchRenderer> m_guiRender;
	GCPtr<sound::ISoundManager> m_soundManager;

	video::ITexturePtr m_screenShot;

	GCPtr<TBee::ApplicationStateManager> m_appStateManager;
	TBee::RenderingStateManager* m_renderingState;

	core::string m_settingsPath;


	TBeeRenderer* m_tbRenderer;
	TBee::EUSBCameraType m_camType;

	int m_cameraID[2];

	bool m_drawUI;

	void _InitResources();
	void RenderUI(const math::rectf& rc);

	void LoadSettingsXML(const core::string &path);
	void WriteSettingsXML();
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


};

}
}
#endif

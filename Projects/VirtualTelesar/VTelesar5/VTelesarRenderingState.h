

/********************************************************************
	created:	2012/10/10
	created:	10:10:2012   19:50
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\VTelesarRenderingState.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
	file base:	VTelesarRenderingState
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___VTelesarRenderingState___
#define ___VTelesarRenderingState___

#include "IRenderingState.h"
#include "IRenderTarget.h"

#include "CommunicationDataRecorder.h"
#include "RobotCommunicatorComponent.h"
#include "StereoRenderer.h"
#include "RobotCommunicator.h"
#include "GUIMotorConnectionPanel.h"
#include "GUIGraphPlotElement.h"

#include "ParsedShaderPP.h"
#include "BallCreator.h"

#include "ContactCollisionDebugger.h"
#include "ViewportListener.h"

#include "GhostProxyManager.h"
#include "CameraCorrectionGrabber.h"

#include "OculusManager.h"
#include "OculusDevice.h"
#include "OculusCameraComponent.h"

namespace mray
{
namespace VT
{
	class Application;

class VTelesarRenderingState:public TBee::IRenderingState,public scene::IViewportListener
{
protected:

	enum ECameraMode
	{
		SwitchHead,
		FreeMode,
		HeadFront,
		Side,
		Front,
		Back,
		Object
	};


	GCPtr<physics::IPhysicManager>		m_phManager;
	GCPtr<game::GameEntityManager> m_gameManager;
	GCPtr<scene::ISceneManager>		m_sceneManager;
	GCPtr<GUI::IGUIManager>				m_guiManager;

	GCPtr<GUI::GUIMotorConnectionPanel> m_connectionPanel;
	GCPtr<GUI::GUIGraphPlotElement> m_gyroElement;

	GCPtr<game::GhostProxyManager> m_ghostProxyManager;


	GCPtr<scene::CameraNode> m_cameraL;
	GCPtr<scene::CameraNode> m_cameraR;
	GCPtr<scene::CameraNode> m_camera3rd;
	//GCPtr<VT::VTSharedMemory> m_telesarLayer;
	GCPtr<VT::ICommunicationLayer> m_telesarLayer;
	GCPtr<VT::CommunicationDataRecorder> m_dataRecorder;
	GCPtr<scene::ViewPort> m_3rdVP;
	GCPtr<VT::ContactCollisionDebugger> m_collisionDebugger;
	GCPtr<scene::IDebugDrawManager> m_debugRenderer;
	GCPtr<video::CameraTextureSource> m_cameraSource[2];
	//GCPtr<CameraCorrectionGrabber> m_cameraSource[2];

	GCPtr<OS::IThread> m_physicsUpdateThread;

	bool m_firstrender;
	bool m_stereo;

	GCPtr<scene::ViewPort> m_vp[2];
	GCPtr<video::IRenderTarget> m_finalRT[2];

	GCPtr<scene::StereoRenderer> m_stereoRenderer;

	VT::RobotCommunicatorComponent* m_robot;
	scene::IMovable* m_headMount;
	bool m_attachToHead;

	Application* m_app;

	math::rectf m_current3rdVPSize;
	float m_vpUpdatetimer;
	float m_simulationStep;

	std::vector<video::ParsedShaderPP::PassValue*> m_dofFocalPoint;
	math::vector3d m_currentFocalPoint;
	video::ParsedShaderPP::PassValue* m_shaderUseLighting[2];

	BallCreator m_ballCreator;
	bool m_generatingBalls;

	scene::IMovable* m_cup;

	std::vector<scene::IMovable*> m_viewObjects;
	int m_selectedObject;

	bool m_seeThrough;


	core::string m_modelName;

	video::ParsedShaderPP::SubPostEffect* m_preProcessPass[2];
	video::ParsedShaderPP::SubPostEffect* m_maskExtractor[2];
	video::ParsedShaderPP::PassValue* m_useCameraFusion[2];

	enum ECurrenViewMode
	{
		StereoCamera,
		ThirdCamera
	}m_viewMode;

//	GCPtr<TrackIRCamera> m_trackIRCam;

	bool m_closed;
	bool m_loaded;

	bool m_previewRenderingSteps;

	void CreatePhysicsSystem();

	void _SwitchCamera(scene::IMovable* m, ECameraMode mode);
	void _CreateGUI();
	video::ITexture* _RenderRobotVision(TBee::ETargetEye eye);

	void LoadEnvironment(const core::string& path);
	void _Reload(const core::string& scene);
	void _Clear();

	void _calculateFocalPoint(float dt);
public:
	VTelesarRenderingState(const core::string& name);
	virtual~VTelesarRenderingState();

	bool IsClosed(){return m_closed;}

	virtual void InitState();

	virtual void OnEvent(Event* e);
	virtual void OnEnter(IRenderingState*prev);
	virtual void OnExit();
	virtual video::IRenderTarget* Render(const math::rectf& rc, TBee::ETargetEye eye);
	virtual video::IRenderTarget* GetLastFrame(bool left);
	void Update(float dt);
	void _Update(float dt);

	virtual void LoadFromXML(xml::XMLElement* e);

	bool CanSleep(){return false;}


	void onRenderDone(scene::ViewPort*vp);
	
};

}
}

#endif



/********************************************************************
	created:	2012/07/14
	created:	14:7:2012   16:24
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VApplication.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar
	file base:	VApplication
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___VApplication___
#define ___VApplication___


#include "CMRayApplication.h"

#include "GUIBatchRenderer.h"
#include "GUIManager.h"
#include "GameEntityManager.h"
#include "ViewPort.h"
#include "AtmosphericCubeMap.h"
#include "StereoRenderer.h"

#include "VTSharedMemory.h"
#include "CommunicationDataRecorder.h"
#include "GUIGraphPlotElement.h"
#include "RobotCommunicatorComponent.h"
#include <ISoundManager.h>

#include "RobotCommunicator.h"

namespace mray
{

class VApplication:public CMRayApplication
{
protected:

	enum ECameraMode
	{
		SwitchHead,
		FreeMode,
		HeadFront,
		Side,
		Front,
		Back
	};

	scene::ViewPort* m_viewPort;

	GCPtr<GUI::GUIBatchRenderer> m_guiRender;
	GCPtr<physics::IPhysicManager> m_phManager;
	GCPtr<GUI::GUIManager> m_guiManager;
	GCPtr<game::GameEntityManager> m_gameManager;

	GCPtr<game::AtmosphericCubeMap> m_atmosphericCubeMap;

	GCPtr<scene::IDebugDrawManager> m_debugRenderer;

	GCPtr<scene::StereoRenderer> m_stereoRenderer;
	GCPtr<scene::CameraNode> m_cameraL;
	GCPtr<scene::CameraNode> m_cameraR;

	GCPtr<VT::VTSharedMemory> m_telesarLayer;
	GCPtr<VT::RobotCommunicator> m_robotCommunicator;
	GCPtr<VT::CommunicationDataRecorder> m_dataRecorder;

	GCPtr<GUI::GUIGraphPlotElement> m_graphGUI;
	GCPtr<GUI::GUIGraphPlotElement> m_sensorData[6];
	GCPtr<sound::ISoundManager> m_soundManager;
	GCPtr<sound::ISound> m_sound;

	GCPtr<game::IGameComponent> m_motor;

	//scene::IMovable* m_head;
	bool m_attachToHead;

	VT::RobotCommunicatorComponent* m_robot;

	scene::IMovable* m_headMount;

	video::RenderWindow* m_2ndWnd;

	video::ITexturePtr m_screenShot;

	bool m_drawUI;

	bool m_isDebugging;

	float m_angles[10];

	void CreatePhysicsSystem();
	void _InitResources();

	void _SwitchCamera(ECameraMode mode);
public:
	VApplication();
	virtual~VApplication();
	
	virtual void onEvent(Event* event);

	virtual void init(const OptionContainer &extraOptions);

	virtual void draw(scene::ViewPort* vp);
	virtual void WindowPostRender(video::RenderWindow* wnd);
	virtual void update(float dt);
	virtual void onDone();
};

}

#endif

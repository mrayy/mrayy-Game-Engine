

/********************************************************************
	created:	2014/03/05
	created:	5:3:2014   1:48
	filename: 	C:\Development\mrayEngine\Projects\NissanCamera\RobotCameraState.h
	file path:	C:\Development\mrayEngine\Projects\NissanCamera
	file base:	RobotCameraState
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __RobotCameraState__
#define __RobotCameraState__

#include "IEyesRenderingBaseState.h"
#include "SceneManager.h"
#include "ViewPort.h"
#include "ARServiceProvider.h"
#include "GUIManager.h"
#include "GUIConsole.h"
#include "CommandManager.h"
#include "ARCommands.h"
#include "ARGroupManager.h"
#include "CameraPlaneRenderer.h"
#include "LocalCameraVideoSource.h"


namespace mray
{
	namespace TBee
	{
		class CRobotConnector;
		class ICameraVideoSource;
		class CalibHeadController;
		class TelubeeCameraConfiguration;
	}
namespace NCam
{
class NissanRobotCommunicator;
class ARGroupManager;
class ConsoleLogDevice;

class CarObjects
{
public:
	// Vehicle
	// |---HeadNode
	// |---|---Left Eye
	// |---|---|---Left Camera
	// |---|---|---Left Screen Node
	// |---|---Right Eye
	// |---|---|---Right Camera
	// |---|---|---Right Screen Node
	// |---Projection Plane
	scene::ISceneNode* Vehicle;
	scene::ISceneNode* Head;
	scene::ISceneNode* Eyes[2];
	scene::CameraNode* Camera[2];
	scene::ISceneNode* CameraPlane[2];
	scene::ISceneNode* ProjectionPlane;
};

class RobotCameraState :public TBee::IRenderingState, public scene::IViewportListener, public IARServiceListener, public IARCommandListener
{
	typedef TBee::IRenderingState Parent;
protected:


//	typedef IRenderingState Parent;

	GCPtr<GUI::GUIManager> m_guimngr;

	GCPtr<scene::SceneManager> m_sceneManager;
	GCPtr<scene::ViewPort> m_viewport[2];

	CarObjects m_carObjects;

	TBee::CRobotConnector* m_robotConnector;
	GUI::IGUIRenderer* m_guiRenderer;

	TBee::CalibHeadController* m_headController;
	NissanRobotCommunicator* m_robotComm;

	ARServiceProvider* m_arServiceProvider;
	ARGroupManager* m_arManager;

	ConsoleLogDevice* m_consoleLogDevice;
	GUI::GUIConsole* m_console;
	ARSceneObject* m_vehicleRef;
	scene::ISceneNode* m_arRoot;
	math::vector3d m_headRotationOffset;
	math::vector3d m_headPosOffset;

	GCPtr<CommandManager> m_commandManager;

	bool m_lockAxis[3];

	math::vector3d m_ARPositionOffset; //used to offset the position of all AR objects, this is due to the low floating point precision at large values


	GCPtr<CameraPlaneRenderer> m_cameraRenderer;

	void GenerateSurface(bool plane,float hfov, float vfov, int segments,  float cameraScreenDistance);
	void _RenderUI(const math::rectf& rc);
	void SetTransformation( const math::vector3d& pos,const math::vector3d &angles);

	void _OnConsoleCommand(GUI::GUIConsole*, const core::string& cmd);
	void _OnCommandMessage(const core::string& msg);

	void _UpdateMovement(float dt);

public:
	RobotCameraState();
	virtual~RobotCameraState();

	void SetCameraConnection(TBee::EUSBCameraType type);
	void SetCameraInfo(TBee::ETargetEye eye, int id);

	virtual void InitState();

	virtual bool OnEvent(Event* e, const math::rectf& rc);
	virtual void OnEnter(TBee::IRenderingState*prev);
	virtual void OnExit();
	virtual void Update(float dt);
	virtual video::IRenderTarget* Render(const math::rectf& rc, TBee::ETargetEye eye);

	virtual void LoadFromXML(xml::XMLElement* e);
	virtual xml::XMLElement* WriteToXML(xml::XMLElement* e);

	virtual void OnARContents(ARCommandAddData* cmd);
	virtual void OnVechicleData();
	virtual void OnDeletedGroup(ARCommandDeleteGroup* cmd);

	virtual void onRenderDone(scene::ViewPort*vp);

	virtual void CreateARObject(uint id, const core::string& name, const math::vector3d& pos, const math::vector3d& dir,bool isVehicle=false);
	virtual void UpdateARObject(uint id, const math::vector3d& pos, const math::vector3d& dir);
	virtual void MoveARObject(uint id, const math::vector3d& pos, const math::vector3d& dir);
	virtual void RemoveARObject(uint id);
	virtual void SelectARObject(uint id);
	virtual void SetARAlpha(uint id, float v);
	virtual bool QueryARObject(uint id, math::vector3d& pos, math::vector3d& dir);
	virtual void ListARObjects(std::vector<uint> &ids);
	virtual void ChangeARFov(float fov);

	virtual bool CanSleep(){ return false; }


};

}
}


#endif

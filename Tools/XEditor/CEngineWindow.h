
#pragma once
#include "afxwin.h"
#include "MFCEngineControl.h"
#include "EditorEventManager.h"
#include "RTPickBuffer.h"
#include "TransformManipulator.h"
#include "TextLogDevice.h"
#include "GameLevel.h"
#include "AIVisualizeRenderNode.h"

class CMaterialToolBox;

namespace mray
{

class CEngineWindow :
	public MFCEngineControl,public IEventReciver
{


protected:
protected:

	GCPtr<scene::ViewPort> m_viewPort;
	GCPtr<scene::SCameraNode> m_camera;
	GCPtr<EditorEventManager> m_eventManager;

	GCPtr<physics::IPhysicManager> m_phManager;
// 	GCPtr<game::GameEntityManager> m_entManager;
// 	GCPtr<game::GameEnvironment> m_envManager;
// 	GCPtr<game::GameGlobals> m_gmGlobals;
// 	GCPtr<AI::AISystem> m_aiSystem;

	GCPtr<game::GameLevel> m_gmLevel;
	GCPtr<sound::ISoundManager> m_soundManager;

	GCPtr<scene::TransformManipulator>  m_manipulator;

	video::RTPickBuffer *m_pickBuffer;
	GCPtr<video::TextureUnit> m_pickBufferTU;

	GCPtr<TextLogDevice> m_textLogger;

	bool m_requestClean;
	bool m_requestLoad;
	core::string m_requestLevelName;


	//TEST ONLY
	GCPtr<video::TextureUnit> m_billboardTU;
	GCPtr<video::IRenderTarget> m_billboardRT;

	void InitApp();

	GCPtr<scene::ISceneNode> m_selectedNode;
	GCPtr<scene::SSkyBoxNode> m_skyBox;

	void OnNodeModel();
	void ClearSettings();

	void OnLMouseDown(int x,int y);

	void CreateBillboardRT();

	void ClearScene();

	void _OnNewScene();
	void _OnLoadScene(const core::string& path);
public:


	CEngineWindow(void);
	virtual~CEngineWindow(void);


	void RemoveObject(scene::ISceneNode*node);

	virtual void OnRender();
	virtual void OnUpdate(float dt);

	game::GameLevel* GetLevel();

	TextLogDevice* getTextLogger();
	GCPtr<scene::TransformManipulator>  GetManipulator();

	void OnExportScene(const core::string& path);
	void OnLoadScene(const core::string& path);

	void SetSelectedNode(scene::ISceneNode*node);
	scene::ISceneNode* GetSelectedNode();
	scene::ISceneNode* GetRootNode();

	void OnNewScene();
	void OnLoadModel(const core::string&name,const core::string& ext);

	void GenerateTerrain(GCPtr<loaders::IHeightMapLoader> hm,const math::vector3d &scale,GCPtr<video::ITexture> base,GCPtr<video::ITexture> baseNormal,
		GCPtr<video::ITexture> detail,GCPtr<video::ITexture> detailNormal,float lod,float lodDist);

	virtual void onEvent(GCPtr<Event> event);

	void EnableShadows(bool e);
};

}


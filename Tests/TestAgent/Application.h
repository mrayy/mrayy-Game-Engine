
/********************************************************************
	created:	2010/04/03
	created:	3:4:2010   20:05
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent\Application.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\TestAgent
	file base:	Application
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Application___
#define ___Application___

#include <CMRayApplication.h>
#include <AISystem.h>
#include <TacticalPointManager.h>
#include "InterstingPointManager.h"

namespace mray{

	class GameEntityManager;

class Application:public CMRayApplication
{
private:
protected:

	enum EMouseMode
	{
		EMM_SetTarget,
		EMM_Create_Agent,
		EMM_Create_SphereObstacle,
		EMM_Create_BoxObstacle
	};

	EMouseMode m_mouseMode;
	math::vector3d m_startCreatingPos;
	math::vector3d m_currentMousePos;
	bool m_isCreating;

	core::BitVector m_visibilityVec;

	typedef std::map<int,AI::AIActor*> ActorIDList;

	ActorIDList m_agents;
	int m_lastAgentID;
	AI::AIActor* m_selectedActor;
	bool m_isCameraAttached;

	GCPtr<GUI::IFont> m_font;
	GCPtr<scene::SCameraNode> m_camera;
	GCPtr<scene::ViewPort> m_viewPort;
	AI::INavWorld *m_navMesh;

	GCPtr<scene::CTriangleSelector> m_worldSelector;
	GCPtr<scene::MeshSceneNode> m_world;
	GCPtr<GUI::GUIComboList> m_agentsCList;

	GCPtr<GameEntityManager> m_gameEntManager;
	GCPtr<physics::IPhysicManager> m_phManager;
	GCPtr<AI::InterstingPointManager> m_interstingMngr;
	GCPtr<AI::AISystem> m_aiSystem;
	GCPtr<AI::TacticalPointManager> m_tacticalManager;

	scene::RenderOperation m_visualizeOP;
	GCPtr<scene::IRenderable> m_visualizeNode;

	float m_aiTime;
	float m_renderTime;
	float m_phTime;

	void createGUI();
	bool loadWorld(const core::string&world);

	void onMouseDown(float x,float y);
	void onMouseMove(float x,float y);
	void onMouseUp(float x,float y);

	void createEntity(const math::vector3d&pos,float rad);
	void createObstacle(const math::vector3d&pos,float rad);

	void attachCameraToAgent();
	GCPtr<scene::ISceneNode> loadNode(const core::string& path);


public:
	Application();
	virtual~Application();

	virtual void onEvent(GCPtr<Event> event);

	virtual void init(core::array<SOptionElement> *extraOptions);
	virtual void update(float dt);
	virtual void draw();
	virtual void onDone();

	GameEntityManager* getEntityManager();
	scene::SCameraNode* getCamera();
	GUI::IFont* getFont(){return m_font;}
	AI::AISystem* getAiSystem(){return m_aiSystem;}
	AI::TacticalPointManager* getTactManager(){return m_tacticalManager;}
};


}


#endif //___Application___

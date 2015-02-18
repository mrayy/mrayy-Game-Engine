

/********************************************************************
	created:	2011/03/21
	created:	21:3:2011   21:21
	filename: 	i:\Programing\GameEngine\mrayEngine\Tests\TestCharacter\Application.h
	file path:	i:\Programing\GameEngine\mrayEngine\Tests\TestCharacter
	file base:	Application
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef Application_h__
#define Application_h__

#include "CMRayApplication.h"
#include "IPhysicManager.h"
#include "GUIBatchRenderer.h"
#include "CharacterManager.h"
#include "SPatchTerrain.h"

using namespace mray;

class Application:public CMRayApplication
{
private:
protected:
	scene::CameraNode* m_camera;
	scene::ViewPort* m_viewPort;
	GCPtr<GUI::GUIBatchRenderer> m_guiRender;
	GCPtr<physics::IPhysicManager> m_physicManager;
	GCPtr<PhysicsBasedCharacter::CharacterManager> m_characterManager;
	scene::SPatchTerrain* loadTerrain();
public:
	Application();
	virtual ~Application();

	virtual void onEvent(Event*event);
	virtual void init(const OptionContainer &extraOptions);
	void update(float dt);
	virtual void draw(scene::ViewPort* vp);
	virtual void onDone();
};

#endif // Application_h__

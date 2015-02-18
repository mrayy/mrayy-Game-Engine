

/********************************************************************
	created:	2013/07/12
	created:	12:7:2013   22:47
	filename: 	C:\Development\mrayEngine\Projects\PIDController\PIDApplication.h
	file path:	C:\Development\mrayEngine\Projects\PIDController
	file base:	PIDApplication
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __PIDApplication__
#define __PIDApplication__



#include "CMRayApplication.h"
#include "GUIBatchRenderer.h"
#include "GameEntityManager.h"
#include "FluidSolver.h"


namespace mray
{
	namespace FX
	{
		class Fluid2DTexture;
	}
class PIDApplication:public CMRayApplication
{
protected:
	void _InitResources();
	void CreatePhysicsSystem();
	GCPtr<GUI::GUIBatchRenderer> m_guiRender;
	GCPtr<physics::IPhysicManager>		m_phManager;
	GCPtr<game::GameEntityManager> m_gameManager;

	GCPtr<FX::FluidSolver> m_fluidSolver;
	FX::Fluid2DTexture* m_fluidTexture;
	video::TextureUnit m_fluidTexUnit;

	scene::ViewPort* m_vp;
	scene::CameraNode* m_camera;
	bool m_animating;

	void CreateFluid();
public:

	static PIDApplication* Instance;

	static PIDApplication* getInstance(){return Instance;}

	PIDApplication();
	virtual~PIDApplication();

	virtual void onEvent(Event* event);

	virtual void init(const OptionContainer &extraOptions);

	virtual void WindowPostViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp);
	virtual void update(float dt);
	virtual void onDone();

	scene::CameraNode* getCamera(){return m_camera;}
	void CreateScene();
	game::GameEntityManager* GetGameManager(){return m_gameManager;}

};

}


#endif

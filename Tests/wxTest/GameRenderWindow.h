
/********************************************************************
	created:	2012/11/22
	created:	22:11:2012   20:46
	filename: 	D:\Development\mrayEngine\Tests\wxTest\GameRenderWindow.h
	file path:	D:\Development\mrayEngine\Tests\wxTest
	file base:	GameRenderWindow
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _GameRenderWindow_
#define _GameRenderWindow_

#include "wxRenderWindow.h"
#include "GameEntityManager.h"

namespace mray
{

class GameRenderWindow:public wxRenderWindow
{
protected:
	GCPtr<scene::ISceneManager> m_sceneManager;
	GCPtr<physics::IPhysicManager> m_phManager;
	GCPtr<game::GameEntityManager> m_gameManager;
	GCPtr<scene::CameraNode> m_cameraL;
	GCPtr<scene::CameraNode> m_cameraR;
public:
	GameRenderWindow(const core::string& path,
		wxFrame* parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, 
		long style = 0,
		const wxString& name = wxControlNameStr);
	virtual~GameRenderWindow();

	virtual void onEvent(Event* e);
	scene::CameraNode* GetCamera(){return m_cameraL;}

	virtual void WindowPostViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp);

	virtual void Update(float dt);
};


}

#endif


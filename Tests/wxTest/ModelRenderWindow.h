

/********************************************************************
	created:	2012/11/22
	created:	22:11:2012   17:57
	filename: 	D:\Development\mrayEngine\Tests\wxTest\ModelRenderWindow.h
	file path:	D:\Development\mrayEngine\Tests\wxTest
	file base:	ModelRenderWindow
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _ModelRenderWindow_
#define _ModelRenderWindow_

#include "wxRenderWindow.h"
#include "ISceneManager.h"

namespace mray
{

class ModelRenderWindow:public wxRenderWindow,public IEventReciver
{
protected:
	scene::ISceneManager* m_sceneManager;
	scene::CameraNode* m_cam;
	scene::ISceneNode* m_node;
	math::vector3d m_angles;
public:
	ModelRenderWindow(const core::string& path,
		wxFrame* parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, 
		long style = 0,
		const wxString& name = wxControlNameStr);
	virtual~ModelRenderWindow();

	virtual void onEvent(Event* e);


	scene::CameraNode* GetCamera(){return m_cam;}
	scene::ISceneNode* GetNode(){return m_node;}

	virtual void WindowPostViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp);

	virtual void Update(float dt);
};


}

#endif

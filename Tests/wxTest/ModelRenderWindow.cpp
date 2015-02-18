

#include "stdafx.h"
#include "ModelRenderWindow.h"
#include "SceneManager.h"
#include "MeshFileCreatorManager.h"
#include "Viewport.h"
#include "RenderPass.h"


namespace mray
{


	ModelRenderWindow::ModelRenderWindow(const core::string& path,wxFrame* parent, wxWindowID id, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = 0 */, const wxString& name /* = wxControlNameStr */)
		:wxRenderWindow(parent,id,pos,size,style,name)
	{
		m_sceneManager=new scene::SceneManager(Engine::getInstance().getDevice());
		m_cam= m_sceneManager->createCamera();

		m_viewPorts[0]->SetClearColor(0);
		m_viewPorts[0]->setCamera(m_cam);
		scene::LightNode* light= m_sceneManager->createLightNode();
		light->setPosition(10);
		light->setRadius(100);

		m_cam->setAutoUpdateAspect(true);

		m_node= m_sceneManager->createSceneNode();
		scene::IRenderablePtr rnode=scene::MeshFileCreatorManager::getInstance().LoadFromFile(path,false);
		m_node->AttachNode(rnode);
		m_cam->setLookAtTarget(m_node);
		m_cam->setUseTarget(true);

		EventQueue::getInstance().addEventHandler(this,BIT(ET_Mouse));
	}
	ModelRenderWindow::~ModelRenderWindow()
	{
	}

	void ModelRenderWindow::onEvent(Event* e)
	{
		if(e->GetOwnerRenderWindow()!=m_renderWindow)
			return;
		if(e->getType()==ET_Mouse)
		{
			MouseEvent* evt=(MouseEvent*)e;
			if(evt->event==MET_MOVED)
			{
				m_angles.x=evt->pos.y*0.01;
				m_angles.y=evt->pos.x*0.01;
			}
		}
	}
	void ModelRenderWindow::WindowPostViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp)
	{
	}
	void ModelRenderWindow::Update(float dt)
	{
		m_angles.y+=dt;
		m_cam->setPosition(math::vector3d(cos(m_angles.y)*cos(m_angles.x)*5,5*sin(m_angles.x),sin(m_angles.y)*cos(m_angles.x)*5));
		m_sceneManager->update(dt);
	}


}




#include "stdafx.h"
#include "GameRenderWindow.h"

#include "SceneManager.h"
#include "MeshFileCreatorManager.h"
#include "Viewport.h"
#include "RenderPass.h"
#include "mrayPhysX.h"
#include "ShaderResourceManager.h"
#include "MaterialResourceManager.h"
#include "XMLTree.h"

#include "ParsedShaderPP.h"

namespace mray
{


	GameRenderWindow::GameRenderWindow(const core::string& path,wxFrame* parent, wxWindowID id, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDefaultSize */, long style /* = 0 */, const wxString& name /* = wxControlNameStr */)
		:wxRenderWindow(parent,id,pos,size,style,name)
	{
		m_sceneManager=new scene::SceneManager(Engine::getInstance().getDevice());

		m_cameraL= m_sceneManager->createCamera();
		m_cameraL->setPosition(math::vector3d(0,2,0));
		m_cameraL->setZFar(20);
		m_viewPorts[0]->setCamera(m_cameraL);
		{
			physics::PhysicsSystemDesc desc;
			desc.gravity.set(0,-9.8*1,0);
			desc.maxIter=32;
			desc.maxTimestep=1/(8*60.0);
			desc.useFixedTimeStep=true;
			m_phManager=new physics::PhysXManager(&desc);
			m_phManager->ConnectToRemoteDebugger();
		}

		m_gameManager=new game::GameEntityManager();
		m_gameManager->SetPhysicsManager(m_phManager);
		m_gameManager->SetSceneManager(m_sceneManager);

		gShaderResourceManager.parseShaderXML(gFileSystem.openFile("VT_Shaders.shd"));
		gMaterialResourceManager.parseMaterialXML(gFileSystem.openFile("VT_materials.mtrl"));

		xml::XMLTree tree;

		if(tree.load(gFileSystem.openFile(path,OS::BIN_READ)) )
		{
			m_gameManager->loadXMLSettings(&tree);

		}


		video::EPixelFormat pf=video::EPixel_Float16_RGBA;//video::EPixel_R8G8B8A8;//
		{

			video::ITexturePtr renderTargetTex=Engine::getInstance().getDevice()->createTexture2D(math::vector2d(1,1),pf,true);
			renderTargetTex->setBilinearFilter(false);
			renderTargetTex->setTrilinearFilter(false);
			renderTargetTex->setMipmapsFilter(false);

			video::IRenderTargetPtr rt=Engine::getInstance().getDevice()->createRenderTarget(mT(""),renderTargetTex,video::IHardwareBufferPtr::Null,video::IHardwareBufferPtr::Null,false);
			m_viewPorts[0]->setRenderTarget(rt);
			m_viewPorts[0]->setOnlyToRenderTarget(false);

			if(true)
			{
				renderTargetTex=Engine::getInstance().getDevice()->createTexture2D(1,pf,true);
				rt->attachRenderTarget(renderTargetTex,1);

				renderTargetTex=Engine::getInstance().getDevice()->createTexture2D(1,pf,true);
				rt->attachRenderTarget(renderTargetTex,2);



				video::ParsedShaderPP* pp=new video::ParsedShaderPP(Engine::getInstance().getDevice());
				pp->LoadXML(gFileSystem.openFile(mT("VTelesarPP.peff")));
				m_viewPorts[0]->setPostProcessing(pp);
				m_viewPorts[0]->enablePostProcessing(true);

			}
		}
	}
	GameRenderWindow::~GameRenderWindow()
	{
	}

	void GameRenderWindow::onEvent(Event* e)
	{
	}
	void GameRenderWindow::WindowPostViweportUpdate(video::RenderWindow* wnd,scene::ViewPort* vp)
	{
	}
	void GameRenderWindow::Update(float dt)
	{
		static math::vector3d m_angles;

		m_angles.y+=dt;
//		m_cameraL->setPosition(math::vector3d(cos(m_angles.y)*cos(m_angles.x)*5,5*sin(m_angles.x),sin(m_angles.y)*cos(m_angles.x)*5));
		m_sceneManager->update(dt);
		m_phManager->update(dt);
		m_gameManager->Update(dt);
	}


}



#include "stdafx.h"
#include "InMapRenderState.h"
#include "GUIPageManager.h"
#include "GUIManager.h"
#include "Application.h"
#include "GUIManagerRT.h"
#include "SceneManager.h"
#include "GUIElement3D.h"
#include "MapCamera.h"
#include "BrowserWindow.h"

#include "JoystickMapController.h"
#include "MapObject.h"
#include "GameEntityManager.h"

#include "ParsedShaderPP.h"
#include "TBAppGlobals.h"
#include "RobotInfoManager.h"

#include "GUIDateElement.h"
#include "IGUIPanelElement.h"
#include "GUIOverlay.h"

#include "GUIMapScreen.h"
#include "GUIRobotPanelImpl.h"
#include "JoystickDefinitions.h"
#include "CinematicController.h"
#include "SceneComponent.h"
#include "TBAppGlobals.h"


namespace mray
{
namespace TBee
{

InMapRenderState::InMapRenderState()
:IRenderingState("Maps")
{
	m_pageManager=0;
	m_mapGUIManager=0;
	m_entManager = 0;
	m_guiManager = 0;
	m_mapObject = 0;
	m_mapScreenGUI=new GUIMapScreen();
	m_robotPanel=new GUI::GUIRobotPanelImpl();
	m_indicatorVisible=true;
}

InMapRenderState::~InMapRenderState()
{
	delete m_mapGUIManager;
	delete m_pageManager;
	delete m_entManager;
	delete m_guiManager;
	delete m_mapScreenGUI;
	delete m_robotPanel;
	delete m_mapObject;
}
void InMapRenderState::SwitchIndicatorVisibility()
{
	m_indicatorVisible=!m_indicatorVisible;
	for (int i=0;i<m_indicatorObjs.size();++i)
	{
		m_indicatorObjs[i]->setVisible(m_indicatorVisible);
	}
}

void InMapRenderState::InitState()
{
	IRenderingState::InitState();

	Application* app = TBAppGlobals::Instance()->App;
	m_pageManager=new GUI::GUIPageManager();
	m_mapGUIManager = new GUI::GUIManager(Engine::getInstance().getDevice());
	m_entManager=new game::GameEntityManager();
	m_mapWebpage=m_pageManager->CreateWebPage(m_mapGUIManager);
	math::vector2d rw=app->GetRenderWindow()->GetSize();
	m_mapWebpage->SetResolution(rw.x,rw.y);
	int scale=1;
	m_mapWebpage->SetSize((rw)*scale);///math::vector2d(1,2));
	m_mapWebpage->SetPosition((rw)*(-(scale-1.0f)/2.0f));
	core::string path;
	gFileSystem.getCorrectFilePath("/HTML/maps.html",path);
	if(path!="")
	{
		m_mapWebpage->NavigateTo("file:///"+path);
	}

	m_sceneManager=new scene::SceneManager(app->getDevice());
	m_mapGUIRT=new scene::GUIManagerRT();
	scene::ISceneNode* snode=m_sceneManager->createSceneNode();
	scene::IGUIElementRT* guiRT= m_mapGUIRT->CreateFromManager(m_mapGUIManager);//m_guiRtManager->CreateFromElement(m_root);
	scene::GUIElement3D* rendNode=new scene::GUIElement3D();
	rendNode->SetElement(guiRT);
	rendNode->setOwner(snode);
	snode->AttachNode(rendNode);
	snode->setScale(100);

	m_mapGUIManager->SetRootElement(m_mapWebpage);

	m_entManager->SetSceneManager(m_sceneManager);

	m_entManager->loadFromFile("Indicator.xml");
	{
		const game::EntityList& ents= m_entManager->GetEntities();
		game::EntityList::const_iterator it=ents.begin();
		for (;it!=ents.end();++it)
		{
			const std::list<IObjectComponent*>& comps= (*it)->GetComponent(game::SceneComponent::getClassRTTI());
			std::list<IObjectComponent*>::const_iterator it2=comps.begin();
			for (;it2!=comps.end();++it2)
			{
				m_indicatorObjs.push_back(((game::SceneComponent*)*it2)->GetSceneNode());
			}
		}
	}

	{
		m_guiManager=new GUI::GUIManager(app->getDevice());
		GUI::IGUIPanelElement* root=new  GUI::IGUIPanelElement(core::string(""),m_guiManager);
		m_guiManager->SetRootElement(root);
		root->SetDocking(GUI::EED_Fill);

		GUI::GUIOverlay* o= GUI::GUIOverlayManager::getInstance().LoadOverlay("TBee\\GUIMapScreen.gui");
		if(o)
		{
			o->CreateElements(m_guiManager,root,0,m_mapScreenGUI);
		}
		o= GUI::GUIOverlayManager::getInstance().LoadOverlay("TBee\\GUIRobotPanel.gui");
		if(o)
		{
			o->CreateElements(m_guiManager,root,0,m_robotPanel);
			m_robotPanel->Hide();
		}

		o= GUI::GUIOverlayManager::getInstance().LoadOverlay("TBee\\GUIMapControls.gui");
		if(o)
		{
			o->CreateElements(m_guiManager,root,0,0);
		}
	}

	MapCamera* cam = new MapCamera(m_sceneManager, AppData::Instance()->inputMngr);
	m_sceneManager->addSceneNode(cam);
	cam->UpdatePosition();

	m_mapController=new CinematicController(AppData::Instance()->inputMngr); //new JoystickMapController(app->GetInputManager());
	m_mapObject=new MapObject();
	m_mapObject->AddListener(this);
	m_mapObject->Init(cam,m_mapWebpage);
	m_mapController->SetMapObject(m_mapObject);

	{
		m_viewport=new scene::ViewPort("MapCam",cam,0,0,math::rectf(0,0.2,1,0.8),0);
		m_viewport->SetClearColor(video::SColor(0,1,0,1));

		video::ITexturePtr renderTargetTex = Engine::getInstance().getDevice()->createTexture2D(math::vector2d(1, 1), video::EPixel_R8G8B8A8, true);
		renderTargetTex->setBilinearFilter(false);
		renderTargetTex->setTrilinearFilter(false);
		renderTargetTex->setMipmapsFilter(false);
		m_viewport->SetClearColor(video::SColor(1,1,1,1));

		video::IRenderTargetPtr rt = Engine::getInstance().getDevice()->createRenderTarget(mT(""), renderTargetTex, video::IHardwareBufferPtr::Null, video::IHardwareBufferPtr::Null, false);
		m_viewport->setRenderTarget(rt);
		m_viewport->setOnlyToRenderTarget(true);
		m_viewport->SetAutoUpdateRTSize(true);
		m_viewport->enablePostProcessing(true);

		video::ParsedShaderPP* pp = new video::ParsedShaderPP(Engine::getInstance().getDevice());
		pp->LoadXML(gFileSystem.openFile("BrowserEffect.peff"));
		m_viewport->setPostProcessing(pp);

		m_mapObject->SetEffect(pp);
	}
}


void InMapRenderState::OnEvent(Event* e)
{
	if(m_mapGUIRT)
		m_mapGUIRT->OnEvent(e);

	if(m_mapController)
	{
		m_mapController->OnEvent(e);
	}
	if(e->getType()==ET_Keyboard)
	{
		KeyboardEvent* ev=(KeyboardEvent*)e;
		if(ev->press&&ev->key==KEY_ESCAPE)
		{
			m_exitCode=BackToTile_Code;
		}
	}
	if(e->getType()==ET_Joystick)
	{
		JoystickEvent* evt=(JoystickEvent*)e;
		if(evt->button==JOYSTICK_StartButton && evt->event==JET_BUTTON_PRESSED)
		{
			m_exitCode=BackToTile_Code;
		}else if(evt->button==JOYSTICK_SquareButton && evt->event==JET_BUTTON_PRESSED)
		{
			SwitchIndicatorVisibility();
		}
	}
// 	if(m_mapGUIManager)
// 		m_mapGUIManager->OnEvent(e);
}

void InMapRenderState::OnEnter(IRenderingState*prev)
{
	m_time=0;
	m_dropedMarkers=0;

	m_mapScreenGUI->UserNameLbl->SetText(TBAppGlobals::Instance()->userName);
}

void InMapRenderState::OnExit()
{
	IRenderingState::OnExit();
}

void InMapRenderState::Update(float dt)
{
	m_mapGUIManager->Update(dt);
	m_guiManager->Update(dt);
	m_sceneManager->update(dt);
	m_pageManager->Update();
	m_mapController->Update(dt);
	m_entManager->Update(dt);

	m_robotPanel->Update(dt);

	m_time+=dt;
	if(m_time>2 && !m_dropedMarkers)
	{
		m_dropedMarkers=true;

		const std::vector<TBRobotInfo>& robots = TBAppGlobals::Instance()->robotInfoManager->GetRobots();
		for(int i=0;i<robots.size();++i)
		{
			m_mapObject->AddMarker(robots[i].lat,robots[i].lng,robots[i].ID);
		}
	}
}

video::IRenderTarget* InMapRenderState::Render(const math::rectf& rc,ETargetEye eye)
{
	IRenderingState::Render(rc,eye);

	m_mapGUIRT->DrawRT();

	video::IVideoDevice* device=Engine::getInstance().getDevice();
	scene::ViewPort* vp=device->getViewport();

	m_viewport->setAbsViewPort(rc);
	m_viewport->draw();
	math::rectf vprect=rc;
	vprect.ULPoint.y=rc.getHeight()*0.1;
	vprect.BRPoint.y=rc.getHeight()*0.9;


	video::TextureUnit tex;
	tex.SetTexture(m_viewport->getRenderTarget()->GetColorTexture());
	device->useShader(0);

	device->set2DMode();
	device->setRenderTarget(m_renderTarget[GetEyeIndex(eye)],1,1,video::SColor(0,0,0,1));
	device->useTexture(0,&tex);
	device->draw2DImage(vprect,1);
	m_guiManager->DrawAll(&rc);// m_renderTarget[GetEyeIndex(eye)]);
	device->setRenderTarget(0,0,0);
	device->setViewport(vp);

	return m_renderTarget[GetEyeIndex(eye)];
}

video::IRenderTarget* InMapRenderState::GetLastFrame(ETargetEye eye)
{
	return m_renderTarget[GetEyeIndex(eye)];
}


void InMapRenderState::LoadFromXML(xml::XMLElement* e)
{
}

void InMapRenderState::OnMarkerlClicked(MapObject* map,float x,float y,int id)
{
	TBRobotInfo* ifo = TBAppGlobals::Instance()->robotInfoManager->GetRobotInfoByID(id);
	if(!ifo)
		return;
	TBAppGlobals::Instance()->selectedRobot = ifo;
	m_exitCode=ConnectToRobot_Code;
}

void InMapRenderState::OnMarkerlHovered(MapObject* map,float x,float y,int id)
{
	TBRobotInfo* ifo = TBAppGlobals::Instance()->robotInfoManager->GetRobotInfoByID(id);
	if(!ifo)
		return;
	m_robotPanel->SetRobotInfo(*ifo);
	m_robotPanel->Show();
}


void InMapRenderState::OnMarkerlOut(MapObject* map,float x,float y,int id)
{
	m_robotPanel->Hide();
}


}
}


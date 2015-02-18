
#include "stdafx.h"
#include "Application.h"

#include "JFStateManager.h"

#include "JFMainMenuState.h"
#include "JFInGameState.h"
#include "JFStateCodes.h"
#include "JFEnvData.h"

namespace mray
{

Application::Application()
{
	m_stateManager=new game::JFStateManager();
	game::JFEnvData::App=this;

	m_activeGUIManager=0;
}
Application::~Application()
{
	delete m_stateManager;
}

void Application::_initStateMachine()
{

	game::IGameState* mainMenuSt,*gameSt;
	mainMenuSt=new game::JFMainMenuState();
	gameSt=new game::JFInGameState();

	m_stateManager->AddState(mainMenuSt,mainMenuSt->GetStateName());
	m_stateManager->AddState(gameSt,gameSt->GetStateName());

	m_stateManager->AddTransition(mainMenuSt->GetStateName(),gameSt->GetStateName(),game::EJF_StartGame);

	m_stateManager->SetInitialState(mainMenuSt->GetStateName());
}

void Application::_initResources()
{

	{
		gImageSetResourceManager.loadImageSet(mT("VistaCG.imageset"));
		GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
		GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
		GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG"));
	}
	{
		//load font
		GCPtr<GUI::IFont>font=gFontResourceManager.getOrCreate(mT("solo5_font.xml"));
		gFontResourceManager.setDefaultFont(font);
	}

	m_guiRender=new GUI::GUIBatchRenderer();
	m_guiRender->SetDevice(getDevice());
}

void Application::_initDefaultViewport()
{
	GetRenderWindow()->CreateViewport(mT("DefaultVP"),0,0,math::rectf(0,0,1,1),0);
}

void Application::init(const OptionContainer &extraOptions)
{
	CMRayApplication::init(extraOptions);

	_initResources();
	_initDefaultViewport();


	_initStateMachine();
}

void Application::onEvent(Event* event)
{
	CMRayApplication::onEvent(event);

	m_stateManager->OnEvent(event);
}
void Application::draw(scene::ViewPort* vp)
{
	CMRayApplication::draw(vp);

	if(m_activeGUIManager)
		m_activeGUIManager->DrawAll();
}

void Application::WindowPostRender(video::RenderWindow* wnd)
{
	getDevice()->set2DMode();

	GCPtr<GUI::IFont> font=gFontResourceManager.getDefaultFont();
	if(font){
		m_guiRender->Prepare();

		float yoffset=50;

		GUI::FontAttributes attr;
		attr.fontColor.Set(0.05,1,0.5,1);
		attr.fontAligment=GUI::EFA_MiddleLeft;
		attr.fontSize=24;
		attr.hasShadow=true;
		attr.shadowColor.Set(0,0,0,1);
		attr.shadowOffset=math::vector2d(2);
		attr.spacing=2;
		attr.wrap=0;
		attr.RightToLeft=0;
		core::string msg=mT("FPS= ");
		msg+=core::StringConverter::toString(gFPS.getFPS());
		font->print(math::rectf(10,yoffset+10,10,10),&attr,0,msg,m_guiRender);
		yoffset+=30;
		msg=mT("State= ");
		msg+=m_stateManager->GetActiveState()->GetStateName();
		font->print(math::rectf(10,yoffset+10,10,10),&attr,0,msg,m_guiRender);
		yoffset+=30;

		m_guiRender->Flush();
		getDevice()->useShader(0);
	}
}

void Application::update(float dt)
{
	CMRayApplication::update(dt);

	m_stateManager->Update(dt);
}


}


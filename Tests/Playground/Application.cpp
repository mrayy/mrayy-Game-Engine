

#include "stdafx.h"
#include "Application.h"
#include "ImageSetResourceManager.h"
#include "GUIThemeManager.h"
#include "FontResourceManager.h"
#include "win32NetInterface.h"
#include "TextureResourceManager.h"
#include "DynamicFontGenerator.h"
#include "AwesomiumManager.h"
#include "WebScene.h"

#include <windows.h>


namespace mray
{


Application::Application()
{
	this->m_limitFps = true;
}

Application::~Application()
{
}

void Application::onEvent(Event* event)
{
	CMRayApplication::onEvent(event);
}


void Application::init(const OptionContainer &extraOptions)
{
	CMRayApplication::init(extraOptions);
	{

		CMRayApplication::loadResourceFile(mT("Resources.stg"));


		gImageSetResourceManager.loadImageSet(mT("VistaCG_Dark.imageset"));
		GCPtr<OS::IStream> themeStream = gFileSystem.createBinaryFileReader(mT("VistaCG_Dark.xml"));
		GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
		GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG_Dark"));

		//load font

		GCPtr<GUI::DynamicFontGenerator> font = new GUI::DynamicFontGenerator("Arial24");
		font->SetFontName(L"Arial");
		font->SetTextureSize(1024);
		font->SetFontResolution(24);
		font->Init();
		gFontResourceManager.setDefaultFont(font);

		gFontResourceManager.loadFontsFromDir(gFileSystem.getAppPath() + "..\\Data\\Fonts\\");

		gLogManager.log("Resources Loaded", ELL_SUCCESS);
	}
	m_mainVP = GetRenderWindow()->CreateViewport("MainVP", 0, 0, math::rectf(0, 0, 1, 1), 0);
	m_mainVP->SetClearColor(video::DefaultColors::White);
	network::createWin32Network();

	m_guiRenderer = new GUI::GUIBatchRenderer();
	m_guiRenderer->SetDevice(getDevice());

	web::AwesomiumManager* m=new web::AwesomiumManager();
	Awesomium::WebConfig conf;
	m->Init(conf);

	m_scene = new WebScene();
	m_scene->OnInit();
}


void Application::draw(scene::ViewPort* vp)
{
}

void Application::WindowPostRender(video::RenderWindow* wnd)
{
	video::TextureUnit tu;
	//getDevice()->setRenderTarget(m_rt);
	web::AwesomiumManager::getInstance().Update();

	m_scene->OnRender(math::rectf(0, wnd->GetSize()));
	getDevice()->set2DMode();

	GCPtr<GUI::IFont> font = gFontResourceManager.getDefaultFont();
	getDevice()->set2DMode();
	if (font){
		m_guiRenderer->Prepare();

		float yoffset = 50;


		GUI::FontAttributes attr;
		attr.fontColor.Set(0.05, 1, 0.5, 1);
		attr.fontAligment = GUI::EFA_MiddleLeft;
		attr.fontSize = 24;
		attr.hasShadow = true;
		attr.shadowColor.Set(0, 0, 0, 1);
		attr.shadowOffset = math::vector2d(2);
		attr.spacing = 2;
		attr.wrap = 0;
		attr.RightToLeft = 0;
		core::string msg = mT("FPS= ");
		msg += core::StringConverter::toString((int)gEngine.getFPS()->getFPS());
		font->print(math::rectf(wnd->GetSize().x - 250, wnd->GetSize().y - 150, 10, 10), &attr, 0, msg, m_guiRenderer);
		yoffset += attr.fontSize;

		m_guiRenderer->Flush();
	}
}

void Application::update(float dt)
{
	CMRayApplication::update(dt);
}

void Application::onDone()
{
}


void Application::onRenderDone(scene::ViewPort*vp)
{
}

}


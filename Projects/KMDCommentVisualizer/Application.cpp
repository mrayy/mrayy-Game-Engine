
#include "stdafx.h"
#include "Application.h"

#include "win32NetInterface.h"

#include "GUIBatchRenderer.h"

#include "TextureResourceManager.h"
#include "DynamicFontGenerator.h"

#include "GUIThemeManager.h"

#include "InternetCacheManager.h"
#include "FontResourceManager.h"
#include "ImageSetResourceManager.h"
#include "SQLDBHandler.h"
#include "GUIElementFactory.h"

#include "GUIUserProfile.h"
#include "GUICommentItem.h"
#include "GUISessionSidePanel.h"
#include "GUISpeakerDetailsPanel.h"
#include "GUICommentDetailsPanel.h"
#include "GUIProfilePicture.h"
#include "GUIFacultyList.h"
#include "GUICommentsLayer.h"
#include "GUIStatusLayer.h"
#include "GUILayersContainer.h"
#include "GUIProjectStatus.h"
#include "GUITransitionBars.h"
#include "GUIPictureWallpaper.h"



#include "SessionScene.h"
#include "AppData.h"
#include "SessionContainer.h"

#include "Viewport.h"
#include "KMDComment.h"
#include "KMDUser.h"
#include "GUISceneSpacePanel.h"

#include "GUISweepingText.h"
#include "GUIFadingText.h"
#include "SFModSoundmanager.h"
#include "KMDUser.h"
#include "XMLTree.h"

namespace mray
{
Application::Application()
{
	m_mainVP = 0;
	m_scene = 0;
	m_limitFps = true;
	m_limitFpsCount = 60;
}
Application::~Application()
{
	if (kmd::IDBHandler::isExist())
	{
		kmd::IDBHandler::getInstance().Close();
		delete kmd::IDBHandler::getInstancePtr();
	}

}

void Application::onEvent(Event* event)
{
	math::rectf rc;
	if (m_mainVP)
	{
		if (event->getType()==ET_ResizeEvent)
			m_mainVP->updateViewPort();
		rc = m_mainVP->getAbsRenderingViewPort();
	}
	if (m_scene)
	{
		m_scene->OnEvent(event, rc);
	}

	if (event->getType() == ET_Keyboard)
	{

		KeyboardEvent* evt = dynamic_cast<KeyboardEvent*>(event);
		if (evt->press)
		{
			if (evt->key == KEY_L  && evt->ctrl)
			{
				m_limitFps = !m_limitFps;
			}
			if (evt->key == KEY_F9 && evt->ctrl)
			{
				gAppData.Debugging = !gAppData.Debugging;
			}
		}
	}
}

void Application::init(const OptionContainer &extraOptions)
{
	CMRayApplication::init(extraOptions);

	{
		new kmd::AppData;
		gAppData.app = this;
		gAppData.Load("kmdSettings.cfg");
		kmd::SQLDBHandler* db=new kmd::SQLDBHandler();

	}
	//ShowCursor(0);

	{
		gAppData.Debugging = extraOptions.GetOptionValue("Debugging")=="Yes";
	}
	{
		//Load users
		xml::XMLTree tree;
		if (tree.load(gAppData.GetValue("App", "Users")))
		{
			xml::XMLElement* e = tree.getSubElement("Users");
			e = e->getSubElement("User");
			while (e)
			{
				kmd::KMDUser* u = new kmd::KMDUser();
				u->LoadXML(e);
				kmd::KMDUser::AddKMDUser(u);
				e = e->nextSiblingElement("User");
			}
		}

		kmd::KMDUser::studentUser = kmd::KMDUser::GetUserByName("Student");
		kmd::KMDUser::facultyUser = kmd::KMDUser::GetUserByName("Faculty");
	}
	{

		gAppData.sessions = new kmd::SessionContainer();
		gAppData.sessions->LoadFromXML(gAppData.GetValue("App","Sessions"));// "Sessions.xml");
	}
	CMRayApplication::loadResourceFile(mT("Resources.stg"));

	network::createWin32Network();

#if 0
	{
		m_soundManager = new sound::SFModSoundManager();
		gAppData.soundManager = m_soundManager;
	}
#endif
	{
		//gImageSetResourceManager.loadImageSet(mT("VistaCG_Dark.imageset"));
// 		GCPtr<OS::IStream> themeStream = gFileSystem.createBinaryFileReader(mT("VistaCG_Dark.xml"));
// 		GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
// 		GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG_Dark"));
		gFontResourceManager.loadFontsFromDir(gFileSystem.getAppPath() + "..\\Data\\Fonts\\");

		gImageSetResourceManager.loadImageSet("TedxTokyo.imageset");
		gImageSetResourceManager.loadImageSet("leapGestures.imageset");

		GCPtr<GUI::DynamicFontGenerator> font = new GUI::DynamicFontGenerator("Arial");
		font->SetFontName(L"Arial");
		font->SetTextureSize(1024);
		font->SetFontResolution(24);
		font->SetBold(true);
		font->Init();
		gFontResourceManager.addResource(font, "Default");
		gFontResourceManager.setDefaultFont(font);
	}
	

	scene::ViewPort* vp = GetRenderWindow()->CreateViewport("", 0, 0, math::rectf(0, 0, 1, 1), 0);


	{
		REGISTER_GUIElement_FACTORY(GUICommentItem);
		REGISTER_GUIElement_FACTORY(GUIUserProfile);
		REGISTER_GUIElement_FACTORY(GUISessionSidePanel);
		REGISTER_GUIElement_FACTORY(GUISpeakerDetailsPanel);
		REGISTER_GUIElement_FACTORY(GUICommentDetailsPanel);
		REGISTER_GUIElement_FACTORY(GUIProfilePicture);
		REGISTER_GUIElement_FACTORY(GUISceneSpacePanel);
		REGISTER_GUIElement_FACTORY(GUISweepingText);
		REGISTER_GUIElement_FACTORY(GUIFadingText);
		REGISTER_GUIElement_FACTORY(GUICommentsLayer);
		REGISTER_GUIElement_FACTORY(GUIStatusLayer);
		REGISTER_GUIElement_FACTORY(GUIFacultyList);
		REGISTER_GUIElement_FACTORY(GUITransitionBars);
		REGISTER_GUIElement_FACTORY(GUILayersContainer);
		REGISTER_GUIElement_FACTORY(GUIProjectStatus);
		REGISTER_GUIElement_FACTORY(GUIPictureWallpaper);
	}

	m_mainVP = GetRenderWindow()->CreateViewport("MainVP", 0, 0, math::rectf(0, 0, 1, 1), 0);


	// 	m_userProfile->GetFontAttributes()->hasShadow = false;
//	m_userProfile->GetFontAttributes()->fontColor.Set(0, 0, 0, 1);
	if (true)
	{
		kmd::IDBHandler::getInstance().LoadDB();
	}
	m_device->setMultiSampling(true);

	m_scene = new kmd::SessionScene();
	m_scene->Init();

	m_scene->OnEnter();
}

void Application::WindowPostRender(video::RenderWindow* wnd)
{
	video::TextureUnit tex;

	m_scene->Update(GetDrawFPS().dt());
	getDevice()->set2DMode();
	getDevice()->setViewport(m_mainVP);
	m_scene->Draw(m_mainVP->getAbsRenderingViewPort());
	getDevice()->setRenderTarget(0);



	getDevice()->set2DMode();

	tex.SetTexture(m_scene->GetRenderTarget()->GetColorTexture());
	getDevice()->useTexture(0, &tex);
	getDevice()->draw2DImage(m_mainVP->getAbsRenderingViewPort(), 1);

	GUI::GUIBatchRenderer m_guiRender;
	m_guiRender.SetDevice(getDevice());

	m_guiRender.Prepare();

	float yoffset = 50;

	GUI::IFont* font = gFontResourceManager.getDefaultFont();

	if (gAppData.Debugging && font)
	{
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
		core::UTFString msg;
		msg = L"FPS: " + core::string_to_wchar(core::StringConverter::toString(gEngine.getFPS()->getFPS()));
		font->print(math::rectf(0, 0, 200, 100), &attr, &math::rectf(0, 0, 200, 100), msg, &m_guiRender);
		
		msg = "Draw Calls=" + core::StringConverter::toString(getDevice()->getBatchDrawnCount());
		font->print(math::rectf(50, 30, 150, 200), &attr, 0, msg, &m_guiRender);

		msg = "Draw Primitives=" + core::StringConverter::toString(getDevice()->getPrimitiveDrawnCount());
		font->print(math::rectf(50, 60, 150, 200), &attr, 0, msg, &m_guiRender);

		m_guiRender.Flush();/**/
	}
}
void Application::update(float dt)
{
	dt = math::clamp(dt, 0.001f, 0.1f);
	CMRayApplication::update(dt);
#if 0
	m_soundManager->runSounds(dt);
#endif
}

void Application::onDone()
{
	CMRayApplication::onDone();
	delete m_scene;
	m_scene = 0;
	kmd::IDBHandler::getInstance().SaveDB();
}

}






#include "stdafx.h"
#include "ARApplication.h"

#include "FontResourceManager.h"
#include "ImageSetResourceManager.h"
#include "GUIThemeManager.h"
#include "StringUtil.h"

#include "win32NetInterface.h"

#include <ViewPort.h>

#include "DirectShowVideoGrabber.h"
#include "IThreadManager.h"
#include <windows.h>

#include "CMemoryStream.h"
#include "DynamicFontGenerator.h"


#define COMMUNICATION_PORT 6000

namespace mray
{

	ARApplication::ARApplication()
	{
		m_debugging = false;
	}

	ARApplication::~ARApplication()
	{
	}



	void ARApplication::_InitResources()
	{
		CMRayApplication::loadResourceFile(mT("Resources.stg"));

		(gLogManager.StartLog(ELL_INFO) << "Initing Resources").flush();;

		gImageSetResourceManager.loadImageSet(mT("VistaCG_Dark.imageset"));
		gImageSetResourceManager.loadImageSet(mT("Icons\\icons.imageset"));
		GCPtr<OS::IStream> themeStream = gFileSystem.createBinaryFileReader(mT("VistaCG_Dark.xml"));
		GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
		GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG_Dark"));

		//load font
		GCPtr<GUI::DynamicFontGenerator> font = new GUI::DynamicFontGenerator("Arial24");
		font->SetFontName(L"Arial");
		font->SetTextureSize(1024);
		font->SetFontResolution(24);
		font->Init();

		//GCPtr<GUI::IFont>font = gFontResourceManager.loadFont(mT("Calibrib_font.fnt"));
		//gFontResourceManager.loadFont(mT("OCRAStd.fnt"));
		gFontResourceManager.setDefaultFont(font);

		gLogManager.log("Resources Loaded", ELL_SUCCESS);
	}


	void ARApplication::onEvent(Event* e)
	{
		//#define JOYSTICK_SelectButton 8
		CMRayApplication::onEvent(e);
		if (e->getType() == ET_Keyboard)
		{
			KeyboardEvent* evt = (KeyboardEvent*)e;
			if (evt->press && evt->key == KEY_S)
			{
			}
			if (evt->press && evt->key == KEY_F9)
			{
				m_debugging = !m_debugging;
			}
		}
	}


	void ARApplication::init(const OptionContainer &extraOptions)
	{
		CMRayApplication::init(extraOptions);
		{
		}
		_InitResources();

		m_limitFps = true;
		network::createWin32Network();


		m_guiRender = new GUI::GUIBatchRenderer();
		m_guiRender->SetDevice(getDevice());



		m_viewPort = GetRenderWindow()->CreateViewport("VP", 0, 0, math::rectf(0, 0, 1, 1), 0);
		m_viewPort->AddListener(this);

	}


	void ARApplication::draw(scene::ViewPort* vp)
	{

	}

	void ARApplication::WindowPostRender(video::RenderWindow* wnd)
	{
	}

	void ARApplication::update(float dt)
	{
		CMRayApplication::update(dt);

	}

	void ARApplication::onDone()
	{
		CMRayApplication::onDone();
	}


	void ARApplication::onRenderDone(scene::ViewPort*vp)
	{
		getDevice()->set2DMode();
		GCPtr<GUI::IFont> font = gFontResourceManager.getDefaultFont();

		if (font)
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

			core::string msg;
			msg = core::string("FPS : ") + core::StringConverter::toString(gEngine.getFPS()->getFPS()) ;
			font->print(math::rectf(20,  40, 10, 10), &attr, 0, msg, m_guiRender);
		}
	}

}
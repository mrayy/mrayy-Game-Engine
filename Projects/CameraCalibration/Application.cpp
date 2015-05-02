

#include "stdafx.h"
#include "Application.h"
#include "ImageSetResourceManager.h"
#include "GUIThemeManager.h"
#include "FontResourceManager.h"
#include "TextureResourceManager.h"
#include "DynamicFontGenerator.h"


#include "DirectShowVideoGrabber.h"
#include "CVChessBoard.h"
#include "CVCalibration.h"

#include <windows.h>


namespace mray
{

	class ApplicationImpl
	{
	public:

		GCPtr<video::ICameraVideoGrabber> camera;
		GCPtr<video::VideoGrabberTexture> grabber;
		video::CVChessBoard chessboard;
	};


Application::Application()
{
	this->m_limitFps = true;

	m_impl = new ApplicationImpl;
}

Application::~Application()
{
	delete m_impl;
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
	
	m_guiRenderer = new GUI::GUIBatchRenderer();
	m_guiRenderer->SetDevice(getDevice());


	m_impl->camera = new video::DirectShowVideoGrabber();
	m_impl->camera->InitDevice(0, 640, 480, 30);
	m_impl->grabber = new video::VideoGrabberTexture();
	m_impl->grabber->Set(m_impl->camera, 0);
	m_impl->chessboard.Setup(math::vector2di(8, 5),2.5);
}


void Application::draw(scene::ViewPort* vp)
{
}

void Application::WindowPostRender(video::RenderWindow* wnd)
{
	video::TextureUnit tu;
	//getDevice()->setRenderTarget(m_rt);

	bool found=false;
	if (m_impl->grabber->Blit())
	{
		std::vector<math::vector2d> points;
		if (m_impl->chessboard.FindInImage(m_impl->camera->GetLastFrame(), points))
		{
			found = true;
			printf("Found\n");
		}
	}
	getDevice()->set2DMode();

	tu.SetTexture(m_impl->grabber->GetTexture());
	getDevice()->useTexture(0, &tu);
	getDevice()->draw2DImage(math::rectf(0,wnd->GetSize()),1);


	GCPtr<GUI::IFont> font = gFontResourceManager.getDefaultFont();
	getDevice()->set2DMode();
	if (font){
		m_guiRenderer->Prepare();

		float yoffset = 50;

#define LOG_OUT(msg,x,y)\
	font->print(math::rectf((x), (y)+yoffset, 10, 10), &attr, 0, msg, m_guiRenderer); \
	yoffset += attr.fontSize;

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

		LOG_OUT(core::string(mT("FPS= ") + core::StringConverter::toString(gEngine.getFPS()->getFPS())), wnd->GetSize().x - 250, wnd->GetSize().y - 150);
		
		
		LOG_OUT(core::string(mT("Found= ")+core::StringConverter::toString(found)), 100,200);

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


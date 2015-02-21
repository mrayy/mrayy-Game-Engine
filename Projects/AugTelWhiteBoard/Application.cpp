

#include "stdafx.h"
#include "Application.h"
#include "ImageSetResourceManager.h"
#include "GUIThemeManager.h"
#include "FontResourceManager.h"
#include "win32NetInterface.h"
#include "OptiTrackDataSource.h"
#include "TextureResourceManager.h"
#include "BrushDrawer.h"

#include <windows.h>
#include "CVCalibration.h"
#include "CVWrappers.h"


namespace mray
{


	class CVData
	{
	public:

		GCPtr<video::CVCalibration> cvCalib;
		cv::Mat prev;
		cv::Mat diff;
		cv::Mat undistort;

		float lastTime;
		bool reset;
		int lastBuffer;
		video::ITexturePtr m_cvTex;
		bool changed;

		CVData()
		{
			changed = false;
			lastBuffer = 0;
			reset = false;
			lastTime = 0;
		}
	};

Application::Application()
{
	m_optiProvider = new OptiTrackDataSource;
	this->m_limitFps = true;
	m_lineDrawer = 0;
}

Application::~Application()
{
	delete m_optiProvider;
	delete m_lineDrawer;
}

void Application::onEvent(Event* event)
{
	CMRayApplication::onEvent(event);
	if (m_lineDrawer)
		m_lineDrawer->OnEvent(event);
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
		GCPtr<GUI::IFont>font = gFontResourceManager.loadFont(mT("Calibrib_font.fnt"));
		gFontResourceManager.loadFont(mT("OCRAStd.fnt"));
		gFontResourceManager.setDefaultFont(font);

		gLogManager.log("Resources Loaded", ELL_SUCCESS);
	}
	m_lineDrawer = new BrushDrawer();
	m_mainVP = GetRenderWindow()->CreateViewport("MainVP", 0, 0, math::rectf(0, 0, 1, 1), 0);
	m_mainVP->SetClearColor(video::DefaultColors::White);
	{
		video::ITexturePtr renderTargetTex = Engine::getInstance().getDevice()->createTexture2D(math::vector2d(1, 1), video::EPixel_R8G8B8A8, true);
		renderTargetTex->setBilinearFilter(false);
		renderTargetTex->setTrilinearFilter(false);
		renderTargetTex->setMipmapsFilter(false);
		m_rt = getDevice()->createRenderTarget("", renderTargetTex, 0, 0, 0);
		renderTargetTex->createTexture(math::vector3di(512, 512, 1), video::EPixel_R8G8B8A8);
	}
	network::createWin32Network();

	m_lineDrawer->StartReciver(5123);
	core::string optiIp=extraOptions.GetOptionValue("OptiServer");
	m_optiProvider->Connect(optiIp);

	if (!m_optiProvider->IsConnected())
		printf("Failed to connect with OptiTrack!\n");

	m_guiRenderer = new GUI::GUIBatchRenderer();
	m_guiRenderer->SetDevice(getDevice());

	m_cvData = new CVData();

	m_cvData->cvCalib = new video::CVCalibration();

	m_cvData->m_cvTex = getDevice()->createEmptyTexture2D(false);
	m_cvData->m_cvTex->setMipmapsFilter(false);

	m_cam = new video::FlyCameraVideoGrabber();
	//m_cam = new video::DirectShowVideoGrabber();
	m_videoGrabber = new video::VideoGrabberTexture();

	int width = 1024;
	int height = 768;

	m_cam->InitDevice(0, width, height, 20);
	m_cam->Start();
	m_videoGrabber->Set(m_cam, 0);


	m_cvData->cvCalib->setPatternSize(7, 10);
	m_cvData->cvCalib->setSquareSize(2.5);
	m_cvData->cvCalib->setPatternType(video::CHESSBOARD);

	video::allocate(m_cvData->prev, width, height, video::getCvImageType(video::EPixel_B8G8R8));
	video::allocate(m_cvData->diff, width, height, video::getCvImageType(video::EPixel_B8G8R8));
	video::allocate(m_cvData->undistort, width, height, video::getCvImageType(video::EPixel_B8G8R8));
}


void Application::draw(scene::ViewPort* vp)
{
}

void Application::WindowPostRender(video::RenderWindow* wnd)
{
	video::TextureUnit tu;
	//getDevice()->setRenderTarget(m_rt);
	getDevice()->set2DMode();
	m_lineDrawer->Draw(m_mainVP->getAbsRenderingViewPort());

// 	tu.SetTexture(gTextureResourceManager.loadTexture2D("FPV.png"));
// 	getDevice()->useTexture(0, &tu);
// 	getDevice()->draw2DImage(math::rectf(0, 100), 1);

	m_videoGrabber->Blit();

	if (m_cvData->changed)
	{
		m_cvData->changed = false;
		video::ImageInfo ifo;
		ifo.autoDel = false;
		cv::Mat gray;
		video::fromCV(m_cvData->undistort, &ifo,true);
		video::ImageInfo *ptrIfo[1] = { &ifo };

		m_cvData->m_cvTex->loadSurfaces((const video::ImageInfo**)ptrIfo, 1);
	}

	tu.SetTexture(m_cvData->m_cvTex);
	getDevice()->useTexture(0, &tu);
	getDevice()->draw2DImage(math::rectf(0, 300), 1);
	/*
	getDevice()->draw2DRectangle(math::rectf(0, 0, 20, 20), 0.5);
	getDevice()->setRenderTarget(0);
	getDevice()->setViewport(m_mainVP);

	tu.SetTexture(m_rt->getColorTexture());
	getDevice()->useTexture(0, &tu);
	getDevice()->draw2DImage(math::rectf(0, m_mainVP->getSize()), 1);*/


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
	m_lineDrawer->Update(dt);
	//Sleep(10);

	if (m_cam->GetBufferID() != m_cvData->lastBuffer )
	{
		if ((gEngine.getTimer()->getSeconds() - m_cvData->lastTime) > 1000 )
		{
			m_cam->Lock();
			m_cvData->lastBuffer = m_cam->GetBufferID();
			cv::Mat cam = video::toCv(m_cam->GetLastFrame());
			//video::copyGray(video::toCv(m_cam->GetLastFrame()), cam);
			m_cam->Unlock();
			int type = video::getGlImageType(cam.type());
			cv::Mat prevMat = video::toCv(m_cvData->prev);
			video::absdiff(prevMat, cam, m_cvData->diff);
			cam.copyTo(m_cvData->prev);

			cv::Scalar m = cv::mean(m_cvData->diff);
			float diffMean = cv::mean(Mat(m))[0];
			//printf("%f\n", diffMean);
			if (diffMean < 2.5)
			{
				vector<Point2f> corners;

				if (!m_cvData->reset && m_cvData->cvCalib->add(cam))
				{
					m_cvData->reset = true;
					m_cvData->cvCalib->calibrate();
					printf("%d - Calibrate\n", m_cvData->cvCalib->size());
					m_cvData->lastTime = gEngine.getTimer()->getSeconds();
				}
			}
			else
				m_cvData->reset = false;
		}
			m_cvData->changed = true;
		if (m_cvData->cvCalib->size() > 0)
		{
			cv::Mat cam = video::toCv(m_cam->GetLastFrame());
			m_cvData->cvCalib->undistort(cam, m_cvData->undistort);
		}
		else
		{
			m_cvData->undistort = video::toCv(m_cam->GetLastFrame());;
		}
	}

}

void Application::onDone()
{
}


void Application::onRenderDone(scene::ViewPort*vp)
{
}

}




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

		float lastTime;
		bool reset;
		int lastBuffer;

		CVData()
		{
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


	m_cvData = new CVData();

	m_cvData->cvCalib = new video::CVCalibration();


	m_cam = new video::DirectShowVideoGrabber();
	m_videoGrabber = new video::VideoGrabberTexture();

	m_cam->InitDevice(0, 640,480, 25);
	m_cam->Start();
	m_videoGrabber->Set(m_cam, 0);


	m_cvData->cvCalib->setPatternSize(7, 10);
	m_cvData->cvCalib->setSquareSize(2.5);
	m_cvData->cvCalib->setPatternType(video::CHESSBOARD);

	video::allocate(m_cvData->prev, 640, 480, video::getCvImageType(video::EPixel_B8G8R8));
	video::allocate(m_cvData->diff, 640, 480, video::getCvImageType(video::EPixel_B8G8R8));
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

	tu.SetTexture(m_videoGrabber->GetTexture());
	getDevice()->useTexture(0, &tu);
	getDevice()->draw2DImage(math::rectf(0, 100), 1);
	/*
	getDevice()->draw2DRectangle(math::rectf(0, 0, 20, 20), 0.5);
	getDevice()->setRenderTarget(0);
	getDevice()->setViewport(m_mainVP);

	tu.SetTexture(m_rt->getColorTexture());
	getDevice()->useTexture(0, &tu);
	getDevice()->draw2DImage(math::rectf(0, m_mainVP->getSize()), 1);*/
}

void Application::update(float dt)
{
	CMRayApplication::update(dt);
	m_lineDrawer->Update(dt);
	//Sleep(10);

	if (m_cam->GetBufferID() != m_cvData->lastBuffer && (gEngine.getTimer()->getSeconds() - m_cvData->lastTime)>1)
	{
		m_cvData->lastBuffer = m_cam->GetBufferID();
		cv::Mat cam= video::toCv(m_cam->GetLastFrame());
		cv::Mat prevMat = video::toCv(m_cvData->prev);
		cv::Mat diffMat = video::toCv(m_cvData->diff);
		video::absdiff(prevMat, cam, diffMat);
		cam.copyTo(m_cvData->prev);

		cv::Scalar m = cv::mean(diffMat);
		float diffMean = cv::mean(Mat(m))[0];
		printf("%f\n", diffMean);
		if (diffMean <1.5 )
		{
			if (!m_cvData->reset && m_cvData->cvCalib->add(cam))
			{
				m_cvData->reset = true;
				m_cvData->cvCalib->calibrate();
				printf("Calibrate\n");
				m_cvData->lastTime = gEngine.getTimer()->getSeconds();
			}
		}
		else
			m_cvData->reset = false;
	}
}

void Application::onDone()
{
}


void Application::onRenderDone(scene::ViewPort*vp)
{
}

}


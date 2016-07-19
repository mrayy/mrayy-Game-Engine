

#include "stdafx.h"
#include "Application.h"
#include "ImageSetResourceManager.h"
#include "GUIThemeManager.h"
#include "FontResourceManager.h"
#include "TextureResourceManager.h"
#include "DynamicFontGenerator.h"
#include "StreamReader.h"
#include "FlyCameraManager.h"

#include "DirectShowVideoGrabber.h"
#include "FlyCameraVideoGrabber.h"
#include "CVChessBoard.h"
#include "CVCalibration.h"
#include "CVUtilities.h"
#include "CVWrappers.h"
#include "CVCameraCalib.h"
#include "IMonitorDeviceManager.h"
#include "IMonitorDevice.h"



namespace mray
{

	class ApplicationImpl
	{
	public:

		GCPtr<video::ICameraVideoGrabber> camera;
		GCPtr<video::VideoGrabberTexture> grabber;
		video::CVCameraCalib cameraCalib;
		video::CVChessBoard chessboard;

		GCPtr<video::ITexture> correctedTex;

		bool captureNext;

		cv::Mat lastImage;
		cv::Mat diffMat;

		float _lastTime;
		math::rectf chessBB;
		bool _found;
		bool _calibrated;

		float _minProjError;

		video::RenderWindow *CVWindow;
		scene::ViewPort* CVVP;
		std::vector<math::vector2d> calibPoints;

		ApplicationImpl()
		{
			_lastTime = gEngine.getTimer()->getSeconds();
			_found = false;
			reset();

		}

		void reset()
		{
			_calibrated = false;
			_found = false;
			cameraCalib.reset();
			_minProjError = 100;
			captureNext = false;
		}

		bool IsDiff(float threshold,float dt)
		{
			float t = gEngine.getTimer()->getSeconds();
			if (t - _lastTime < dt)
				return false;
			_lastTime = t;
			cv::Mat cam = video::toCv(camera->GetLastFrame());
			float absDiff;
			video::absdiff(cam, lastImage, diffMat);
			cv::Mat m = cv::Mat(video::mean(diffMat));
			absDiff = video::mean(m)[0];
			if (absDiff > threshold)
			{
				cam.copyTo(lastImage);
				return true;
			}
			return false;
		}
		math::vector2d ToScreenSpace(const math::vector2d &pt, const math::vector2d &screenSize)
		{
			return (pt* screenSize) / (math::vector2d)camera->GetFrameSize();
		}
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

	if (event->getType() == ET_Keyboard)
	{
		KeyboardEvent* e = (KeyboardEvent*)event;
		if (e->press)
		{
			if (e->key == KEY_R)
			{
				m_impl->reset();
			}
			if (e->key == KEY_SPACE)
			{
				m_impl->captureNext = true;
			}
			if (e->key == KEY_C)
			{
				_Calibrate();
			}
		}
	}
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

	new video::FlyCameraManager();
	m_impl->camera = new video::DirectShowVideoGrabber();
	//m_impl->camera = new video::FlyCameraVideoGrabber();
	m_impl->camera->InitDevice(1, 640,480, 60);
	m_impl->grabber = new video::VideoGrabberTexture();
	m_impl->grabber->Set(m_impl->camera, 0);
	m_impl->chessboard.Setup(math::vector2di(7, 5),2.8);
	m_impl->cameraCalib.setPatternSize(7, 5);
	m_impl->cameraCalib.setPatternType(video::CHESSBOARD);
	m_impl->camera->Start();
	m_impl->correctedTex = getDevice()->createEmptyTexture2D(false);
	m_impl->correctedTex->setMipmapsFilter(false);
	m_impl->correctedTex->createTexture(math::vector3di(m_impl->camera->GetFrameSize().x, m_impl->camera->GetFrameSize().y,1),video::EPixel_R8G8B8);

	video::allocate(m_impl->lastImage, m_impl->camera->GetFrameSize().x, m_impl->camera->GetFrameSize().y, CV_MAKETYPE(CV_8U, 3));
	video::allocate(m_impl->diffMat, m_impl->camera->GetFrameSize().x, m_impl->camera->GetFrameSize().y, CV_MAKETYPE(CV_8U, 3));

	{
		OS::IStreamPtr s = gFileSystem.openFile(gFileSystem.getAppPath() + "ProjectionSettings.txt", OS::TXT_READ);
		if (!s.isNull())
		{
			OS::StreamReader rdr(s);
			core::string l = rdr.readLine();
			core::StringConverter::parse(l, m_impl->chessBB);
			s->close();
			m_impl->_found = true;

		}
	}
	if (false)
	{
		{
			int targetMonitor = 2;
			targetMonitor = core::StringConverter::toInt(extraOptions.GetOptionByName("CVDisplay")->getValue());
			OptionContainer opt;
			opt["title"].value = "Hands Window";
			opt["VSync"].value = "false";
			opt["top"].value = "0";
			opt["left"].value = "0";
			//opt["border"].value = "none";
			video::IMonitorDevice* monitor = video::IMonitorDeviceManager::getInstance().GetMonitor(targetMonitor);
			if (!monitor)
			{
				targetMonitor = 0;
				monitor = video::IMonitorDeviceManager::getInstance().GetMonitor(targetMonitor);
			}
			opt["Monitor"].value = core::StringConverter::toString(targetMonitor);
			m_impl->CVWindow = gEngine.getDevice()->CreateRenderWindow("CV Window", monitor->GetSize(), false, opt, 0);
			m_impl->CVVP = m_impl->CVWindow->CreateViewport(mT("Main"), 0, 0, math::rectf(0, 0, 1, 1), 0);
			AddRenderWindow(m_impl->CVWindow);
			m_impl->CVVP->AddListener(this);

		}
	}
}


void Application::draw(scene::ViewPort* vp)
{
}

void Application::_Calibrate()
{

	{
		if (m_impl->cameraCalib.calibrate())
		{
			printf("Camera calibrated\n");
			printf("Projection error: %f\n", m_impl->cameraCalib.getReprojectionError());
			if (m_impl->cameraCalib.getReprojectionError() < m_impl->_minProjError)
			{
				m_impl->_minProjError = m_impl->cameraCalib.getReprojectionError();
				m_impl->_calibrated = true;

				OS::IStreamPtr stream = gFileSystem.openFile(gFileSystem.getAppPath() + "CameraCalibration.txt", OS::TXT_WRITE);
				OS::StreamWriter wrtr(stream);
				std::stringstream ss;

				cv::Mat distCoeff = m_impl->cameraCalib.getDistCoeffs();
				video::Intrinsics intr = m_impl->cameraCalib.getDistortedIntrinsics();

				math::vector2d center;
				math::vector2d focalXY;
				center.x = intr.getCameraMatrix().at<double>(2) / m_impl->camera->GetFrameSize().x;
				center.y = intr.getCameraMatrix().at<double>(5) / m_impl->camera->GetFrameSize().y;
				focalXY.x = intr.getCameraMatrix().at<double>(0) / m_impl->camera->GetFrameSize().x;
				focalXY.y = intr.getCameraMatrix().at<double>(4) / m_impl->camera->GetFrameSize().y;

				math::vector4df coeff(distCoeff.at<double>(0), distCoeff.at<double>(1), distCoeff.at<double>(2), distCoeff.at<double>(3));
				ss << intr.getFov() << "\n"
					<< intr.getFocalLength() << "\n"
					<< core::StringConverter::toString(center) << "\n"
					<< core::StringConverter::toString(focalXY) << "\n"
					<< core::StringConverter::toString(coeff) << "\n";


				wrtr.writeLine(ss.str());
				stream->close();

			}
		}
	}
}
void Application::_RenderMain(video::RenderWindow* wnd)
{
	video::TextureUnit tu;

	if (m_impl->grabber->Blit())
	{
		if (m_impl->captureNext && !m_impl->_calibrated && m_impl->IsDiff(5, 500))
		{
			if (m_impl->cameraCalib.add(toCv(m_impl->grabber->GetGrabber()->GetLastFrame())))
			{
				m_impl->captureNext = false;
				printf("Added new frame\n");
			}
		}
	}
	if (!m_impl->_calibrated)
	{
		tu.SetTexture(m_impl->grabber->GetTexture());
	}
	else
	{
		const video::ImageInfo* ifo = m_impl->grabber->GetGrabber()->GetLastFrame();
		m_impl->cameraCalib.undistort(toCv(ifo));

		video::LockedPixelBox box(math::box3d(0, ifo->Size), ifo->format, ifo->imageData);
		m_impl->correctedTex->getSurface(0)->blitFromMemory(box);
		tu.SetTexture(m_impl->correctedTex);
	}
	getDevice()->useTexture(0, &tu);
	getDevice()->draw2DImage(math::rectf(0, wnd->GetSize()), 1);

	for (int i = 0; i < m_impl->calibPoints.size(); ++i)
	{
		math::vector2d pt = m_impl->calibPoints[i] * wnd->GetSize();
		getDevice()->draw2DRectangle(math::rectf(pt - 5, pt + 5), video::SColor((float)i / (float)m_impl->calibPoints.size(), 0, 0, 1));
	}

	if (m_impl->_found)
	{
		getDevice()->setLineWidth(1);
		math::rectf rc = m_impl->chessBB;
		rc.ULPoint *= wnd->GetSize();
		rc.BRPoint *= wnd->GetSize();
		getDevice()->draw2DRectangle(rc, video::SColor(1, 0, 0, 1), false);
		getDevice()->setLineWidth(3);
	}

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

		LOG_OUT(core::string(mT("Count= ") + core::StringConverter::toString(m_impl->cameraCalib.size())), 100,100);

	//	LOG_OUT(core::string(mT("Found= ") + core::StringConverter::toString(found)), 100, 10);

		m_guiRenderer->Flush();
	}
}
void Application::_RenderCV(video::RenderWindow* wnd)
{
	//getDevice()->setRenderTarget(m_rt);

	std::vector<math::vector2d> points;

	//if (m_impl->grabber->Blit())
	{
		if (!m_impl->_found && m_impl->IsDiff(5, 0.5))
		{
			if (m_impl->chessboard.FindInImage(m_impl->camera->GetLastFrame(), points))
			{
				m_impl->calibPoints.resize(points.size());
				m_impl->chessBB.reset(m_impl->calibPoints[0] = m_impl->ToScreenSpace(points[0], 1));
				for (int i = 1; i < points.size(); ++i)
				{
					m_impl->calibPoints[i] = m_impl->ToScreenSpace(points[i], 1);
					m_impl->chessBB.addPoint(m_impl->calibPoints[i]);
				}
				m_impl->_found = true;
				printf("Found\n");

				math::vector2d squareSize = m_impl->chessBB.getSize() / m_impl->chessboard.GetSquares();
				m_impl->chessBB.addPoint(m_impl->chessBB.ULPoint - squareSize);
				m_impl->chessBB.addPoint(m_impl->chessBB.BRPoint + squareSize);

				OS::IStreamPtr stream = gFileSystem.openFile(gFileSystem.getAppPath()+ "ProjectionSettings.txt", OS::TXT_WRITE);
				OS::StreamWriter wrtr(stream);
				wrtr.writeLine(core::StringConverter::toString(m_impl->chessBB));
				wrtr.writeLine(core::StringConverter::toString(wnd->GetSize()));
				stream->close();
			}
		}
	}
	getDevice()->set2DMode();
	m_impl->chessboard.Draw(math::rectf(0, wnd->GetSize()));

}
void Application::WindowPostRender(video::RenderWindow* wnd)
{
	if (wnd == m_impl->CVWindow)
	{
		_RenderCV(wnd);
	}
	else
	{
		_RenderMain(wnd);
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


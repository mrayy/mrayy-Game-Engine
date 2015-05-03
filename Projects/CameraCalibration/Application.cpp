

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
#include "CVUtilities.h"
#include "CVWrappers.h"

#include <windows.h>


namespace mray
{

	class ApplicationImpl
	{
	public:

		GCPtr<video::ICameraVideoGrabber> camera;
		GCPtr<video::VideoGrabberTexture> grabber;
		video::CVChessBoard chessboard;

		cv::Mat lastImage;
		cv::Mat diffMat;

		float _lastTime;
		math::rectf chessBB;

		video::RenderWindow *CVWindow;
		scene::ViewPort* CVVP;
		std::vector<math::vector2d> calibPoints;

		ApplicationImpl()
		{
			_lastTime = gEngine.getTimer()->getSeconds();
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


	video::allocate(m_impl->lastImage, m_impl->camera->GetFrameSize().x, m_impl->camera->GetFrameSize().y, CV_MAKETYPE(CV_8U, 3));
	video::allocate(m_impl->diffMat, m_impl->camera->GetFrameSize().x, m_impl->camera->GetFrameSize().y, CV_MAKETYPE(CV_8U, 3));

	{
		{
			OptionContainer opt;
			opt["title"].value = "Hands Window";
			opt["VSync"].value = "false";
			opt["top"].value = "0";
			opt["left"].value = "0";
			//opt["border"].value = "none";
			opt["Monitor"].value = core::StringConverter::toString(2);
			m_impl->CVWindow = gEngine.getDevice()->CreateRenderWindow("CV Window", GetRenderWindow(0)->GetSize(), false, opt, 0);
			m_impl->CVVP = m_impl->CVWindow->CreateViewport(mT("Main"), 0, 0, math::rectf(0, 0, 1, 1), 0);
			AddRenderWindow(m_impl->CVWindow);
			m_impl->CVVP->AddListener(this);

		}
	}
}


void Application::draw(scene::ViewPort* vp)
{
}

void Application::_RenderMain(video::RenderWindow* wnd)
{
	video::TextureUnit tu;

	tu.SetTexture(m_impl->grabber->GetTexture());
	getDevice()->useTexture(0, &tu);
	getDevice()->draw2DImage(math::rectf(0,wnd->GetSize()),1);

	getDevice()->setLineWidth(3);
	getDevice()->draw2DRectangle(m_impl->chessBB, video::SColor(1, 0, 0, 1), false);
	for (int i = 0; i < m_impl->calibPoints.size(); ++i)
	{
		getDevice()->draw2DRectangle(math::rectf(m_impl->calibPoints[i] - 5, m_impl->calibPoints[i] + 5), video::SColor((float)i / (float)m_impl->calibPoints.size(), 0, 0, 1));
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


	//	LOG_OUT(core::string(mT("Found= ") + core::StringConverter::toString(found)), 100, 10);

		m_guiRenderer->Flush();
	}
}
void Application::_RenderCV(video::RenderWindow* wnd)
{
	//getDevice()->setRenderTarget(m_rt);

	bool found = false;

	std::vector<math::vector2d> points;

	if (m_impl->grabber->Blit())
	{
		if (m_impl->IsDiff(10, 0.5))
		{
			if (m_impl->chessboard.FindInImage(m_impl->camera->GetLastFrame(), points))
			{
				m_impl->calibPoints.resize(points.size());
				m_impl->chessBB.reset(m_impl->calibPoints[0] = m_impl->ToScreenSpace(points[0], wnd->GetSize()));
				for (int i = 1; i < points.size(); ++i)
				{
					m_impl->calibPoints[i] = m_impl->ToScreenSpace(points[i], wnd->GetSize());
					m_impl->chessBB.addPoint(m_impl->calibPoints[i]);
				}
				found = true;
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
	}else
		_RenderMain(wnd);

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


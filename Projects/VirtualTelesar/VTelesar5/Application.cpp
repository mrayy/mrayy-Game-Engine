



#include "stdafx.h"
#include "Application.h"
#include "FontResourceManager.h"
#include "ImageSetResourceManager.h"
#include "GUIThemeManager.h"
#include "StringUtil.h"

#include <TheoraManager.h>
#include <FSLManager.h>


#include "IThreadManager.h"
#include "RenderWindowUtils.h"

#include "VTAppGlobals.h"
#include "ApplicationMenuState.h"
#include "TextureResourceManager.h"
#include "win32NetInterface.h"
#include "GameComponentCreator.h"
#include "TextureSourceFactory.h"
#include "CameraTextureSource.h"

#include "mrayOIS.h"
#include "DynamicFontGenerator.h"
#include "VTelesarRenderingState.h"
#include "NullRenderState.h"

namespace mray
{
namespace VT
{


Application::Application():BenchmarkItem("Application")
{
	new VTAppGlobals();
	VTAppGlobals::Instance()->App = this;
	m_renderBenchMark=new BenchmarkItem("Render");
	m_updateBenchMark=new BenchmarkItem("Update");
	addSubItem(m_renderBenchMark);
	addSubItem(m_updateBenchMark);
	m_flipEyes=false;

	m_limitFps=true;
	m_tbRenderer = 0;

}
Application::~Application()
{
	m_videoManager=0;
	m_soundManager = 0;
	delete m_tbRenderer;
}

void Application::_InitResources()
{
	CMRayApplication::loadResourceFile(mT("Resources.stg"));


	gImageSetResourceManager.loadImageSet(mT("VistaCG_Dark.imageset"));
	GCPtr<OS::IStream> themeStream = gFileSystem.createBinaryFileReader(mT("VistaCG_Dark.xml"));
	GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
	GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG_Dark"));

	//load font
	GCPtr<GUI::DynamicFontGenerator> font = new GUI::DynamicFontGenerator();
	font->SetFontName(L"Arial");
	font->SetTextureSize(1024);
	font->SetFontResolution(24);
	font->Init();

	//GCPtr<GUI::IFont>font = gFontResourceManager.loadFont(mT("Calibrib_font.fnt"));
	//gFontResourceManager.loadFont(mT("OCRAStd.fnt"));
	gFontResourceManager.setDefaultFont(font);

	gLogManager.log("Resources Loaded", ELL_SUCCESS);
}
void Application::onEvent(Event* event)
{
	CMRayApplication::onEvent(event);

	if(event->getType()==ET_Mouse)
	{
		MouseEvent* e=(MouseEvent* )event;
		if(m_horizontalFlip)
		{
			e->pos.x=event->GetOwnerRenderWindow()->GetSize().x- e->pos.x;
		}
	}
	if (m_tbRenderer)
	{
		for (int i = 0; i < m_tbRenderer->GetViewportCount(); ++i)
		{
			if (m_renderingStateManager->OnEvent(event, m_tbRenderer->GetViewport(i)->getAbsRenderingViewPort()))
				break;
		}
	}



	if(event->getType()==ET_Keyboard)
	{
		KeyboardEvent* e=(KeyboardEvent*)event;
		if(e->press && e->key==KEY_F9)
		{
			VTAppGlobals::Instance()->IsDebugging = !VTAppGlobals::Instance()->IsDebugging;
		}else if(e->press && e->key==KEY_F10)
		{
			m_horizontalFlip=!m_horizontalFlip;
		}else if(e->press && e->key==KEY_F11)
		{
			m_flipEyes=!m_flipEyes;
		}
		/*	if(e->press && e->key==KEY_F12)
		{

			video::LockedPixelBox box=m_screenShot->getSurface(0)->lock(math::box3d(0,0,0,GetRenderWindow()->GetSize().x,GetRenderWindow()->GetSize().y,1),video::IHardwareBuffer::ELO_Discard);
			GetRenderWindow()->TakeScreenShot(box);
			m_screenShot->getSurface(0)->unlock();
			gTextureResourceManager.writeResourceToDist(m_screenShot,"screenShot.tga");

		}*/
	}
}

void Application::init(const OptionContainer &extraOptions)
{
	CMRayApplication::init(extraOptions);

	VTAppGlobals::Instance()->Init();

	m_2ndWnd=0;

	VT::VTAppGlobals::Instance()->Load("VTSettings.conf");
	{

		core::string v=extraOptions.GetOptionValue("Debugging");
		if(v=="Yes")
			VT::VTAppGlobals::Instance()->IsDebugging = true;
		else
			VT::VTAppGlobals::Instance()->IsDebugging = false;
	}
	{

		// 		core::string v=extraOptions.GetOptionValue("Camera");
		// 		if(v=="USB")
		VT::VTAppGlobals::Instance()->CameraType = EWebCamera;
		// 		else
		//			VTVT::VTAppGlobals::Instance()->CameraType=EFlyCapture;
	}
	{
		m_tbRenderer = new TBee::TBeeRenderer();
		m_tbRenderer->AddListener(this);
		m_tbRenderer->Init(GetRenderWindow(0));
	}
	{

		core::string v=extraOptions.GetOptionValue("Physics");
		if(v=="Yes")
			VT::VTAppGlobals::Instance()->Physics = true;
		else
			VT::VTAppGlobals::Instance()->Physics = false;
	}
	{

		core::string v=extraOptions.GetOptionValue("FlipCameras");
		if(v=="Yes")
			VT::VTAppGlobals::Instance()->FlipCameras = true;
		else
			VT::VTAppGlobals::Instance()->FlipCameras = false;
	}
	{

		VT::VTAppGlobals::Instance()->GhostServerIP = core::StringUtil::Trim(extraOptions.GetOptionValue("GhostServer"));
	}
	{
		core::string v=extraOptions.GetOptionValue("HorizontalFlip");
		if(v=="Yes")
			m_horizontalFlip=true;
		else
			m_horizontalFlip=false;
	}

	video::TextureSourceFactory::getInstance().RegisterFactory(new video::TextureSourceFactory_CameraTextureSource());

	_InitResources();

	network::createWin32Network();


	
	m_drawUI=false;

	m_guiRender=new GUI::GUIBatchRenderer();
	m_guiRender->SetDevice(getDevice());

// 	m_guiManager=new GUI::GUIManager(getDevice());
// 	m_guiManager->SetActiveTheme(GUI::GUIThemeManager::getInstance().getActiveTheme());

	m_soundManager=new sound::FSLManager();
	m_videoManager=new video::TheoraManager();
	m_videoManager->setSoundManager(m_soundManager);


	m_screenShot=getDevice()->createEmptyTexture2D(true);
	m_screenShot->setMipmapsFilter(false);
	m_screenShot->createTexture(math::vector3d(GetRenderWindow()->GetSize().x,GetRenderWindow()->GetSize().y,1),video::EPixel_B8G8R8A8);


	{
	//	m_stereoVP[0]=new scene::ViewPort(mT("Left"),0,0,GetRenderWindow(0),math::rectf(0,0,1,1),0);
		core::string v=extraOptions.GetOptionValue("Stereoscopic");
		if(v=="DVI")
		{
			OptionContainer opt=extraOptions;
			opt["Monitor"].value="1";
			opt["title"].value="Second Window";
			m_2ndWnd= getDevice()->CreateRenderWindow(mT("Secondary"),GetRenderWindow()->GetSize(),GetRenderWindow()->IsFullScreen(),opt,0);
			AddRenderWindow(m_2ndWnd);
			/*
			m_mainVP[1]=GetRenderWindow(1)->CreateViewport(mT("2nd"),0,0,math::rectf(0,0,1,1),0);
			m_stereoVP[1]=new scene::ViewPort(mT("Right"),0,0,GetRenderWindow(1),math::rectf(0,0,1,1),0);//GetRenderWindow(1)->CreateViewport(mT("Right"),0,0,math::rectf(0,0,1,1),0);
			*/
			GetRenderWindow(0)->SetActive(true);
			VTAppGlobals::Instance()->stereoMode = TBee::ERenderStereoMode::SideBySide;
		}
		/*else
		{
			m_stereoVP[1]=new scene::ViewPort(mT("Right"),0,0,GetRenderWindow(0),math::rectf(0,0,1,1),0);//GetRenderWindow(0)->CreateViewport(mT("Right"),0,0,math::rectf(0,0,1,1),0);
			m_stereoVP[1]->SetClearColor(video::DefaultColors::Green);
		}


		m_stereoRenderer=new scene::StereoRenderer(m_stereoVP[0],m_stereoVP[1],0);
		m_stereoRenderer->SetOffset(0.06);
		m_stereoRenderer->SetMode(scene::EStereo_None);
		if(v=="OverUnder" || v=="DVI")
			m_stereoRenderer->SetMode(scene::EStereo_TopDown);
		else if(v=="SideBySide")
			m_stereoRenderer->SetMode(scene::EStereo_SideBySide);
		getDevice()->setClearColor(video::SColor(0.6,0.8,0.9,0));
		*/
	}
#if 0
	{
		int c = IsStereo() ? 2 : 1;
		for (int i = 0; i < c; ++i)
		{

			m_rtTexture[i] = getDevice()->createEmptyTexture2D(true);
			m_rtTexture[i]->setMipmapsFilter(false);

			m_renderTarget[i] = getDevice()->createRenderTarget("", m_rtTexture[i], 0, 0, 0);
		}
	}
	m_mainVP[0]=GetRenderWindow()->CreateViewport(mT("Main"),0,0,math::rectf(0,0,1,1),0);
	m_mainVP[1]=0;
	m_stereoVP[0]=m_stereoVP[1]=0;


#endif

	m_mainVP = GetRenderWindow()->CreateViewport("MainVP", 0, 0, math::rectf(0, 0, 1, 1), 0);

	m_renderingStateManager = new TBee::RenderingStateManager();

	{
		VTelesarRenderingState* vtRS = new VTelesarRenderingState("TelesarState");
		TBee::IRenderingState* nullSt = new TBee::NullRenderState();
		m_renderingStateManager->AddState(nullSt);
		m_renderingStateManager->AddState(vtRS);
		m_renderingStateManager->AddTransition(nullSt, vtRS, 0);
		m_renderingStateManager->SetInitialState(nullSt);

		nullSt->InitState();
		vtRS->InitState();
	}
	{
		m_previewWnd = 0;
		if (extraOptions.GetOptionValue("Preview") == "Yes")
		{
			OptionContainer opt;
			opt["title"].value = "Preview";
			opt["VSync"].value = "false";
			opt["Monitor"].value = "0";
			m_previewWnd = getDevice()->CreateRenderWindow("Preview", math::vector2di(1280, 720), false, opt, 0);
			m_previewWnd->CreateViewport(mT("Main"), 0, 0, math::rectf(0, 0, 1, 1), 0);
			AddRenderWindow(m_previewWnd);
			InputCreationPack pack(m_previewWnd);
			pack.WinSize = m_previewWnd->GetSize();
			pack.exclusiveMouse = false;
			m_inputManager = CreateOISInputManager(pack);
			VTAppGlobals::Instance()->inputMngr = m_inputManager;


			video::ITexturePtr t = gEngine.getDevice()->createEmptyTexture2D(false);
			t->setMipmapsFilter(false);
			m_previewRT = gEngine.getDevice()->createRenderTarget("", t, 0, 0, 0);

		}

	}
}

int Application::getEyeIndex(bool left)
{
	int index[2]={0,1};
	if(m_flipEyes && IsStereo())
		math::Swap(index[0],index[1]);
	return index[IsStereo()?(left?0:1):0];
}

void Application::RenderUI(const math::rectf& rc)
{
	if(m_drawUI )
	{


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
			msg+=core::StringConverter::toString((int)gEngine.getFPS()->getFPS());
			font->print(math::rectf(rc.BRPoint.x-250,rc.BRPoint.y-50,10,10),&attr,0,msg,m_guiRender);
			yoffset+=attr.fontSize;

			yoffset=0;
			attr.hasShadow=true;
			attr.fontColor.Set(1,1,1,1);
			attr.fontSize=14;
			msg=mT("Render: ");
			msg+=core::StringConverter::toString(m_renderBenchMark->getAverageTime(),2);
			font->print(math::rectf(rc.ULPoint.x+50,rc.ULPoint.y+50+yoffset,10,10),&attr,0,msg,m_guiRender);
			yoffset+=attr.fontSize;

			msg=mT("Update: ");
			msg+=core::StringConverter::toString(m_updateBenchMark->getAverageTime(),2);
			font->print(math::rectf(rc.ULPoint.x+50,rc.ULPoint.y+50+yoffset,10,10),&attr,0,msg,m_guiRender);
			yoffset+=attr.fontSize;


		}

		m_guiRender->Flush();
		getDevice()->useShader(0);
	}
}

void Application::WindowPostRender(video::RenderWindow* wnd)
{
	_AUTO_BENCHMARK(m_renderBenchMark);

	getDevice()->set2DMode();

	if (wnd == m_previewWnd)
	{
		math::rectf rc(0, wnd->GetSize());
		video::TextureUnit tex;

		if (m_previewRT->GetSize() != wnd->GetSize())
		{
			m_previewRT->GetColorTexture()->createTexture(math::vector3d(wnd->GetSize().x, wnd->GetSize().y, 1), video::EPixel_R8G8B8);
		}
		m_renderingStateManager->OnDraw(rc, m_previewRT, TBee::Eye_Right);
		RenderUI(rc);
		getDevice()->setRenderTarget(0);
		//	tex.SetTexture(m_tbRenderer->GetEyeImage(0)->GetColorTexture());
		tex.SetTexture(m_previewRT->GetColorTexture());
		getDevice()->useTexture(0, &tex);

		math::rectf trc;
		getDevice()->draw2DImage(rc, 1);
		return;
	}
	getDevice()->setViewport(m_mainVP);
	m_tbRenderer->Render(m_mainVP);
	return;

#if 0
	if(wnd==m_previewWnd)
	{
		getDevice()->set2DMode();
		math::rectf rc(0,wnd->GetSize());
		video::TextureUnit tex;
		tex.SetTexture(m_rtTexture[0]);
		getDevice()->useTexture(0,&tex);

		math::rectf trc;
		if(m_horizontalFlip)
			trc=math::rectf(1,0,0,1);
		else 
			trc=math::rectf(0,0,1,1);
		getDevice()->draw2DImage(rc,1,0,&trc);
		return;
	}
	scene::ViewPort* viewpt=m_mainVP[0];
	int inds[]={0,1};
	if(wnd==m_2ndWnd)
	{
		math::Swap(inds[0],inds[1]);
		viewpt=m_mainVP[1];
	//if(viewpt==m_mainVP[1])//m_stereoRenderer->GetLeftVP())
	}
	bool singleWindow=true;
		
	if(!IsStereo())
	{
	}else
	{
		if(m_flipEyes)
		{
			math::Swap(inds[0],inds[1]);
		}
		if(m_2ndWnd!=0)
		{
			singleWindow=false;
		}
	}

	

	int index=0;

	int c= IsStereo()&&singleWindow ? 2:1;

	math::vector2d szOffset=viewpt->GetSize()/c;
	
	for(int i=0;i<c;++i)
	{
		int index=inds[i];
		scene::ViewPort* vp=m_stereoVP[index];
		math::rectf rc=math::rectf(0,vp->getAbsRenderingViewPort().getSize());
		if(m_rtTexture[index]->getSize().x!=rc.getWidth() ||
			m_rtTexture[index]->getSize().y!=rc.getHeight())
		{
			m_rtTexture[index]->createTexture(math::vector3d(rc.getWidth(),rc.getHeight(),1),video::EPixel_R8G8B8);
		}
		getDevice()->set2DMode();
		getDevice()->setRenderTarget(m_rtTexture[index],true,true,video::SColor(1,1,1,0));
			
		m_renderingStateManager->OnDraw(rc, m_renderTarget[index], index == 0?TBee::Eye_Left:TBee::Eye_Right);
//			getDevice()->draw2DRectangle(rc,video::SColor(index,1,0,1));

	}

	getDevice()->setRenderTarget(0,true,true,video::SColor(1,0,0,0));
	for(int i=0;i<c;++i)
	{
		int index=inds[i];

		scene::ViewPort* vp=m_stereoVP[index];
		vp->updateViewPort();
		getDevice()->setViewport(vp);

		math::rectf rc=math::rectf(0,vp->getAbsRenderingViewPort().getSize());//vp->getAbsRenderingViewPort().getSize());
		if(singleWindow)
		{
			if(m_stereoRenderer->GetMode()==scene::EStereo_SideBySide)
			{
				rc.ULPoint.x=index*szOffset.x;
				rc.BRPoint.x+=index*szOffset.x;
			}else if(m_stereoRenderer->GetMode()==scene::EStereo_TopDown)
			{
				rc.ULPoint.y=index*szOffset.y;
				rc.BRPoint.y+=index*szOffset.y;
			}
		}

		video::TextureUnit tex;
		tex.SetTexture(m_rtTexture[index]);
		getDevice()->useTexture(0,&tex);

		math::rectf trc;
		if(m_horizontalFlip)
			trc=math::rectf(1,0,0,1);
		else 
			trc=math::rectf(0,0,1,1);
		getDevice()->draw2DImage(rc,1,0,&trc);
	//	getDevice()->draw2DRectangle(rc,video::SColor(index,1,0,1));
		RenderUI(rc);
		//getDevice()->setScissorTest(false);
		//getDevice()->setViewport(vp);
		//getDevice()->useShader(0);

	}
#endif
}
void Application::update(float dt)
{
	_AUTO_BENCHMARK(m_updateBenchMark);

	CMRayApplication::update(dt);

	if(m_videoManager)
		m_videoManager->Update(dt);
	if(m_soundManager)
		m_soundManager->runSounds(dt);

	m_renderingStateManager->OnUpdate(dt);

	//OS::IThreadManager::getInstance().sleep(1000/30);

}
void Application::onDone()
{
	//VT::VTAppGlobals::Instance()->Save("VTSettings.conf");

}

void Application::OnRendererDraw(TBee::TBeeRenderer* r, const math::rectf& vp, video::IRenderTarget* rt, TBee::ETargetEye eye)
{

	m_renderingStateManager->OnDraw(vp, rt, eye);
	RenderUI(vp);

}
/*
scene::ViewPort* Application::GetViewport(bool left)
{
	return m_stereoVP[getEyeIndex(left)];
}*/

bool Application::IsStereo()
{
	return m_tbRenderer->GetViewportCount() == 2;
//	return m_stereoRenderer->GetMode()!=scene::SCT_NONE;
}

void Application::OnStateChanged(TBee::IRenderingState* old, TBee::IRenderingState* state)
{
	m_renderBenchMark->reset();
	m_updateBenchMark->reset();
}

}
}

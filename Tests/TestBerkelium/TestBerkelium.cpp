// TestBerkelium.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ImageSetResourceManager.h"
#include "CMRayApplication.h"
#include "GUIThemeManager.h"
#include "FontResourceManager.h"
#include "GUIBatchRenderer.h"
#include "CFPS.h"
#include "GCCollector.h"
#include "ViewPort.h"
#include "JobPool.h"
#include "IGUIElement.h"
#include "GUIManager.h"
#include "GUIElementRegion.h"
#include "IGUIPanelElement.h"

using namespace mray;
using namespace math;
using namespace core;


class BrowserWindow:public Berkelium::WindowDelegate
{
protected:
	math::vector2di m_size;
	GCPtr<video::ITexture> m_texture;

	Berkelium::Window* m_window;
	uchar* m_flipped;

	bool m_refresh;

public:
	BrowserWindow(int w,int h,Berkelium::Context* c)
		:m_size(w,h)
	{
		m_texture=Engine::getInstance().getDevice()->createEmptyTexture2D(true);
		m_texture->setMipmapsFilter(false);
		m_window=Berkelium::Window::create(c);
		m_window->setDelegate(this);

		m_refresh=true;
		m_flipped=0;
		Resize(w,h);
	}
	~BrowserWindow()
	{
		delete m_window;
		delete [] m_flipped;

	}
	video::ITexture* GetTexture(){return m_texture;}

	Berkelium::Window* GetWindow(){return m_window;}

	void Resize(int w,int h)
	{
		m_size.x=w;
		m_size.y=h;
		delete [] m_flipped;
		m_window->resize(w,h);
		m_texture->setAnisotropicFilter(true);

		m_texture->createTexture(math::vector3di(m_size.x,m_size.y,1),video::EPixel_X8R8G8B8);
		m_flipped=new uchar[w*h*4];


	}

	math::vector2di GetSize(){return m_size;}

	virtual void onStartLoading(Berkelium::Window *win, Berkelium::URLString newURL) 
	{
		std::cout << "onStartLoading: " << newURL.data() << std::endl;
	}

	math::vector2d mapCoord(const math::vector2d& c,const math::vector2d& screen)
	{
		return (c*m_size)/screen;
	}

	virtual void onPaint(
		Berkelium::Window *win,
		const unsigned char *sourceBuffer,
		const Berkelium::Rect &sourceBufferRect,
		size_t numCopyRects,
		const Berkelium::Rect *copyRects,
		int dx, int dy,
		const Berkelium::Rect &scrollRect)
	{
		std::cout<<"Paint:"<<std::endl;
		video::IHardwarePixelBuffer* surface= m_texture->getSurface(0);


		video::ColorConverter::convert32BitTo32Bit(sourceBuffer,m_flipped,math::vector2d(sourceBufferRect.width(),sourceBufferRect.height()),0,1,0);

		video::LockedPixelBox lockedBox(
			box3d(sourceBufferRect.left(),sourceBufferRect.top(),0,
			sourceBufferRect.right(),sourceBufferRect.bottom(),1),video::EPixel_X8R8G8B8,(void*)m_flipped);

		lockedBox.box.MinP.y=m_size.y-lockedBox.box.MinP.y;
		lockedBox.box.MaxP.y=m_size.y-lockedBox.box.MaxP.y;

		math::Swap(lockedBox.box.MinP.y,lockedBox.box.MaxP.y);

		if(m_refresh)
		{
			surface->blitFromMemory(lockedBox);
			m_refresh=false;
			return;
		}
		
		for(int i=0;i<numCopyRects;++i)
		{
			std::cout << "\tRect: " << copyRects[i].width()<<" "<<copyRects[i].height()<< std::endl;

	 		math::box3d lockBox(copyRects[i].left(),copyRects[i].top(),0,
				copyRects[i].right(),copyRects[i].bottom(),1);
			lockBox.MinP.y=m_size.y-lockBox.MinP.y;
			lockBox.MaxP.y=m_size.y-lockBox.MaxP.y;
			math::Swap(lockBox.MinP.y,lockBox.MaxP.y);

			video::LockedPixelBox lb=lockedBox.getSubBox(lockBox);
			surface->blitFromMemory(lb);
// 			const video::LockedPixelBox& lb= surface->lock(lockBox,video::IHardwareBuffer::ELO_Discard);
// 			memcpy(lb.data,sourceBuffer,sourceBufferRect.width()*sourceBufferRect.height()*4
		}


	}

	virtual void onLoadError(Berkelium::Window *win, const char* error, size_t errorLength) {
		std::cout << "Error! " << std::string(error, errorLength) << std::endl;
	}

};

namespace mray
{


namespace GUI
{

class GUIWebPageControl:public IGUIElement
{
protected:
	GCPtr<BrowserWindow> m_window;
	video::TextureUnit m_texUnit;


	bool _OnKeyboardEvent(KeyboardEvent* e)
	{
		if(!m_window)
			return false;
		wchar_t c[2];
		c[0]=e->Char;
		c[1]=0;
		m_window->GetWindow()->textEvent(c,1);

		return true;
	}
	bool _OnMouseEvent(MouseEvent* e)
	{
		if(!m_window)
			return false;
		math::rectf rc=GetDefaultRegion()->GetRect();
		if(!rc.IsPointInside(e->pos))
			return false;
		if(e->event==MET_MOVED)
		{
			math::vector2d mapped=e->pos-rc.ULPoint;
			mapped=m_window->mapCoord(mapped,rc.getSize());
			//mapped=mapped*m_window->GetSize()/GetDefaultRegion()->GetRect().getSize();
			m_window->GetWindow()->mouseMoved(mapped.x,mapped.y);
		}
		if(e->event==MET_LEFTDOWN)
		{
			m_window->GetWindow()->mouseButton(0,true);
			GetCreator()->SetFocus(this);
		}
		if(e->event==MET_LEFTUP)
			m_window->GetWindow()->mouseButton(0,false);

		if(e->event==MET_MIDDLEDOWN)
			m_window->GetWindow()->mouseButton(1,true);
		if(e->event==MET_MIDDLEUP)
			m_window->GetWindow()->mouseButton(1,true);
		if(e->event==MET_WHEEL)
			m_window->GetWindow()->mouseWheel(0,e->MouseWheel);
		return true;
	}

	int m_tabID;
public:
	static const GUID ElementType;
	GUIWebPageControl(IGUIManager* creator):IGUIElement(ElementType,creator),m_window(0)
	{
	}
	~GUIWebPageControl()
	{
	}

	void SetWindow(const GCPtr<BrowserWindow>& w)
	{
		m_window=w;
		if(m_window)
			m_texUnit.SetTexture(m_window->GetTexture());
		else
			m_texUnit.SetTexture(0);
	}
	BrowserWindow* GetWindow(){return m_window;}
	virtual void Draw()
	{

		if(!IsVisible())return;
		math::rectf rc=GetDefaultRegion()->GetClippedRect();

		GetCreator()->GetDevice()->useTexture(0,&m_texUnit);
		GetCreator()->GetDevice()->draw2DImage(rc,1);
	}
	virtual IGUIElement* Duplicate(){return new GUIWebPageControl(GetCreator());}
	virtual FontAttributes* GetFontAttributes(){return 0;}

};
const GUID GUIWebPageControl::ElementType(mT("WebPage"));


class GUIWebPageOrganizer:public IGUIPanelElement
{
protected:

	bool m_fullScreen;
	std::vector<GUIWebPageControl*> m_pages;
	int m_activePage;

	void _SwitchFullscreen()
	{
		m_fullScreen=!m_fullScreen;
		if(m_fullScreen)
		{
			for(int i=0;i<m_pages.size();++i)
			{
				if(i!=m_activePage)
					m_pages[i]->SetVisible(false);
			}
		}else
		{
			for(int i=0;i<m_pages.size();++i)
			{
				m_pages[i]->SetVisible(true);
			}
		}
	}
public:
	static const GUID ElementType;
	GUIWebPageOrganizer(IGUIManager* creator):IGUIPanelElement(ElementType,creator),m_fullScreen(false)
	{
		m_fullScreen=false;
		m_activePage=0;
	}

	~GUIWebPageOrganizer()
	{
	}

	void toFront(IGUIElementPtr e)
	{
		ElementList::iterator it=m_elements.begin();
		for(;it!=m_elements.end();++it)
		{
			if(*it==e)
			{
				m_elements.erase(it);
				m_elements.push_back(e);
				break;
			}
		}

	}

	bool OnEvent(Event*e)
	{
		bool res=IGUIPanelElement::OnEvent(e);
		if(e->getType()==ET_Keyboard)
		{
			KeyboardEvent* evt=(KeyboardEvent*)e;

			if(evt->Char=='`')
			{
				_SwitchFullscreen();
			}
		}
		if(res)
		{
			for(int i=0;i<m_pages.size();++i)
			{
				if(GetCreator()->GetFocusElement()==m_pages[i])
				{
					m_activePage=i;
					toFront(m_pages[i]);


					break;
				}
			}
		}
		return res;
	}
	void AddNewTab(GCPtr<GUIWebPageControl> ctrl)
	{
		this->AddElement(ctrl);
		m_pages.push_back(ctrl);
	}

	virtual void Update(float dt)
	{
		IGUIElement::Update(dt);
		if(m_pages.size()==0)
			return;
		float speed=10;
		if(m_fullScreen)
		{
			math::rectf trc=GetDefaultRegion()->GetRect();
			math::rectf rc=m_pages[m_activePage]->GetDefaultRegion()->GetRect();

			rc.ULPoint+=(trc.ULPoint-rc.ULPoint)*speed*dt;
			rc.BRPoint+=(trc.BRPoint-rc.BRPoint)*speed*dt;
			m_pages[m_activePage]->SetPosition(rc.ULPoint);
			m_pages[m_activePage]->SetSize(rc.getSize());
		}else
		{
			for(int i=0;i<m_pages.size();++i)
			{
				math::rectf trc(100+i*50,100+i*20,500+i*50,500+i*20);
				math::rectf rc=m_pages[i]->GetDefaultRegion()->GetRect();

				rc.ULPoint+=(trc.ULPoint-rc.ULPoint)*speed*dt;
				rc.BRPoint+=(trc.BRPoint-rc.BRPoint)*speed*dt;
				m_pages[i]->SetPosition(rc.ULPoint);
				m_pages[i]->SetSize(rc.getSize());
			}
		}
	}
	virtual IGUIElement* Duplicate(){return new GUIWebPageControl(GetCreator());}
	virtual FontAttributes* GetFontAttributes(){return 0;}

};
const GUID GUIWebPageOrganizer::ElementType(mT("WebPageOrganizer"));



}
}

class Application:public CMRayApplication
{

protected:
	GCPtr<GUI::GUIBatchRenderer> m_guiRender;
	scene::ViewPort* m_viewPort;
	scene::CameraNode* m_camera;

	Berkelium::Context* m_context;


	GCPtr<GUI::GUIManager> m_guiManager;

public:


	Application():CMRayApplication()
	{
	}

	virtual ~Application()
	{
		m_guiManager=0;
		Berkelium::destroy();
	}
	virtual void onEvent(Event* event)
	{
		CMRayApplication::onEvent(event);

		if(m_guiManager)
		m_guiManager->OnEvent(event);
	/*

		if(event->getType()==ET_ResizeEvent)
		{
			ResizeEvent* e=(ResizeEvent*) event;
			if(m_bwindow)
				m_bwindow->Resize(e->width,e->height);
		}*/
		
	}


	virtual void init(const OptionContainer &extraOptions){
		CMRayApplication::init(extraOptions);

		CMRayApplication::loadResourceFile(mT("Resources.stg"));

		core::stringw path=L".\\berk";
		Berkelium::FileString str;
		str=Berkelium::FileString::point_to(path.c_str(),path.length());


		{
			gImageSetResourceManager.loadImageSet(mT("VistaCG.imageset"));
			GCPtr<OS::IStream> themeStream=gFileSystem.createBinaryFileReader(mT("VistaCG.xml"));
			GUI::GUIThemeManager::getInstance().loadTheme(themeStream);
			GUI::GUIThemeManager::getInstance().setActiveTheme(mT("VistaCG"));
			//load font
			GCPtr<GUI::IFont>font=gFontResourceManager.getOrCreate(mT("solo5_font.xml"));
			gFontResourceManager.setDefaultFont(font);
		}

		m_guiManager=new GUI::GUIManager(getDevice());
		Berkelium::init(str);
		core::string urls[]=
		{
			"http://www.google.com",
			"http://www.hotmail.com",
			"http://www.facebook.com",
			"http://www.iclarified.com",
		};
		GUI::GUIWebPageOrganizer* rootElem=new GUI::GUIWebPageOrganizer(m_guiManager);
		rootElem->SetSize(GetRenderWindow()->GetSize());
		for(int i=0;i<4;++i)
		{
			GCPtr<BrowserWindow> m_bwindow;

			m_context=Berkelium::Context::create();
			m_bwindow=new BrowserWindow(1024,1024,m_context);
			core::string url=urls[i];//"http://alfagiulietta.com/virtualownership/";//"https://developers.google.com/maps/customize";
			bool res=m_bwindow->GetWindow()->navigateTo(url.c_str(),url.length());

			m_bwindow->GetWindow()->focus();

			GUI::GUIWebPageControl* ctrl=new GUI::GUIWebPageControl(m_guiManager);
			ctrl->SetPosition(math::vector2d(100+i*50,100+i*50));
			ctrl->SetSize(math::vector2d(500,500));
			ctrl->SetWindow(m_bwindow);


			rootElem->AddNewTab(ctrl);;
		}
		m_guiManager->SetRootElement(rootElem);

		m_camera=getSceneManager()->createCamera();

		m_viewPort=GetRenderWindow()->CreateViewport(mT("MainVP"),m_camera,0,math::rectf(0,0,1,1),0);


		m_guiRender=new GUI::GUIBatchRenderer();
		m_guiRender->SetDevice(getDevice());
	}
	virtual void draw(scene::ViewPort* vp)
	{
		CMRayApplication::draw(vp);
		m_guiManager->DrawAll();

	}
	virtual void WindowPostRender(video::RenderWindow* wnd)
	{
		getDevice()->set2DMode();
/*
		video::TextureUnit tex;
		tex.SetTexture(m_bwindow->GetTexture());
		getDevice()->useTexture(0,&tex);
		getDevice()->draw2DImage(math::rectf(0,m_bwindow->GetSize()),1);
*/
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
			attr.shadowOffset=vector2d(2);
			attr.spacing=2;
			attr.wrap=0;
			attr.RightToLeft=0;
			core::string msg=mT("FPS= ");
			msg+=core::StringConverter::toString(CFPS::getInstance().getFPS());
			font->print(math::rectf(10,yoffset+10,10,10),&attr,0,msg,m_guiRender);
			yoffset+=30;
			msg=mT("Primitives= ");
			msg+=core::StringConverter::toString(getDevice()->getPrimitiveDrawnCount());
			font->print(math::rectf(10,yoffset+10,10,10),&attr,0,msg,m_guiRender);
			yoffset+=30;

			m_guiRender->Flush();
			getDevice()->useShader(0);
		}
	}
	virtual void update(float dt)
	{
		CMRayApplication::update(dt);
		Berkelium::update();
		m_guiManager->Update(dt);
	}

	virtual void onDone(){
	}
};


APPLICATION_ENTRY_POINT
{
#if defined(DEBUG) | defined(_DEBUG)
	//	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );//_crtBreakAlloc
	long temp=-1;
	_crtBreakAlloc=temp;
#endif

	GCPtr<Application> app=new Application();

	core::string resFileName=mT("plugins.stg");

#ifdef UNICODE
	resFileName=mT("pluginsU.stg");
#endif

	std::vector<SOptionElement> extraOptions;
	app->loadResourceFile(mT("dataPath.stg"));
	if(app->startup(mT("MRaY App"),vector2di(800,600),false,extraOptions,resFileName,0,true,false))
	{
		app->run();
	}

	//	VLDDisable();
	app=0;

	GCCollector::shutdown();
}


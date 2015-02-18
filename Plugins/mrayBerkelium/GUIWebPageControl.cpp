
#include "stdafx.h"
#include "GUIWebPageControl.h"
#include "GUIElementRegion.h"

#include "KeyboardEvent.h"
#include "IGUIManager.h"
#include "BrowserWindow.h"
#include "TextureUnit.h"
#include "MouseEvent.h"
#include "IVideoDevice.h"

namespace mray
{
namespace GUI
{


	class GUIWebPageControlImpl
	{
	public:
		GUIWebPageControlImpl(void* context)
		{
			this->context=context;
			SetWindow(new web::BrowserWindow(1,1,context));
		}
		GCPtr<web::BrowserWindow> window;
		video::TextureUnit texUnit;
		void* context;

		void SetWindow(const GCPtr<web::BrowserWindow>& w)
		{
			window=w;
			if(window)
				texUnit.SetTexture(window->GetTexture());
			else
				texUnit.SetTexture(0);

		}
		web::BrowserWindow* GetWindow(){return window;}
	};

	bool GUIWebPageControl::_OnKeyboardEvent(KeyboardEvent* e)
	{
		if(!m_impl->window)
			return false;
		return m_impl->GetWindow()->_OnKeyboardEvent(e);
	}
	bool GUIWebPageControl::_OnMouseEvent(MouseEvent* e)
	{
		if(!m_impl->window)
			return false;
		math::rectf rc=GetDefaultRegion()->GetRect();
		bool res=m_impl->GetWindow()->_OnMouseEvent(e,rc);
		if(e->event==MET_LEFTDOWN && res)
		{
			GetCreator()->SetFocus(this);
		}
		return res;
	}

	bool GUIWebPageControl::NavigateTo(const core::string& url)
	{
		return m_impl->GetWindow()->NavigateToUrl(url);
	}
	void GUIWebPageControl::ExecuteJavascript(const core::stringw& script)
	{
		m_impl->GetWindow()->ExecuteScript(script);
	}
	void GUIWebPageControl::SetResolution(int x,int y)
	{
		m_impl->GetWindow()->Resize(x,y);
	}
	GUIWebPageControl::GUIWebPageControl(IGUIManager* creator,void* context):IGUIElement(ElementType,creator)
	{
		m_impl=new GUIWebPageControlImpl(context);
	}
	GUIWebPageControl::~GUIWebPageControl()
	{
		delete m_impl;
	}

	web::BrowserWindow* GUIWebPageControl::GetBrowserWindow()
	{
		return m_impl->GetWindow();
	}

	void GUIWebPageControl::Draw(video::IRenderArea* vp)
	{

		if(!IsVisible())return;
		math::rectf rc=GetDefaultRegion()->GetClippedRect();
		GetCreator()->GetDevice()->useTexture(0,&m_impl->texUnit);
		GetCreator()->GetDevice()->draw2DImage(rc,1);
	}
	IGUIElement* GUIWebPageControl::Duplicate(){return new GUIWebPageControl(GetCreator(),m_impl->context);}
	FontAttributes* GUIWebPageControl::GetFontAttributes(){return 0;}

const GUID GUIWebPageControl::ElementType(mT("WebPage"));

}
}

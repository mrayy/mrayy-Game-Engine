
#include "stdafx.h"
#include "GUIWebPageControl.h"
#include "GUIElementRegion.h"
#include "ParsedShaderPP.h"


namespace mray
{
namespace GUI
{


	bool GUIWebPageControl::_OnKeyboardEvent(KeyboardEvent* e)
	{
		if(!m_window)
			return false;
		wchar_t c[2];
		c[0]=e->Char;
		c[1]=0;
		m_window->GetWindow()->textEvent(c,1);

		if(e->press)
		{
			static float value=0;
			if(e->Char=='1')
			{
				value+=1;
				m_shader->GetParam("RadialBlur")->SetValue(value);
			}else if(e->Char=='2')
			{
				value-=1;
				m_shader->GetParam("RadialBlur")->SetValue(value);
			}

			if(e->key==KEY_LEFT)
			{
				wchar_t cmdStr[]=L"panBy(0.1,0)";
				m_window->GetWindow()->executeJavascript(Berkelium::WideString::point_to(cmdStr));
			}
		}

		return true;
	}
	bool GUIWebPageControl::_OnMouseEvent(MouseEvent* e)
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
			m_window->GetWindow()->mouseWheel(0,e->MouseWheelRel);
		}
		if(e->event==MET_LEFTDOWN)
		{
			m_window->GetWindow()->mouseButton(0,true);
			GetCreator()->SetFocus(this);
		}else
		if(e->event==MET_LEFTUP)
			m_window->GetWindow()->mouseButton(0,false);
		else
		if(e->event==MET_MIDDLEDOWN)
			m_window->GetWindow()->mouseButton(1,true);
		else
		if(e->event==MET_MIDDLEUP)
			m_window->GetWindow()->mouseButton(1,false);
		else
			if(e->event==MET_RIGHTDOWN)
				m_window->GetWindow()->mouseButton(2,true);
			else
				if(e->event==MET_RIGHTUP)
					m_window->GetWindow()->mouseButton(2,false);
		return true;
	}


	GUIWebPageControl::GUIWebPageControl(IGUIManager* creator):IGUIElement(ElementType,creator),m_window(0)
	{
		m_rt=creator->GetDevice()->createEmptyTexture2D(true);
		m_rt->setMipmapsFilter(false);
		m_rt->createTexture(math::vector3d(128,128,1),video::EPixel_B8G8R8A8);
		m_inputRT=creator->GetDevice()->createRenderTarget("",m_rt,0,0,0);
		m_shader=new video::ParsedShaderPP(creator->GetDevice());
		m_shader->LoadXML(gFileSystem.openFile("BrowserEffect.peff"));

		m_texCoords=math::rectf(0.1,0.1,0.9,0.9);
		
	}
	GUIWebPageControl::~GUIWebPageControl()
	{
	}

	void GUIWebPageControl::SetWindow(const GCPtr<BrowserWindow>& w)
	{
		m_window=w;
		if(m_window)
			//m_inputRT->attachRenderTarget(m_window->GetTexture());
			m_texUnit.SetTexture(m_window->GetTexture());
		else
			//m_inputRT->attachRenderTarget(0);
			m_texUnit.SetTexture(0);
//		s->GetParam("inputTex")->SetTexture(m_texUnit.GetTexture());
		m_rtUnit.SetTexture(m_shader->getOutput()->getColorTexture(0));
		
	}
	BrowserWindow* GUIWebPageControl::GetWindow(){return m_window;}
	void GUIWebPageControl::Draw(video::IRenderArea* vp)
	{

		if(!IsVisible())return;
		math::rectf rc=GetDefaultRegion()->GetClippedRect();
		/*
		math::rectf clip(0,vp->getSize());
		if(m_inputRT->getSize()!=rc.getSize())
		{
			m_rt->createTexture(math::vector3d(rc.getWidth(),rc.getHeight(),1),video::EPixel_B8G8R8A8);
		}
		GetCreator()->GetDevice()->setRenderTarget(m_inputRT,1,1,video::SColor(1,0,0,1));
		GetCreator()->GetDevice()->useTexture(0,&m_texUnit);
		GetCreator()->GetDevice()->draw2DImage(math::rectf(0,m_inputRT->getSize()),1,0,&m_texCoords);

		GetCreator()->GetDevice()->setRenderTarget(0,0,0,0);

 		m_shader->Setup(rc);
 		m_shader->render(m_inputRT);
		GetCreator()->GetDevice()->useTexture(0,&m_rtUnit);
		GetCreator()->GetDevice()->draw2DImage(rc,1);*/
		GetCreator()->GetDevice()->useTexture(0,&m_texUnit);
		GetCreator()->GetDevice()->draw2DImage(rc,1,0,&m_texCoords);
	}
	IGUIElement* GUIWebPageControl::Duplicate(){return new GUIWebPageControl(GetCreator());}
	FontAttributes* GUIWebPageControl::GetFontAttributes(){return 0;}

const GUID GUIWebPageControl::ElementType(mT("WebPage"));

}
}

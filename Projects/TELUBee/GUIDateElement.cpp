
#include "stdafx.h"
#include "GUIDateElement.h"
#include "GUIElementRegion.h"
#include "IGUIManager.h"
#include "IVideoDevice.h"
#include "FontResourceManager.h"
#include "IGUIRenderer.h"



namespace mray
{
namespace GUI
{

	IMPLEMENT_RTTI(GUIDateElement,IGUIElement)

const GUID GUIDateElement::ElementType("GUIBlinkingText");

GUIDateElement::GUIDateElement(IGUIManager* mngr)
	:IGUIElement(ElementType,mngr)
{
	m_displayDate=true;
	m_displayTime=true;
}

GUIDateElement::~GUIDateElement()
{
}



void GUIDateElement::Draw(video::IRenderArea*vp)
{
	if(!IsVisible())return;
	IGUIManager* creator=GetCreator();
	const math::rectf& rect=GetDefaultRegion()->GetRect();
	const math::rectf& clip=GetDefaultRegion()->GetClippedRect();
	video::IVideoDevice*device=creator->GetDevice();

	IFont* font= gFontResourceManager.getFontByName(m_fontAttrs.fontName);
	if(!font)
		font=gFontResourceManager.getDefaultFont();

	math::rectf oldScissor=GetCreator()->GetDevice()->getScissorRect();
	GetCreator()->GetDevice()->setScissorRect(rect);
	if(font)
	{
		font->print(rect,&m_fontAttrs,&clip,m_text,creator->GetRenderQueue());
	}
	GetCreator()->GetRenderQueue()->Flush();
	GetCreator()->GetDevice()->setScissorRect(oldScissor);

	math::rectf rc(0, vp->GetSize());
	IGUIElement::Draw(&rc);// vp);
}

void GUIDateElement::Update(float dt)
{
	
	time_t t=time(0);
	struct tm *now=0;
	gmtime_s(now, &t);//localtime(&t);
	
	m_text="";/*
	if(m_displayDate)
	{
		m_text+=core::StringConverter::toString(now->tm_mon)+"/"+core::StringConverter::toString(now->tm_mday)+"/"+core::StringConverter::toString(now->tm_year+1900);
	}
	if(m_displayDate && m_displayTime)
	{
		m_text+=" - ";
	}
	if(m_displayTime)
	{*/
		//m_text+=core::StringConverter::toString(asctime(now));
	m_text+=core::StringConverter::toString(now->tm_hour)+":"+core::StringConverter::toString(now->tm_min);
//	}

}



FontAttributes* GUIDateElement::GetFontAttributes()
{
	return &m_fontAttrs;
}

IMPLEMENT_ELEMENT_FACTORY(GUIDateElement);


}
}



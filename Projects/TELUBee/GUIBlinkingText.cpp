
#include "stdafx.h"
#include "GUIBlinkingText.h"
#include "GUIElementRegion.h"
#include "IGUIManager.h"
#include "IVideoDevice.h"
#include "FontResourceManager.h"
#include "IGUIRenderer.h"



namespace mray
{
namespace GUI
{

	IMPLEMENT_RTTI(GUIBlinkingText,IGUIElement)

		IMPLEMENT_SETGET_PROP(GUIBlinkingText,BlinkSpeed,float,m_blinkSpeed,10,core::StringConverter::toFloat,core::StringConverter::toString)

const GUID GUIBlinkingText::ElementType("GUIBlinkingText");

GUIBlinkingText::GUIBlinkingText(IGUIManager* mngr)
	:IGUIElement(ElementType,mngr)
{
	m_time=0;
	CPropertieDictionary *dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeBlinkSpeed::instance,mT("Design"));
	}
}

GUIBlinkingText::~GUIBlinkingText()
{
}



void GUIBlinkingText::Draw(const math::rectf*vp)
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
		m_fontAttrs.fontColor.A=fabs(math::sind(m_time*m_blinkSpeed))*0.8+0.2;
		m_fontAttrs.shadowColor.A=fabs(math::sind(m_time*m_blinkSpeed))*0.8+0.2;
		font->print(rect,&m_fontAttrs,&clip,GetText(),creator->GetRenderQueue());
	}
	GetCreator()->GetRenderQueue()->Flush();
	GetCreator()->GetDevice()->setScissorRect(oldScissor);

	IGUIElement::Draw( vp);
}

void GUIBlinkingText::Update(float dt)
{
	m_time+=dt;
}



FontAttributes* GUIBlinkingText::GetFontAttributes()
{
	return &m_fontAttrs;
}

IMPLEMENT_ELEMENT_FACTORY(GUIBlinkingText);


}
}



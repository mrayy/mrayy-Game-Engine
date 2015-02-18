 
#include "stdafx.h"
#include "GUIStaticText.h"
#include "GUIThemeManager.h"
#include "IGUITheme.h"
#include "GUIManager.h"
#include "GUIStaticTextComponent.h"


namespace mray{
namespace GUI{


	IMPLEMENT_RTTI(GUIStaticText,IGUIStaticText)
IMPLEMENT_ELEMENT_FACTORY(GUIStaticText);

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Background,GUIStaticText,bool,mT("Background"),EPBT_Basic,mT("Draw background"),true);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Background, GUIStaticText, bool, SetBackground, GetBackground, core::StringConverter::toString, core::StringConverter::toBool, false)

IMPLEMENT_PROPERTY_TYPE_HEADER(Borders, GUIStaticText, bool, mT("Borders"), EPBT_Basic, mT("Draw borders"), true);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Borders, GUIStaticText, bool, SetBorder, GetBorder, core::StringConverter::toString, core::StringConverter::toBool, false)

GUIStaticText::GUIStaticText(IGUIManager* creator)
:IGUIStaticText(creator)
{
	m_component=new GUIStaticTextComponent();
	m_component->owner=this;
	CPropertieDictionary *dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeBackground::instance, mT("Apperance"));
		dic->addPropertie(&PropertyTypeBorders::instance, mT("Apperance"));
	}
}
GUIStaticText::~GUIStaticText()
{
	delete m_component;
}
const bool& GUIStaticText::GetBackground() 
{
	return m_component->m_background;
}
bool GUIStaticText::SetBackground(const bool& val)
{
	m_component->m_background=val;
	return true;
}

const bool& GUIStaticText::GetBorder()
{
	return m_component->m_border;
}
bool GUIStaticText::SetBorder(const bool& val)
{
	m_component->m_border=val;
	return true;
}

 bool GUIStaticText::SetText(const core::UTFString&t)
{
	if(IGUIElement::SetText(t))
	{
		m_component->text=t;
		return true;
	}
	return false;
}
void GUIStaticText::Draw(const math::rectf*vp)
{
	if(!IsVisible())
		return;

	m_component->rc=GetDefaultRegion()->GetClippedRect();
	math::rectf oldScissor=GetCreator()->GetDevice()->getScissorRect();
	GetCreator()->GetDevice()->setScissorRect(m_component->rc);
	m_component->Draw();
	GetCreator()->GetRenderQueue()->Flush();
	GetCreator()->GetDevice()->setScissorRect(oldScissor);
}


IGUIElement* GUIStaticText::Duplicate()
{
	GUIStaticText*dup=new GUIStaticText(m_creator);
	dup->CopyProperties(this);
	dup->m_FontAttributs=m_FontAttributs;
	return dup;
}
FontAttributes* GUIStaticText::GetFontAttributes(){
	return &m_FontAttributs;
}



}
}

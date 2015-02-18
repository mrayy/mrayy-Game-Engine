
#include "stdafx.h"
#include "GUIPanel.h"

#include "IGUIManager.h"
#include "IGUITheme.h"
#include "TextureUnit.h"

namespace mray
{
namespace GUI
{

	IMPLEMENT_ELEMENT_FACTORY(GUIPanel);
	
IMPLEMENT_PROPERTY_TYPE_HEADER(Background,GUIPanel,bool,mT("Background"),EPBT_Basic,mT("Draw background"),true);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Background,GUIPanel,bool,SetBackground,GetBackground,core::StringConverter::toString,core::StringConverter::toBool,false)

GUIPanel::GUIPanel(IGUIManager* mngr)
:IGUIPanel(mngr)
{
	m_drawBackground=true;
	m_bgTexture=new video::TextureUnit();
	CPropertieDictionary *dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeBackground::instance,mT("Apperance"));
	}
}
GUIPanel::~GUIPanel()
{
	delete m_bgTexture;
}

bool GUIPanel::GetBackground() 
{
	return m_drawBackground;
}
bool GUIPanel::SetBackground(bool val)
{
	m_drawBackground=val;
	return true;
}
void GUIPanel::Draw(const math::rectf*vp)
{
	_UpdateRegion(vp);
	if(!IsVisible())return;
	IGUIManager* creator=GetCreator();
	IGUITheme* skin=creator->GetActiveTheme();
	const math::rectf& clipRect=GetDefaultRegion()->GetClippedRect();

	if(m_drawBackground)
	{
		video::SColor clr(GetColor());
		clr.A = GetDerivedAlpha();
		math::rectf oldScissor=creator->GetDevice()->getScissorRect();
		creator->GetDevice()->setScissorRect(clipRect);
		if(skin)
		{
			skin->drawSizableBox(creator->GetRenderQueue(), clipRect, 0, mT("Frame"), clr);
			creator->GetRenderQueue()->Flush();
		}else
		{
			creator->GetDevice()->draw2DRectangle(clipRect, clr);

		}
		creator->GetDevice()->setScissorRect(oldScissor);

	}
	IGUIPanelElement::Draw(vp);
}

void GUIPanel::SetBackGroundTexture(video::ITextureCRef  tex)
{
	m_bgTexture->SetTexture(tex);
}
video::TextureUnit* GUIPanel::GetBackGroundTexture()
{
	return m_bgTexture;
}
IGUIElement* GUIPanel::Duplicate()
{
	return new GUIPanel(GetCreator());
}
bool GUIPanel::_OnMouseEvent(MouseEvent*evt)
{
	const math::rectf& rc=GetDefaultRegion()->GetClippedRect();
	if(rc.IsPointInside(evt->pos))
	{
		if(!GetCreator()->HasMouseOn(this))
		{
			GetCreator()->SetMouseOn(this);
		}
		evt->setRecived();
		//return true;
	}else
	{
		if(GetCreator()->HasMouseOn(this))
		{
			GetCreator()->SetMouseOn(0);
		}
	}

	return false;
}

}
}

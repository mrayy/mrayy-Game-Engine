
#include "stdafx.h"
#include "GUIComboListComponent.h"

#include "IGUIElement.h"
#include "IGUIManager.h"
#include "IGUITheme.h"
#include "IListItem.h"


namespace mray
{
namespace GUI
{

GUIComboListComponent::GUIComboListComponent(IGUIElement* owner):GUIListBoxComponent(owner),
m_maxItemsCount(10),m_ListboxVisible(false)
{
}
GUIComboListComponent::~GUIComboListComponent()
{
}

float GUIComboListComponent::_CalcListBoxHeight(int itmsCount)
{
	float h=GetLineHeight();
	return h*math::Min(m_maxItemsCount,itmsCount);
}

GUIComboListComponent::EResultEvent GUIComboListComponent::LBOnKeyboardEvent(KeyboardEvent* e,const math::rectf& rc)
{
	return GUIListBoxComponent::LBOnKeyboardEvent(e,GetListBoxRect(rc,items.size()));
}

GUIComboListComponent::EResultEvent GUIComboListComponent::LBOnMouseEvent(MouseEvent* e,const math::rectf& rc)
{

	EResultEvent res=ENone;
	math::rectf lstRect=GetListBoxRect(rc,items.size());
	if(!rc.IsPointInside(e->pos) && !IsTracking())
	{
		if(!ItemsVisible())
			return ENone;
		// 		if(!lstRect.IsPointInside(e->pos))
		// 			return ENone;
	}
	switch(e->event)
	{
	case MET_LEFTDOWN:
		if(rc.IsPointInside(e->pos))
		{
			m_ListboxVisible=!m_ListboxVisible;
			res=EReceived;
			owner->GetCreator()->SetFocus(owner);
			e->setRecived();
		}else if(!lstRect.IsPointInside(e->pos))
		{
			m_ListboxVisible=false;
			res=EReceived;
			e->setRecived();
		}
		break;
	}
	if(m_ListboxVisible)
	{
		EResultEvent r= GUIListBoxComponent::LBOnMouseEvent(e,GetListBoxRect(rc,items.size()));
		switch(r)
		{
		case ENone:
			return res;
		case EReceived:
			return EReceived;
		case ESelectionChange:
			m_ListboxVisible=false;
			owner->GetCreator()->SetFocus(owner);
			return ESelectionChange;
		}
	}
	return res;
}

bool GUIComboListComponent::OnKeyboardEvent(KeyboardEvent* e)
{
	return LBOnKeyboardEvent(e,rc)!=ENone;
}
bool GUIComboListComponent::OnMouseEvent(MouseEvent* e)
{
	return LBOnMouseEvent(e,rc)!=ENone;
}

void GUIComboListComponent::SetMaxItems(int i)
{
	m_maxItemsCount=i;
}
int GUIComboListComponent::GetMaxItems()
{
	return m_maxItemsCount;
}
int GUIComboListComponent::GetItemFromPos(const math::vector2d& pt,const math::rectf& clippedRect)
{
	return GUIListBoxComponent::GetItemFromPos(pt,GetListBoxRect(clippedRect,items.size()));
}

bool GUIComboListComponent::ItemsVisible()
{
	return m_ListboxVisible;
}


void GUIComboListComponent::Draw()
{
	IGUIManager* creator=owner->GetCreator();
	IGUITheme* skin=creator->GetActiveTheme();
	GUI::IFont* font=gFontResourceManager.getFontByName(owner->GetFontAttributes()->fontName);
	math::rectf innerRect;
	if(skin)
	{
		innerRect= skin->getRect3PH(1,rc,0,mT("ComboList"));
	}else
		innerRect=rc;


	if(skin)
	{
		skin->drawSizableBox3PH(creator->GetRenderQueue(),rc,0,mT("ComboList"));
	}

	creator->GetRenderQueue()->Flush();
	if(font && GetSelectedItem()!=-1)
	{
		math::rectf oldScissor=creator->GetDevice()->getScissorRect();
		creator->GetDevice()->setScissorRect(innerRect);
		font->print(innerRect,owner->GetFontAttributes(),0,items[GetSelectedItem()]->toString(),
			creator->GetRenderQueue());
		creator->GetRenderQueue()->Flush();
		creator->GetDevice()->setScissorRect(oldScissor);
	}

	if(m_ListboxVisible)
		creator->AddPostDrawElement(owner);
}
void GUIComboListComponent::PostDraw()
{
	if(m_ListboxVisible)
		GUIListBoxComponent::LBDraw(GetListBoxRect(rc,items.size()));
}



math::rectf GUIComboListComponent::GetListBoxRect(const math::rectf& clippedRect,int itmsCount)
{
	math::rectf rc(clippedRect.ULPoint.x,clippedRect.BRPoint.y,
		clippedRect.BRPoint.x,clippedRect.BRPoint.y+_CalcListBoxHeight(itmsCount));
	return rc;
}

void GUIComboListComponent::LostFocus()
{
	m_ListboxVisible=false;
	GUIListBoxComponent::LostFocus();
}

}
}


#include "stdafx.h"
#include "GUIPropGridItem.h"
#include "IGUIPropertyGrid.h"

#include "GUIManager.h"
#include "GUIThemeManager.h"
#include "FontResourceManager.h"
#include "IGUITheme.h"


namespace mray
{
namespace GUI
{

GUIPropGridItem::GUIPropGridItem(IGUIPropertyGrid* grid,IGUIPropGridItem* itm)
	:IGUIPropGridItem(grid),m_item(itm)
{
	m_name=m_item->GetProperty()->getName();
}
GUIPropGridItem::~GUIPropGridItem()
{
	delete m_item;
}


math::rectf GUIPropGridItem::Draw(const math::rectf& rc)
{

	IGUIManager* creator=m_ownerGrid->GetCreator();
	IGUITheme* skin=creator->GetActiveTheme();
	GUI::IFont* font=gFontResourceManager.getFontByName(m_ownerGrid->GetFontAttributes()->fontName);

	math::rectf tmpRC=rc;
	tmpRC.BRPoint.x=tmpRC.ULPoint.x+tmpRC.getWidth()*0.5;

//	creator->GetRenderQueue()->AddQuad(0,tmpRC,math::rectf(0,0,1,1),video::DefaultColors::Wheat);
	font->print(tmpRC,m_ownerGrid->GetFontAttributes(),&tmpRC,m_name,creator->GetRenderQueue());

	tmpRC.ULPoint.x=tmpRC.BRPoint.x;
	tmpRC.BRPoint.x=rc.BRPoint.x;

	return m_item->Draw(tmpRC);

}

bool GUIPropGridItem::HandleMouseEvent(MouseEvent*e)
{
	return m_item->HandleMouseEvent(e);
}
bool GUIPropGridItem::HandleKeyboardEvent(KeyboardEvent*e)
{
	return m_item->HandleKeyboardEvent(e);
}



}
}
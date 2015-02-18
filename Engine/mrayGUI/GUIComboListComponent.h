
/********************************************************************
	created:	2011/11/22
	created:	22:11:2011   15:39
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\GUIComboListComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUIComboListComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUIComboListComponent__
#define __GUIComboListComponent__

#include "GUIListBoxComponent.h"

namespace mray
{
namespace GUI
{
	class IGUIElement;

class GUIComboListComponent:public GUIListBoxComponent
{
private:
protected:
	int m_maxItemsCount;

	bool m_ListboxVisible;

	float _CalcListBoxHeight(int itmsCount);


public:
	GUIComboListComponent(IGUIElement* owner);
	virtual~GUIComboListComponent();

	virtual bool OnKeyboardEvent(KeyboardEvent* e);
	virtual bool OnMouseEvent(MouseEvent* e);

	virtual EResultEvent LBOnKeyboardEvent(KeyboardEvent* e,const math::rectf& rc);
	virtual EResultEvent LBOnMouseEvent(MouseEvent* e,const math::rectf& rc);

	void SetMaxItems(int i);
	int GetMaxItems();

	int GetItemFromPos(const math::vector2d& pt,const math::rectf& clippedRect);

	bool ItemsVisible();

	virtual void Draw();
	virtual void PostDraw();

	math::rectf GetListBoxRect(const math::rectf& clippedRect,int itmsCount);
		
	virtual void LostFocus();
};

}
}

#endif


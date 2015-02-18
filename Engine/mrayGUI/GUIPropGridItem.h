


/********************************************************************
	created:	2011/12/15
	created:	15:12:2011   9:10
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\GUIPropGridItem.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUIPropGridItem
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUIPropGridItem__
#define __GUIPropGridItem__

#include "IGUIPropGridItem.h"

namespace mray
{
namespace GUI
{

class GUIPropGridItem:public IGUIPropGridItem
{
private:
protected:
	core::string m_name;
	IGUIPropGridItem* m_item;
public:
	GUIPropGridItem(IGUIPropertyGrid* grid,IGUIPropGridItem* itm);
	virtual~GUIPropGridItem();

	virtual math::rectf Draw(const math::rectf& rc);

	virtual bool HandleMouseEvent(MouseEvent*e);
	virtual bool HandleKeyboardEvent(KeyboardEvent*e);

	virtual void SetActive(bool a){m_item->SetActive(a);}
	virtual bool IsActive(){return m_item->IsActive();}

};

}
}

#endif
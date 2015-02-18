

/********************************************************************
	created:	2011/12/13
	created:	13:12:2011   10:02
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\GUIPropGridStack.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUIPropGridStack
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUIPropGridStack__
#define __GUIPropGridStack__

#include "IGUIElement.h"

namespace mray
{
namespace GUI
{
	class GUIPropertyGrid;
	class IGUIPropGridItem;

class GUIPropGridStack:public IGUIElement
{
private:
protected:
	typedef std::vector<IGUIPropGridItem*> PropItemsList;
	PropItemsList m_items;

	GUIPropertyGrid* m_grid;
	IGUIPropGridItem* m_activeItem;
	bool m_isActive;

	float m_height;
	virtual bool _UpdateRegion(const math::rectf*vp);
public:
	GUIPropGridStack(GUIPropertyGrid* grid);
	virtual~GUIPropGridStack();

	void AddProperty(IGUIPropGridItem* itm);
	void Clear();

	//virtual const math::rectf& GetUnclippedRect();
	virtual void Draw(const math::rectf*vp);
 
 	virtual bool _OnMouseEvent(MouseEvent*e);
 	virtual bool _OnKeyboardEvent(KeyboardEvent*e);
};


}
}

#endif


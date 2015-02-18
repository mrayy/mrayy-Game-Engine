


/********************************************************************
	created:	2011/12/11
	created:	11:12:2011   20:33
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\GUIStaticTextComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUIStaticTextComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUIStaticTextComponent__
#define __GUIStaticTextComponent__

#include "IGUIComponent.h"

namespace mray
{
namespace GUI
{

class GUIStaticTextComponent:public IGUIComponent
{
private:
protected:
public:
	bool m_background;
	bool m_border;
	video::SColor m_backColor;
	video::SColor m_borderColor;

	GUIStaticTextComponent();
	virtual~GUIStaticTextComponent();


	virtual void Draw();

	virtual bool OnKeyboardEvent(KeyboardEvent* e);
	virtual bool OnMouseEvent(MouseEvent* e);
};


}
}

#endif




/********************************************************************
	created:	2011/12/13
	created:	13:12:2011   12:59
	filename: 	d:\Development\mrayEngine\Engine\mrayGUI\GUIGroupComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUIGroupComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUIGroupComponent__
#define __GUIGroupComponent__

#include "IGUIComponent.h"

namespace mray
{
namespace GUI
{

class GUIGroupComponent:public IGUIComponent
{
private:
protected:
public:
	GUIGroupComponent();
	virtual~GUIGroupComponent();

	virtual void Draw();

	bool OnKeyboardEvent(KeyboardEvent* e);
	bool OnMouseEvent(MouseEvent* e);


	bool SetText(const core::UTFString& cap);
};

}
}

#endif

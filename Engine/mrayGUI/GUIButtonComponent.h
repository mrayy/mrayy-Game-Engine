

/********************************************************************
	created:	2011/01/11
	created:	11:1:2011   22:52
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGUI\GUIButtonComponent.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGUI
	file base:	GUIButtonComponent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GUIButtonComponent_h__
#define GUIButtonComponent_h__

#include "CompileConfig.h"
#include "IGUIComponent.h"

namespace mray
{
	class MouseEvent;
	class KeyboardEvent;
namespace GUI
{

class MRAYGUI_API GUIButtonComponent:public IGUIComponent
{
private:
protected:
public:

	bool m_isPressed;
	bool m_mouseOver;


	GUIButtonComponent();
	virtual ~GUIButtonComponent();

	virtual void Draw();

	bool OnKeyboardEvent(KeyboardEvent* e);
	bool OnMouseEvent(MouseEvent* e);

	void LostFocus();
	void LostMouseOn();
};

}
}


#endif // GUIButtonComponent_h__

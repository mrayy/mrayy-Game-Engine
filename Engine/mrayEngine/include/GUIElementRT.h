

/********************************************************************
	created:	2013/01/28
	created:	28:1:2013   11:18
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\GUIElementRT.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	GUIElementRT
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GUIElementRT__
#define __GUIElementRT__


#include "IGUIElementRT.h"

namespace mray
{
namespace GUI
{
	class IGUIElement;
}
namespace scene
{
class MRAY_DLL GUIElementRT:public IGUIElementRT
{
protected:
	GUI::IGUIElement* m_element;

	virtual bool _OnMouseEvent(MouseEvent* e);
	virtual bool _OnKeyboardEvent(KeyboardEvent* e);

	virtual math::vector2d _GetElementSize();
	virtual void _DrawElement(const math::rectf* vp);
public:
	GUIElementRT();
	virtual~GUIElementRT();

	void SetElement(GUI::IGUIElement* elem);
	GUI::IGUIElement* GetElement();
};

}
}


#endif

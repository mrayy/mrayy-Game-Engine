

/********************************************************************
	created:	2013/01/28
	created:	28:1:2013   12:10
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\GUIElementRT_Manager.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	GUIElementRT_Manager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GUIElementRT_Manager__
#define __GUIElementRT_Manager__

#include "IGUIElementRT.h"


namespace mray
{
	namespace GUI
	{
		class IGUIManager;
	}
namespace scene
{

class GUIElementRT_Manager:public IGUIElementRT
{
protected:
	GUI::IGUIManager* m_mngr;

	virtual bool _OnMouseEvent(MouseEvent* e);
	virtual bool _OnKeyboardEvent(KeyboardEvent* e);

	virtual math::vector2d _GetElementSize();
	virtual void _DrawElement(const math::rectf* vp);
public:
	GUIElementRT_Manager();
	virtual~GUIElementRT_Manager();

	void SetMananger(GUI::IGUIManager* m){m_mngr=m;}
	GUI::IGUIManager* GetMananger(){return m_mngr;}
};

}
}


#endif

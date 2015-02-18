

/********************************************************************
	created:	2013/01/28
	created:	28:1:2013   11:18
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\GUIManagerRT.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	GUIManagerRT
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GUIManagerRT__
#define __GUIManagerRT__


#include "IGUIElementRT.h"
#include "GUIElementRT.h"
#include "GUIElementRT_Manager.h"

namespace mray
{
	namespace GUI
	{
		class IGUIElement;
		class IGUIManager;
	}
namespace scene
{


class MRAY_DLL GUIManagerRT
{
protected:
	std::list<IGUIElementRTPtr> m_elements;
public:
	GUIManagerRT();
	virtual~GUIManagerRT();


	void AddElemenet(IGUIElementRT* e);
	GCPtr<GUIElementRT> CreateFromElement(GUI::IGUIElement* elem);
	GCPtr<GUIElementRT_Manager> CreateFromManager(GUI::IGUIManager* mngr);

	void Clear();
	void RemoveElement(IGUIElementRT* e);


	bool OnEvent(Event* e);
	void DrawRT();

};

}
}


#endif

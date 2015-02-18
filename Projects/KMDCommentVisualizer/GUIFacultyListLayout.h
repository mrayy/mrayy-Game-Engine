#ifndef __GUIFacultyListLayout__
#define __GUIFacultyListLayout__
#include "IGUISchemeBase.h"
#include "GUIPanel.h"
#include "GUIStackPanel.h"
#include "GUIStaticText.h"
namespace mray{

using namespace GUI;
class GUIFacultyListLayout:public GUI::IGUISchemeBase
{

public:
	GUIPanel* Root;
	GUIStackPanel* Advisers;
	GUIStackPanel* Committee;

public:

	GUIFacultyListLayout():Root(0),Advisers(0),Committee(0)
	{		
		m_elementsMap["Root"]=(IGUIElement**)&Root;
		m_elementsMap["Advisers"]=(IGUIElement**)&Advisers;
		m_elementsMap["Committee"]=(IGUIElement**)&Committee;

	}

};
}
#endif

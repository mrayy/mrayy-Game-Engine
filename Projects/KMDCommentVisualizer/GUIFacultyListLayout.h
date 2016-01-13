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
	GUIStaticText* AdvisersLbl;
	GUIStackPanel* Advisers;
	GUIStaticText* committeeLbl;
	GUIStackPanel* Committee;

public:

	GUIFacultyListLayout():Root(0),AdvisersLbl(0),Advisers(0),committeeLbl(0),Committee(0)
	{		
		m_elementsMap["Root"]=(IGUIElement**)&Root;
		m_elementsMap["AdvisersLbl"]=(IGUIElement**)&AdvisersLbl;
		m_elementsMap["Advisers"]=(IGUIElement**)&Advisers;
		m_elementsMap["committeeLbl"]=(IGUIElement**)&committeeLbl;
		m_elementsMap["Committee"]=(IGUIElement**)&Committee;

	}

};
}
#endif

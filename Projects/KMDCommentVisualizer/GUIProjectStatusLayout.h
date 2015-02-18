#ifndef __GUIProjectStatusLayout__
#define __GUIProjectStatusLayout__
#include "IGUISchemeBase.h"
#include "GUIPanel.h"
#include "GUIProfilePicture.h"
#include "GUIStaticText.h"
namespace mray{

using namespace GUI;
class GUIProjectStatusLayout:public GUI::IGUISchemeBase
{

public:
	GUIPanel* Root;
	GUIProfilePicture* ProjectImage;
	GUIStaticText* ProjectName;
	GUIStaticText* TotalComments;
	GUIStaticText* FaculyComments;
	GUIStaticText* StudentsComments;

public:

	GUIProjectStatusLayout():Root(0),ProjectImage(0),ProjectName(0),TotalComments(0),FaculyComments(0),StudentsComments(0)
	{		
		m_elementsMap["Root"]=(IGUIElement**)&Root;
		m_elementsMap["ProjectImage"]=(IGUIElement**)&ProjectImage;
		m_elementsMap["ProjectName"]=(IGUIElement**)&ProjectName;
		m_elementsMap["TotalComments"]=(IGUIElement**)&TotalComments;
		m_elementsMap["FaculyComments"]=(IGUIElement**)&FaculyComments;
		m_elementsMap["StudentsComments"]=(IGUIElement**)&StudentsComments;

	}

};
}
#endif

#ifndef __GUISessionSidePanelLayout__
#define __GUISessionSidePanelLayout__
#include "IGUISchemeBase.h"
#include "GUIPanel.h"
#include "GUIStaticText.h"
namespace mray{

using namespace GUI;
class GUISessionSidePanelLayout:public GUI::IGUISchemeBase
{

public:
	GUIPanel* Root;

public:

	GUISessionSidePanelLayout():Root(0)
	{		
		m_elementsMap["Root"]=(IGUIElement**)&Root;

	}

};
}
#endif

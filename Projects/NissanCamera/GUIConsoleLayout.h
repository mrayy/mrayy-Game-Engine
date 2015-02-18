#ifndef __GUIConsoleLayout__
#define __GUIConsoleLayout__
#include "IGUISchemeBase.h"
#include "GUIEditBox.h"
#include "GUIListBox.h"
#include "GUIPanel.h"
namespace mray{

using namespace GUI;
class GUIConsoleLayout:public GUI::IGUISchemeBase
{

public:
	GUIPanel* Background;
	GUIListBox* History;
	GUIEditBox* Command;

public:

	GUIConsoleLayout():Background(0),History(0),Command(0)
	{		
		m_elementsMap["Background"]=(IGUIElement**)&Background;
		m_elementsMap["History"]=(IGUIElement**)&History;
		m_elementsMap["Command"]=(IGUIElement**)&Command;

	}

};
}
#endif

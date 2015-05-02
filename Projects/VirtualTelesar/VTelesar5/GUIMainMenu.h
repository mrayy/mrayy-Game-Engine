#ifndef __GUIMainMenu__
#define __GUIMainMenu__
#include "IGUISchemeBase.h"
#include "GUIButton.h"
#include "GUIPanel.h"
namespace mray{

using namespace GUI;
class GUIMainMenu:public GUI::IGUISchemeBase
{

public:
	GUIButton* StartBtn;
	GUIButton* OptionsBtn;
	GUIButton* ExitBtn;

public:

	GUIMainMenu():StartBtn(0),OptionsBtn(0),ExitBtn(0)
	{		
		m_elementsMap["StartBtn"]=(IGUIElement**)&StartBtn;
		m_elementsMap["OptionsBtn"]=(IGUIElement**)&OptionsBtn;
		m_elementsMap["ExitBtn"]=(IGUIElement**)&ExitBtn;

	}

};
}
#endif

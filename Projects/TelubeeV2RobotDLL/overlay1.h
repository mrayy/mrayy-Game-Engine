#include "IGUISchemeBase.h"
#include "GUIButton.h"
#include "GUIEditBox.h"
#include "GUIPanel.h"
namespace mray{

using namespace GUI;
class overlay1
{

public:
	GUIButton* Button1;
	GUIButton* Button2;
	GUIButton* Button3;
	GUIButton* Button4;
	GUIPanel* m_GUIPanel;
	GUIPanel* m_GUIPanel0;
	GUIEditBox* Button40;
	GUIEditBox* Button41;
	GUIEditBox* Button42;
	GUIEditBox* Button43;

public:

	overlay1()
	{		
		m_elementsMap["Button1"]=&Button1;
		m_elementsMap["Button2"]=&Button2;
		m_elementsMap["Button3"]=&Button3;
		m_elementsMap["Button4"]=&Button4;
		m_elementsMap["m_GUIPanel"]=&m_GUIPanel;
		m_elementsMap["m_GUIPanel0"]=&m_GUIPanel0;
		m_elementsMap["Button40"]=&Button40;
		m_elementsMap["Button41"]=&Button41;
		m_elementsMap["Button42"]=&Button42;
		m_elementsMap["Button43"]=&Button43;

	}

};
}
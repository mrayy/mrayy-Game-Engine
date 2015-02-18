#ifndef __GUIScenePanelLayout__
#define __GUIScenePanelLayout__
#include "IGUISchemeBase.h"
#include "GUIFadingText.h"
#include "GUIPanel.h"
#include "GUIStaticImage.h"
#include "GUIStaticText.h"
#include "GUISweepingText.h"
#include "GUITransitionBars.h"
namespace mray{

using namespace GUI;
class GUIScenePanelLayout:public GUI::IGUISchemeBase
{

public:
	GUIPanel* Background;
	GUITransitionBars* Transition;
	GUISweepingText* Theme;
	GUISweepingText* Speaker;
	GUIFadingText* Info;
	GUIStaticImage* TimerIco;
	GUIStaticText* Time;

public:

	GUIScenePanelLayout():Background(0),Transition(0),Theme(0),Speaker(0),Info(0),TimerIco(0),Time(0)
	{		
		m_elementsMap["Background"]=(IGUIElement**)&Background;
		m_elementsMap["Transition"]=(IGUIElement**)&Transition;
		m_elementsMap["Theme"]=(IGUIElement**)&Theme;
		m_elementsMap["Speaker"]=(IGUIElement**)&Speaker;
		m_elementsMap["Info"]=(IGUIElement**)&Info;
		m_elementsMap["TimerIco"]=(IGUIElement**)&TimerIco;
		m_elementsMap["Time"]=(IGUIElement**)&Time;

	}

};
}
#endif

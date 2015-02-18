#ifndef __GUIStatusLayerLayout__
#define __GUIStatusLayerLayout__
#include "IGUISchemeBase.h"
#include "GUIPanel.h"
namespace mray{

using namespace GUI;
class GUIStatusLayerLayout:public GUI::IGUISchemeBase
{

public:
	GUIPanel* Background;

public:

	GUIStatusLayerLayout():Background(0)
	{		
		m_elementsMap["Background"]=(IGUIElement**)&Background;

	}

};
}
#endif

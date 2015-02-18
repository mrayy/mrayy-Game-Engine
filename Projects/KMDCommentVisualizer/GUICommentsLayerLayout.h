#ifndef __GUICommentsLayerLayout__
#define __GUICommentsLayerLayout__
#include "IGUISchemeBase.h"
#include "GUIPanel.h"
namespace mray{

using namespace GUI;
class GUICommentsLayerLayout:public GUI::IGUISchemeBase
{

public:
	GUIPanel* Background;

public:

	GUICommentsLayerLayout():Background(0)
	{		
		m_elementsMap["Background"]=(IGUIElement**)&Background;

	}

};
}
#endif

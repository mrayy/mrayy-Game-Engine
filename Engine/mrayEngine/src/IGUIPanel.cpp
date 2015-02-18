
#include "stdafx.h"
#include "IGUIPanel.h"


namespace mray
{
namespace GUI
{
	const GUID IGUIPanel::ElementType("GUIPanel");

	IGUIPanel::IGUIPanel(IGUIManager* mngr)
		:IGUIPanelElement(ElementType,mngr)
	{
	}
	IGUIPanel::~IGUIPanel()
	{
	}
}
}



#include "stdafx.h"
#include "DebugRenderSettings.h"


namespace mray
{
namespace VT
{

	float DebugRenderSettings::Scale=0.01;
	float DebugRenderSettings::LineWidth=2;
	IVTDebugInterface* DebugRenderSettings::DebugInterface=0;

	GUI::IGUIManager* DebugRenderSettings::guiManager=0;
	std::map<core::string,GUI::IGUIElement*> DebugRenderSettings::CreatedElements;
	GUI::IGUIPanelElement* DebugRenderSettings::rootElement=0;
}
}

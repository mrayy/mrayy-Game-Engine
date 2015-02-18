
/********************************************************************
	created:	2012/07/24
	created:	24:7:2012   22:22
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\DebugRenderSettings.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	DebugRenderSettings
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___DebugRenderSettings___
#define ___DebugRenderSettings___



namespace mray
{
	namespace GUI
	{
		class IGUIManager;
		class IGUIPanelElement;
		class IGUIElement;
	}
namespace VT
{
	class IVTDebugInterface;

class DebugRenderSettings
{
protected:

public:

	static float Scale;
	static float LineWidth;

	static IVTDebugInterface* DebugInterface;
	static GUI::IGUIManager* guiManager;
	
	static std::map<core::string,GUI::IGUIElement*> CreatedElements;
	static GUI::IGUIPanelElement* rootElement;
};

}
}

#endif


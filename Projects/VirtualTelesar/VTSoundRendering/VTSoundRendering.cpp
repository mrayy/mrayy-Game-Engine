// VTSoundRendering.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "IPlugin.h"



namespace mray
{
namespace VT
{

class VTSoundRendering:public IPlugin
{
public:
	VTSoundRendering()
	{
	}

	virtual core::string getName()
	{
		return mT("VTSoundRendering");
	}

	virtual void install()
	{
		IPlugin::install();
	}
	virtual void uninstall()
	{
		IPlugin::uninstall();
	}
};


}
}

IMPLEMET_PLUGIN_FUNCTION(mray::VT::VTSoundRendering);



BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved )
{
	return TRUE; 
}


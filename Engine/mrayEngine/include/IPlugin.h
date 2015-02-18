

#ifndef ___IPlugin___
#define ___IPlugin___

#include "GCPtr.h"
#include "mString.h"

namespace mray{


class MRAY_DLL IPlugin
{
	bool m_installed;
public:
	IPlugin(){m_installed=0;}
	virtual~IPlugin(){
		uninstall();
	}

	virtual core::string getName()=0;

	virtual bool isInstalled(){
		return m_installed;
	}
	virtual void install(){
		m_installed=true;
	}
	virtual void uninstall(){
		m_installed=false;
	}
};

typedef IPlugin*(*PluginCreateFunc)() ;

#define IMPLEMET_PLUGIN_FUNCTION(PluginName) \
	extern "C" __declspec(dllexport) mray::IPlugin* dllCreatePlugin() \
	{                                    \
		return new PluginName();      \
	}

}




#endif



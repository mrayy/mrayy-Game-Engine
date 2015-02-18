

#ifndef ___PluginManager___
#define ___PluginManager___

#include "ISingleton.h"
#include "IPlugin.h"
#include "IDynamicLibrary.h"


namespace mray{

class MRAY_DLL PluginManager:public ISingleton<PluginManager>
{
	typedef std::map<int,GCPtr<IPlugin>> PluginList;

	PluginList m_plugins;
	int m_lastID;

public:

	PluginManager();
	virtual~PluginManager();

	int addPlugin(const  core::string&fileName);
	int addPluginFromDLL(GCPtr<OS::IDynamicLibrary> lib);
	int addPlugin(GCPtr<IPlugin> plugin);
	std::list<IPlugin*> loadPluginsFromDir(const  core::string&path,const core::string& ext=mT("dll"));

	void removePlugin(int id);
	IPlugin* getPlugin(int id);

	void listPlugins(std::vector<int> &out);
};

}



#endif




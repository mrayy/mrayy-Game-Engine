#include "stdafx.h"

#include "PluginManager.h"
#include "IDllManager.h"
#include "ILogManager.h"
#include "IDirOS.h"
#include "IOSystem.h"
#include "StringUtil.h"


namespace mray{
	


PluginManager::PluginManager()
{
	m_lastID=0;
}
PluginManager::~PluginManager(){
	PluginList::iterator it= m_plugins.begin();
	for(;it!=m_plugins.end();++it){
		it->second->uninstall();
	}
	m_plugins.clear();
}

int PluginManager::addPlugin(const core::string&fileName){
	GCPtr<OS::IDynamicLibrary> dll=OS::IDllManager::getInstance().getLibrary(fileName);
	return addPluginFromDLL(dll);
}
int PluginManager::addPluginFromDLL(GCPtr<OS::IDynamicLibrary> lib){
	if(!lib)return -1;

	PluginCreateFunc func=(PluginCreateFunc)lib->getSymbolName("dllCreatePlugin");
	if(!func){
		gLogManager.log(mT("dllCreatePlugin() Not Found in Dynamic Lib: ")+core::string(lib->getLibName()),ELL_WARNING);
		return -1;
	}

	GCPtr<IPlugin> p=func();
	return addPlugin(p);

}
int PluginManager::addPlugin(GCPtr<IPlugin> plugin){
	plugin->install();
	++m_lastID;
	m_plugins.insert(PluginList::value_type(m_lastID,plugin));

	gLogManager.log(core::string(mT("Plugin loaded: "))+plugin->getName(),ELL_SUCCESS);
	return m_lastID;
}

IPlugin* PluginManager::getPlugin(int id)
{
	PluginList::iterator it=m_plugins.find(id);
	if(it==m_plugins.end())
		return 0;
	return it->second;
}

void PluginManager::removePlugin(int id){
	PluginList::iterator it= m_plugins.find(id);
	if(it!=m_plugins.end()){
		it->second->uninstall();
		m_plugins.erase(it);
	}

}

void PluginManager::listPlugins(std::vector<int> &out){
	PluginList::iterator it= m_plugins.begin();
	for(;it!=m_plugins.end();++it){
		out.push_back(it->first);
	}

}

std::list<IPlugin*> PluginManager::loadPluginsFromDir(const  core::string&path,const core::string& ext)
{
	std::list<IPlugin*> ret;
	GCPtr<OS::IDirOS> dir=OS::IOSystem::getInstance().createDirSystem();
	if(!dir->changeDir(path))
		return ret;
	core::string fileName,base,fext;
	int cnt=dir->getFilesCount();
	for(int i=0;i<cnt;++i){
		fileName=dir->getShortFileName(i);
		core::StringUtil::SplitPathExt(fileName,base,fext);

		if(fext.equals_ignore_case(ext))
		{
			core::string fullName=dir->getFullFileName(i);
			int id=addPlugin(fullName);
			if(id==-1)
			{
				gLogManager.log("PluginManager::loadPluginsFromDir()-failed to load plugin: "+fullName,ELL_WARNING,EVL_Heavy);
			}else
				ret.push_front(getPlugin(id));
		}
	}
	return ret;
}

}



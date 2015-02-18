/********************************************************************
	created:	2008/12/18
	created:	18:12:2008   18:23
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\ScriptResourceManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	ScriptResourceManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ScriptResourceManager___
#define ___ScriptResourceManager___

#include "IResourceManager.h"
#include "IScriptManager.h"

namespace mray{

class MRAY_DLL ScriptResourceManager:public IResourceManager,public ISingleton<ScriptResourceManager>
{
protected:
	virtual IResourcePtr createInternal(const core::string& name);

	virtual IResourcePtr loadResourceFromFile(OS::IStream* file);
	virtual core::string getDefaultGroup();

	GCPtr<script::IScriptManager> m_scriptManager;
public:
	ScriptResourceManager();
	virtual~ScriptResourceManager();

	void setScriptManager(GCPtr<script::IScriptManager> manager);

	virtual void writeResourceToDist(const core::string&resName,const core::string&fileName);
	virtual void writeResourceToDist(const IResourcePtr& resource,const core::string&fileName);
	virtual void writeResourceToDist(ResourceHandle resource,const core::string&fileName);
};
#define gScriptResourceManager mray::ScriptResourceManager::getInstance()

}


#endif //___IScriptManager___
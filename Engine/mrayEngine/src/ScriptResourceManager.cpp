#include "stdafx.h"

#include "ScriptResourceManager.h"
#include "ILogManager.h"
namespace mray{


IResourcePtr ScriptResourceManager::createInternal(const core::string& name){
	if(!m_scriptManager){
		gLogManager.log(mT("ScriptResourceManager::createInternal() - please set script manager."),ELL_WARNING);
		return 0;
	}
	return m_scriptManager->createScript(name);
}


IResourcePtr ScriptResourceManager::loadResourceFromFile(OS::IStream* file){
	if(!m_scriptManager){
		gLogManager.log(mT("ScriptResourceManager::loadResourceFromFile() - please set script manager."),ELL_WARNING);
		return 0;
	}
	return m_scriptManager->createScript(file->getStreamName(),file);
}
core::string ScriptResourceManager::getDefaultGroup(){
	return mT("script");
}


ScriptResourceManager::ScriptResourceManager():IResourceManager(mT("Script Resource Manager")){
	m_scriptManager=0;
}
ScriptResourceManager::~ScriptResourceManager(){
	m_scriptManager=0;
}

void ScriptResourceManager::setScriptManager(GCPtr<script::IScriptManager> manager){
	m_scriptManager=manager;
}


void ScriptResourceManager::writeResourceToDist(const core::string&resName,const core::string&fileName){

}
void ScriptResourceManager::writeResourceToDist(const IResourcePtr& resource,const core::string&fileName){

}
void ScriptResourceManager::writeResourceToDist(ResourceHandle resource,const core::string&fileName){

}


}
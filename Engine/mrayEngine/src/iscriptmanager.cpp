#include "stdafx.h"


#include "IScriptManager.h"
#include "common.h"
#include "ScriptResourceManager.h"

namespace mray{
namespace script{

IScriptManager::IScriptManager(){
	m_lastID=0;
}
IScriptManager::~IScriptManager(){
	m_scripts.clear();
}

void IScriptManager::addScript(const IScriptPtr& s){
	if(!s)
		return;
	if(getScript(s->getResourceName()))
		return;
	m_scripts.push_back(s);
}

GCPtr<IScript> IScriptManager::getScript(const core::string&name){

	foreachIt(ScriptsList::iterator,it,m_scripts){
		if((*it)->getResourceName()==name)
			return *it;
	}
	return 0;
}
void IScriptManager::removeScript(const core::string&name){

	foreachIt(ScriptsList::iterator,it,m_scripts){
		if((*it)->getResourceName()==name){
			m_scripts.erase(it);
			return;
		}
	}
}
GCPtr<IScript> IScriptManager::createScript(const core::string&name){
	GCPtr<IScript> s;

	core::string scriptName=name;
	if(scriptName==mT("")){
		scriptName=mT("#")+core::StringConverter::toString(++m_lastID);
	}

	s=getScript(scriptName);
	if(s)
		return s;

	s=gScriptResourceManager.getResource(scriptName);
	if(s){
		addScript(s);
		return s;
	}
	s=createScriptInternal();
	if(s){
		gScriptResourceManager.addResource(s,scriptName);
		addScript(s);
	}
	return s;
}
IScriptPtr IScriptManager::createScript(const core::string&name,OS::IStream* file){
	if(!file)
		return 0;

	GCPtr<IScript> s=createScript(name);
	
	if(s){
		s->SetScript(file);
	}
	return s;
}

//update scripts
void IScriptManager::Update(float dt){
	/*
	foreachIt(ScriptsList::iterator,it,m_scripts){
		(*it)->update(dt);
	}*/
}



}
}

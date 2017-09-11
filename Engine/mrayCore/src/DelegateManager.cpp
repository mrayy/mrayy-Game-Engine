#include "stdafx.h"

#include "DelegateManager.h"
#include "StringUtil.h"


namespace mray{

DelegateManager::DelegateManager(){
}

DelegateManager::~DelegateManager(){
	clear();
}

void DelegateManager::registerDelegate(const GCPtr<ObjectDelegate>& d){
	core::string s=core::StringUtil::ToLower(d->getName());
	m_delegates.insert(DelegateMap::value_type(s,d));
}

GCPtr<ObjectDelegate> DelegateManager::getDelegate(const core::string&name){
	core::string s=core::StringUtil::ToLower(name);
	DelegateMap::iterator it= m_delegates.find(s);
	if(it==m_delegates.end())
		return 0;
	return it->second;
}

void DelegateManager::removeDelegate(const core::string&name){
	core::string s=core::StringUtil::ToLower(name);
	m_delegates.erase(s);
}

void DelegateManager::clear(){
	m_delegates.clear();
}

}


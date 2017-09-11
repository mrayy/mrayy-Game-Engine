#include "stdafx.h"

#include "IResourceManager.h"
#include "ILogManager.h"
#include "StringConverter.h"
#include "IStream.h"
#include "IFileSystem.h"
#include "ResourceGroup.h"
#include "IThreadManager.h"
#include "IMutex.h"

#include <limits>

namespace mray{


IResourceManager::IResourceManager(const mchar*resourceType){
	m_resourceType=resourceType;

	m_lastHandle=0;
	m_usedMemory=0;

	m_allowedMemory = ULONG_MAX;

	m_resMutex=OS::IThreadManager::getInstance().createMutex();

	ResourceGroup::getInstance().addManager(this);
}
IResourceManager::~IResourceManager(){
	removeAll();
	delete m_resMutex;
}


ResourceHandle IResourceManager::getNextHandle(){
	return m_lastHandle++;
}

void IResourceManager::removeInternal(const IResourcePtr& res){
	/*
	m_resMutex->lock();
	ResourceHandleMap::iterator hIt=m_resourceHandleMap.find(res->getResourceHandle().ID());
	if(hIt!=m_resourceHandleMap.end()){
		hIt->second=0;
		m_resourceHandleMap.erase(hIt);
	}
	ResourceMap::iterator nIt=m_resourceMap.find(res->getResourceName());
	if(nIt!=m_resourceMap.end()){
		nIt->second=0;
		m_resourceMap.erase(nIt);
	}
	m_resMutex->unlock();*/
}
bool IResourceManager::addInternal(const IResourcePtr& resource,const core::string& name){
	core::string shortName=name;//=gFileSystem.getShortFileName(name);
	if(getResource(shortName))
		return true;
	
	if(!resource)return 0;

	resource->setResourceName(shortName);
	resource->setResourceGroup(getDefaultGroup());
	resource->setResourceManager(this);
	GUID handle;
	core::stringc guidStr;
	core::string_to_char(shortName,guidStr);
	handle=guidStr;
	resource->setResourceHandle(handle);

	m_resMutex->lock();
	std::pair<ResourceMap::iterator,bool> resN= m_resourceMap.insert(ResourceMap::value_type(shortName,resource));
	if(!resN.second){
		gLogManager.log(mT("IResourceManager::addInternal()- Resource already exists with name:\"")+name+mT("\""),ELL_WARNING);
		m_resMutex->unlock();
		return 0;
	}
	std::pair<ResourceHandleMap::iterator,bool> resH= m_resourceHandleMap.insert(ResourceHandleMap::value_type(resource->getResourceHandle().ID(),resource));
	if(!resH.second){
		gLogManager.log(mT("IResourceManager::addInternal()- Resource already exists with Handle:\"")+
			core::string(core::StringConverter::toString(resource->getResourceHandle().ID()))+mT("\""),ELL_WARNING);
		m_resMutex->unlock();
		return 0;
	}
	m_resMutex->unlock();


	m_usedMemory+=resource->recalcSize();
	return true;
}

IResourcePtr IResourceManager::create(const core::string& name){
	IResourcePtr resource=createInternal(name);

	if(!resource)
		return IResourcePtr::Null;
	core::string shortName=name;
	int i=name.findlast('\\');
	if(i!=-1)
	{
		shortName=name.substr(i+1,name.length()-i);
	}
	addInternal(resource,shortName);
/**/
	return resource;
}
IResourcePtr IResourceManager::getOrCreate(const core::string& name){
	const IResourcePtr& resource=getResource(name);
	if(!resource){
		return create(name);
	}
	return resource;
}
IResourcePtr IResourceManager::createFromFile(OS::IStreamPtr file){
	core::string name=gFileSystem.getShortFileName(file->getStreamName());
	IResourcePtr resource=getResource(name);
	if(!resource){
		resource= loadResourceFromFile(file);
		if(!resource)
			return IResourcePtr::Null;

		if(!addInternal(resource,name))
			return IResourcePtr::Null;
	}
	return resource;
}

void IResourceManager::setAllowedMemory(ulong size){
	m_allowedMemory=size;
	checkMemory();
}
void IResourceManager::checkMemory(){
}

ulong IResourceManager::getAllowedMemory(){
	return m_allowedMemory;
}

ulong IResourceManager::getUsedMemory(bool recalc){
	if(!recalc)
		return m_usedMemory;
	m_usedMemory=0;
	m_resMutex->lock();
	ResourceMap::iterator it=m_resourceMap.begin();
	ResourceMap::iterator end=m_resourceMap.end();
	while(it!=end){
		m_usedMemory+=it->second->recalcSize();
		++it;
	}
	m_resMutex->unlock();
	return m_usedMemory;
}


void IResourceManager::unload(const core::string&name){
	ResourceMap::iterator it= m_resourceMap.find(name);
	if(it==m_resourceMap.end())
		return;
	it->second->unload();
}
void IResourceManager::unload(IResourcePtr resource){
	resource->unload();
}
void IResourceManager::unload(ResourceHandle resource){
	ResourceHandleMap::iterator it= m_resourceHandleMap.find(resource);
	if(it==m_resourceHandleMap.end())
		return;
	it->second->unload();
}

void IResourceManager::unloadAll(bool unusedOnly){
	ResourceMap::iterator it=m_resourceMap.begin();
	for(;it!=m_resourceMap.end();++it){
		if (!unusedOnly || it->second.getRefCount()==2)
			unload(it->second);
	}
}

void IResourceManager::reload(const core::string&name){
	IResourcePtr res=getResource(name);
	if(!res){
		return;
	}
	res->reload();
}
void IResourceManager::reload(IResourcePtr resource){
	if(!resource){
		return;
	}
	resource->reload();
}
void IResourceManager::reload(ResourceHandle resource){
	IResourcePtr res=getResourceByHandle(resource);
	if(!res){
		return;
	}
	res->reload();
}
void IResourceManager::reloadAll(){
	ResourceMap::iterator it=m_resourceMap.begin();
	for(;it!=m_resourceMap.end();++it){
		it->second->reload();
	}
}


void IResourceManager::addResource(const IResourcePtr& resource,const core::string&name){
	addInternal(resource,name);

}

void IResourceManager::remove(const core::string&name){
	IResourcePtr res=getResource(name);
	if(!res){
		return;
	}
	removeInternal(res);
	_removeResource(res);
}
void IResourceManager::remove(const IResourcePtr& resource){
	if(!resource){
		return;
	}
	removeInternal(resource);
	_removeResource(resource);
}
void IResourceManager::remove(ResourceHandle resource){
	IResourcePtr res=getResourceByHandle(resource);
	if(!res){
		return;
	}
	removeInternal(res);
	_removeResource(res);
}

void IResourceManager::_removeResource(const IResourcePtr& res)
{

	m_resMutex->lock();
	ResourceHandleMap::iterator hIt = m_resourceHandleMap.find(res->getResourceHandle().ID());
	if (hIt != m_resourceHandleMap.end()){
		hIt->second = 0;
		m_resourceHandleMap.erase(hIt);
	}
	ResourceMap::iterator nIt = m_resourceMap.find(res->getResourceName());
	if (nIt != m_resourceMap.end()){
		nIt->second = 0;
		m_resourceMap.erase(nIt);
	}
	m_resMutex->unlock();
}
void IResourceManager::removeAll(){
	m_resMutex->lock();
	ResourceMap::iterator it=m_resourceMap.begin();
	ResourceMap::iterator end=m_resourceMap.end();
	for (; it != end;++it)
	{
		removeInternal(it->second);
	}
	m_resourceMap.clear();
	m_resourceHandleMap.clear();
	m_resMutex->unlock();
}


const IResourcePtr& IResourceManager::getResource(const core::string&name){
	ResourceMap::iterator it= m_resourceMap.find( name);
	if(it==m_resourceMap.end())
		return IResourcePtr::Null;
	return it->second;
}
const IResourcePtr& IResourceManager::getResourceByHandle(ResourceHandle resource){
	ResourceHandleMap::iterator it= m_resourceHandleMap.find(resource);
	if(it==m_resourceHandleMap.end())
		return IResourcePtr::Null;
	return it->second;
}

bool IResourceManager::isResourceExists(const core::string&name){
	return getResource(name)!=IResourcePtr::Null;
}
bool IResourceManager::isResourceExists(ResourceHandle resource){
	return getResourceByHandle(resource)!=IResourcePtr::Null;
}

void IResourceManager::resourceLoaded(IResource*res){
	m_usedMemory+=res->getResourceSize();
}
void IResourceManager::resourceUnloaded(IResource*res){
	m_usedMemory-=res->getResourceSize();
}

void IResourceManager::_requestLoad(IResource*r)
{
}
void IResourceManager::_requestUnload(IResource*r)
{
}
void IResourceManager::listResources(std::vector<core::string>&outList){
	m_resMutex->lock();
	ResourceMap::iterator it=m_resourceMap.begin();
	for(;it!=m_resourceMap.end();++it){
		outList.push_back(it->second->getResourceName());
	}
	m_resMutex->unlock();
}

void IResourceManager::listResources(std::vector<IResourcePtr>&outList){
	m_resMutex->lock();
	ResourceMap::iterator it=m_resourceMap.begin();
	for(;it!=m_resourceMap.end();++it){
		outList.push_back(it->second);
	}
	m_resMutex->unlock();
}

const mchar*IResourceManager::getResourceType(){
	return m_resourceType.c_str();
}


}


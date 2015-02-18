#include "stdafx.h"

#include "IResource.h"
#include "IResourceManager.h"
#include "IAsyncLoadRequest.h"
#include "IThreadManager.h"


namespace mray{

	
IResource::IResource()
:m_manager(0),m_name(mT("")),m_group(mT("")),m_loadRequest(0)
{
	m_lastAccess=time(0);
	m_size=0;
	m_loadState=LoadState_Unloaded;
	m_usedCounter=0;
}

IResource::~IResource()
{
//	destroy();
	unload();
}

void IResource::preLoadInternal(bool async)
{
}
void IResource::loadInternal()
{
	if(m_manager)
		m_manager->_requestLoad(this);
}
void IResource::postLoadInternal()
{
	if(m_manager)
		m_manager->resourceLoaded(this);
}


void IResource::preUnloadInternal()
{
	if(m_manager)
		m_manager->_requestUnload(this);
}
void IResource::unloadInternal()
{
}
void IResource::postUnloadInternal()
{
	if(m_manager)
		m_manager->resourceUnloaded(this);
}

void IResource::_onResourceUsed()
{
	if(m_loadState!=LoadState_Loaded)
		load();
}

void IResource::useResource()
{
	_onResourceUsed();
	++m_usedCounter;
}
void IResource::unuseResource()
{
	if(m_usedCounter==1)
		unload();
	--m_usedCounter;
}

bool IResource::load(bool async)
{
	if(m_loadState==LoadState_Loaded)
		return true;
	if(m_loadRequest && m_loadRequest->IsLoaded())
	{
		delete m_loadRequest;
		m_loadRequest=0;
		_onLoaded();
	}else
	{
		if(m_loadState!=LoadState_Loading)
		{

			try{
				preLoadInternal(async);
				if(!async)
				{
					loadInternal();
					m_loadState=LoadState_Loaded;
					_onLoaded();
				}
			}catch(...){
				m_loadState=LoadState_Unloaded;
				throw;
			}
		}else if(!async && m_loadRequest)
		{
			//worst case,we are asked to use it,but it is already in the loading queue
			while(!m_loadRequest->IsLoaded())
			{
				OS::IThreadManager::getInstance().sleep(50);
			}
			delete m_loadRequest;
			m_loadRequest=0;
			_onLoaded();
		}
	}
	if(m_loadState==LoadState_Unloaded)
		m_loadState=LoadState_Loading;
	return true;
}
void IResource::_onLoaded()
{
	m_size=calcSizeInternal();
	postLoadInternal();
	m_loadState=LoadState_Loaded;

	notifyResourceListners(true);

	if(m_manager)
		m_manager->resourceLoaded(this);
}
void IResource::unload()
{

	m_loadState=LoadState_Unloading;

	preUnloadInternal();
	unloadInternal();
	postUnloadInternal();


	m_loadState=LoadState_Unloaded;

	notifyResourceListners(false);

// 	if(m_manager)
// 		m_manager->resourceUnloaded(this);
}
bool IResource::reload()
{
	if(m_loadState==LoadState_Loaded){
		unload();
	}

	if(!load())return false;

	return true;
}

IResource::ELoadingState IResource::getResourceState()const
{
	return m_loadState;
}

void IResource::fillProperties(){
}

void IResource::setResourceManager(IResourceManager*manager)
{
	m_manager=manager;
}
void IResource::setResourceHandle(const GUID& handler)
{
	m_handle=handler;
}

void IResource::setResourceName(const core::string&name)
{
	m_name=name;
}

void IResource::setResourceGroup(const core::string&group)
{
	m_group=group;
}

void IResource::addResourceListner(ResourceListner*listner)
{
	m_listners.push_back(listner);
}

void IResource::notifyResourceListners(bool load){
	for(int i=0;i<m_listners.size();++i){
		if(load)
			m_listners[i]->loadComplete(this);
		else
			m_listners[i]->unloadComplete(this);
	}
}

bool IResource::operator <(const IResource&o)const
{
	if(m_priorityLevel<o.m_priorityLevel)
		return true;
	else if(m_priorityLevel>o.m_priorityLevel) 
		return false;

	if(m_lastAccess<o.m_lastAccess)
		return true;
	else if(m_lastAccess>o.m_lastAccess)
		return false;

	return false;
}

xml::XMLElement* IResource::loadXMLSettings(xml::XMLElement*elem)
{
	IObject::loadFromXML(elem);
	return elem;
}
xml::XMLElement* IResource::exportXMLSettings(xml::XMLElement*elem){
	//elem->addAttribute(mT("Name"),m_name);
	IObject::exportToXML(elem);

	return elem;
}

}



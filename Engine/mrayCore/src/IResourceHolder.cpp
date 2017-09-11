#include "stdafx.h"

#include "IResourceHolder.h"
#include "IResource.h"


namespace mray
{



IResourceHolder::IResourceHolder()
{
}
IResourceHolder::~IResourceHolder()
{
	//UnuseResources();
}


void IResourceHolder::_AddDependedResource(IResource*res)
{
	m_dependendResources.push_back(res);
}
void IResourceHolder::_RemoveDependedResource(IResource*res)
{
	std::list<IResource*>::iterator it=m_dependendResources.begin();
	for(;it!=m_dependendResources.end();++it)
	{
		if(*it==res){
			m_dependendResources.erase(it);
			break;
		}
	}
}
void IResourceHolder::_AddDependedResourceHolders(IResourceHolder*res)
{
	m_dependendResourcesHolders.push_back(res);
}
void IResourceHolder::_RemoveDependedResourceHolders(IResourceHolder*res)
{
	std::list<IResourceHolder*>::iterator it=m_dependendResourcesHolders.begin();
	for(;it!=m_dependendResourcesHolders.end();++it)
	{
		if(*it==res){
			m_dependendResourcesHolders.erase(it);
			break;
		}
	}
}
const std::list<IResource*>& IResourceHolder::GetRequiredResources()
{
	return m_dependendResources;
}
const std::list<IResourceHolder*>& IResourceHolder::GetRequiredResourcesHolders()
{
	return m_dependendResourcesHolders;
}

void IResourceHolder::UnuseResources()
{
	{
		std::list<IResource*>::iterator it=m_dependendResources.begin();
		for(;it!=m_dependendResources.end();++it)
		{
			(*it)->unuseResource();
		}
	}
	{
		std::list<IResourceHolder*>::iterator it=m_dependendResourcesHolders.begin();
		for(;it!=m_dependendResourcesHolders.end();++it)
		{
			(*it)->UnuseResources();
		}
	}
}
void IResourceHolder::UseResources()
{
	{
		std::list<IResource*>::iterator it=m_dependendResources.begin();
		for(;it!=m_dependendResources.end();++it)
		{
			(*it)->useResource();
		}
	}
	{
		std::list<IResourceHolder*>::iterator it=m_dependendResourcesHolders.begin();
		for(;it!=m_dependendResourcesHolders.end();++it)
		{
			(*it)->UseResources();
		}
	}
}
void IResourceHolder::ReloadResources()
{
	std::list<IResource*>::iterator it=m_dependendResources.begin();
	for(;it!=m_dependendResources.end();++it)
	{
		(*it)->reload();
	}
}

}


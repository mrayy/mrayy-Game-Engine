#include "stdafx.h"

#include "ResourceGroup.h"


namespace mray{


ResourceGroup::ResourceGroup(){
}
ResourceGroup::~ResourceGroup(){
	m_managers.clear();
}

void ResourceGroup::addManager(GCPtr<IResourceManager> manager){
	m_managers.push_back(manager);
}
void ResourceGroup::removeManager(GCPtr<IResourceManager> manager){
	ResManagersListIT it=m_managers.begin();
	for(;it!=m_managers.end();++it){
		if((*it)==manager)
		{
			m_managers.erase(it);
			return;
		}
	}
}

ResManagersListIT ResourceGroup::begin(){
	return m_managers.begin();
}
ResManagersListIT ResourceGroup::end(){
	return m_managers.end();
}


}


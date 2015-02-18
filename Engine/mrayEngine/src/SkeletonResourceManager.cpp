#include "stdafx.h"

#include "SkeletonResourceManager.h"
#include "IStream.h"
#include "IFileSystem.h"
#include "SkeletonLoader.h"
#include "SkeletonExporter.h"



namespace mray{


SkeletonResourceManager::SkeletonResourceManager()
:IResourceManager(mT("Skeleton Resource Manager"))
{
	addSkeletonLoader(new loaders::SkeletonLoader());
	addSkeletonWriter(new loaders::SkeletonExporter());
}
SkeletonResourceManager::~SkeletonResourceManager(){

}


void SkeletonResourceManager::addSkeletonLoader(GCPtr<loaders::ISkeletonLoader> loader){
	m_Loaders.push_back(loader);
}
void SkeletonResourceManager::addSkeletonWriter(GCPtr<loaders::ISkeletonExporter> e){
	m_exporters.push_back(e);
}

IResourcePtr SkeletonResourceManager::createInternal(const core::string& name){
	IResourcePtr r;
	std::list<GCPtr<loaders::ISkeletonLoader>>::iterator p=m_Loaders.begin();
	std::list<GCPtr<loaders::ISkeletonLoader>>::iterator end=m_Loaders.end();
	for(;p!=end;++p)
	{
		if((*p)->canLoad(name)){
			r=(*p)->load(name);
			break;
		}
	}
	return r;
}

IResourcePtr SkeletonResourceManager::loadResourceFromFile(OS::IStream* file){
	core::string name=file->getStreamName();
	IResourcePtr r;
	std::list<GCPtr<loaders::ISkeletonLoader>>::iterator p=m_Loaders.begin();
	std::list<GCPtr<loaders::ISkeletonLoader>>::iterator end=m_Loaders.end();
	for(;p!=end;++p)
	{
		if((*p)->canLoad(name)){
			r=(*p)->load(file);
			break;
		}
	}
	return r;
}

core::string SkeletonResourceManager::getDefaultGroup(){
	return mT("Skeleton");
}

GCPtr<scene::Skeleton> SkeletonResourceManager::loadSkeleton(const core::string& name){
	return createInternal(name);
}

GCPtr<scene::Skeleton>SkeletonResourceManager::createSkeleton(const core::string& name){
	GCPtr<scene::Skeleton>s=new scene::Skeleton(name);
	addResource(s,name);
	return s;
}

void SkeletonResourceManager::writeResourceToDist(const core::string&resName,const core::string&fileName){
	IResourcePtr res=getResource(resName);
	if(!res)return;
	writeResourceToDist(res,fileName);
}
void SkeletonResourceManager::writeResourceToDist(const IResourcePtr& resource,const core::string&fileName){
	
	OS::IStreamPtr s=gFileSystem.openFile(fileName,OS::BIN_WRITE);
	if(!s)
		return;

	std::list<GCPtr<loaders::ISkeletonExporter>>::iterator p=m_exporters.begin();
	std::list<GCPtr<loaders::ISkeletonExporter>>::iterator end=m_exporters.end();
	for(;p!=end;++p)
	{
		if((*p)->canWrite(fileName)){
			(*p)->write((scene::Skeleton*)resource.pointer(),s);
			break;
		}
	}
	s->close();

}
void SkeletonResourceManager::writeResourceToDist(ResourceHandle resource,const core::string&fileName){
	IResourcePtr res=getResourceByHandle(resource);
	if(!res)return;
	writeResourceToDist(res,fileName);
}


}


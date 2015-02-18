#include "stdafx.h"

#include "IMovable.h"
#include "AnimationResourceManager.h"
#include "AnimLoader.h"

#include "TransAnimationGroup.h"
#include "IOSystem.h"
#include "IFileSystem.h"
#include "IDirOS.h"


namespace mray{


AnimationResourceManager::AnimationResourceManager()
	:IResourceManager(mT("Animation Resource Manager"))
{
	addAnimLoader(new loaders::AnimLoader());
}
AnimationResourceManager::~AnimationResourceManager(){
}
IResourcePtr AnimationResourceManager::createInternal(const core::string& name){
	IResourcePtr r;
	std::list<GCPtr<loaders::IAnimLoader>>::iterator p=m_Loaders.begin();
	std::list<GCPtr<loaders::IAnimLoader>>::iterator end=m_Loaders.end();
	for(;p!=end;++p)
	{
		if((*p)->canLoad(name)){
			r=(*p)->load(name);
			break;
		}
	}
	return r;
}


void AnimationResourceManager::loadAnimationFolder(const core::string&fname){
	GCPtr<OS::IDirOS>dos=OS::IOSystem::getInstance().createDirSystem();
	core::string path=gFileSystem.getAppPath();
	core::string path2;
	dos->changeDir(path.c_str());
	bool index=dos->changeDir(fname);

	std::vector<core::string>*pathLst=gFileSystem.getPathes();
	int i=0;
	while(!index && i<pathLst->size()){
		path2=path+(*pathLst)[i];
		dos->changeDir(path2.c_str());
		index=dos->changeDir(fname);
		i++;
	}
	if(index==0){
		return;
	}
	//	fos->changeDir(index);
	for(i=0;i<dos->getFilesCount();++i){
		IResourcePtr r=createInternal(dos->getShortFileName(i));
		if(r){
			addResource(r,r->getResourceName());
		}
	}
	dos->changeDir(path.c_str());
}

IResourcePtr AnimationResourceManager::loadResourceFromFile(OS::IStream* file){
	if(file==IResourcePtr::Null)
		return IResourcePtr::Null;
	core::string name=file->getStreamName();
	GCPtr<animation::TransformationAnimationMixer> r;
	std::list<GCPtr<loaders::IAnimLoader>>::iterator p=m_Loaders.begin();
	std::list<GCPtr<loaders::IAnimLoader>>::iterator end=m_Loaders.end();
	for(;p!=end;++p)
	{
		if((*p)->canLoad(name)){
			r=(*p)->load(file);
			break;
		}
	}
	return r;
}

core::string AnimationResourceManager::getDefaultGroup(){
	return mT("anim");
}

void AnimationResourceManager::addAnimLoader(const loaders::IAnimLoaderPtr& loader){
	m_Loaders.push_back(loader);
}

GCPtr<animation::TransformationAnimationMixer> AnimationResourceManager::getAnimation(const core::string&name){
	//return getResource(name);
	OS::IStreamPtr anim=gFileSystem.openFile(name);

	core::string resName=name;
	int i=resName.findlast('\\');
	if(i!=-1)
	{
		resName=resName.substr(i+1,resName.length()-i);
	}

	GCPtr<animation::TransformationAnimationMixer> res= loadResourceFromFile(anim);
	if(res)
		res->setResourceName(resName);
	return res;
}

void AnimationResourceManager::writeResourceToDist(const core::string&resName,const core::string&fileName){
}
void AnimationResourceManager::writeResourceToDist(const IResourcePtr& resource,const core::string&fileName){
}
void AnimationResourceManager::writeResourceToDist(ResourceHandle resource,const core::string&fileName){
}

}



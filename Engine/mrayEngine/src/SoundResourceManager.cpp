#include "stdafx.h"


#include "SoundResourceManager.h"

#include "ILogManager.h"
#include "IFileSystem.h"
#include "ISoundStreamFrame.h"


// #include "IWaveLoader.h"
// #include "IOggLoader.h"

namespace mray{


core::string SoundResourceManager::getDefaultGroup(){
	return mT("sound");
}

SoundResourceManager::SoundResourceManager()
:IResourceManager(mT("Sound Resource Manager"))
{
	m_soundLoader=new loaders::SoundLoader();
// 	addSoundLoader(new loaders::IWaveLoader());
// 	addSoundLoader(new loaders::IOGGLoader());
}

SoundResourceManager::~SoundResourceManager()
{
	m_soundLoader=0;
}

void SoundResourceManager::addSoundLoader(loaders::ISoundLoader*loader){
	m_soundLoader->addLoader(loader);
}

IResourcePtr SoundResourceManager::createInternal(const core::string& name){

	IResourcePtr r=m_soundLoader->load(name.c_str());
	if(r)
	{
		int i=name.findlast('\\');
		core::string shortName;
		if(i!=-1)
		{
			shortName=name.substr(i,name.length()-i);
			r->setResourceName(shortName);
		}
	}
	return r;
}
IResourcePtr SoundResourceManager::loadResourceFromFile(OS::IStream* file){
	return m_soundLoader->load(file);
}

IResourcePtr SoundResourceManager::getOrCreate(const core::string& name){
	GCPtr<sound::ISoundStreamFrame> resource=getResource(name);
	if(!resource){
		return create(name);
	}else{
		return resource->duplicate();
	}
}
sound::ISoundPtr SoundResourceManager::loadSound(const core::string& name){
	return getOrCreate(name);
}

void SoundResourceManager::writeResourceToDist(const core::string&resName,const core::string&fileName){
	gLogManager.log(mT("SoundResourceManager::writeResourceToDist() - not supported!"),ELL_WARNING);
}
void SoundResourceManager::writeResourceToDist(const IResourcePtr& resource,const core::string&fileName){
	gLogManager.log(mT("SoundResourceManager::writeResourceToDist() - not supported!"),ELL_WARNING);
}
void SoundResourceManager::writeResourceToDist(ResourceHandle resource,const core::string&fileName){
	gLogManager.log(mT("SoundResourceManager::writeResourceToDist() - not supported!"),ELL_WARNING);
}

}





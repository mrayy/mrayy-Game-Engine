#include "stdafx.h"

#include "SoundLoader.h"
#include "ISoundLoader.h"
#include "ISound.h"
#include "IFileSystem.h"

namespace mray{
namespace loaders{

SoundLoader::~SoundLoader(){
	Loaders.clear();
}
void SoundLoader::addLoader(GCPtr<ISoundLoader>loader){
	Loaders.push_back(loader);
}

sound::ISoundStreamFrame* SoundLoader::load(const  mchar*name){


	sound::ISoundStreamFrame* dataFile=0;
	for(int i=0;i<Loaders.size();++i){
		if(Loaders[i]->canLoad(name)){
			core::string str;
			gFileSystem.getCorrectFilePath(name,str);
			dataFile=Loaders[i]->load(str.c_str());
			break;
		}
	}
	return dataFile;
}
sound::ISoundStreamFrame*  SoundLoader::load(OS::IStream* file){

	sound::ISoundStreamFrame* dataFile=0;
	for(int i=0;i<Loaders.size();++i){
		if(Loaders[i]->canLoad(file->getStreamName().c_str())){
			dataFile=Loaders[i]->load(file);
			break;
		}
	}
	return dataFile;
}

};//loaders
};//mray


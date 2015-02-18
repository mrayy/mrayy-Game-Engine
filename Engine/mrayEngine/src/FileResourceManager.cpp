#include "stdafx.h"

#include "FileResourceManager.h"
#include "IFileSystem.h"
#include "StringUtil.h"
#include "InternetCacheManager.h"


namespace mray{

FileResourceManager::FileResourceManager()
	:IResourceManager(mT("File Resource Manager"))
{
}
FileResourceManager::~FileResourceManager(){
}

IResourcePtr FileResourceManager::createInternal(const core::string& name){
	OS::IStreamPtr f;
	if (core::StringUtil::BeginsWith(name, "url="))
	{
		core::string www = name.substr(4, name.length() - 4);
		f = network::InternetCacheManager::getInstance().GetOrCreateItem(www);

	}else
		f=gFileSystem.createBinaryFileReader(name.c_str());
	if(f)
		f->setResourceName(name);
	return f;
}

IResourcePtr FileResourceManager::loadResourceFromFile(OS::IStream* file){
	return 0;
}

core::string FileResourceManager::getDefaultGroup(){
	return mT("Files");
}

OS::IStreamPtr FileResourceManager::getFile(const core::string&name, OS::FILE_MODE mode)
{
	OS::IStreamPtr  stream=getOrCreate(name);
	if (stream != OS::IStreamPtr::Null)
		stream->reopen(mode);
	return stream;
}

void FileResourceManager::writeResourceToDist(const core::string&resName,const core::string&fileName){
}
void FileResourceManager::writeResourceToDist(const IResourcePtr& resource,const core::string&fileName){
}
void FileResourceManager::writeResourceToDist(ResourceHandle resource,const core::string&fileName){
}


}
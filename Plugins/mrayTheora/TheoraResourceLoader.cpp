

#include "stdafx.h"
#include "TheoraResourceLoader.h"
#include "TheoraManager.h"
#include "Stringutil.h"

namespace mray
{
namespace video
{

TheoraResourceLoader::TheoraResourceLoader()
{
}

TheoraResourceLoader::~TheoraResourceLoader()
{
}

video::IVideoSource* TheoraResourceLoader::load(OS::IStream *file)
{
	if(!file)
		return 0;
	return TheoraManager::getInstance().CreateVideoFromStream(file);
	
}

video::IVideoSource* TheoraResourceLoader::load(const  core::string&name)
{
	return TheoraManager::getInstance().CreateVideoFromFile(name);
}
bool TheoraResourceLoader::canLoad(OS::IStream*file)
{
	return canLoad(file->getStreamName());
}

bool TheoraResourceLoader::canLoad(const  core::string&name)
{
	core::string p,ext;
	core::StringUtil::SplitPathExt(name,p,ext);
	return ext.equals_ignore_case(getExt());
}


}
}

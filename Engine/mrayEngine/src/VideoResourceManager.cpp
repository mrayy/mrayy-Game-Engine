
#include "stdafx.h"
#include "VideoResourceManager.h"

namespace mray
{


VideoResourceManager::VideoResourceManager()
	:IResourceManager(mT("Video Resource Manager"))
{
}

VideoResourceManager::~VideoResourceManager()
{
}


video::IVideoSourcePtr VideoResourceManager::LoadVideo(const core::string& path)
{
	video::IVideoSourcePtr res=getResource(path);
	if(res)
		return res;

	LoaderList::iterator it=m_loaders.begin();
	for (;it!=m_loaders.end();++it)
	{
		if((*it)->canLoad(path))
		{
			res=(*it)->load(path);
			break;
		}
	}
	if(!res.isNull())
		addResource(res,path);
	return res;
}


IResourcePtr VideoResourceManager::createInternal(const core::string& name)
{
	return IResourcePtr::Null;
}


IResourcePtr VideoResourceManager::loadResourceFromFile(OS::IStream* file)
{
	LoaderList::iterator it=m_loaders.begin();
	for (;it!=m_loaders.end();++it)
	{
		if((*it)->canLoad(file))
		{
			return (*it)->load(file);
			break;
		}
	};
	return IResourcePtr::Null;
}


core::string VideoResourceManager::getDefaultGroup()
{
	return "Video";
}


void VideoResourceManager::AddVideoLoader(IVideoResourceLoader* loader)
{
	m_loaders.push_back(loader);
}

void VideoResourceManager::RemoveVideoLoader(IVideoResourceLoader* loader)
{
	LoaderList::iterator it=m_loaders.begin();
	for (;it!=m_loaders.end();++it)
	{
		if(*it==loader)
		{
			m_loaders.erase(it);
			break;
		}
	}
}

bool VideoResourceManager::IsLoaderSupported(const core::string& ext)
{
	LoaderList::iterator it=m_loaders.begin();
	for (;it!=m_loaders.end();++it)
	{
		if((*it)->getExt().equals_ignore_case(ext))
		{
			return true;
		}
	}
	return false;
}


}
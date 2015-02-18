#include "stdafx.h"

#include "ITexLoader.h"
#include "TexLoader.h"
#include "StringUtil.h"

namespace mray{
namespace loaders{

TexLoader::TexLoader()
{

}

TexLoader::~TexLoader()
{
	LoadersMap::iterator it= m_loaders.begin();
	for(;it!=m_loaders.end();++it)
		delete it->second;
	m_loaders.clear();
}



void TexLoader::addLoader(ITexLoader* loader)
{
	core::string str=core::StringUtil::ToLower(loader->getExt());
	m_loaders[str]=loader;
}

void TexLoader::removeLoader(const core::string& ext)
{

	core::string str=core::StringUtil::ToLower(ext);
	LoadersMap::iterator it= m_loaders.find(str);
	if(it!=m_loaders.end())
	{
		delete it->second;
		m_loaders.erase(it);
	}
}
ITexLoader* TexLoader::getLoader(const core::string&ext)
{
	core::string str=core::StringUtil::ToLower(ext);
	LoadersMap::iterator it= m_loaders.find(str);
	if(it!=m_loaders.end())
		return it->second;
	return 0;
}
bool TexLoader::load(const core::string&name,video::ImageInfo* tex,video::ETextureType target)
{
	core::string ext,path;
	core::StringUtil::SplitPathExt(name,path,ext);

	LoadersMap::iterator it= m_loaders.find(ext);
	if(it!=m_loaders.end())
	{
		return it->second->load(name,tex,target);
	}
	return 0;
}

bool TexLoader::load(OS::IStream* file,video::ImageInfo* tex,video::ETextureType target)
{
	if(!file)return 0;

	core::string ext,path;
	core::StringUtil::SplitPathExt(file->getStreamName(),path,ext);
	ext=core::StringUtil::ToLower(ext);

	LoadersMap::iterator it= m_loaders.find(ext);
	if(it!=m_loaders.end())
	{
		return it->second->load(file,tex,target);
	}else
	{
		it= m_loaders.begin();
		for(;it!=m_loaders.end();++it)
		{
			if(it->second->canLoad(file))
			{
				return it->second->load(file,tex,target);
			}
		}
	}
	return 0;
}



}
}

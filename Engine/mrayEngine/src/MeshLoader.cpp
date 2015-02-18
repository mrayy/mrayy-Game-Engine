#include "stdafx.h"


#include "MeshLoader.h"
#include "IFileSystem.h"
#include "ILogManager.h"
#include "IMeshLoader.h"
#include "StringUtil.h"


namespace mray{
namespace loaders{

MeshLoader::~MeshLoader()
{
	std::map<core::string, IMeshLoader*>::iterator it = m_Loaders.begin();
	for (; it != m_Loaders.end(); ++it)
		delete it->second;
	m_Loaders.clear();
}

void MeshLoader::addLoader(IMeshLoader* loader)
{
	core::string s = loader->getExtension();
	s.make_lower();
	m_Loaders[s]=loader;
}

scene::SMesh* MeshLoader::Load(const  core::string&name)
{/*
	core::string fileName;
	OS::FileSystem::getCorrectFilePath(name,fileName);*/

	core::string path, ext;
	core::StringUtil::SplitPathExt(name, path, ext);
	scene::SMesh* mesh = 0;
	IMeshLoader* loader = GetLoader(ext);
	if (!loader)
		return 0;

	mesh = loader->load(name);
	
	if(!mesh)
		gLogManager.log(core::string(mT("Mesh File couldn't be loaded: "))+name,ELL_WARNING);
	return mesh;
}
scene::SMesh* MeshLoader::Load(OS::IStream* file){
	scene::SMesh* mesh = 0;
	core::string path, ext;
	core::StringUtil::SplitPathExt(file->getStreamName(), path, ext);
	IMeshLoader* loader = GetLoader(ext);
	if (!loader)
		return 0;
	mesh = loader->load(file);

	if(!mesh)
		gLogManager.log(core::string(mT("Mesh File couldn't be loaded: "))+file->getStreamName(),ELL_WARNING);
	return mesh;
}



IMeshLoader* MeshLoader::GetLoader(const core::string& ext)
{
	core::string s = ext;
	s.make_lower();
	std::map<core::string, IMeshLoader*>::iterator it = m_Loaders.find(s);
	if (it == m_Loaders.end())
		return 0;
	return it->second;
}



}
}

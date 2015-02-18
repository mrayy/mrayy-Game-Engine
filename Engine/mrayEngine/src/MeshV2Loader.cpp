#include "stdafx.h"

#include "MeshV2Loader.h"
#include "MeshSerializer.h"
#include "IFileSystem.h"

#include "StringUtil.h"

namespace mray
{
namespace loaders
{

MeshV2Loader::MeshV2Loader()
{
}
MeshV2Loader::~MeshV2Loader()
{
}

scene::SMesh* MeshV2Loader::load(const core::string& name)
{
	if(!canLoad(name))return 0;
	OS::IStreamPtr file=gFileSystem.openFile(name);
	if(!file)return 0;
	return load(file);
}
scene::SMesh* MeshV2Loader::load(OS::IStream* stream)
{
	scene::MeshSerializer s;
	return s.Deserialize(stream);
}
bool MeshV2Loader::canLoad(const  core::string& name)
{
	core::string str=core::StringUtil::ToLower(name);
	return strSearch(str.c_str(),mT(".mesh"))!=0;
}
const core::string g_sMeshV2LoaderExt=mT("mesh");
const core::string&MeshV2Loader::getExtension(){
	return g_sMeshV2LoaderExt;
}


}
}
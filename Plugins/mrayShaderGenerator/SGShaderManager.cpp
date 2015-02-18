
#include "stdafx.h"
#include "SGShaderManager.h"
#include "SGShaderLibrary.h"
#include "IOSystem.h"
#include "IDirOS.h"


namespace mray
{
namespace shader
{


SGShaderManager::SGShaderManager()
{
}
SGShaderManager::~SGShaderManager()
{
	LibrayList::iterator it;
	it=m_libraries.begin();
	for(;it!=m_libraries.end();++it)
	{
		delete *it;
	}
}

bool SGShaderManager::LoadShaderLibrary(const core::string& path)
{
	OS::IStreamPtr stream=gFileSystem.createTextFileReader(path);
	if(stream.isNull())
	{
		gLogManager.log(mT("SGShaderManager::LoadShaderLibrary() - library:[")+path+mT("] was not found!"),ELL_WARNING);
		return false;
	}

	xml::XMLTree tree;
	if(!tree.load(stream))
	{
		gLogManager.log(mT("SGShaderManager::LoadShaderLibrary() - cannot load library:[")+path+mT("], invalid xml file!"),ELL_WARNING);
		return false;
	}
	xml::XMLElement * root=tree.getSubElement(mT("ShaderLib"));
	if(!root)
	{
		gLogManager.log(mT("SGShaderManager::LoadShaderLibrary() - cannot load library:[")+path+mT("], ShaderLib was not found!"),ELL_WARNING);
		return false;
	}
	SGShaderLibrary* lib=new SGShaderLibrary();
	lib->Load(root);
	m_libraries.push_back(lib);

	return true;
}
void SGShaderManager::LoadShaderLibraries(const core::string& directory)
{
	GCPtr<OS::IDirOS> dir=gOSystem.createDirSystem();
	dir->changeDir(directory);
	int filesCnt=dir->getFilesCount();
	for(int i=0;i<filesCnt;++i)
	{
		core::string ext=dir->getFileType(i);
		ext.make_lower();
		if(ext==mT("xml"))
		{
			LoadShaderLibrary(dir->getFullFileName(i));
		}
	}
}

}
}

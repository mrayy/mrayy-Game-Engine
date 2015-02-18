#include "stdafx.h"

#include "WinDllManager.h"
#include "IFileSystem.h"
#include "WinDirOS.h"
#include "WinDynamicLibrary.h"

#include "TraceManager.h"
#include "common.h"

namespace mray{
namespace OS{


 
WinDllManager::WinDllManager(){
}
WinDllManager::~WinDllManager(){
	ClearLibraries();
}

void WinDllManager::ClearLibraries()
{
	m_libraries.clear();
}

IDynamicLibrary* WinDllManager::loadLibrary(const  core::string&name){

	traceFunction(eEngine);
	core::string path;
	gFileSystem.getCorrectFilePath(name,path);
	if(path==mT(""))return 0;
	IDynamicLibrary*lib=new WinDynamicLibrary(path);
	m_libraries.insert(LibraryMap::value_type(name,lib));
	
	return lib;
}

GCPtr<IDynamicLibrary> WinDllManager::getLibrary(const  core::string&name){
	IDynamicLibrary*lib=0;

	LibraryMapIT it=m_libraries.find(name);
	if(it==m_libraries.end()){
		lib=loadLibrary(name);
	}else{
		lib=it->second;
	}
	return lib;
}
void WinDllManager::loadLibrariesFromDir(const  core::string&path){

	traceFunction(eEngine);
	GCPtr<OS::IDirOS> dir=new OS::WinDirOS();
	if(!dir->changeDir(path))
		return;
	core::string fileName;
	int cnt=dir->getFilesCount();
	for(int i=0;i<cnt;++i){
		fileName=dir->getShortFileName(i);
		core::string ext=fileName.substr(fileName.length()-3,3);
		if(ext.equals_ignore_case(mT("dll"))){
			core::string fullName=dir->getFullFileName(i);
			loadLibrary(fullName);
		}
	}
	
}

}
}



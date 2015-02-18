#include "stdafx.h"

#include "WinDynamicLibrary.h"
#include "ILogManager.h"
#include "IFileSystem.h"

namespace mray{
namespace OS{



WinDynamicLibrary::WinDynamicLibrary(const  core::string&libName)
{
	m_libName=gFileSystem.getShortFileName(libName);
	
	m_handle=0;
	if(m_libName==mT(""))return;

	core::string path;
	gFileSystem.getCorrectFilePath(libName,path);

	m_handle=LoadLibrary(path.c_str());

	if(!m_handle){
		gLogManager.log(mT("WinDynamicLibrary::WinDynamicLibrary() - failed to load Library: "),libName,ELL_WARNING);
	}
}

WinDynamicLibrary::~WinDynamicLibrary()
{
	FreeLibrary(m_handle);
}

const  core::string&WinDynamicLibrary::getLibName()const
{
	return m_libName;
}

void* WinDynamicLibrary::getSymbolName(const char*name)const
{
	return GetProcAddress(m_handle,name);
}


}
}




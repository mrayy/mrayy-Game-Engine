

#ifndef ___WinDllManager___
#define ___WinDllManager___

#include <IDllManager.h>
#include <IDynamicLibrary.h>
#include <GCPtr.h>
#include <map>
#include <string>


#include "compileConfig.h"
namespace mray{
namespace OS{


class MRAY_Win32_DLL WinDllManager:public IDllManager
{
	typedef std::map<core::string,GCPtr<IDynamicLibrary>> LibraryMap;
	typedef std::map<core::string,GCPtr<IDynamicLibrary>>::iterator LibraryMapIT;

	LibraryMap m_libraries;

	IDynamicLibrary* loadLibrary(const  core::string&name);
public:

	WinDllManager();
	virtual~WinDllManager();


	void ClearLibraries();
	GCPtr<IDynamicLibrary> getLibrary(const  core::string&name);
	void loadLibrariesFromDir(const  core::string&path);
};

}
}



#endif





#ifndef ___WinDynamicLibrary___
#define ___WinDynamicLibrary___


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <IDynamicLibrary.h>
#include "CompileConfig.h"

namespace mray{
namespace OS{


class MRAY_Win32_DLL WinDynamicLibrary:public IDynamicLibrary
{
	core::string m_libName;
	HINSTANCE m_handle;

public:
	WinDynamicLibrary(const  core::string&libName);

	virtual~WinDynamicLibrary();

	const  core::string&getLibName()const;

	void* getSymbolName(const char*name)const;

};

}
}




#endif






#ifndef __Win32_compileConfig___
#define __Win32_compileConfig___


#ifndef MRAY_Win32_DLL

#ifndef Win32_LIB_STATIC
#ifdef MRAY_Win32_DLL_EXPORT
#define MRAY_Win32_DLL __declspec(dllexport)
#else
#define MRAY_Win32_DLL __declspec(dllimport)

#endif

#else
#define MRAY_Win32_DLL
#endif

#endif




#endif


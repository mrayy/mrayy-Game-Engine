

#ifndef __WinApp_compileConfig___
#define __WinApp_compileConfig___


#ifndef MRAY_WinApp_DLL

#ifndef WinApp_LIB_STATIC
#ifdef MRAY_WinApp_DLL_EXPORT
#define MRAY_WinApp_DLL __declspec(dllexport)
#else
#define MRAY_WinApp_DLL __declspec(dllimport)

#endif

#else
#define MRAY_WinApp_DLL
#endif

#endif




#endif


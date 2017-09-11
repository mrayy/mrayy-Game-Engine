


#ifndef ___CoreCompileConfig___
#define ___CoreCompileConfig___

//#define ENABLE__DLL

#ifndef MRAY_CORE_DLL

#ifndef CORE_STATIC_LIB_
#ifdef MRAY_CORE_DLL_EXPORT
#define MRAY_CORE_DLL __declspec(dllexport)
#else
#define MRAY_CORE_DLL __declspec(dllimport)

#endif

#else
#define MRAY_CORE_DLL
#endif

#endif

#endif
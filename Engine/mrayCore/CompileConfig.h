


#ifndef ___CompileConfig___
#define ___CompileConfig___

//#define ENABLE__DLL

#ifndef MRAY_NET_DLL

#ifndef NET_STATIC_LIB_
#ifdef MRAY_NET_DLL_EXPORT
#define MRAY_NET_DLL __declspec(dllexport)
#else
#define MRAY_NET_DLL __declspec(dllimport)

#endif

#else
#define MRAY_NET_DLL
#endif

#endif

#endif





#ifndef ___PHYSX_CompileConfig___
#define ___PHYSX_CompileConfig___

#ifndef MRAY_PHYSX_DLL

#ifndef PHYSX_STATIC_LIB
#ifdef MRAY_PHYSX_DLL_EXPORT
#define MRAY_PHYSX_DLL __declspec(dllexport)
#else
#define MRAY_PHYSX_DLL __declspec(dllimport)

#endif

#else
#define MRAY_PHYSX_DLL
#endif

#endif


#endif



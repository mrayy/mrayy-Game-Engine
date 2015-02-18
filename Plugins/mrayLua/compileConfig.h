
#ifndef ___LUA_CompileConfig___
#define ___LUA_CompileConfig___

#ifndef MRAY_LUA_DLL

#ifndef LUA_STATIC_LIB_
#ifdef MRAY_LUA_DLL_EXPORT
#define MRAY_LUA_DLL __declspec(dllexport)
#else
#define MRAY_LUA_DLL __declspec(dllimport)

#endif

#else
#define MRAY_LUA_DLL
#endif

#endif


#endif


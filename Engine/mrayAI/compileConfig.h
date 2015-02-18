



#ifndef ___AI_CompileConfig___
#define ___AI_CompileConfig___

#ifndef MRAY_AI_DLL

#ifndef AI_STATIC_LIB
#ifdef MRAY_AI_DLL_EXPORT
#define MRAY_AI_DLL __declspec(dllexport)
#else
#define MRAY_AI_DLL __declspec(dllimport)

#endif

#else
#define MRAY_AI_DLL
#endif

#endif


#endif



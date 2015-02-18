

#ifndef ___PH_CHARACTER_CompileConfig___
#define ___PH_CHARACTER_CompileConfig___

#ifndef MRAY_PH_CHARACTER_DLL

#ifndef PH_CHARACTER_STATIC_LIB
#ifdef PH_CHARACTER_EXPORT
#define MRAY_PH_CHARACTER_DLL __declspec(dllexport)
#else
#define MRAY_PH_CHARACTER_DLL __declspec(dllimport)

#endif

#else
#define MRAY_PH_CHARACTER_DLL
#endif

#endif


#endif






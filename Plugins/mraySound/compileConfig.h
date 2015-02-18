
#ifndef ___SOUND_CompileConfig___
#define ___SOUND_CompileConfig___

#ifndef SOUND_STATIC_LIB_
#ifdef MRAY_SOUND_DLL_EXPORT
	#define MRAY_SOUND_DLL __declspec(dllexport)
#else
	#define MRAY_SOUND_DLL __declspec(dllimport)

#endif

#else
#define MRAY_SOUND_DLL
#endif

//#define MRAY_SOUND_DLL

#endif


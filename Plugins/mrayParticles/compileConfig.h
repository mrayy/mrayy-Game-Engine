

#ifndef __PS_compileConfig___
#define __PS_compileConfig___


#ifndef MRAY_PS_DLL

#ifndef PS_LIB_STATIC
#ifdef MRAY_PS_DLL_EXPORT
	#define MRAY_PS_DLL __declspec(dllexport)
#else
	#define MRAY_PS_DLL __declspec(dllimport)

#endif

#else
#define MRAY_PS_DLL
#endif

#endif




#endif


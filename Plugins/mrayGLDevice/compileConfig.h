
#ifndef ___GL_CompileConfig___
#define ___GL_CompileConfig___

#include <DisableWarn.h>

#ifndef MRAY_GL_DLL
#	ifndef MRAY_GLDEVICE_LIB
#		ifdef MRAY_GLDEVICE_EXPORTS
#			define MRAY_GL_DLL __declspec(dllexport)
#		else
#			define MRAY_GL_DLL __declspec(dllimport)
#		endif
#	else
#		define MRAY_GL_DLL
#	endif
#endif


#endif





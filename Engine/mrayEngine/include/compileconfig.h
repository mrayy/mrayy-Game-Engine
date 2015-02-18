
#ifndef ____COMPILECONFIG______
#define ____COMPILECONFIG______



#define MRAY_SDK_VERSION "1.0"

#ifndef MRAY_STATIC_LIB
#ifdef MRAY_DLL_EXPORT
	#define MRAY_DLL __declspec(dllexport)
#else
	#define MRAY_DLL __declspec(dllimport)

#endif

#else
#define MRAY_DLL
#endif

#include "DisableWarn.h"


// to Create Properties(set/get)
//#define rwProperty(setFun,getFun)__declspec(rwProperty(get=getFun,put=setFun))
#ifndef MRAY_DLL_EXPORT
 
#endif
#ifndef _CONSOLE
   #define APPLICATION_ENTRY_POINT int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//  #pragma comment(linker, "/subsystem:windows" )
#else
   #define APPLICATION_ENTRY_POINT int main(int argc, char* argv[])
 // #pragma comment(linker, "/subsystem:console" )
#endif

//#define MRAY_MEMORY_MANAGMENT


#pragma warning(disable : 4100) // unreferenced formal parameter
#pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable : 4311) // 'm_type cast' : pointer truncation from 'FARPROC' to 'unsigned int'
#pragma warning(disable : 4408) // anonymous union did not declare any data members
#pragma warning(disable : 4511) // copy constructor could not be generated
#pragma warning(disable : 4512) // assignment operator could not be generated
#pragma warning(disable : 4530) // C++ exception handler used, but unwind semantics are not enabled
#pragma warning(disable : 4996) // was declared deprecated
#pragma warning(disable : 4267) // convesion warning
#pragma warning(disable : 4800) // Performance warning
#pragma warning(disable : 4244) // conversion warning
#pragma warning(disable : 4018) // 




//if you comment this you will let m-ray engine to compile using jpeg libe installed in the 
//system
#define ___USE_NON_SYSTEM_JPEG_LIB___
 /*
#pragma comment(lib, "lib/opengl32.lib")
#pragma comment(lib, "lib/glu32.lib")
#pragma comment(lib, "lib/glaux.lib")
#pragma comment(lib, "lib/alut.lib" )
#pragma comment(lib, "lib/OpenAL32.lib" )
#pragma comment (lib, "lib/ogg.lib")
#pragma comment (lib, "lib/ogg_static.lib")
#pragma comment (lib, "lib/vorbis.lib")
#pragma comment (lib, "lib/vorbisfile.lib")
#pragma comment (lib, "lib/vorbisfile_static.lib")
#pragma comment (lib, "lib/ijl15.lib")
#pragma comment (lib, "vfw32.lib")
// performance tuning
#pragma comment( lib, "lib/nvAPI.lib")
#pragma comment( lib, "lib/NVPerfSDK.lib")

*/
#define GL_GLEXT_PROTOm_typeS 1
#define GLX_GLXEXT_PROTOm_typeS 1

#ifdef ___USE_FMOD_SOUND___
//#pragma comment (lib, "lib/fmodex_vc.lib")

#endif


#endif






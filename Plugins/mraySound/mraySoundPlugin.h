

#ifndef ___mraySoundPlugin___
#define ___mraySoundPlugin___

#include <IPlugin.h>
#include <GCPtr.h>
#include "compileConfig.h"
#include "ObjectFactory.h"

namespace mray{
	namespace sound{
		class ISoundManager;
	}
namespace plugins{

class MRAY_SOUND_DLL mraySoundPlugin:public IPlugin
{
	ObjectFactory<sound::ISoundManager> *m_fslCreater;
	ObjectFactory<sound::ISoundManager> *m_fmodCreater;
public:
	mraySoundPlugin();
	virtual~mraySoundPlugin();

	virtual core::string getName();

	virtual void install();
	virtual void uninstall();
};
extern "C" MRAY_SOUND_DLL IPlugin* dllCreatePlugin();
}
}


BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved );

#endif



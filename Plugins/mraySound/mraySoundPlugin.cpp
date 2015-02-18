#include "stdafx.h"

#include <windows.h>

#include "mraySoundPlugin.h"
#include <SoundManagerFactory.h>

#include "FSLManager.h"
#include "SFModSoundmanager.h"


namespace mray{
namespace plugins{

	
mraySoundPlugin::mraySoundPlugin(){
}
mraySoundPlugin::~mraySoundPlugin(){
}

 core::string mraySoundPlugin::getName(){
	 return mT("mraySound");
}

void mraySoundPlugin::install(){
	if(isInstalled())return;
	IPlugin::install();

	m_fslCreater=new mray::sound::FSLManagerCreater();
	m_fmodCreater=new mray::sound::FModManagerCreater();
	mray::sound::SoundManagerFactory::getInstance().RegisterFactory(m_fslCreater);
	mray::sound::SoundManagerFactory::getInstance().RegisterFactory(m_fmodCreater);
}
void mraySoundPlugin::uninstall(){
	if(!isInstalled())return;
	IPlugin::uninstall();
	mray::sound::SoundManagerFactory::getInstance().UnregisterFactory(m_fslCreater->GetType());
	mray::sound::SoundManagerFactory::getInstance().UnregisterFactory(m_fmodCreater->GetType());
	delete m_fslCreater;
	delete m_fmodCreater;

	m_fslCreater=0;
	m_fmodCreater=0;
}


extern "C" MRAY_SOUND_DLL IPlugin* dllCreatePlugin(){
	return new mraySoundPlugin();

}


}
}


BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved )
{
	switch( fdwReason )
	{
	case DLL_PROCESS_ATTACH:
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE; 
}
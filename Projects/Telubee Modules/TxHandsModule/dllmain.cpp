// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "StringUtil.h"

HMODULE g_hModule;
mray::core::string ModuleName;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		g_hModule = hModule;
		char buffer[MAX_PATH]; ; //or wchar_t * buffer;
		GetModuleFileNameA(g_hModule, buffer, MAX_PATH);

		mray::core::string tmp;
		mray::core::StringUtil::SplitPathFileName(buffer, tmp, ModuleName);
		mray::core::StringUtil::SplitPathExt(ModuleName, ModuleName, tmp);
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


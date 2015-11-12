

#include "stdafx.h"
#include "UnityPlugin.h"
#include <windows.h>


void DebugLog(const std::string &str)
{
#if UNITY_WIN
	OutputDebugStringA(str.c_str());
#else
	printf("%s", m.c_str());
#endif
}
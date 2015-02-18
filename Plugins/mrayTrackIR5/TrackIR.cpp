

#include "Stdafx.h"
#include "TrackIR.h"
#include "ILogManager.h"
#include "IOSRegistry.h"
#include "IDllManager.h"
#include "StringConverter.h"

#include <windows.h>

namespace mray
{

core::string  TrackIR::GetDLLLocation()
{
	core::string dllPath;

	if(!OS::IOSRegistry::getInstance().getKeyValue(OS::ERK_Current_User,"Software\\NaturalPoint\\NATURALPOINT\\NPClient Location","Path",dllPath))
	{
		return "";
	}
	return dllPath;
}

TrackIR::TrackIR()
{
	m_isInited=false;
}
TrackIR::~TrackIR()
{
	Shutdown();
}

NPRESULT TrackIR::InitInternal()
{
	NPFrameSignature = 0;
	NPStaleFrames = 0;

	NPRESULT result;
	core::string path=GetDLLLocation()+"NPClient.dll";
	OS::IDynamicLibraryPtr lib= OS::IDllManager::getInstance().getLibrary(path);
	if(lib.isNull())
	{
		gLogManager.log("TrackIR::Init() - Couldn't open TrackIR5 library, make sure it is installed",ELL_WARNING);
		return NP_ERR_DLL_NOT_FOUND;
	}
	NP_GetSignatureDelegate = (dNP_GetSignatureDelegate)lib->getSymbolName("NP_GetSignature");

	LPTRACKIRSIGNATUREDATA lPTRACKIRSIGNATUREDATA ;
	const char DllSignature[]="precise head tracking\n put your head into the game\n now go look around\n\n Copyright EyeControl Technologies";
	const char AppSignature[]="hardware camera\n software processing data\n track user movement\n\n Copyright EyeControl Technologies";

	if (NP_GetSignatureDelegate(&lPTRACKIRSIGNATUREDATA) == NP_OK)
	{
		if (strcmp(DllSignature, lPTRACKIRSIGNATUREDATA.DllSignature) == 0 && 
			strcmp(AppSignature, lPTRACKIRSIGNATUREDATA.AppSignature) == 0)
		{
			result = NP_OK;
			NP_RegisterWindowHandle = (dNP_RegisterWindowHandle)(lib->getSymbolName( "NP_RegisterWindowHandle"));
			NP_UnregisterWindowHandle = (dNP_UnregisterWindowHandle)(lib->getSymbolName( "NP_UnregisterWindowHandle"));
			NP_RegisterProgramProfileID = (dNP_RegisterProgramProfileID)(lib->getSymbolName( "NP_RegisterProgramProfileID"));
			NP_QueryVersion = (dNP_QueryVersion)(lib->getSymbolName( "NP_QueryVersion"));
			NP_RequestData = (dNP_RequestData)(lib->getSymbolName( "NP_RequestData"));
			NP_GetData = (dNP_GetData)(lib->getSymbolName( "NP_GetData"));
			NP_StartCursor = (dNP_StartCursor)(lib->getSymbolName( "NP_StartCursor"));
			NP_StopCursor = (dNP_StopCursor)(lib->getSymbolName( "NP_StopCursor"));
			NP_ReCenter = (dNP_ReCenter)(lib->getSymbolName( "NP_ReCenter"));
			NP_StartDataTransmission = (dNP_StartDataTransmission)(lib->getSymbolName( "NP_StartDataTransmission"));
			NP_StopDataTransmission = (dNP_StopDataTransmission)(lib->getSymbolName( "NP_StopDataTransmission"));
		}
		else
		{
			result = NP_ERR_DLL_NOT_FOUND;
		}
	}
	else
	{
		result = NP_ERR_DLL_NOT_FOUND;
	}
	return result;	
}
NPRESULT TrackIR::Init(ushort programID)
{
	if(m_isInited)
		Shutdown();
	NPRESULT res;

	res=InitInternal();
	if(res!=NP_OK)
	{
		return res;
	}

	HWND forground=GetForegroundWindow();
	res=NP_RegisterWindowHandle((int)forground);
	if(res!=NP_OK)
	{
		gLogManager.log("TrackIR::Init()- failed to register window",ELL_WARNING);
		return res;
	}
	ushort ver=0;
	res=NP_QueryVersion(&ver);
	if(res!=NP_OK)
	{
		gLogManager.log("TrackIR::Init()- failed to query version",ELL_WARNING);
		return res;
	}
	core::string versionStr;
	versionStr=core::StringConverter::toString(ver>>8);
	versionStr+="."+core::StringConverter::toString(ver&255);
	gLogManager.log("TrackIR::Init()-NaturalPoint Version: "+versionStr,ELL_INFO);


	uint num2 = 0u;
	num2 |= 2u;
	num2 |= 4u;
	num2 |= 1u;
	num2 |= 16u;
	num2 |= 32u;
	num2 |= 64u;
	NP_RequestData((ushort)num2);
	NP_RegisterProgramProfileID(programID);
	res=NP_StopCursor();
	if(res!=NP_OK)
	{
		gLogManager.log("TrackIR::Init()- failed to stop cursor",ELL_WARNING);
		return res;
	}
	res=NP_StartDataTransmission();
	if(res!=NP_OK)
	{
		gLogManager.log("TrackIR::Init()- failed to start data transmission",ELL_WARNING);
		return res;
	}

	m_isInited=true;
	return NP_OK;
}


bool TrackIR::Shutdown()
{
	if(!m_isInited)
		return true;
	core::string text = "";
	if (NP_StopDataTransmission() == NP_OK)
	{
		text += "StopDataTransmission() OK\r\n";
	}
	else
	{
		text += "StopDataTransmission() ERROR!!\r\n";
	}
	if (NP_StartCursor() == NP_OK)
	{
		text += "StartCursor() OK\r\n";
	}
	else
	{
		text += "StartCursor() ERROR!!\r\n";
	}
	if (NP_UnregisterWindowHandle() == NP_OK)
	{
		text += "UnregisterWindowHandle() OK\r\n";
	}
	else
	{
		text += "UnregisterWindowHandle() ERROR!!\r\n";
	}

	gLogManager.log("TrackIR Shutdown log: "+text,ELL_INFO);
	m_isInited=false;
	return true;
}

NPRESULT TrackIR::HandleTrackIRData(LPTRACKIRDATA &result)
{
	if(!m_isInited)
		return NP_ERR_NO_DATA;
	;
	if (NP_GetData(&result) != NP_OK)
	{
		return NP_ERR_NO_DATA;
	}
	if (result.wNPStatus != 0)
	{
		return NP_OK;
	}
	if (NPFrameSignature != (ulong)result.wPFrameSignature)
	{
		NPFrameSignature = (ulong)result.wPFrameSignature;
		NPStaleFrames = 0uL;
		return NP_OK;
	}
	if (NPStaleFrames > 30uL)
	{
		return NP_OK;
	}
	NPStaleFrames += 1uL;
	return NP_OK;
}


}

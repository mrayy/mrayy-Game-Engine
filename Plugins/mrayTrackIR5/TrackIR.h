//////////////////////////////////////////////////////////////////////////
///	TrackIR5 Communication Class									 /////
//////////////////////////////////////////////////////////////////////////
///	This code is based on Unity plug in								 /////
///	This code is not to be published for public	usage				 /////
///	Please use make use of it personally							 /////
///	Email: mrayyamen@gmail.com										 /////
//////////////////////////////////////////////////////////////////////////

#ifndef ___TrackIR___
#define ___TrackIR___

#include <string>
#include "mtypes.h"
#include "mstring.h"

namespace mray
{

struct LPTRACKIRDATA
{
	ushort wNPStatus;
	ushort wPFrameSignature;
	uint dwNPIOData;
	float fNPRoll;
	float fNPPitch;
	float fNPYaw;
	float fNPX;
	float fNPY;
	float fNPZ;
	float fNPRawX;
	float fNPRawY;
	float fNPRawZ;
	float fNPDeltaX;
	float fNPDeltaY;
	float fNPDeltaZ;
	float fNPSmoothX;
	float fNPSmoothY;
	float fNPSmoothZ;
};


struct LPTRACKIRSIGNATUREDATA
{
	char DllSignature[200];
	char AppSignature[200];
};
enum NPSTATUS
{
	NPSTATUS_REMOTEACTIVE,
	NPSTATUS_REMOTEDISABLED
};
enum NPRESULT
{
	NP_OK,
	NP_ERR_DEVICE_NOT_PRESENT,
	NP_ERR_UNSUPPORTED_OS,
	NP_ERR_INVALID_ARG,
	NP_ERR_DLL_NOT_FOUND,
	NP_ERR_NO_DATA,
	NP_ERR_INTERNAL_DATA
};


class TrackIR
{
protected:
	typedef NPRESULT (_stdcall*PF_NOTIFYCALLBACK)(ushort a, ushort b);
	typedef NPRESULT (_stdcall *dNP_GetSignatureDelegate)(LPTRACKIRSIGNATUREDATA* signature);
	typedef NPRESULT (_stdcall*dNP_RegisterWindowHandle)(int hWnd);
	typedef NPRESULT (_stdcall*dNP_RegisterProgramProfileID)(ushort wPPID);
	typedef NPRESULT (_stdcall*dNP_UnregisterWindowHandle)();
	typedef NPRESULT (_stdcall*dNP_QueryVersion)( ushort *pwVersion);
	typedef NPRESULT (_stdcall*dNP_RequestData)(ushort wDataReq);
	typedef NPRESULT (_stdcall*dNP_GetData)(LPTRACKIRDATA *pTID);
	typedef NPRESULT (_stdcall*dNP_RegisterNotify)(PF_NOTIFYCALLBACK pfNotify);
	typedef NPRESULT (_stdcall*dNP_UnregisterNotify)();
	typedef NPRESULT (_stdcall*dNP_StartCursor)();
	typedef NPRESULT (_stdcall*dNP_StopCursor)();
	typedef NPRESULT (_stdcall*dNP_ReCenter)();
	typedef NPRESULT (_stdcall*dNP_StartDataTransmission)();
	typedef NPRESULT (_stdcall*dNP_StopDataTransmission)();

	dNP_GetSignatureDelegate NP_GetSignatureDelegate;
	dNP_RegisterWindowHandle NP_RegisterWindowHandle;
	dNP_UnregisterWindowHandle NP_UnregisterWindowHandle;
	dNP_RegisterProgramProfileID NP_RegisterProgramProfileID;
	dNP_QueryVersion NP_QueryVersion;
	dNP_RequestData NP_RequestData;
	dNP_GetData NP_GetData;
	dNP_UnregisterNotify NP_UnregisterNotify;
	dNP_StartCursor NP_StartCursor;
	dNP_StopCursor NP_StopCursor;
	dNP_ReCenter NP_ReCenter;
	dNP_StartDataTransmission NP_StartDataTransmission;
	dNP_StopDataTransmission NP_StopDataTransmission;

	ulong NPFrameSignature;
	ulong NPStaleFrames;

	bool m_isInited;


	core::string  GetDLLLocation();
	NPRESULT InitInternal();
public:

	TrackIR();
	virtual~TrackIR();

	bool IsInited(){return m_isInited;}
	bool Shutdown();
	NPRESULT Init(ushort programID);
	NPRESULT HandleTrackIRData(LPTRACKIRDATA &result);
};

}


#endif


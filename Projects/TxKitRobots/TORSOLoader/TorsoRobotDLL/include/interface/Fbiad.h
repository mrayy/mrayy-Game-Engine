// -----------------------------------------------------------------------
//		
//		ヘッダファイル
//
//		File Name	:	FbiAd.h
//
//										Copyright (C) 1999-2001 Interface Corp.
// -----------------------------------------------------------------------

#include "GpcAd.h"

#ifdef __cplusplus
extern	"C" {
#endif

//-----------------------------------------------------------------------------------------------
//
//		関数宣言
//
//-----------------------------------------------------------------------------------------------
HANDLE WINAPI AdOpen(LPCTSTR);
int WINAPI AdClose(HANDLE);
int WINAPI AdGetDeviceInfo(HANDLE, PADBOARDSPEC);
INT WINAPI AdSetBoardConfig(HANDLE, HANDLE, LPADCALLBACK, DWORD);
INT WINAPI AdGetBoardConfig(HANDLE, ULONG *);
int WINAPI AdSetSamplingConfig(HANDLE, PADSMPLREQ);
int WINAPI AdGetSamplingConfig(HANDLE, PADSMPLREQ);
int WINAPI AdGetSamplingData(HANDLE, PVOID, ULONG *);
int WINAPI AdClearSamplingData(HANDLE);
int WINAPI AdStartSampling(HANDLE, ULONG);
int WINAPI AdStartFileSampling(HANDLE, LPCTSTR, ULONG);
int WINAPI AdTriggerSampling(HANDLE, ULONG, ULONG, ULONG, ULONG, ULONG, ULONG);
int WINAPI AdMemTriggerSampling(HANDLE, ULONG, PADSMPLCHREQ, ULONG, ULONG, ULONG, float, ULONG, ULONG);
int WINAPI AdSyncSampling(HANDLE, ULONG);
int WINAPI AdStopSampling(HANDLE);
int WINAPI AdGetStatus(HANDLE, ULONG *, ULONG *, ULONG *);
int WINAPI AdInputAD(HANDLE, ULONG, ULONG, PADSMPLCHREQ, LPVOID);
int WINAPI AdInputDI(HANDLE, DWORD *);
int WINAPI AdOutputDO(HANDLE, DWORD);
int WINAPI AdAdjustVR(HANDLE, ULONG, ULONG, ULONG, ULONG, ULONG);

int WINAPI AdBmSetSamplingConfig(HANDLE, PADBMSMPLREQ);
int WINAPI AdBmGetSamplingConfig(HANDLE, PADBMSMPLREQ);
int WINAPI AdBmGetSamplingData(HANDLE, PVOID, ULONG *);
int WINAPI AdBmStartFileSampling(HANDLE, LPCTSTR, ULONG);

int WINAPI AdLvSetSamplingConfig(HANDLE, ULONG, ULONG, float, ULONG, HANDLE, LPADCALLBACK, DWORD);
int WINAPI AdLvGetSamplingConfig(HANDLE, ULONG, ULONG *, float *, ULONG *);
int WINAPI AdLvCalibration(HANDLE, ULONG, ULONG);
int WINAPI AdLvGetSamplingData(HANDLE, ULONG, PVOID, ULONG *);
int WINAPI AdLvStartSampling(HANDLE, ULONG);
int WINAPI AdLvStopSampling(HANDLE, ULONG);
int WINAPI AdLvGetStatus(HANDLE, ULONG, ULONG *, ULONG *, ULONG *);
int WINAPI AdMeasureTemperature(HANDLE, float *);

int WINAPI AdDataConv(UINT, PVOID, UINT, PADSMPLREQ, UINT, PVOID, PUINT, PADSMPLREQ, UINT, UINT, LPCONVPROC);
int WINAPI AdReadFile(LPCTSTR, PVOID, ULONG);

#ifdef __cplusplus
}
#endif

//#endif

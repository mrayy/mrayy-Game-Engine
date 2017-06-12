// -----------------------------------------------------------------------
//		
//		ヘッダファイル
//
//		File Name	:	FbiDa.h
//
//										Copyright (C) 1999 Interface Corp.
// -----------------------------------------------------------------------

#include "GpcDa.h"

#ifdef __cplusplus
extern	"C" {
#endif

//-----------------------------------------------------------------------------------------------
//
//		関数宣言
//
//-----------------------------------------------------------------------------------------------
HANDLE WINAPI DaOpen(LPCTSTR);
int WINAPI DaClose(HANDLE);
int WINAPI DaGetDeviceInfo(HANDLE, PDABOARDSPEC);
int WINAPI DaSetBoardConfig(HANDLE, ULONG, HANDLE, LPDACALLBACK, DWORD);
int WINAPI DaGetBoardConfig(HANDLE, ULONG *, ULONG *);
int WINAPI DaSetSamplingConfig(HANDLE, PDASMPLREQ);
int WINAPI DaGetSamplingConfig(HANDLE, PDASMPLREQ);
int WINAPI DaSetMode(HANDLE, PDAMODEREQ);
int WINAPI DaGetMode(HANDLE, PDAMODEREQ);
int WINAPI DaSetSamplingData(HANDLE, PVOID, ULONG);
int WINAPI DaClearSamplingData(HANDLE);
int WINAPI DaStartSampling(HANDLE, ULONG);
int WINAPI DaSyncSampling(HANDLE, ULONG);
int WINAPI DaStartFileSampling(HANDLE, LPCTSTR, ULONG, ULONG);
int WINAPI DaStopSampling(HANDLE);
int WINAPI DaGetStatus(HANDLE, ULONG *, ULONG *, ULONG *, ULONG *);
int WINAPI DaOutputDA(HANDLE, ULONG, PDASMPLCHREQ, LPVOID);
int WINAPI DaInputDI(HANDLE, DWORD *);
int WINAPI DaOutputDO(HANDLE, DWORD);
int WINAPI DaAdjustVR(HANDLE, ULONG, ULONG, ULONG, ULONG);

int WINAPI DaDataConv(UINT, PVOID, UINT, PDASMPLREQ, UINT, PVOID, PUINT, PDASMPLREQ, UINT, UINT, LPDACONVPROC);
int WINAPI DaWriteFile(LPCTSTR, PVOID, ULONG, ULONG, ULONG);

#ifdef __cplusplus
}
#endif

//#endif

///////////////////////////////////////////////////////////////////////////////
//
//  FbiDio .H
//
// Declare function prototypes and structures and symbols exported from the FBIDIO.DLL.
//
// Copyright 2000, 2002 Interface Corporation. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined( _FBIDIO_H_ )
#define _FBIDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------
//	Symbols and/or identifiers
// -----------------------------------------------------------------------
#define FBIDIO_RSTIN_MASK	1		// The symbols used when carrying out the mask of the RSTIN

#define FBIDIO_FLAG_NORMAL	0x0000
#define FBIDIO_FLAG_SHARE	0x0002	// The flag is applicable to the DioOpen function. This flag shows that the device is opened as shareable.

#define	FBIDIO_IN1_8	0	// Read the data from IN1 through IN8.
#define	FBIDIO_IN9_16	1	// Read the data from IN9 through IN16.
#define	FBIDIO_IN17_24	2	// Read the data from IN17 through IN24.
#define	FBIDIO_IN25_32	3	// Read the data from IN25 through IN32.
#define	FBIDIO_IN33_40	4	// Read the data from IN33 through IN40.
#define	FBIDIO_IN41_48	5	// Read the data from IN41 through IN48.
#define	FBIDIO_IN49_56	6	// Read the data from IN49 through IN56.
#define	FBIDIO_IN57_64	7	// Read the data from IN57 through IN64.


#define	FBIDIO_IN1_16	0	// Read the data from IN1 through IN16.
#define	FBIDIO_IN17_32	2	// Read the data from IN17 through IN32.
#define	FBIDIO_IN33_48	4	// Read the data from IN33 through IN48.
#define	FBIDIO_IN49_64	6	// Read the data from IN49 through IN64.

#define	FBIDIO_IN1_32	0	// Read the data from IN1 through IN32.
#define	FBIDIO_IN33_64	4	// Read the data from IN33 through IN64.

#define	FBIDIO_OUT1_8	0	// Write the data to OUT1 through OUT8.
#define	FBIDIO_OUT9_16	1	// Write the data to OUT9 through OUT16.
#define	FBIDIO_OUT17_24	2	// Write the data to OUT17 through OUT24.
#define	FBIDIO_OUT25_32	3	// Write the data to OUT25 through OUT32.
#define	FBIDIO_OUT33_40	4	// Write the data to OUT33 through OUT40.
#define	FBIDIO_OUT41_48	5	// Write the data to OUT41 through OUT48.
#define	FBIDIO_OUT49_56	6	// Write the data to OUT49 through OUT56.
#define	FBIDIO_OUT57_64	7	// Write the data to OUT57 through OUT64.


#define	FBIDIO_OUT1_16	0	// Write the data to OUT1 through OUT16.
#define	FBIDIO_OUT17_32	2	// Write the data to OUT17 through OUT32.
#define	FBIDIO_OUT33_48	4	// Write the data to OUT33 through OUT48.
#define	FBIDIO_OUT49_64	6	// Write the data to OUT49 through OUT64.

#define	FBIDIO_OUT1_32	0	// Write the data to OUT1 through OUT32.
#define	FBIDIO_OUT33_64	4	// Write the data to OUT33 through OUT64.

#define FBIDIO_STB1_ENABLE		0x01	// The STB1 event is eabled.
#define FBIDIO_STB1_HIGH_EDGE	0x10	// The rising edge for STB1 is enabled.

#define FBIDIO_ACK2_ENABLE		0x04	// The ACK2 event is enabled.
#define FBIDIO_ACK2_HIGH_EDGE	0x40	// The rising edge for ACK2 is enabled.

#define FBIDIO_WAIT_LAST_ACK2	0x100	// Complete the function by the final ACK2.

#define FBIDIO_STB2_DELAY_10US	0x000A0000	// The STB2 delay time.(10us)
#define FBIDIO_STB2_DELAY_30US	0x001E0000	// The STB2 delay time.(30us)
#define FBIDIO_STB2_DELAY_50US	0x00320000	// The STB2 delay time.(50us)
#define FBIDIO_STB2_DELAY_100US	0x00640000	// The STB2 delay time.(100us)
#define FBIDIO_STB2_DELAY_300US	0x012C0000	// The STB2 delay time.(300us)
#define FBIDIO_STB2_DELAY_500US	0x01F40000	// The STB2 delay time.(500us)
#define FBIDIO_STB2_DELAY_1MS	0x03E80000	// The STB2 delay time.(1ms)
#define FBIDIO_STB2_DELAY_3MS	0x0BB80000	// The STB2 delay time.(3ms)
#define FBIDIO_STB2_DELAY_5MS	0x13880000	// The STB2 delay time.(5ms)
#define FBIDIO_STB2_DELAY_10MS	0x27100000	// The STB2 delay time.(10ms)
#define FBIDIO_STB2_DELAY_30MS	0x75300000	// The STB2 delay time.(30ms)
#define FBIDIO_STB2_DELAY_50MS	0xC3500000	// The STB2 delay time.(50ms)

#define FBIDIO_IRIN1_2_STB1	32	// IR.IN1, IR.IN2, STB1

// -----------------------------------------------------------------------
//	Return Value
// -----------------------------------------------------------------------
#define	FBIDIO_ERROR_SUCCESS				0			// The process was successfully completed.
#define FBIDIO_ERROR_NOT_DEVICE				0xC0000001	// The specified driver cannot be called.
#define	FBIDIO_ERROR_NOT_OPEN				0xC0000002	// The specified driver cannot be opened.
#define FBIDIO_ERROR_INVALID_HANDLE			0xC0000003	// The device handle is invalid.
#define FBIDIO_ERROR_ALREADY_OPEN			0xC0000004	// The device has already been opened.
#define FBIDIO_ERROR_HANDLE_EOF				0xC0000005	// End of file is reached.
#define FBIDIO_ERROR_MORE_DATA				0xC0000006	// More available data exists.
#define FBIDIO_ERROR_INSUFFICIENT_BUFFER	0xC0000007	// The data area passed to the system call is too small.
#define FBIDIO_ERROR_IO_PENDING				0xC0000008	// Overlapped I/O operations are in progress.
#define FBIDIO_ERROR_NOT_SUPPORTED			0xC0000009	// The specified function is not supported.
#define	FBIDIO_ERROR_MEMORY_NOTALLOCATED	0xC0001000	// Not enough memory.
#define	FBIDIO_ERROR_PARAMETER				0xC0001001	// The specified parameter is invalid.
#define	FBIDIO_ERROR_INVALID_CALL			0xC0001002	// Invalid function is called.
#define	FBIDIO_ERROR_DRVCAL					0xC0001003	// Failed to call the driver.
#define	FBIDIO_ERROR_NULL_POINTER			0xC0001004	// A NULL pointer is passed between the driver and the DLL.
#define	FBIDIO_ERROR_IO_INCOMPLETE			0xC0001005	// I/O event is not signaled asynchronously.

#if !defined(_FBIDIOLIB_)
#define FBIDIOAPI
#else
#define FBIDIOAPI __declspec(dllexport)
#endif

FBIDIOAPI	HANDLE WINAPI	DioOpen( LPCTSTR lpszName, DWORD  fdwFlags );
FBIDIOAPI	INT WINAPI	DioClose( HANDLE hDeviceHandle );
FBIDIOAPI	INT WINAPI	DioInputPoint( HANDLE hDeviceHandle, PINT pBuffer, DWORD dwStartNum, DWORD dwNum );
FBIDIOAPI	INT WINAPI	DioOutputPoint( HANDLE hDeviceHandle, PINT pBuffer, DWORD dwStartNum, DWORD dwNum );
FBIDIOAPI	INT WINAPI	DioGetBackGroundUseTimer( HANDLE hDeviceHandle, PINT pnUse );
FBIDIOAPI	INT WINAPI	DioSetBackGroundUseTimer( HANDLE hDeviceHandle, INT nUse);
FBIDIOAPI	HANDLE WINAPI	DioSetBackGround( HANDLE hDeviceHandle, DWORD dwStartPoint, DWORD dwPointNum ,DWORD dwValueNum, DWORD dwCycle, DWORD dwCount, DWORD dwOption );
FBIDIOAPI	INT WINAPI	DioFreeBackGround( HANDLE hDeviceHandle, HANDLE hBackGroundHandle );
FBIDIOAPI	INT WINAPI	DioStopBackGround( HANDLE hDeviceHandle, HANDLE hBackGroundHandle );
FBIDIOAPI	INT WINAPI	DioGetBackGroundStatus( HANDLE hDeviceHandle, HANDLE hBackGroundHandle,	PINT pnStartPoint, PINT	pnPointNum,	PINT pnValueNum, PINT pnCycle, PINT	pnCount, PINT pnOption,	PINT pnExecute, PINT pnExecCount, PINT pnBufferOffset, PINT pnOver);
FBIDIOAPI	INT WINAPI	DioInputPointBack( HANDLE hDeviceHandle, HANDLE hBackGroundHandle, PINT pBuffer, DWORD nNumOfBytesToRead, LPOVERLAPPED lpOverlapped );
FBIDIOAPI	INT WINAPI	DioOutputPointBack( HANDLE hDeviceHandle, HANDLE hBackGroundHandle,	PINT pBuffer,DWORD nNumOfBytesToWrite, LPOVERLAPPED lpOverlapped	);
FBIDIOAPI	INT WINAPI	DioWatchPointBack( HANDLE hDeviceHandle, HANDLE hBackGround, PINT pBuffer, DWORD nNumOfBytesToRead,LPOVERLAPPED lpOverlapped );
FBIDIOAPI	INT WINAPI	DioGetInputHandShakeConfig( HANDLE hDeviceHandle, PINT pnInputHandShakeConfig, PDWORD pdwBitMask1, PDWORD pdwBitMask2 );
FBIDIOAPI	INT WINAPI	DioSetInputHandShakeConfig( HANDLE hDeviceHandle, INT nInputHandShakeConfig, DWORD dwBitMask1, DWORD dwBitMask2 );
FBIDIOAPI	INT WINAPI	DioGetOutputHandShakeConfig( HANDLE hDeviceHandle, PINT pnOutputHandShakeConfig, PDWORD pdwBitMask1, PDWORD pdwBitMask2 );
FBIDIOAPI	INT WINAPI	DioSetOutputHandShakeConfig( HANDLE hDeviceHandle,	INT nOutputHandShakeConfig, DWORD dwBitMask1, DWORD dwBitMask2 );
FBIDIOAPI	INT WINAPI	DioInputHandShake( HANDLE hDeviceHandle, LPVOID lpBuffer, DWORD nNumOfBytesToRead, LPDWORD lpNumOfBytesRead, LPOVERLAPPED lpOverlapped );
FBIDIOAPI	INT WINAPI	DioInputHandShakeEx( HANDLE hDeviceHandle, LPVOID lpBuffer,	DWORD nNumOfBytesToRead, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine );
FBIDIOAPI	INT WINAPI	DioOutputHandShake( HANDLE hDeviceHandle, LPVOID lpBuffer, DWORD nNumOfBytesToWrite, LPDWORD lpNumOfBytesWritten, LPOVERLAPPED lpOverlapped	);
FBIDIOAPI	INT WINAPI	DioOutputHandShakeEx( HANDLE hDeviceHandle,	LPVOID lpBuffer, DWORD nNumOfBytesToWrite, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine );
FBIDIOAPI	INT WINAPI	DioStopInputHandShake( HANDLE  hDeviceHandle );
FBIDIOAPI	INT WINAPI	DioStopOutputHandShake( HANDLE  hDeviceHandle );
FBIDIOAPI	INT WINAPI	DioGetHandShakeStatus( HANDLE hDeviceHandle, PDWORD pdwDeviceStatus, PDWORD pdwInputedBuffNum, PDWORD pdwOutputedBuffNum );
FBIDIOAPI	INT WINAPI	DioInputByte( HANDLE  hDeviceHandle, INT  nNo, PBYTE  pbValue );
FBIDIOAPI	INT WINAPI	DioInputWord( HANDLE  hDeviceHandle, INT  nNo, PWORD  pwValue );
FBIDIOAPI	INT WINAPI	DioInputDword( HANDLE  hDeviceHandle, INT  nNo, PDWORD  pdwValue );
FBIDIOAPI	INT WINAPI	DioOutputByte( HANDLE  hDeviceHandle, INT  nNo, BYTE  bValue );
FBIDIOAPI	INT WINAPI	DioOutputWord( HANDLE  hDeviceHandle, INT  nNo, WORD  wValue );
FBIDIOAPI	INT WINAPI	DioOutputDword( HANDLE  hDeviceHandle,	INT  nNo, DWORD  dwValue );
FBIDIOAPI	INT WINAPI	DioGetAckStatus( HANDLE  hDeviceHandle, PBYTE  pbAckStatus );
FBIDIOAPI	INT WINAPI	DioSetAckPulseCommand( HANDLE  hDeviceHandle, BYTE  bCommand );
FBIDIOAPI	INT WINAPI	DioGetStbStatus( HANDLE  hDeviceHandle, PBYTE  pbStbStatus );
FBIDIOAPI	INT WINAPI	DioSetStbPulseCommand( HANDLE  hDeviceHandle, BYTE  bCommand );
FBIDIOAPI	INT WINAPI	DioInputUniversalPoint( HANDLE hDeviceHandle, PDWORD pdwUniversalPoint );
FBIDIOAPI	INT WINAPI	DioOutputUniversalPoint( HANDLE hDeviceHandle, DWORD dwUniversalPoint );
FBIDIOAPI	INT WINAPI	DioSetTimeOut( HANDLE hDeviceHandle, DWORD dwInputTotalTimeout, DWORD dwInputIntervalTimeout, DWORD dwOutputTotalTimeout, DWORD dwOutputIntervalTimeout	);
FBIDIOAPI	INT WINAPI	DioGetTimeOut( HANDLE hDeviceHandle, PDWORD pdwInputTotalTimeout, PDWORD pdwInputIntervalTimeout,	PDWORD pdwOutputTotalTimeout, PDWORD pdwOutputIntervalTimeout );
FBIDIOAPI	INT WINAPI	DioSetIrqMask( HANDLE  hDeviceHandle, BYTE bIrqMask	);
FBIDIOAPI	INT WINAPI	DioGetIrqMask( HANDLE  hDeviceHandle, PBYTE pbIrqMask );
FBIDIOAPI	INT WINAPI	DioSetIrqConfig( HANDLE  hDeviceHandle,	BYTE bIrqConfig	);
FBIDIOAPI	INT WINAPI	DioGetIrqConfig( HANDLE  hDeviceHandle,	PBYTE  pbIrqConfig );
FBIDIOAPI	INT WINAPI	DioGetDeviceConfig( HANDLE hDeviceHandle, PDWORD pdwDeviceConfig );
FBIDIOAPI	INT WINAPI	DioSetTimerConfig( HANDLE hDeviceHandle, BYTE bTimerConfigValue );
FBIDIOAPI	INT WINAPI	DioGetTimerConfig( HANDLE hDeviceHandle, PBYTE pbTimerConfigValue );
FBIDIOAPI	INT WINAPI	DioGetTimerCount( HANDLE hDeviceHandle, PBYTE pbTimerCount );
FBIDIOAPI	INT WINAPI	DioSetLatchStatus( HANDLE  hDeviceHandle, BYTE bLatchStatus );
FBIDIOAPI	INT WINAPI	DioGetLatchStatus( HANDLE  hDeviceHandle, PBYTE pbLatchStatus );
FBIDIOAPI	INT WINAPI	DioGetResetInStatus( HANDLE  hDeviceHandle, PBYTE  pbResetInStatus );
FBIDIOAPI	INT WINAPI	DioEventRequestPending( HANDLE  hDeviceHandle, DWORD dwEventEnableMask, PDWORD pEventBuf, LPOVERLAPPED lpOverlapped );
FBIDIOAPI	INT WINAPI	DioGetDeviceDesc( HANDLE hDeviceHandle, PCHAR pDeviceDesc, int *pLen);
FBIDIOAPI	INT WINAPI	DioGetOverlappedResult ( HANDLE hDeviceHandle, LPOVERLAPPED lpOverlapped, LPDWORD lpNumberOfBytes, BOOL bWait);

FBIDIOAPI	INT WINAPI DioEintSetIrqMask(HANDLE hDeviceHandle,DWORD dwSetIrqMask);
FBIDIOAPI	INT WINAPI DioEintGetIrqMask(HANDLE hDeviceHandle, PDWORD pdwGetIrqMask);
FBIDIOAPI	INT WINAPI DioEintSetEdgeConfig(HANDLE hDeviceHandle, DWORD dwSetFallEdgeConfig, DWORD dwSetRiseEdgeConfig);
FBIDIOAPI	INT WINAPI DioEintGetEdgeConfig (HANDLE hDeviceHandle, PDWORD pdwGetFallEdgeConfig, PDWORD pdwGetRiseEdgeConfig);
FBIDIOAPI	INT WINAPI DioEintInputPoint(HANDLE hDeviceHandle,PINT pBuffer,DWORD dwStartNum,DWORD dwNum);
FBIDIOAPI	INT WINAPI DioEintInputByte(HANDLE hDeviceHandle,INT nNo,PBYTE pbFallValue,PBYTE pbRiseValue);
FBIDIOAPI	INT WINAPI DioEintInputWord(HANDLE hDeviceHandle,INT nNo,PWORD pwFallValue,PWORD pwRiseValue);
FBIDIOAPI	INT WINAPI DioEintInputDword (HANDLE hDeviceHandle,INT nNo,PDWORD pdwFallValue,PDWORD pdwRiseValue);
FBIDIOAPI	INT WINAPI DioEintSetFilterConfig (HANDLE hDeviceHandle,INT nNo,INT nSetFilterConfig);
FBIDIOAPI	INT WINAPI DioEintGetFilterConfig(HANDLE hDeviceHandle,INT nNo,PINT pnGetFilterConfig);
FBIDIOAPI	INT WINAPI DioGetDeviceConfigEx(HANDLE hDeviceHandle,PDWORD pdwDeviceConfig,PDWORD pdwDeviceConfigEx);
FBIDIOAPI	INT WINAPI DioEventRequestPendingEx(HANDLE hDeviceHandle,PDWORD pdwEventEnableMask,PDWORD pEventBuf,LPOVERLAPPED lpOverlapped);

FBIDIOAPI	INT WINAPI	DioCommonGetPciDeviceInfo( HANDLE  hDeviceHandle, PDWORD  pdwDeviceID, PDWORD  pdwVenderID,
								          PDWORD  pdwClassCode, PDWORD  pdwRevisionID, PDWORD  pdwBaseAddress0,
								          PDWORD  pdwBaseAddress1, PDWORD  pdwBaseAddress2, PDWORD  pdwBaseAddress3,
								          PDWORD  pdwBaseAddress4, PDWORD  pdwBaseAddress5, PDWORD  pdwSubsystemID,
								          PDWORD  pdwSubsystemVenderID, PDWORD  pdwInterruptLine, PDWORD  pdwBoardID);
FBIDIOAPI	INT WINAPI	DioSetRstinMask( HANDLE  hDeviceHandle,	DWORD dwRstinMask);
FBIDIOAPI	INT WINAPI	DioGetRstinMask( HANDLE  hDeviceHandle,	PDWORD pdwRstinMask);

FBIDIOAPI INT WINAPI DioEintSetIrqMaskEx(HANDLE DeviceHandle, INT No, DWORD SetIrqMask);
FBIDIOAPI INT WINAPI DioEintGetIrqMaskEx(HANDLE DeviceHandle, INT No, PDWORD GetIrqMask);
FBIDIOAPI INT WINAPI DioEintSetEdgeConfigEx(HANDLE DeviceHandle, INT No, DWORD SetFallEdgeConfig, DWORD SetRiseEdgeConfig);
FBIDIOAPI INT WINAPI DioEintGetEdgeConfigEx(HANDLE DeviceHandle, INT No, PDWORD GetFallEdgeConfig, PDWORD GetRiseEdgeConfig);

#ifdef __cplusplus
}
#endif

#endif

//.FBIDIO.H

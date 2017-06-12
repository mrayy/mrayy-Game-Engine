///////////////////////////////////////////////////////////////////////////////
//																		       
//  Windows NT/98/95     FbiMl.h    Include file for Memolink Driver
//				   
//	 	    内部定義ファイル
//																		       
///////////////////////////////////////////////////////////////////////////////
#if !defined( _FBIML_H_ )
#define _FBIML_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (CALLBACK MLCALLBACK)(DWORD dwMlEvent, DWORD dwUser);
typedef MLCALLBACK FAR *LPMLCALLBACK;

// -----------------------------------------------------------------------
//	オープンフラグ識別子
// -----------------------------------------------------------------------
#define	ML_FLAG_NORMAL			0x0000
#define	ML_FLAG_SHARE			0x0002

// -----------------------------------------------------------------------
//	パリティエラー識別子
// -----------------------------------------------------------------------
#define ML_PARITY_NONE			0		// パリティエラーなし
#define ML_PARITY_WRITE			1		// 書き込みパリティエラー発生
#define ML_PARITY_READ			2		// 読み込みパリティエラー発生
#define ML_PARITY_BOTH			3		// 書き込み/読み込みパリティエラー発生

// -----------------------------------------------------------------------
//	戻り値
// -----------------------------------------------------------------------
#define	ML_ERROR_SUCCESS				0			// 正常終了
#define ML_ERROR_NOT_DEVICE				0xC0000001	// 指定されたデバイスを見つけることができません
#define	ML_ERROR_NOT_OPEN				0xC0000002	// システムがデバイスをオープンできません
#define ML_ERROR_INVALID_HANDLE			0xC0000003	// デバイスハンドルが正しくありません
#define ML_ERROR_ALREADY_OPEN			0xC0000004  // すでにOPENしているデバイスをOPENしようとしました
#define ML_ERROR_HANDLE_EOF				0xC0000005	// EOFに達しました
#define ML_ERROR_MORE_DATA				0xC0000006	// さらに多くのデータが利用可能です
#define ML_ERROR_INSUFFICIENT_BUFFER	0xC0000007	// システムコールに渡されたデータ領域が小さすぎます
#define ML_ERROR_IO_PENDING				0xC0000008	// 非同期I/O操作が進行中です
#define ML_ERROR_NOT_SUPPORTED			0xC0000009	// サポートされていない機能です

#define ML_ERROR_INITIALIZE_IRQ			0xC0001000	// 割り込みの初期化に失敗しました
#define	ML_ERROR_INVALID_SLAVENUM		0xC0001001	// 不正なスレーブ番号を指定しました
#define	ML_ERROR_INVALID_OFFSET			0xC0001002	// 不正なオフセットを指定しました
#define	ML_ERROR_INVALID_VALUE			0xC0001003	// 不正な書き込み値を指定しました
#define	ML_ERROR_INVALID_SIZE			0xC0001004	// 不正なサイズを指定しました
#define	ML_ERROR_INVALID_ACCONTROL		0xC0001005	// 不正なＡＣコントロール設定値を指定しました
#define	ML_ERROR_INVALID_MASK			0xC0001006	// 不正なイベントマスクを指定しました
#define ML_ERROR_ALREADY_REGISTRATION	0xC0001007	// イベントはすでに登録済みです
#define ML_ERROR_ALREADY_DELETE			0xC0001008	// イベントはすでに削除されています
#define	ML_ERROR_MEMORY_NOTALLOCATED	0xC0001009	// 作業用メモリの確保に失敗しました	
#define	ML_ERROR_MEMORY_FREE			0xC000100A	// メモリの解放に失敗しました	
#define	ML_ERROR_TIMER					0xC000100B	// タイマリソースの取得に失敗しました
#define	ML_ERROR_DRVCAL					0xC000100C	// ドライバが呼び出せません
#define	ML_ERROR_NULL_POINTER			0xC000100D	// ドライバ、DLL間でNULLポインタが渡されました
#define ML_ERROR_PARAMETER				0xC000100E	// 引数パラメータの値が不正です


#if !defined(_FBIMLLIB_)
#define FBIMLAPI
#else
#define FBIMLAPI __declspec(dllexport)
#endif


FBIMLAPI	HANDLE WINAPI	MlOpen( LPCTSTR lpszName, DWORD  fdwFlags );
FBIMLAPI	INT WINAPI		MlClose( HANDLE hDeviceHandle );
FBIMLAPI	INT WINAPI		MlGetInformation( HANDLE hDeviceHandle, PINT pnBoardType, PINT pnRswNo );
FBIMLAPI	INT WINAPI		MlReadByte( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, PBYTE pbValue );
FBIMLAPI	INT WINAPI		MlReadWord( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, PWORD pwValue );
FBIMLAPI	INT WINAPI		MlWriteByte( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, BYTE bValue );
FBIMLAPI	INT WINAPI		MlWriteWord( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, WORD wValue );
FBIMLAPI	INT WINAPI		MlFillByte( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, BYTE bValue, DWORD dwSize );
FBIMLAPI	INT WINAPI		MlFillWord( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, WORD wValue, DWORD dwSize );
FBIMLAPI	INT WINAPI		MlReadMemoryByte( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, PVOID pDistination, DWORD dwSize );
FBIMLAPI	INT WINAPI		MlReadMemoryWord( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, PVOID pDistination, DWORD dwSize );
FBIMLAPI	INT WINAPI		MlWriteMemoryByte( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, PVOID pSource, DWORD dwSize );
FBIMLAPI	INT WINAPI		MlWriteMemoryWord( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, PVOID pSource, DWORD dwSize );
FBIMLAPI	INT WINAPI		MlCopyMemoryByte( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwSourceOffset, DWORD dwDistinationOffset, DWORD dwSize );
FBIMLAPI	INT WINAPI		MlCopyMemoryWord( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwSourceOffset, DWORD dwDistinationOffset, DWORD dwSize );
FBIMLAPI	INT WINAPI		MlSetEventMask( HANDLE hDeviceHandle, INT nEventMask );
FBIMLAPI	INT WINAPI		MlGetEventMask( HANDLE hDeviceHandle, PINT pnEventMask );
FBIMLAPI	INT WINAPI		MlSetEvent( HANDLE hDeviceHandle, LPMLCALLBACK lpEventProc, DWORD  dwUser );
FBIMLAPI	INT WINAPI		MlKillEvent( HANDLE hDeviceHandle );
FBIMLAPI	INT WINAPI		MlGenerateIrq( HANDLE hDeviceHandle, INT nSlaveNo );
FBIMLAPI	INT WINAPI		MlSetAcControl( HANDLE hDeviceHandle, INT nSlaveNo, INT nAcControl );
FBIMLAPI	INT WINAPI		MlGetAcControl( HANDLE hDeviceHandle, INT nSlaveNo, PINT pnAcControl );
FBIMLAPI	INT WINAPI		MlSetSlaveNo( HANDLE hDeviceHandle, INT nSlaveNo );
FBIMLAPI	INT WINAPI		MlGetSlaveNo( HANDLE hDeviceHandle, PINT pnSlaveNo );
FBIMLAPI	INT WINAPI		MlParityCheck( HANDLE hDeviceHandle, PINT pnParityError );

FBIMLAPI	INT WINAPI		MlSetEventEx( HANDLE hDeviceHandle, HANDLE hEvent, HWND hWnd, UINT uMsg, LPMLCALLBACK lpEventProc, DWORD dwUser );
FBIMLAPI    INT WINAPI      MlGetEventStat( HANDLE hDeviceHandle, DWORD *dwEvent );

#ifdef __cplusplus
}
#endif

#endif

//.FBIML.H

///////////////////////////////////////////////////////////////////////////////
//																		       
//  Windows NT/95     FbiEnc.h    Include file for Encorder Driver
//				   
//	 	    内部定義ファイル
//																		       
///////////////////////////////////////////////////////////////////////////////
#if !defined( _FBIENC_H_ )
#define _FBIENC_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (CALLBACK ENCCALLBACK)(DWORD dwEncEvent, DWORD dwUser);
typedef ENCCALLBACK FAR *LPENCCALLBACK;

// -----------------------------------------------------------------------
//	識別子
// -----------------------------------------------------------------------
#define MAX_COUNTER		0xffffff


// -----------------------------------------------------------------------
//	オープンフラグ
// -----------------------------------------------------------------------
#define ENC_FLAG_NORMAL	0x0000		//EncOpenに指定可能なフラグです。
#define ENC_FLAG_SHARE	0x0002		//EncOpenに指定可能なフラグです。デバイスの重複オープンを許可します。


// -----------------------------------------------------------------------
//	戻り値
// -----------------------------------------------------------------------
#define	ENC_ERROR_SUCCESS				0			// 正常終了
#define ENC_ERROR_NOT_DEVICE			0xC0000001	// 指定されたデバイスを見つけることができません
#define	ENC_ERROR_NOT_OPEN				0xC0000002	// システムがデバイスをオープンできません
#define ENC_ERROR_INVALID_HANDLE		0xC0000003	// デバイスハンドルが正しくありません
#define ENC_ERROR_ALREADY_OPEN			0xC0000004  // すでにOPENしているデバイスをOPENしようとしました
#define ENC_ERROR_HANDLE_EOF			0xC0000005	// EOFに達しました
#define ENC_ERROR_MORE_DATA				0xC0000006	// さらに多くのデータが利用可能です
#define ENC_ERROR_INSUFFICIENT_BUFFER	0xC0000007	// システムコールに渡されたデータ領域が小さすぎます
#define ENC_ERROR_IO_PENDING			0xC0000008	// 非同期I/O操作が進行中です
#define ENC_ERROR_NOT_SUPPORTED			0xC0000009	// サポートされていない機能です
#define ENC_ERROR_INITIALIZE_IRQ		0xC0001000	// 割り込みの初期化に失敗しました
#define	ENC_ERROR_INVALID_CHANNEL		0xC0001001	// 不正なチャンネル番号を指定しました
#define	ENC_ERROR_INVALID_MODE			0xC0001002	// 不正なモードを指定しました
#define	ENC_ERROR_INVALID_DIRECT		0xC0001003	// 不正なカウンタ方向を指定しました
#define	ENC_ERROR_INVALID_COUNTER		0xC0001004	// 不正なカウンタ値を指定しました
#define ENC_ERROR_INVALID_COMPARATOR	0xC0001005	// 不正な比較カウンタを指定しました
#define	ENC_ERROR_INVALID_ZMODE			0xC0001006	// 不正なＺ相論理値を指定しました
#define	ENC_ERROR_INVALID_FILTER		0xC0001007	// 不正なフィルタ値を指定しました
#define	ENC_ERROR_INVALID_MASK			0xC0001008	// 不正なイベントマスク値を指定しました
#define	ENC_ERROR_INVALID_ITIMER		0xC0001009	// 不正なインターバルタイマ設定値を指定しました
#define ENC_ERROR_ALREADY_REGISTRATION	0xC000100A	// イベントはすでに登録済みです
#define ENC_ERROR_ALREADY_DELETE		0xC000100B	// イベントはすでに削除されています
#define	ENC_ERROR_MEMORY_NOTALLOCATED	0xC000100C	// 作業用メモリの確保に失敗しました	
#define	ENC_ERROR_MEMORY_FREE			0xC000100D	// メモリの解放に失敗しました	
#define	ENC_ERROR_TIMER					0xC000100E	// タイマリソースの取得に失敗しました
#define	ENC_ERROR_DRVCAL				0xC000100F	// ドライバが呼び出せません
#define	ENC_ERROR_NULL_POINTER			0xC0001010	// ドライバ、DLL間でNULLポインタが渡されました
#define ENC_ERROR_PARAMETER				0xC0001011	// 引数パラメータの値が不正です


#if !defined(_FBIENCLIB_)
#define FBIENCAPI
#else
#define FBIENCAPI __declspec(dllexport)
#endif

FBIENCAPI	HANDLE WINAPI	EncOpen( LPCTSTR lpszName, DWORD  fdwFlags );
FBIENCAPI	INT WINAPI	EncClose( HANDLE hDeviceHandle );
FBIENCAPI	INT WINAPI	EncSetSystemMode( HANDLE hDeviceHandle, INT nChannel, INT nMode );
FBIENCAPI	INT WINAPI	EncSetMode( HANDLE hDeviceHandle, INT nChannel, INT nMode, INT nDirect, INT nPulseEvent );
FBIENCAPI	INT WINAPI	EncGetMode( HANDLE hDeviceHandle, INT nChannel, PINT pnMode, PINT pnDirect, PINT pnPulseEvent );
FBIENCAPI	INT WINAPI	EncSetCounter( HANDLE hDeviceHandle, INT nChannel, DWORD dwCounter );
FBIENCAPI	INT WINAPI	EncGetCounter( HANDLE hDeviceHandle, INT nChannel, PDWORD pdwCounter );
FBIENCAPI	INT WINAPI	EncSetCounterEx( HANDLE hDeviceHandle, INT nChannel, PDWORD pdwCounter );
FBIENCAPI	INT WINAPI	EncGetCounterEx( HANDLE hDeviceHandle, INT nChannel, PDWORD pdwCounter );
FBIENCAPI	INT WINAPI	EncSetComparator( HANDLE hDeviceHandle, INT nChannel, INT nCounter, DWORD dwComparator );
FBIENCAPI	INT WINAPI	EncGetComparator( HANDLE hDeviceHandle, INT nChannel, INT nCounter, PDWORD pdwComparator );
FBIENCAPI	INT WINAPI	EncSetZMode( HANDLE hDeviceHandle, INT nChannel, INT nZMode );
FBIENCAPI	INT WINAPI	EncGetZMode( HANDLE hDeviceHandle, INT nChannel, PINT pnZMode );
FBIENCAPI	INT WINAPI	EncSetFilter( HANDLE hDeviceHandle, INT nFilter );
FBIENCAPI	INT WINAPI	EncGetFilter( HANDLE hDeviceHandle, PINT pnFilter );
FBIENCAPI	INT WINAPI	EncSetEventMask( HANDLE hDeviceHandle, INT nChannel, INT nEventMask );
FBIENCAPI	INT WINAPI	EncGetEventMask( HANDLE hDeviceHandle, INT nChannel, PINT pnEventMask );
FBIENCAPI	INT WINAPI	EncSetEvent( HANDLE hDeviceHandle, LPENCCALLBACK lpEventProc, DWORD  dwUser );
FBIENCAPI	INT WINAPI	EncKillEvent( HANDLE hDeviceHandle );
FBIENCAPI	INT WINAPI	EncSetTimerConfig( HANDLE hDeviceHandle,	BYTE bTimerConfigValue );
FBIENCAPI	INT WINAPI	EncGetTimerConfig( HANDLE hDeviceHandle, PBYTE pbTimerConfigValue );
FBIENCAPI	INT WINAPI	EncGetTimerCount( HANDLE hDeviceHandle, PBYTE pbTimerCount );
FBIENCAPI	INT WINAPI	EncGetStatus( HANDLE hDeviceHandle, INT nChannel, PINT pnStatus );
FBIENCAPI	INT WINAPI	EncReset( HANDLE hDeviceHandle );

#ifdef __cplusplus
}
#endif

#endif

//.FBIENC.H

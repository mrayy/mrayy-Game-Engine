// -----------------------------------------------------------------------
//
//		GPC-6204	定義ファイル
//
//		FbiPenc.h
//
//		Copyright 1999-2000 Interface Corporation. All rights reserved.
// -----------------------------------------------------------------------
#include	<Winioctl.h>

#if !defined( _FBIPENC_H_ )
#define _FBIPENC_H_

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------
//	コールバック関数
//
//		LPENCCALLBACK		PencSetEvent によるｺｰﾙﾊﾞｯｸへのﾎﾟｲﾝﾀ
//		LPENCCALLBACKEX		PencSetEventEx によるｺｰﾙﾊﾞｯｸへのﾎﾟｲﾝﾀ
//
// -----------------------------------------------------------------------
typedef void (CALLBACK PENCCALLBACK)(DWORD dwPencEvent, DWORD dwUser);
typedef PENCCALLBACK *LPPENCCALLBACK;

typedef void (CALLBACK PENCCALLBACKEX)(INT nChannel,DWORD dwPencEvent,DWORD dwUser);
typedef PENCCALLBACKEX *LPPENCCALLBACKEX;

// -----------------------------------------------------------------------
//	オープンフラグ識別子
// -----------------------------------------------------------------------
#define	PENC_FLAG_NORMAL		0x0000
#define	PENC_FLAG_SHARE			0x0002

// -----------------------------------------------------------------------
//		デバイス情報
// -----------------------------------------------------------------------
typedef struct {
    USHORT  VendorID;                   // (ro)
    USHORT  DeviceID;                   // (ro)
    USHORT  Command;                    // Device control
    USHORT  Status;
    UCHAR   RevisionID;                 // (ro)
    UCHAR   ProgIf;                     // (ro)
    UCHAR   SubClass;                   // (ro)
    UCHAR   BaseClass;                  // (ro)
    UCHAR   CacheLineSize;              // (ro+)
    UCHAR   LatencyTimer;               // (ro+)
    UCHAR   HeaderType;                 // (ro)
    UCHAR   BIST;                       // Built in self test
    union {
        struct {
            ULONG   BaseAddresses[6];
            ULONG   CardBusCISPointer;
			ULONG	SubsystemVenderID;
			ULONG	SubsystemID;
            ULONG   ROMBaseAddress;
            ULONG   Reserved2[2];
            UCHAR   InterruptLine;      //
            UCHAR   InterruptPin;       // (ro)
            UCHAR   MinimumGrant;       // (ro)
            UCHAR   MaximumLatency;     // (ro)
        } type0;
    } u;
    UCHAR   DeviceSpecific[192];
} FBIPENCINFORMATION, *PFBIPENCINFORMATION;

// -----------------------------------------------------------------------
//	戻り値
// -----------------------------------------------------------------------
#define	PENC_ERROR_SUCCESS				0			// 正常終了
#define PENC_ERROR_NOT_DEVICE			0xC0000001	// 指定されたデバイスを見つけることができません
#define	PENC_ERROR_NOT_OPEN				0xC0000002	// システムがデバイスをオープンできません
#define PENC_ERROR_INVALID_HANDLE		0xC0000003	// デバイスハンドルが正しくありません
#define PENC_ERROR_ALREADY_OPEN			0xC0000004  // すでにOPENしているデバイスをOPENしようとしました
#define PENC_ERROR_HANDLE_EOF			0xC0000005	// EOFに達しました
#define PENC_ERROR_MORE_DATA			0xC0000006	// さらに多くのデータが利用可能です
#define PENC_ERROR_INSUFFICIENT_BUFFER	0xC0000007	// システムコールに渡されたデータ領域が小さすぎます
#define PENC_ERROR_IO_PENDING			0xC0000008	// 非同期I/O操作が進行中です
#define PENC_ERROR_NOT_SUPPORTED		0xC0000009	// サポートされていない機能です
#define PENC_ERROR_INITIALIZE_IRQ		0xC0001000	// 割り込みの初期化に失敗しました
#define	PENC_ERROR_INVALID_CHANNEL		0xC0001001	// 不正なチャンネル番号を指定しました
#define	PENC_ERROR_INVALID_MODE			0xC0001002	// 不正なモードを指定しました
#define	PENC_ERROR_INVALID_DIRECT		0xC0001003	// 不正なカウンタ方向を指定しました
#define	PENC_ERROR_INVALID_EQUALS		0xC0001004	// 不正な一致検出フラグを指定しました
#define	PENC_ERROR_INVALID_LATCH		0xC0001005	// 不正なラッチ設定値を指定しました
#define	PENC_ERROR_INVALID_COUNTER		0xC0001006	// 不正なカウンタ値を指定しました
#define	PENC_ERROR_INVALID_COMPARATOR	0xC0001007	// 不正な比較カウンタ値を指定しました
#define	PENC_ERROR_INVALID_ZMODE		0xC0001008	// 不正なＺ相論理設定値を指定しました
#define	PENC_ERROR_INVALID_MASK			0xC0001009	// 不正なイベントマスクを指定しました
#define	PENC_ERROR_INVALID_ITIMER		0xC000100A	// 不正なインターバルタイマ設定値を指定しました
#define PENC_ERROR_ALREADY_REGISTRATION	0xC000100B	// イベントはすでに登録済みです
#define PENC_ERROR_ALREADY_DELETE		0xC000100C	// イベントはすでに削除されています
#define	PENC_ERROR_MEMORY_NOTALLOCATED	0xC000100D	// 作業用メモリの確保に失敗しました	
#define	PENC_ERROR_MEMORY_FREE			0xC000100E	// メモリの解放に失敗しました	
#define	PENC_ERROR_TIMER				0xC000100F	// タイマリソースの取得に失敗しました
#define	PENC_ERROR_DRVCAL				0xC0001010	// ドライバが呼び出せません
#define	PENC_ERROR_NULL_POINTER			0xC0001011	// ドライバ、DLL間でNULLポインタが渡されました
#define PENC_ERROR_PARAMETER			0xC0001012	// 引数パラメータの値が不正です

#if !defined(_FBIPENCLIB_)
#define FBIPENCAPI
#else
#define FBIPENCAPI __declspec(dllexport)
#endif

FBIPENCAPI HANDLE WINAPI PencOpen(LPCTSTR lpszName,DWORD fdwFlags);
FBIPENCAPI INT WINAPI PencClose(HANDLE hDevice);
FBIPENCAPI INT WINAPI PencSetMode(HANDLE hDevice,INT nChannel,INT nMode,INT nDirection,INT nEqual,INT nLatch);
FBIPENCAPI INT WINAPI PencGetMode(HANDLE hDevice,INT nChannel,PINT pnMode,PINT pnDirection,PINT nEqual,PINT nLatch);
FBIPENCAPI INT WINAPI PencSetCounter(HANDLE hDevice,INT nChannel,DWORD dwCounter);
FBIPENCAPI INT WINAPI PencGetCounter(HANDLE hDevice,INT nChannel,PDWORD pdwCounter);
FBIPENCAPI INT WINAPI PencSetCounterEx(HANDLE hDevice,DWORD dwChSel,PDWORD pdwCounter);
FBIPENCAPI INT WINAPI PencGetCounterEx(HANDLE hDevice,DWORD dwChSel,PDWORD pdwCounter);
FBIPENCAPI INT WINAPI PencSetComparator(HANDLE hDevice,INT nChannel,DWORD dwCounter);
FBIPENCAPI INT WINAPI PencGetComparator(HANDLE hDevice,INT nChannel,PDWORD pdwCounter);
FBIPENCAPI INT WINAPI PencSetZMode(HANDLE hDevice,INT nChannel,INT nZMode);
FBIPENCAPI INT WINAPI PencGetZMode(HANDLE hDevice,INT nChannel,PINT pnZMode);
FBIPENCAPI INT WINAPI PencGetStatus(HANDLE hDevice,INT nChannel,PINT pnStatus);
FBIPENCAPI INT WINAPI PencGetStatusEx(HANDLE hDevice,DWORD dwChSel,PDWORD pdwCounter,PDWORD pdwStatus);
FBIPENCAPI INT WINAPI PencEnableCount(HANDLE hDevice,DWORD dwChSel,INT nEnable);
FBIPENCAPI INT WINAPI PencReset(HANDLE hDevice,INT nChannel);
FBIPENCAPI INT WINAPI PencSetEventMask(HANDLE hDevice,INT nChannel,INT nEventMask,INT nTimerMask);
FBIPENCAPI INT WINAPI PencGetEventMask(HANDLE hDevice,INT nChannel,PINT pnEventMask,PINT pnTimerMask);
FBIPENCAPI INT WINAPI PencSetEvent(HANDLE hDevice,LPPENCCALLBACK lpEventProc,DWORD dwUser);
FBIPENCAPI INT WINAPI PencSetEventEx(HANDLE hDevice,LPPENCCALLBACKEX lpEventProcEx,DWORD dwUser);
FBIPENCAPI INT WINAPI PencKillEvent(HANDLE hDevice);
FBIPENCAPI INT WINAPI PencEventRequestPending(HANDLE hDevice,INT nChannel,DWORD dwEventMask,PDWORD pdwEventBuf,LPOVERLAPPED lpOverlapped);
FBIPENCAPI INT WINAPI PencSetTimerConfig(HANDLE hDevice,BYTE bTimerConfig);
FBIPENCAPI INT WINAPI PencGetTimerConfig(HANDLE hDevice,PBYTE pbTimerConfig);
FBIPENCAPI INT WINAPI PencGetTimerCount(HANDLE hDevice,PBYTE pbTimerCount);

#ifdef __cplusplus
}
#endif

#endif

//.FBIPENC.H

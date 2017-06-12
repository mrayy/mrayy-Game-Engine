// -----------------------------------------------------------------------
//		
//		ヘッダファイル
//
//		File Name	:	GpcAd.h
//
//		Ver 1.02
//
//									Copyright (C) 1999-2001 Interface Corp.
// -----------------------------------------------------------------------

#if !defined( _FbiAd_H_ )
#define _FbiAd_H_

#ifdef __cplusplus
extern	"C" {
#endif

//-----------------------------------------------------------------------------------------------
//
//		同期処理識別子
//
//-----------------------------------------------------------------------------------------------
#define FLAG_SYNC	1	// 同期でサンプリング入力
#define FLAG_ASYNC	2	// 非同期でサンプリング入力

//-----------------------------------------------------------------------------------------------
//
//		ファイル形式識別子
//
//-----------------------------------------------------------------------------------------------
#define FLAG_BIN	1	// バイナリ形式ファイル
#define FLAG_CSV	2	// ＣＳＶ形式ファイル

//-----------------------------------------------------------------------------------------------
//
//		サンプリング状態識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_STATUS_STOP_SAMPLING		1	// サンプリングは停止しています
#define AD_STATUS_WAIT_TRIGGER		2	// サンプリングはトリガ待ち状態です
#define AD_STATUS_NOW_SAMPLING		3	// サンプリング動作中です

//-----------------------------------------------------------------------------------------------
//
//		イベント要因識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_EVENT_SMPLNUM			1	// 指定件数イベント
#define AD_EVENT_STOP_TRIGGER		2	// トリガによるサンプリング停止
#define AD_EVENT_STOP_FUNCTION		3	// 関数によるサンプリング停止
#define AD_EVENT_STOP_TIMEOUT		4	// TIMEOUTによるサンプリング停止
#define AD_EVENT_STOP_SAMPLING		5	// サンプリング終了
#define	AD_EVENT_STOP_SCER			6	// サンプリングクロックエラーによるサンプリング停止
#define	AD_EVENT_STOP_ORER			7	// オーバランエラーによるサンプリング停止
#define	AD_EVENT_SCER				8	// サンプリングクロックエラー
#define	AD_EVENT_ORER				9	// オーバランエラー
#define AD_EVENT_STOP_LV_1			10	// PCI-3179チャンネル１サンプリング終了
#define AD_EVENT_STOP_LV_2			11	// PCI-3179チャンネル２サンプリング終了
#define AD_EVENT_STOP_LV_3			12	// PCI-3179チャンネル３サンプリング終了
#define AD_EVENT_STOP_LV_4			13	// PCI-3179チャンネル４サンプリング終了

//-----------------------------------------------------------------------------------------------
//
//		入力仕様識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_INPUT_SINGLE				1	// シングルエンド入力
#define AD_INPUT_DIFF				2	// 差動入力

//-----------------------------------------------------------------------------------------------
//
//		調整ＶＲ識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_ADJUST_BIOFFSET			1	// バイポーラオフセット調整
#define AD_ADJUST_UNIOFFSET			2	// ユニポーラオフセット調整
#define AD_ADJUST_BIGAIN			3	// バイポーラゲイン調整
#define AD_ADJUST_UNIGAIN			4	// ユニポーラゲイン調整

//-----------------------------------------------------------------------------------------------
//
//		調整操作識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_ADJUST_UP				1	// アップ
#define AD_ADJUST_DOWN				2	// ダウン
#define AD_ADJUST_STORE				3	// ストアする
#define AD_ADJUST_STANDBY			4	// スタンバイ
#define AD_ADJUST_NOT_STORE			5	// ストアしない

//-----------------------------------------------------------------------------------------------
//
//		データ識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_DATA_PHYSICAL			1	// 物理量(電圧[V]/電流[mA])
#define AD_DATA_BIN8				2	// 8bitバイナリ
#define AD_DATA_BIN12				3	// 12bitバイナリ
#define AD_DATA_BIN16				4	// 16bitバイナリ
#define AD_DATA_BIN24				5	// 24bitバイナリ

//-----------------------------------------------------------------------------------------------
//
//		データ変換識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_CONV_SMOOTH				1	// データに対し、スムージングによる変換を行います。
#define AD_CONV_AVERAGE1		0x100	// データに対し、単純平均による変換を行います。
#define AD_CONV_AVERAGE2		0x200	// データに対し、移動平均による変換を行います。

//-----------------------------------------------------------------------------------------------
//
//		サンプリング指定識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_IO_SAMPLING				1	// I/O方式
#define AD_FIFO_SAMPLING			2	// FIFO方式
#define AD_MEM_SAMPLING				4	// メモリ方式
#define	AD_BM_SAMPLING				8	// バスマスタ方式

//-----------------------------------------------------------------------------------------------
//
//		トリガポイント指定識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_TRIG_START				1	// スタートトリガ(デフォルト)
#define AD_TRIG_STOP				2	// ストップトリガ
#define AD_TRIG_START_STOP			3	// スタートストップトリガ

//-----------------------------------------------------------------------------------------------
//
//		トリガ指定識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_FREERUN					1	// トリガなし(デフォルト)
#define AD_EXTTRG					2	// 外部トリガ
#define AD_EXTTRG_DI				3	// 外部+DIトリガ
#define AD_LEVEL_P					4	// レベルトリガプラス
#define AD_LEVEL_M					5	// レベルトリガマイナス
#define AD_LEVEL_D					6	// レベルトリガデュアル
#define AD_INRANGE					7	// レベルトリガインレンジ
#define AD_OUTRANGE					8	// レベルトリガアウトレンジ
#define AD_ETERNITY					9	// 無限サンプリング
#define AD_SMPLNUM					10	// 指定件数
#define AD_START_P1			0x00000010	// スタートトリガ レベル１立ち上がり
#define AD_START_M1			0x00000020	// スタートトリガ レベル１立ち下がり
#define AD_START_D1			0x00000040	// スタートトリガ レベル１立ち上がりまたは立ち下がり
#define AD_START_P2			0x00000080	// スタートトリガ レベル２立ち上がり
#define AD_START_M2			0x00000100	// スタートトリガ レベル２立ち下がり
#define AD_START_D2			0x00000200	// スタートトリガ レベル２立ち上がりまたは立ち下がり
#define AD_STOP_P1			0x00000400	// ストップトリガ レベル１立ち上がり
#define AD_STOP_M1			0x00000800	// ストップトリガ レベル１立ち下がり
#define AD_STOP_D1			0x00001000	// ストップトリガ レベル１立ち上がりまたは立ち下がり
#define AD_STOP_P2			0x00002000	// ストップトリガ レベル２立ち上がり
#define AD_STOP_M2			0x00004000	// ストップトリガ レベル２立ち下がり
#define AD_STOP_D2			0x00008000	// ストップトリガ レベル２立ち上がりまたは立ち下がり
#define AD_ANALOG_FILTER	0x00010000	// アナログトリガフィルタを使用する

//-----------------------------------------------------------------------------------------------
//
//		極性指定識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_DOWN_EDGE				1	// 立ち下がりエッジ(デフォルト)
#define AD_UP_EDGE					2	// 立ち上がりエッジ
#define AD_EXTRG_IN					3	// 外部トリガ入力
#define AD_EXCLK_IN					4	// 外部クロック入力

#define AD_EDGE_P1				0x0010	// レベル１立ち下がりエッジ
#define AD_EDGE_M1				0x0020	// レベル１立ち上がりエッジ
#define AD_EDGE_D1				0x0040	// レベル１レベル１立ち上がりまたは立ち下がり
#define AD_EDGE_P2				0x0080	// レベル２立ち下がりエッジ
#define AD_EDGE_M2				0x0100	// レベル２立ち上がりエッジ
#define AD_EDGE_D2				0x0200	// レベル２レベル１立ち上がりまたは立ち下がり
#define	AD_DISABLE			0x80000000	// 無効（トリガパルス発生条件）

//-----------------------------------------------------------------------------------------------
//
//		パルス極性指定識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_LOW_PULSE				1	// LOWパルス(デフォルト)
#define AD_HIGH_PULSE				2	// HIGHパルス

//-----------------------------------------------------------------------------------------------
//
//		倍速モード指定識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_NORMAL_MODE				1	// 通常モードで使用する（未対応ボード）
#define AD_FAST_MODE				2	// 倍速モードで使用する

//-----------------------------------------------------------------------------------------------
//
//		ステータス付加指定識別子（バスマスタ方式）
//
//-----------------------------------------------------------------------------------------------
#define AD_NO_STATUS				1	// ステータスなし（デフォルト）
#define AD_ADD_STATUS				2	// ステータスあり

//-----------------------------------------------------------------------------------------------
//
//		エラー制御指定識別子（バスマスタ方式）
//
//-----------------------------------------------------------------------------------------------
#define AD_STOP_SCER				2	// スキャンクロックエラーで停止
#define AD_STOP_ORER				4	// オーバランエラーで停止

//-----------------------------------------------------------------------------------------------
//
//		サンプリングデータ保存指定識別子（バスマスタ方式）
//
//-----------------------------------------------------------------------------------------------
#define AD_APPEND					1	// 追加書き込み
#define AD_OVERWRITE				2	// 上書き書き込み

//-----------------------------------------------------------------------------------------------
//
//		レンジ指定識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_0_1V				0x00000001	// 電圧　ユニポーラ0～1V
#define AD_0_2P5V			0x00000002	// 電圧　ユニポーラ0～2.5V
#define AD_0_5V				0x00000004	// 電圧　ユニポーラ0～5V
#define AD_0_10V			0x00000008	// 電圧　ユニポーラ0～10V
#define AD_1_5V				0x00000010	// 電圧　ユニポーラ1～5V
#define AD_0_2V				0x00000020	// 電圧　ユニポーラ0～2V
#define AD_0_0P125V			0x00000040	// 電圧　ユニポーラ0～0.125V
#define AD_0_1P25V			0x00000080	// 電圧　ユニポーラ0～1.25v
#define AD_0_0P625V			0x00000100	// 電圧　ユニポーラ0～0.625V
#define AD_0_20mA			0x00001000	// 電流　ユニポーラ0～20mA
#define AD_4_20mA			0x00002000	// 電流　ユニポーラ4～20mA
#define AD_1V				0x00010000	// 電圧　バイポーラ ±1V
#define AD_2P5V				0x00020000	// 電圧　バイポーラ ±2.5V
#define AD_5V				0x00040000	// 電圧　バイポーラ ±5V
#define AD_10V				0x00080000	// 電圧　バイポーラ ±10V
#define AD_0P125V			0x00400000	// 電圧　バイポーラ ±0.125V
#define AD_1P25V			0x00800000	// 電圧　バイポーラ ±1.25V
#define AD_0P625V			0x01000000	// 電圧　バイポーラ ±0.625V

//-----------------------------------------------------------------------------------------------
//
//		絶縁指定識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_ISOLATION				1	// 絶縁ボード
#define AD_NOT_ISOLATION			2	// 非絶縁ボード

//-----------------------------------------------------------------------------------------------
//
//		複数枚同期サンプリングモード指定識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_MASTER_MODE				1	// マスタモード
#define AD_SLAVE_MODE				2	// スレーブモード

//-----------------------------------------------------------------------------------------------
//
//		較正指定識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_SELF_CALIBRATION			1	// 自己較正
#define AD_ZEROSCALE_CALIBRATION	2	// ゼロスケール較正
#define AD_FULLSCALE_CALIBRATION	3	// フルスケール較正

//-----------------------------------------------------------------------------------------------
//
//		エラー識別子
//
//-----------------------------------------------------------------------------------------------
#define AD_ERROR_SUCCESS						0x00000000
#define AD_ERROR_NOT_DEVICE						0xC0000001
#define AD_ERROR_NOT_OPEN						0xC0000002
#define AD_ERROR_INVALID_HANDLE					0xC0000003
#define AD_ERROR_ALREADY_OPEN					0xC0000004
#define AD_ERROR_NOT_SUPPORTED					0xC0000009
#define AD_ERROR_NOW_SAMPLING					0xC0001001
#define AD_ERROR_STOP_SAMPLING					0xC0001002
#define AD_ERROR_START_SAMPLING					0xC0001003
#define AD_ERROR_SAMPLING_TIMEOUT				0xC0001004
#define AD_ERROR_INVALID_PARAMETER				0xC0001021
#define AD_ERROR_ILLEGAL_PARAMETER				0xC0001022
#define AD_ERROR_NULL_POINTER					0xC0001023
#define AD_ERROR_GET_DATA						0xC0001024
#define AD_ERROR_FILE_OPEN						0xC0001041
#define AD_ERROR_FILE_CLOSE						0xC0001042
#define AD_ERROR_FILE_READ						0xC0001043
#define AD_ERROR_FILE_WRITE						0xC0001044
#define AD_ERROR_INVALID_DATA_FORMAT			0xC0001061
#define AD_ERROR_INVALID_AVERAGE_OR_SMOOTHING	0xC0001062
#define AD_ERROR_INVALID_SOURCE_DATA			0xC0001063
#define AD_ERROR_NOT_ALLOCATE_MEMORY			0xC0001081
#define AD_ERROR_NOT_LOAD_DLL					0xC0001082
#define AD_ERROR_CALL_DLL						0xC0001083

// -----------------------------------------------------------------------
//
//		ユーザ関数　型指定
//
// -----------------------------------------------------------------------
typedef void (CALLBACK CONVPROC)(
	WORD wCh,		// チャンネル番号
	DWORD dwCount,	// データ件数
	LPVOID lpData	// データへのポインタ
);
typedef CONVPROC FAR *LPCONVPROC;

typedef void (CALLBACK ADCALLBACK)(DWORD dwUser);
typedef ADCALLBACK FAR *LPADCALLBACK;

// -----------------------------------------------------------------------
//	各チャンネル毎のサンプリング条件構造体
// -----------------------------------------------------------------------
typedef struct {
	ULONG			ulChNo; 
	ULONG			ulRange; 
} ADSMPLCHREQ, *PADSMPLCHREQ;

// -----------------------------------------------------------------------
//	トリガ条件構造体
// -----------------------------------------------------------------------
typedef struct {
	ULONG			ulChNo; 
	float			fTrigLevel;
	float			fHysteresis;
} ADTRIGCHREQ, *PADTRIGCHREQ;

// -----------------------------------------------------------------------
//	サンプリング条件構造体
// -----------------------------------------------------------------------
typedef struct {
	ULONG			ulChCount;
	ADSMPLCHREQ		SmplChReq[256]; 
	ULONG			ulSamplingMode; 
	ULONG			ulSingleDiff; 
	ULONG			ulSmplNum; 
	ULONG			ulSmplEventNum;
	float			fSmplFreq; 
	ULONG			ulTrigPoint;
	ULONG			ulTrigMode; 
	LONG			lTrigDelay; 
	ULONG			ulTrigCh; 
	float			fTrigLevel1; 
	float			fTrigLevel2; 
	ULONG			ulEClkEdge; 
	ULONG			ulATrgPulse; 
	ULONG			ulTrigEdge; 
	ULONG			ulTrigDI; 
	ULONG			ulFastMode; 
} ADSMPLREQ, *PADSMPLREQ;

// -----------------------------------------------------------------------
//	バスマスタ方式サンプリング条件構造体
// -----------------------------------------------------------------------
typedef struct {
	ULONG			ulChCount;
	ADSMPLCHREQ		SmplChReq[256];
	ULONG			ulSingleDiff;
	ULONG			ulSmplNum;
	ULONG			ulSmplEventNum;
	ULONG			ulSmplRepeat;
	ULONG			ulBufferMode;
	float			fSmplFreq;
	float			fScanFreq;
	ULONG			ulStartMode;
	ULONG			ulStopMode;
	ULONG			ulPreTrigDelay;
	ULONG			ulPostTrigDelay;
	ADTRIGCHREQ		TrigChReq[2];
	ULONG			ulATrgMode;
	ULONG			ulATrgPulse;
	ULONG			ulStartTrigEdge;
	ULONG			ulStopTrigEdge;
	ULONG			ulTrigDI;
	ULONG			ulEClkEdge;
	ULONG			ulFastMode; 
	ULONG			ulStatusMode;
	ULONG			ulErrCtrl;
} ADBMSMPLREQ, *PADBMSMPLREQ;

// -----------------------------------------------------------------------
//	ボード仕様構造体
// -----------------------------------------------------------------------
typedef struct {
	ULONG			ulBoardType; 
	ULONG			ulBoardID; 
	DWORD			dwSamplingMode; 
	ULONG			ulChCountS; 
	ULONG			ulChCountD; 
	ULONG			ulResolution; 
	DWORD			dwRange;	
	ULONG			ulIsolation; 
	ULONG			ulDi; 
	ULONG			ulDo; 
} ADBOARDSPEC, *PADBOARDSPEC;

#ifdef __cplusplus
}
#endif

#endif

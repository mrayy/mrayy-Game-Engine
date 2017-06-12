#ifndef		_TEXARTNCORDLIB_H_
#define		_TEXARTNCORDLIB_H_
//*******************************************/
//*    include								*/
//*******************************************/
#include	<windows.h>
#include	<stdio.h>
#ifndef	LIBLARY
#include	"TexARTNCorddriverif.h"
#endif

//*******************************************/
//*    type									*/
//*******************************************/
//*******************************************/
//*    define								*/
//*******************************************/
//関数戻り値
#define	NCORD_NORMAL		(0)		//正常終了
#define	NCORD_ERR_PARAM		(-100)	//パラメータエラー
#define	NCORD_ERR_COMMAND	(-101)	//コマンド送受信エラー
#define	NCORD_ERR_GETLOG	(-102)	//ログ情報受信エラー
#define	NCORD_ERR_REG_WRITE	(-103)	//レジスタライトエラー
#define	NCORD_ERR_REG_READ	(-104)	//レジスタリードエラー

#define	NCord_DEVERR		(-1)	// デバイスハンドルが異常
#define	NCord_PARAERR1		(-10)	// パラメータファイルがみつからない
//#define	NCord_PARAERR2		(-11)	// パラメータファイルの内容が不正
#define	NCord_GAINERR		(-12)	// 比例ゲインの送信エラー
#define	NCord_STARTERR		(-13)	// システムの起動失敗
#define	NCord_DRVERR		(-14)	// ドライバコマンドの送信エラー
#define	NCord_VALUEERR		(-15)	// コマンドに対する応答受信エラー
#define	NCord_RSTERR		(-16)	// リセットエラー
#define	NCord_CNTERR		(-17)	// カウンタ設定エラー
#define	NCord_FRERR			(-19)	// 待機状態遷移失敗

// モータドライバ種別 '09.09.18
#define		NCORD_MOTER_TYTPE01	0x01			// Hibot製 1AxisDCPower  Module （10A Module） 
#define		NCORD_MOTER_TYTPE02	0x02			// Hibot製 3Axes DC Power Module （5A×3 Module）
#define		NCORD_MOTER_TYTPE80	0x80			// Allegro製 A3995 Driver （Volt Reference Control）

#define		NCORD_RSLT_OK		0x0000			// 正常終了
#define		NCORD_RSLT_NG		0x0001			// 異常終了

#define		NCORD_DRV_CNT		16				// コマンドレスポンス 構造体で使用
#define		NCORD_RSV1			3				// コマンドレスポンス 構造体で使用
#define		NCORD_AD_CNT		24				// コマンドレスポンス 構造体で使用
#define		NCORD_NUM_CNT		4				// コマンドレスポンス 構造体で使用
//*******************************************/
//*    struct								*/
//*******************************************/
typedef	struct
{
	unsigned short		cmnd;				// コマンド
	unsigned short		trm;				// ターミナル番号
	unsigned short		cnt;				// リセットするカウンタ番号(カウンタリセットコマンド時使用)
//	unsigned long		val;				// カウンタに設定する値(カウンタリセットコマンド時使用)
	unsigned short		valL;				// カウンタに設定する値(カウンタリセットコマンド時使用)
	unsigned short		valH;				// カウンタに設定する値(カウンタリセットコマンド時使用)
	unsigned short		rsv1[NCORD_RSV1];	// 空き
	unsigned short		drv[NCORD_DRV_CNT];	// モータドライバXに対するドライバコマンド又はゲイン値
	unsigned char		moter[NCORD_DRV_CNT];	// モータドライバ0～Fに対するモータドライバ種別(1Byte)。
}NCORD_CMND,*PNCORD_CMND;
typedef	struct
{
	unsigned short		resp;				// レスポンス
	unsigned short		trm;				// ターミナル番号
	unsigned short		rslt;				// 結果
	unsigned short		cnt_bit;			// カウンタ値上位4ビット ×4カウンタ分 '09.09.24 M.S
	unsigned short		cnt[NCORD_NUM_CNT];	// モータドライバJA1～JA4のみで使用するカウンタ値
	unsigned short		ad[NCORD_AD_CNT];	// モータドライバJA1～JA7/JH1～JH16で使用するポテンショメータAD値(16ch分)
											// モータドライバJH1～JH16で使用する力センサAD値(8ch分)
}NCORD_RESP,*PNCORD_RESP;
typedef union
{
	NCORD_CMND	cmnd;
	NCORD_RESP	resp;
}NCORD_CMND_RESP,*PNCORD_CMND_RESP;
//*******************************************/
//*    function								*/
//*******************************************/
#ifdef __cplusplus
extern "C" {
#endif
extern	HANDLE
NCord_OpenDevice(
	int						nDevNum
);
extern	void
NCord_CloseDevice(
	HANDLE					hDev
);
extern	int
NCord_Command(
	HANDLE					hDev,
	BYTE					*inBuffer,
	int						insize,
	BYTE					*outbuffer,
	int						outsize,
	unsigned long			*recv,
	int						flag
);
extern	int
NCord_GetLog(
	HANDLE					hDev,
	BYTE					*buffer,
	int						size
);
extern	int
NCord_WriteRegister(
	HANDLE					hDev,
	DWORD					offset,
	DWORD					value
);
extern	int
NCord_ReadRegister(
	HANDLE					hDev,
	DWORD					offset,
	DWORD*					pvalue
);

#ifdef __cplusplus
}
#endif

#endif

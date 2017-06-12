/*******************************************************************************

	Project        : TexART NCord-System

	Component      : NCord-System PCI Driver

	Module         : TexARTNCorddriverif.h

	Function       : interface header

-------------------------------------------------------------------------------
	[history]
	2009-06-01	Ver 1.00   M.Shiroma    新規作成

******************************************************************************/
#ifndef		_TEXARTNCORDDRIVERIF_H_				//２重定義禁止
#define		_TEXARTNCORDDRIVERIF_H_
//*******************************************/
//*    include								*/
//*******************************************/
#ifndef	DRIVER
#include	<windows.h>
#include	<winioctl.h>
#endif
//*******************************************/
//*    define								*/
//*******************************************/
// デバイス名
#define		NCORD_DEVICE_NAME		"\\\\.\\NCord-%d"
// データサイズ
#define		NCORD_DATA_SIZE			(32)			// 転送データサイズ
#define		MAX_TERMINAL			(8)				// 最大ターミナル数

// 同期カウントログ用
#define		LOG_AREA_SIZE			(64*1024)		// LOG Area Size(128Kbyte)

// コマンド及びレスポンス
#define		NCORD_CMND_GAIN			(0x0001)		// 比例ゲインコマンド(モータ制御用パラメータ)
#define		NCORD_CMND_DRV			(0x0002)		// ドライバコマンド
#define		NCORD_CMND_TRM_RST		(0x0003)		// ターミナルリセットコマンド
#define		NCORD_CMND_CNT_RST		(0x0004)		// カウンタリセットコマンド
#define		NCORD_CMND_TRM_SRT		(0x0010)		// ターミナルスタートコマンド
#define		NCORD_CMND_TRM_STP		(0x0020)		// ターミナルストップコマンド
#define		NCORD_RESP_GAIN			(0x8001)		// 比例ゲインレスポンス
#define		NCORD_RESP_DRV			(0x8002)		// ドライバレスポンス
#define		NCORD_RESP_TRM_RST		(0x8003)		// ターミナルリセットレスポンス
#define		NCORD_RESP_CNT_RST		(0x8004)		// カウンタリセットレスポンス
#define		NCORD_RESP_TRM_SRT		(0x8010)		// ターミナルスタートレスポンス
#define		NCORD_RESP_TRM_STP		(0x8020)		// ターミナルストップレスポンス

#define		BIT_CYCLE_INTR			(0x10000000)	// 周期割込み用ビット
#define		BIT_RECV_COMPLETE		(0x00000100)	// 受信割込み全コントローラー完了
#define		BIT_RECV_CH1			(0x00000001)	// CH1 受信完了
#define		BIT_RECV_CH2			(0x00000002)	// CH2 受信完了
#define		BIT_RECV_CH3			(0x00000004)	// CH3 受信完了
#define		BIT_RECV_CH4			(0x00000008)	// CH4 受信完了
#define		BIT_RECV_CH5			(0x00000010)	// CH5 受信完了
#define		BIT_RECV_CH6			(0x00000020)	// CH6 受信完了
#define		BIT_RECV_CH7			(0x00000040)	// CH7 受信完了
#define		BIT_RECV_CH8			(0x00000080)	// CH8 受信完了
#define		BIT_CH1_CRC_ERR			(0x00001000)	// CH1 CRC ERROR発生
#define		BIT_CH2_CRC_ERR			(0x00002000)	// CH2 CRC ERROR発生
#define		BIT_CH3_CRC_ERR			(0x00004000)	// CH3 CRC ERROR発生
#define		BIT_CH4_CRC_ERR			(0x00008000)	// CH4 CRC ERROR発生
#define		BIT_CH5_CRC_ERR			(0x00010000)	// CH5 CRC ERROR発生
#define		BIT_CH6_CRC_ERR			(0x00020000)	// CH6 CRC ERROR発生
#define		BIT_CH7_CRC_ERR			(0x00040000)	// CH7 CRC ERROR発生
#define		BIT_CH8_CRC_ERR			(0x00080000)	// CH8 CRC ERROR発生
#define		BIT_CH1_OVRFLOW			(0x00100000)	// CH1 オーバーフロー発生
#define		BIT_CH2_OVRFLOW			(0x00200000)	// CH2 オーバーフロー発生
#define		BIT_CH3_OVRFLOW			(0x00400000)	// CH3 オーバーフロー発生
#define		BIT_CH4_OVRFLOW			(0x00800000)	// CH4 オーバーフロー発生
#define		BIT_CH5_OVRFLOW			(0x01000000)	// CH5 オーバーフロー発生
#define		BIT_CH6_OVRFLOW			(0x02000000)	// CH6 オーバーフロー発生
#define		BIT_CH7_OVRFLOW			(0x04000000)	// CH7 オーバーフロー発生
#define		BIT_CH8_OVRFLOW			(0x08000000)	// CH8 オーバーフロー発生
//*******************************************/
//*    struct								*/
//*******************************************/
// 同期カウントログ用 M.S
typedef	struct	_CYCLE_COUNTER_LOG
{
	USHORT				Location;			// Log書込み位置
	USHORT				Log[LOG_AREA_SIZE];	// Logバッファ
}
CYCLE_COUNTER_LOG,*PCYCLE_COUNTER_LOG;
typedef	struct	_CYCLE_COUNTER
{
	USHORT				Counter;
	CYCLE_COUNTER_LOG	CountLog;
}
CYCLE_COUNTER,*PCYCLE_COUNTER;

typedef	struct
{
	ULONG			value;				// データ
	ULONG			offset;				// offset
#define		REG_SEND_CH1_MEM		(0x0000)	// Ch1(RW) 送信メモリ開始オフセット (初期値0x00でfill)
#define		REG_RECV_CH1_MEM		(0x0040)	// Ch1(R ) 受信メモリ開始オフセット (初期値0x00でfill)
#define		REG_SEND_CH2_MEM		(0x0080)	// Ch2(RW) 送信メモリ開始オフセット (初期値0x00でfill)
#define		REG_RECV_CH2_MEM		(0x00C0)	// Ch2(R ) 受信メモリ開始オフセット (初期値0x00でfill)
#define		REG_SEND_CH3_MEM		(0x0100)	// Ch3(RW) 送信メモリ開始オフセット (初期値0x00でfill)
#define		REG_RECV_CH3_MEM		(0x0140)	// Ch3(R ) 受信メモリ開始オフセット (初期値0x00でfill)
#define		REG_SEND_CH4_MEM		(0x0180)	// Ch4(RW) 送信メモリ開始オフセット (初期値0x00でfill)
#define		REG_RECV_CH4_MEM		(0x01C0)	// Ch4(R ) 受信メモリ開始オフセット (初期値0x00でfill)
#define		REG_SEND_CH5_MEM		(0x0200)	// Ch1(RW) 送信メモリ開始オフセット (初期値0x00でfill)
#define		REG_RECV_CH5_MEM		(0x0240)	// Ch1(R ) 受信メモリ開始オフセット (初期値0x00でfill)
#define		REG_SEND_CH6_MEM		(0x0280)	// Ch2(RW) 送信メモリ開始オフセット (初期値0x00でfill)
#define		REG_RECV_CH6_MEM		(0x02C0)	// Ch2(R ) 受信メモリ開始オフセット (初期値0x00でfill)
#define		REG_SEND_CH7_MEM		(0x0300)	// Ch3(RW) 送信メモリ開始オフセット (初期値0x00でfill)
#define		REG_RECV_CH7_MEM		(0x0340)	// Ch3(R ) 受信メモリ開始オフセット (初期値0x00でfill)
#define		REG_SEND_CH8_MEM		(0x0380)	// Ch4(RW) 送信メモリ開始オフセット (初期値0x00でfill)
#define		REG_RECV_CH8_MEM		(0x03C0)	// Ch4(R ) 受信メモリ開始オフセット (初期値0x00でfill)
#define			OFFSET_MEM_NEXT			(0x0080)	// Next Ch Offset
#define		REG_SEND_CTRL			(0x0800)	// 送信制御レジスタ
#define			BIT_CH1_SEND			(0x01)		// CH1 送信開始指示 (W) 1:送信開始 0:送信停止 / (R) 1:送信中
#define			BIT_CH2_SEND			(0x02)		// CH2 送信開始指示 (W) 1:送信開始 0:送信停止 / (R) 1:送信中
#define			BIT_CH3_SEND			(0x04)		// CH3 送信開始指示 (W) 1:送信開始 0:送信停止 / (R) 1:送信中
#define			BIT_CH4_SEND			(0x08)		// CH4 送信開始指示 (W) 1:送信開始 0:送信停止 / (R) 1:送信中
#define			BIT_CH5_SEND			(0x10)		// CH5 送信開始指示 (W) 1:送信開始 0:送信停止 / (R) 1:送信中
#define			BIT_CH6_SEND			(0x20)		// CH6 送信開始指示 (W) 1:送信開始 0:送信停止 / (R) 1:送信中
#define			BIT_CH7_SEND			(0x40)		// CH7 送信開始指示 (W) 1:送信開始 0:送信停止 / (R) 1:送信中
#define			BIT_CH8_SEND			(0x80)		// CH8 送信開始指示 (W) 1:送信開始 0:送信停止 / (R) 1:送信中
#define		REG_RECV_STATUS			(0x0840)	// 受信ステータスレジスタ
#define			BIT_CH1_DTRDY			(0x00000001)	// 1:CH1 受信データ準備完了 '1'ライトで受信割り込みクリア
#define			BIT_CH1_CRCERR			(0x00000002)	// 1:CH1 受信データCRC ERROR発生 '1'ライトでエラークリア
#define			BIT_CH1_OVERR			(0x00000004)	// 1:CH1 受信データオーバーフロー ERROR発生 '1'ライトでエラークリア
#define			BIT_CH2_DTRDY			(0x00000010)	// 1:CH2 受信データ準備完了 '1'ライトで受信割り込みクリア
#define			BIT_CH2_CRCERR			(0x00000020)	// 1:CH2 受信データCRC ERROR発生 '1'ライトでエラークリア
#define			BIT_CH2_OVERR			(0x00000040)	// 1:CH2 受信データオーバーフロー ERROR発生 '1'ライトでエラークリア
#define			BIT_CH3_DTRDY			(0x00000100)	// 1:CH3 受信データ準備完了 '1'ライトで受信割り込みクリア
#define			BIT_CH3_CRCERR			(0x00000200)	// 1:CH3 受信データCRC ERROR発生 '1'ライトでエラークリア
#define			BIT_CH3_OVERR			(0x00000400)	// 1:CH3 受信データオーバーフロー ERROR発生 '1'ライトでエラークリア
#define			BIT_CH4_DTRDY			(0x00001000)	// 1:CH4 受信データ準備完了 '1'ライトで受信割り込みクリア
#define			BIT_CH4_CRCERR			(0x00002000)	// 1:CH4 受信データCRC ERROR発生 '1'ライトでエラークリア
#define			BIT_CH4_OVERR			(0x00004000)	// 1:CH4 受信データオーバーフロー ERROR発生 '1'ライトでエラークリア
#define			BIT_CH5_DTRDY			(0x00010000)	// 1:CH5 受信データ準備完了 '1'ライトで受信割り込みクリア
#define			BIT_CH5_CRCERR			(0x00020000)	// 1:CH5 受信データCRC ERROR発生 '1'ライトでエラークリア
#define			BIT_CH5_OVERR			(0x00040000)	// 1:CH5 受信データオーバーフロー ERROR発生 '1'ライトでエラークリア
#define			BIT_CH6_DTRDY			(0x00100000)	// 1:CH6 受信データ準備完了 '1'ライトで受信割り込みクリア
#define			BIT_CH6_CRCERR			(0x00200000)	// 1:CH6 受信データCRC ERROR発生 '1'ライトでエラークリア
#define			BIT_CH6_OVERR			(0x00400000)	// 1:CH6 受信データオーバーフロー ERROR発生 '1'ライトでエラークリア
#define			BIT_CH7_DTRDY			(0x01000000)	// 1:CH7 受信データ準備完了 '1'ライトで受信割り込みクリア
#define			BIT_CH7_CRCERR			(0x02000000)	// 1:CH7 受信データCRC ERROR発生 '1'ライトでエラークリア
#define			BIT_CH7_OVERR			(0x04000000)	// 1:CH7 受信データオーバーフロー ERROR発生 '1'ライトでエラークリア
#define			BIT_CH8_DTRDY			(0x10000000)	// 1:CH8 受信データ準備完了 '1'ライトで受信割り込みクリア
#define			BIT_CH8_CRCERR			(0x20000000)	// 1:CH8 受信データCRC ERROR発生 '1'ライトでエラークリア
#define			BIT_CH8_OVERR			(0x40000000)	// 1:CH8 受信データオーバーフロー ERROR発生 '1'ライトでエラークリア
#define		REG_READ_END			(0x0844)	// 読み取り完了レジスタ
#define			BIT_CH1_RDEND			(0x0001)	// 1:CH1受信データ読み取り完了
#define			BIT_CH2_RDEND			(0x0002)	// 1:CH2受信データ読み取り完了
#define			BIT_CH3_RDEND			(0x0004)	// 1:CH3受信データ読み取り完了
#define			BIT_CH4_RDEND			(0x0008)	// 1:CH4受信データ読み取り完了
#define			BIT_CH5_RDEND			(0x0010)	// 1:CH5受信データ読み取り完了
#define			BIT_CH6_RDEND			(0x0020)	// 1:CH6受信データ読み取り完了
#define			BIT_CH7_RDEND			(0x0040)	// 1:CH7受信データ読み取り完了
#define			BIT_CH8_RDEND			(0x0080)	// 1:CH8受信データ読み取り完了
#define		REG_ALARM_CH1			(0x0850)	// Ch1 アラームレジスタ
#define		REG_ALARM_CH2			(0x0854)	// Ch2 アラームレジスタ
#define		REG_ALARM_CH3			(0x0858)	// Ch3 アラームレジスタ
#define		REG_ALARM_CH4			(0x085C)	// Ch4 アラームレジスタ
#define		REG_ALARM_CH5			(0x0860)	// Ch5 アラームレジスタ
#define		REG_ALARM_CH6			(0x0864)	// Ch6 アラームレジスタ
#define		REG_ALARM_CH7			(0x0868)	// Ch7 アラームレジスタ
#define		REG_ALARM_CH8			(0x086C)	// Ch8 アラームレジスタ
#define			BIT_MTALM1				(0x00000001)	// 1:モータ1でアラーム発生
#define			BIT_MTALM2				(0x00000002)	// 1:モータ2でアラーム発生
#define			BIT_MTALM3				(0x00000004)	// 1:モータ3でアラーム発生
#define			BIT_MTALM4				(0x00000008)	// 1:モータ4でアラーム発生
#define			BIT_MTALM5				(0x00000010)	// 1:モータ5でアラーム発生
#define			BIT_MTALM6				(0x00000020)	// 1:モータ6でアラーム発生
#define			BIT_MTALM7				(0x00000040)	// 1:モータ7でアラーム発生
#define			BIT_MTALM8				(0x00000080)	// 1:モータ8でアラーム発生
#define			BIT_MTALM9				(0x00000100)	// 1:モータ9でアラーム発生
#define			BIT_MTALM10				(0x00000200)	// 1:モータ10でアラーム発生
#define			BIT_MTALM11				(0x00000400)	// 1:モータ11でアラーム発生
#define			BIT_MTALM12				(0x00000800)	// 1:モータ12でアラーム発生
#define			BIT_MTALM13				(0x00001000)	// 1:モータ13でアラーム発生
#define			BIT_MTALM14				(0x00002000)	// 1:モータ14でアラーム発生
#define			BIT_MTALM15				(0x00004000)	// 1:モータ15でアラーム発生
#define			BIT_MTALM16				(0x00008000)	// 1:モータ16でアラーム発生
#define			BIT_TMNL_CRCERR			(0x00010000)	// 1:Terminal側で受信CRC ERROR発生
#define		REG_TIMER_SET			(0x0880)	// タイマレジスタ 100μsタイマのカウントアップ時間を設定
#define			BIT_TIMER_STOP			(0x0000)	// タイマ停止
#define			BIT_TIMER_100			(0x0001)		// 100μs 0x0002:200μs 0x0003:300μs 0xFFFF:6553500μs
#define		REG_TIMER_STATUS		(0x0884)	// タイマー割り込みステータスレジスタ
#define			BIT_TIMINTON			(0x0001)	// 1:タイマ割り込みクリア
#define		REG_INTR_STATUS			(0x0900)	//割り込みステータスレジスタ
#define			BIT_CH1_TXINT			(0x00000001)	// 1:CH1データ送信終了割り込み
#define			BIT_CH2_TXINT			(0x00000002)	// 1:CH2データ送信終了割り込み
#define			BIT_CH3_TXINT			(0x00000004)	// 1:CH3データ送信終了割り込み
#define			BIT_CH4_TXINT			(0x00000008)	// 1:CH4データ送信終了割り込み
#define			BIT_CH5_TXINT			(0x00000010)	// 1:CH5データ送信終了割り込み
#define			BIT_CH6_TXINT			(0x00000020)	// 1:CH6データ送信終了割り込み
#define			BIT_CH7_TXINT			(0x00000040)	// 1:CH7データ送信終了割り込み
#define			BIT_CH8_TXINT			(0x00000080)	// 1:CH8データ送信終了割り込み
#define			BIT_CH1_RXINT			(0x00000100)	// 1:CH1データ受信終了割り込み
#define			BIT_CH2_RXINT			(0x00000200)	// 1:CH2データ受信終了割り込み
#define			BIT_CH3_RXINT			(0x00000400)	// 1:CH3データ受信終了割り込み
#define			BIT_CH4_RXINT			(0x00000800)	// 1:CH4データ受信終了割り込み
#define			BIT_CH5_RXINT			(0x00001000)	// 1:CH5データ受信終了割り込み
#define			BIT_CH6_RXINT			(0x00002000)	// 1:CH6データ受信終了割り込み
#define			BIT_CH7_RXINT			(0x00004000)	// 1:CH7データ受信終了割り込み
#define			BIT_CH8_RXINT			(0x00008000)	// 1:CH8データ受信終了割り込み
#define			BIT_TIMINT				(0x80000000)	// 1:タイマ割り込み
#define		REG_INTR_MASK			(0x0904)	// 割り込みマスクレジスタ
#define			BIT_CH1_TXINTMSK		(0x00000001)	// 0:CH1データ送信終了割り込み有効 1:CH1データ送信終了割り込みマスク
#define			BIT_CH2_TXINTMSK		(0x00000002)	// 0:CH2データ送信終了割り込み有効 1:CH2データ送信終了割り込みマスク
#define			BIT_CH3_TXINTMSK		(0x00000004)	// 0:CH3データ送信終了割り込み有効 1:CH3データ送信終了割り込みマスク
#define			BIT_CH4_TXINTMSK		(0x00000008)	// 0:CH4データ送信終了割り込み有効 1:CH3データ送信終了割り込みマスク
#define			BIT_CH5_TXINTMSK		(0x00000010)	// 0:CH5データ送信終了割り込み有効 1:CH1データ送信終了割り込みマスク
#define			BIT_CH6_TXINTMSK		(0x00000020)	// 0:CH6データ送信終了割り込み有効 1:CH2データ送信終了割り込みマスク
#define			BIT_CH7_TXINTMSK		(0x00000040)	// 0:CH7データ送信終了割り込み有効 1:CH3データ送信終了割り込みマスク
#define			BIT_CH8_TXINTMSK		(0x00000080)	// 0:CH8データ送信終了割り込み有効 1:CH3データ送信終了割り込みマスク
#define			BIT_CH1_RXINTMSK		(0x00000100)	// 0:CH1データ受信終了割り込み有効 1:CH1データ受信終了割り込みマスク
#define			BIT_CH2_RXINTMSK		(0x00000200)	// 0:CH2データ受信終了割り込み有効 1:CH2データ受信終了割り込みマスク
#define			BIT_CH3_RXINTMSK		(0x00000400)	// 0:CH3データ受信終了割り込み有効 1:CH3データ受信終了割り込みマスク
#define			BIT_CH4_RXINTMSK		(0x00000800)	// 0:CH4データ受信終了割り込み有効 1:CH3データ受信終了割り込みマスク
#define			BIT_CH5_RXINTMSK		(0x00001000)	// 0:CH5データ受信終了割り込み有効 1:CH1データ受信終了割り込みマスク
#define			BIT_CH6_RXINTMSK		(0x00002000)	// 0:CH6データ受信終了割り込み有効 1:CH2データ受信終了割り込みマスク
#define			BIT_CH7_RXINTMSK		(0x00004000)	// 0:CH7データ受信終了割り込み有効 1:CH3データ受信終了割り込みマスク
#define			BIT_CH8_RXINTMSK		(0x00008000)	// 0:CH8データ受信終了割り込み有効 1:CH3データ受信終了割り込みマスク
#define			BIT_TIMINTMSK			(0x80000000)	// 0:タイマ割り込み有効 1タイマ割り込みマスク
#define		REG_DIPSWITCH			(0x0A00)	// DIP SWITCHレジスタ
#define			BIT_DIP1				(0x0001)	// DIP SW1 1: ON 0:OFF
#define			BIT_DIP2				(0x0002)	// DIP SW2 1: ON 0:OFF
#define			BIT_DIP3				(0x0004)	// DIP SW3 1: ON 0:OFF
#define			BIT_DIP4				(0x0008)	// DIP SW4 1: ON 0:OFF
#define			BIT_DIP5				(0x0010)	// DIP SW5 1: ON 0:OFF
#define			BIT_DIP6				(0x0020)	// DIP SW6 1: ON 0:OFF
#define			BIT_DIP7				(0x0040)	// DIP SW7 1: ON 0:OFF
#define			BIT_DIP8				(0x0080)	// DIP SW8 1: ON 0:OFF
#define		REG_LED					(0x0A04)	// LEDレジスタ
#define		REG_DUMMY_COM			(0x0FF0)	// ダミー通信レジスタ
#define		REG_RESET				(0x0FFC)	// RESETレジスタ
}
NCORD_REG;

typedef	struct
{
	USHORT				CmndResp[NCORD_DATA_SIZE];
}
COMMAND_RESP,*PCOMMAND_RESP;
typedef	struct
{
	USHORT			flag;							// 周期タイマーに同期する場合にTRUEを設定
	USHORT			terminal_count;					// コマンドレスポンス領域の個数
	COMMAND_RESP	cmnd_resp[MAX_TERMINAL];		// コマンドレスポンス領域
}
NCORD_COMMAND,*PNCORD_COMMAND;
//*******************************************/
//*    control code							*/
//*******************************************/
#define		DEV_TYPE				430226
//IOCTLのベース番号
#define		NCORD_IOCTL_BASE	0x1000
#define	IOCTL_COMMAND \
	CTL_CODE(DEV_TYPE,NCORD_IOCTL_BASE+0,METHOD_IN_DIRECT,FILE_ANY_ACCESS)
#define	IOCTL_GETLOG \
	CTL_CODE(DEV_TYPE,NCORD_IOCTL_BASE+1,METHOD_IN_DIRECT,FILE_ANY_ACCESS)
#define	IOCTL_WRITE_REG \
	CTL_CODE(DEV_TYPE,NCORD_IOCTL_BASE+2,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define	IOCTL_READ_REG \
	CTL_CODE(DEV_TYPE,NCORD_IOCTL_BASE+3,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define	IOCTL_READ_DEV_EXT \
	CTL_CODE(DEV_TYPE,NCORD_IOCTL_BASE+4,METHOD_BUFFERED,FILE_ANY_ACCESS)

#endif

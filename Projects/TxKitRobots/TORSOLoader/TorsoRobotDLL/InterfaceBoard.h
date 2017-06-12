//InterfaceBoard.h
// 作成者: 梶本博之
// 改定履歴
// 改定日：改定者：改定内容
// 2002/2/27 : 野嶋　琢也: インターフェースのカウンタボード用関数には２系統あるため、
//                      その両方に対応できるように、デバイス名から適切な関数系を利用するようにした。
//                      また、さまざまな初期設定に対応できるように初期化関数を追加した。
// 2007/12/4 : Pana : トルソ用に大幅書き換え

#ifndef ___INTERFACE
#define ___INTERFACE

#include <windows.h>
#include <stdio.h>

// Header files for PC-Board producted by Interface Co.
#include "interface/Fbida.h"     // DAボード用ヘッダファイル
#include "interface/Fbiad.h"     // ADボード用ヘッダファイル
#include "interface/FbiPenc.h"   // PCI-6204系列カウンタボード用ヘッダファイル
#include "interface/FbiMl.h"     // メモリンクボード用ヘッダファイル(追加：Watanabe)
#include "interface/FbiDio.h"    // 

// InitMode関数引数パラメータ
// 一番目の引数：モード
#define UseEqual    0x20 // 一致検出を利用
#define NotUseEqual 0x21 // 一致検出を利用しない
#define SoftLatch   0x22 // ソフトラッチ。コマンドを発効したときのエンコーダのカウント値が得られる
#define ExtLatch    0x24 // 外部ラッチ。外部からラッチ信号が入力されたときの値が得られる
#define CountUp     0x40 // アップ方向にカウント
#define CountDown   0x80 // ダウン方向にカウント
// 二番目の引数: 入力パルス
//入力パルスモード
#define SinglePulse     0x10    // ゲートつき単相パルスモード
#define DiffPhasePulse  0x20    // 位相差パルスモード
#define TwoPulse        0x40    // ２パルスモード   (PCI-6204系列のみ）
#define UpDownCount     0x80    // アップダウンカウントモード　(PCI-6201系列のみ）
#define Multi_1         0x00    // 標準
#define Multi_2         0x01    // ２逓倍
#define Multi_4         0x02    // ４逓倍
// 以下の設定は位相差パルスモードのときのみ有効
#define SyncClear       0x04    // 同期クリア
#define ASyncClear      0x08    // 非同期クリア
// 三番目の引数：Z相
//Z相設定
#define NotClear        0x00 // Z相クリアをしない
#define ClearWithEveryZ 0x01 // 毎回Z相クリアする
#define ClearWithZLatch 0x02 // Z相とラッチ信号でクリアする
#define ClearWithOneZ   0x04 // １度だけZ相でクリアする
#define PosiZ           0x00 // Z相通常
#define NegaZ           0x10 // Z相反転


#define	SAFE_DELETE(x)		if(x){delete x; x = NULL;}
#define	SAFE_DELETE_ARRAY(x)	if(x){delete [] x; x = NULL;}

#define DA_MAXCHANNEL	16


//--------------------------------
//インターフェース社ボード共通クラス
//--------------------------------
class InterfaceBoard
{
protected:
    int nRet;                       //関数返し値
    char        DeviceName[16];     //デバイス名
    HANDLE      DeviceHandle;       //デバイスハンドル
    int ChannelNum;                 //チャネル数
};

//--------------------------------
//インターフェース社DAボードクラス：I/O方式にのみ対応
//(メモリストアを持つボード等は別クラスとして定義せよ）
//--------------------------------
class InterfaceDABoard :public InterfaceBoard
{
private:
    DASMPLREQ   DeviceConfig;       // 出力制御構造体
    DABOARDSPEC DeviceInfo;         // ボード情報
    float Volt[6];                  //現在の出力電圧
    //データタイプ
	BYTE  Data8[DA_MAXCHANNEL];					// エラー回避のために、決め打ち宣言(2007/7/27)
    WORD  Data12[DA_MAXCHANNEL],Data16[DA_MAXCHANNEL];
    DWORD Data24[DA_MAXCHANNEL];
    void DisplayErrorMessage(void); //ボードのエラー情報を出力
public:
    InterfaceDABoard(char *name,int ChNum, int Range = DA_10V);//コンストラクタ。チャネル数とレンジを指定
	InterfaceDABoard(char *name);// コンストラクタのオーバーロード(変更：Watanabe)
	void initialize(int *ChArray, int ChNum, int Range=DA_10V); // チャンネル設定等をコンストラクタから分離(追加：Watanabe)
    ~InterfaceDABoard();
    void DAOut(int Channel,double Value); //Value(物理量[V])をChannelに出力
    void DAOut(double* Value);            //配列Value(物理量[V])を出力
    void Show();                          //ボードの状態を表示
    void SetChannel(int Channel, int Range= DA_10V); //あるチャネルのレンジを設定。もしそのチャネルが開いていなければ開く
    int  TEX_DaInputDI(void){
      DWORD dwData;
      DaInputDI( DeviceHandle, &dwData );
      return dwData;
    };
    void TEX_DaOutputDO(DWORD value){
      DaOutputDO( DeviceHandle, value );
    }

	};

//--------------------------------
//インターフェース社エンコーダボードクラス
//主にPCI6204,6205用
//--------------------------------
class InterfaceCNTBoard :public InterfaceBoard
{
private:
    void DisplayErrorMessage(void);      //内部関数。エラー表示
public:
    InterfaceCNTBoard(char *name, int Channel);
    ~InterfaceCNTBoard();
    void InitMode(int nCh, int CountMode, int PulseMode, int ZPhaseMode);       // チャンネルの初期化設定(nChチャンネルをCountModeで設定されたモードにより、PulseModeで設定されたパルスを読み込む.Z相の設定はZModeにより指定）
    void InitMode(int CountMode, int PulseMode, int ZPhaseMode);                // チャンネルの初期化設定(全チャンネルをCountModeで設定されたモードにより、PulseModeで設定されたパルスを読み込む.Z相の設定はZModeにより指定）
    void Show();                                                                //ボードの状態を表示
    DWORD Get(int Channel);                                                     //指定Channelをサンプリングして返す
    void Get(DWORD *EncValue);                                                  //全チャネル同時サンプリング
    void SetCount(DWORD Offset);                                                //全チャネルの値をOffsetに変更。
    void SetCount(int Channel, DWORD Offset);                                   //指定チャネルの値をOffsetに変更。
};

//--------------------------------
//インターフェース社MLボードクラス：
//--------------------------------
class InterfaceMLBoard :public InterfaceBoard
{
private:
	void DisplayErrorMessage(void);		//ボードのエラー情報を出力
	int nWriteSlaveNo,nReadSlaveNo;//スレーブ番号
	DWORD dwWriteOffset,dwReadOffset;//書き込み、読み込みオフセット
public:
	InterfaceMLBoard(char *name);//コンストラクタ。チャネル数、レンジ、入力モード(single or diff)を指定
	~InterfaceMLBoard();
	void MLRead(float* read,DWORD offset,int datasize);
	void MLWrite(float* write,DWORD offset,int datasize);
	void MLRead(double* read,DWORD offset,int datasize);
	void MLWrite(double* write,DWORD offset,int datasize);
};

//--------------------------------
//インターフェース社DIOボードクラス：
//--------------------------------
class InterfaceDIOBoard :public InterfaceBoard
{
private:
	void DisplayErrorMessage(void);		//ボードのエラー情報を出力
public:
	InterfaceDIOBoard(char *name);//コンストラクタ。チャネル数、レンジ、入力モード(single or diff)を指定
	~InterfaceDIOBoard();
	void DInput(int *Value, int offset, int num);
	void DOutput(BYTE value);
};

#endif
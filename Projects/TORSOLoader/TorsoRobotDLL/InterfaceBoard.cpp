//
// InterfaceBoard.c: インターフェース社ボード用クラス定義
// 2007/7/27 Torso用に大幅書き換え(削除)
//  New Delete宣言がなぜかうまく動かなかったので削除->決め打ちで宣言
//

//#define WIN32_LEAN_AND_MEAN
#include "stdafx.h"
#include <windows.h>
#include "InterfaceBoard.h"
#include "variables.h"
#include <string.h>
#include <math.h>
//#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>

using namespace std;

#pragma warning ( disable : 4996 )  // VS2005では警告されるので無効化

//--------------------------------
//インターフェース社DAボードクラス
//引数：デバイス名(例："FBIDA1")、チャネル数(例：4)、レンジ(例：DA_10V,FBIDA.hで定義)
//--------------------------------
InterfaceDABoard::InterfaceDABoard(char *name, int ChNum, int Range)
{
    int n;
    ChannelNum = ChNum;

    strcpy(DeviceName,name);
    DeviceHandle=DaOpen((LPCTSTR)DeviceName);
    if(DeviceHandle == INVALID_HANDLE_VALUE){
        DaClose(DeviceHandle);
        cout << "Error. Can't Open Device" << DeviceName << endl;
        exit(-1);
    }
    DaGetDeviceInfo(DeviceHandle,&DeviceInfo);
    DaGetSamplingConfig(DeviceHandle, &DeviceConfig);
    for(n=0;n<ChannelNum;n++){
        Volt[n]=0.0;
    }
    //チャネル設定。チャネル数、レンジ,サンプルモード、周波数,繰り返し回数。

    DeviceConfig.ulChCount=ChNum;
    for(n=0 ; n<ChannelNum ; n++){
        DeviceConfig.SmplChReq[n].ulChNo = n+1;
        DeviceConfig.SmplChReq[n].ulRange = Range;
    }
    DeviceConfig.ulSamplingMode = DA_IO_SAMPLING;
    DeviceConfig.fSmplFreq =10000.0;
    DeviceConfig.ulSmplRepeat = 0;
    DeviceConfig.ulTrigMode = DA_FREERUN;
    DaSetSamplingConfig(DeviceHandle , &DeviceConfig );
}

// コンストラクタのオーバーロード作成(変更：Watanabe)
// 

InterfaceDABoard::InterfaceDABoard(char *name)
{
    strcpy(DeviceName,name);
    DeviceHandle=DaOpen((LPCTSTR)DeviceName);
    if(DeviceHandle == INVALID_HANDLE_VALUE){
        DaClose(DeviceHandle);
        cout << "Error. Can't Open Device" << DeviceName << endl;
        exit(-1);
    }
    DaGetDeviceInfo(DeviceHandle,&DeviceInfo);

}

// initializeをコンストラクタから分離
// チャンネル組の二次元配列を与えて、一括取得したいチャンネル組分
// だけ構造体を作成する
// 今回は分割なしなので一次元配列にしていることに注意！！
// (汎用性を持たせるならばADと同様に**ChArray, *ChNumで定義すべき)
void InterfaceDABoard::initialize(int *ChArray, int ChNum, int Range)
{
    int n;
	ChannelNum = ChNum;

	for(n=0;n<ChannelNum;n++){
        Volt[n]=0.0;
    }
    //チャネル設定。チャネル数、レンジ,サンプルモード、周波数,繰り返し回数。

    DeviceConfig.ulChCount=ChNum;
    for(n=0 ; n<ChannelNum ; n++){
        DeviceConfig.SmplChReq[n].ulChNo = ChArray[n];
        DeviceConfig.SmplChReq[n].ulRange = Range;
    }
    DeviceConfig.ulSamplingMode = DA_IO_SAMPLING;
    DeviceConfig.fSmplFreq =10000.0;
    DeviceConfig.ulSmplRepeat = 0;
    DeviceConfig.ulTrigMode = DA_FREERUN;
    DaSetSamplingConfig(DeviceHandle , &DeviceConfig );
}


//あるチャネルのレンジを設定。もしそのチャネルが開いていなければ開く
void InterfaceDABoard::SetChannel(int Channel, int Range)
{
    if(Channel > ChannelNum){
        ChannelNum = Channel;
        DeviceConfig.ulChCount=ChannelNum;
    }
    DeviceConfig.SmplChReq[Channel].ulChNo = Channel+1;
    DeviceConfig.SmplChReq[Channel].ulRange = Range;
    DaSetSamplingConfig(DeviceHandle , &DeviceConfig );
}


InterfaceDABoard::~InterfaceDABoard(void)
{
    DaClose(DeviceHandle);
}


//ボードの状態を表示
void InterfaceDABoard::Show(void)
{
    cout << "Board Information" << endl
         << "Type " <<DeviceInfo.ulBoardType << endl
         << "ID " << DeviceInfo.ulBoardID << endl // ボード識別番号
         << "Channel Number " << DeviceInfo.ulChCount << endl// ボードアナログ出力チャンネル数
         << "Resolution " << DeviceInfo.ulResolution << endl// ボード分解能
         << "Digital Input Number " << DeviceInfo.ulDi << endl// ボードデジタル入力点数
         << "Digital Output Number " << DeviceInfo.ulDo << endl;// ボードデジタル出力点数
}

//
//ChannelにValue[V]を出力しつつ、それ以外のChannelの値は保持する。
//
void InterfaceDABoard::DAOut(int Channel,double Value)
{
	UINT MaxCh=DeviceConfig.ulChCount;
	UINT MaxSmplNum=1;

	if(Channel>(int)MaxCh-1 ||Channel<0){
        cout << "No such DA Channel " << Channel << endl;
        exit(-1);
	}

	Volt[Channel]=(float)Value;

	//出力
	switch(DeviceInfo.ulResolution){
		case 8 :
			DaDataConv(DA_DATA_PHYSICAL,Volt,MaxSmplNum,&DeviceConfig,
						DA_DATA_BIN8,Data8,&MaxSmplNum,&DeviceConfig,0,0,NULL);
			nRet=DaOutputDA(DeviceHandle,MaxCh,DeviceConfig.SmplChReq,Data8);
			break;
		case 12:
			DaDataConv(DA_DATA_PHYSICAL,Volt,MaxSmplNum,&DeviceConfig,
						DA_DATA_BIN12,Data12,&MaxSmplNum,&DeviceConfig,0,0,NULL);
			nRet=DaOutputDA(DeviceHandle,MaxCh,DeviceConfig.SmplChReq,Data12);
			break;
		case 16:
			DaDataConv(DA_DATA_PHYSICAL,Volt,MaxSmplNum,&DeviceConfig,
						DA_DATA_BIN16,Data16,&MaxSmplNum,&DeviceConfig,0,0,NULL);
			nRet=DaOutputDA(DeviceHandle,MaxCh,DeviceConfig.SmplChReq,Data16);
			break;
		case 24:
			DaDataConv(DA_DATA_PHYSICAL,Volt,MaxSmplNum,&DeviceConfig,
						DA_DATA_BIN24,Data24,&MaxSmplNum,&DeviceConfig,0,0,NULL);
			nRet=DaOutputDA(DeviceHandle,MaxCh,DeviceConfig.SmplChReq,Data24);
			break;
	}

    if(nRet!=0){
        DisplayErrorMessage();
        exit(-1);
    }
}


//配列Value(物理量[V])を出力。チャネル同時
//配列サイズはチャネル数分無ければならない
//(特にエラー処理していないので容易にSegmentation Faultする。注意）
void InterfaceDABoard::DAOut(double* Value)
{
	UINT MaxCh=DeviceConfig.ulChCount;
	UINT MaxSmplNum=1;

    //Segmentation Fault起きやすいところ。チャネル数分データを指定していることを確認せよ
    for(int n=0;n<(int)MaxCh;n++){
        Volt[n]=(float)Value[n];
    }

	//出力
	switch(DeviceInfo.ulResolution){
		case 8 :
			DaDataConv(DA_DATA_PHYSICAL,Volt,MaxSmplNum,&DeviceConfig,
						DA_DATA_BIN8,Data8,&MaxSmplNum,&DeviceConfig,0,0,NULL);
			nRet=DaOutputDA(DeviceHandle,MaxCh,DeviceConfig.SmplChReq,Data8);
			break;
		case 12:
			nRet=DaDataConv(DA_DATA_PHYSICAL,Volt,MaxSmplNum,&DeviceConfig,
						DA_DATA_BIN12,Data12,&MaxSmplNum,&DeviceConfig,0,0,NULL);
			nRet=DaOutputDA(DeviceHandle,MaxCh,DeviceConfig.SmplChReq,Data12);
			break;
		case 16:
			DaDataConv(DA_DATA_PHYSICAL,Volt,MaxSmplNum,&DeviceConfig,
						DA_DATA_BIN16,Data16,&MaxSmplNum,&DeviceConfig,0,0,NULL);
			nRet=DaOutputDA(DeviceHandle,MaxCh,DeviceConfig.SmplChReq,Data16);
			break;
		case 24:
			DaDataConv(DA_DATA_PHYSICAL,Volt,MaxSmplNum,&DeviceConfig,
						DA_DATA_BIN24,Data24,&MaxSmplNum,&DeviceConfig,0,0,NULL);
			nRet=DaOutputDA(DeviceHandle,MaxCh,DeviceConfig.SmplChReq,Data24);
			break;
	}

    if(nRet!=0){
        DisplayErrorMessage();
        exit(-1);
    }
}

//エラー表示：この辺の値はgpcda.h内に定義されている
void InterfaceDABoard::DisplayErrorMessage()
{
    cout <<"DA Error: ";
    switch(nRet) {
        case DA_ERROR_NOT_DEVICE:
            cout << "The device couldn't be found."<<endl;
            break;
        case DA_ERROR_NOT_OPEN:
            cout << "The system couldn't found the device."<<endl;
            break;
        case DA_ERROR_INVALID_HANDLE:
            cout << "Invalid device handle is specified."<<endl;
            break;
        case DA_ERROR_ALREADY_OPEN:
            cout << "The device has been already opened."<<endl;
            break;
        case DA_ERROR_NOT_SUPPORTED:
            cout << "It is not supported."<<endl;
            break;
        case DA_ERROR_NOW_SAMPLING:
            cout << "The analog output is running."<<endl;
            break;
        case DA_ERROR_STOP_SAMPLING:
            cout << "The analog output is stopped."<<endl;
            break;
        case DA_ERROR_START_SAMPLING:
            cout << "The analog output couldn't be performed."<<endl;
            break;
        case DA_ERROR_SAMPLING_TIMEOUT:
            cout << "The timeout interval elapsed while the analog output is running."<<endl;
            break;
        case DA_ERROR_INVALID_PARAMETER:
            cout << "Invalid parameters are specified."<<endl;
            break;
        case DA_ERROR_ILLEGAL_PARAMETER:
            cout << "Invalid analog output conditions are specified."<<endl;
            break;
        case DA_ERROR_NULL_POINTER:
            cout << "A NULL pointer is specified."<<endl;
            break;
        case DA_ERROR_SET_DATA:
            cout << "The analog output data couldn't be configured."<<endl;
            break;
        case DA_ERROR_FILE_OPEN:
            cout << "Opening the file failed."<<endl;
            break;
        case DA_ERROR_FILE_CLOSE:
            cout << "Closing the file failed."<<endl;
            break;
        case DA_ERROR_FILE_READ:
            cout << "Reading the file failed."<<endl;
            break;
        case DA_ERROR_FILE_WRITE:
            cout << "Writing the file failed."<<endl;
            break;
        case DA_ERROR_INVALID_DATA_FORMAT:
            cout << "Invalid data format is specified."<<endl;
            break;
        case DA_ERROR_INVALID_AVERAGE_OR_SMOOTHING:
            cout << "Invalid averaging configuration or invalid smoothing configuration is specified."<<endl;
            break;
        case DA_ERROR_INVALID_SOURCE_DATA:
            cout <<  "Invalid source data are specified."<<endl;
            break;
        case DA_ERROR_NOT_ALLOCATE_MEMORY:
            cout << "Enough memory couldn't be allocated."<<endl;
            break;
        case DA_ERROR_NOT_LOAD_DLL:
            cout << "Loading the DLL failed."<<endl;
            break;
        case DA_ERROR_CALL_DLL:
            cout << "Calling the DLL failed."<<endl;
            break;
        default:
            cout << "Unexpected error is occurred."<<endl;
            break;
    }

}


//エラー表示：この辺の値はfbienc.h内に定義されている
void InterfaceCNTBoard::DisplayErrorMessage()
{
    cout << "CNTBoard::";
    switch(nRet){
        case PENC_ERROR_SUCCESS:   // 正常終了
            cout << "正常終了"<<endl;
            break;
        case PENC_ERROR_NOT_DEVICE:
            cout << "指定されたデバイスを見つけることができません"<<endl;
            break;
        case PENC_ERROR_NOT_OPEN:
            cout << "システムがデバイスをオープンできません"<<endl;
            break;
        case PENC_ERROR_INVALID_HANDLE:
            cout << "デバイスハンドルが正しくありません"<<endl;
            break;
        case PENC_ERROR_ALREADY_OPEN:
            cout << "すでにOPENしているデバイスをOPENしようとしました"<<endl;
            break;
        case PENC_ERROR_HANDLE_EOF:
            cout << "EOFに達しました"<<endl;
            break;
        case PENC_ERROR_MORE_DATA:
            cout << "さらに多くのデータが利用可能です"<<endl;
            break;
        case PENC_ERROR_INSUFFICIENT_BUFFER:
            cout << "システムコールに渡されたデータ領域が小さすぎます"<<endl;
            break;
        case PENC_ERROR_IO_PENDING:
            cout << "非同期I/O操作が進行中です"<<endl;
            break;
        case PENC_ERROR_NOT_SUPPORTED:
            cout << "サポートされていない機能です"<<endl;
            break;
        case PENC_ERROR_INITIALIZE_IRQ:
            cout << "割り込みの初期化に失敗しました"<<endl;
            break;
        case PENC_ERROR_INVALID_CHANNEL:
            cout << "不正なチャンネルを指定しました"<<endl;
            break;
        case PENC_ERROR_INVALID_MODE:
            cout << "不正なモードを指定しました"<<endl;
            break;
        case PENC_ERROR_INVALID_DIRECT:
            cout << "不正なカウンタ方向を指定しました"<<endl;
            break;
        case PENC_ERROR_INVALID_COUNTER:
            cout << "不正なカウンタ値を指定しました"<<endl;
            break;
        case PENC_ERROR_INVALID_COMPARATOR:
            cout << "不正な比較カウンタを指定しました"<<endl;
            break;
        case PENC_ERROR_INVALID_ZMODE:
            cout << "不正なＺ相論理値を指定しました"<<endl;
            break;
        case PENC_ERROR_INVALID_MASK:
            cout << "不正なイベントマスク値を指定しました"<<endl;
            break;
        case PENC_ERROR_INVALID_ITIMER:
            cout << "不正なインターバルタイマ設定値を指定しました"<<endl;
            break;
        case PENC_ERROR_ALREADY_REGISTRATION:
            cout << "イベントはすでに登録済みです"<<endl;
            break;
        case PENC_ERROR_ALREADY_DELETE:
            cout << "イベントはすでに削除されています"<<endl;
            break;
        case PENC_ERROR_MEMORY_NOTALLOCATED:
            cout << "作業用メモリの確保に失敗しました"<<endl;
            break;
        case PENC_ERROR_MEMORY_FREE:
            cout << "メモリの解放に失敗しました"<<endl;
            break;
        case PENC_ERROR_TIMER:
            cout << "タイマリソースの取得に失敗しました"<<endl;
            break;
        case PENC_ERROR_DRVCAL:
            cout << "ドライバが呼び出せません"<<endl;
            break;
        case PENC_ERROR_NULL_POINTER:
            cout << "ドライバ、DLL間でNULLポインタが渡されました"<<endl;
            break;
        case PENC_ERROR_PARAMETER:
            cout << "引数パラメータの値が不正です"<<endl;
            break;
        default:
            cout << "予期しないエラーが発生しました"<<endl;
            break;
    }
}


//--------------------------------
//インターフェース社カウンタボードクラス
//引数：デバイス名(例："FBIPENC1")、チャネル数(例：2)
//PCI-6204,PCI-6205系列専用
//--------------------------------
InterfaceCNTBoard::InterfaceCNTBoard(char *name, int Channel)
{
    ChannelNum = Channel;

    strcpy(DeviceName,name);
    DeviceHandle=PencOpen((LPCTSTR)DeviceName, PENC_FLAG_SHARE);
    if(DeviceHandle == INVALID_HANDLE_VALUE){
        PencClose(DeviceHandle);
        cout << "Error. Can't Open Device" << DeviceName << endl;
        exit(-1);
    }
    // デフォルトの初期化情報
    // カウントアップ、一致検出しない、ソフトラッチ、位相差入力パルス、４逓倍、Z相クリアせず
    InitMode(CountUp | NotUseEqual |SoftLatch , DiffPhasePulse | Multi_4 , NotClear);
}

InterfaceCNTBoard::~InterfaceCNTBoard(void)
{
    PencClose(DeviceHandle);
}

// チャンネルの初期化設定(nChチャンネルをCountModeで設定されたモードにより、
// PulseModeで設定されたパルスを読み込む.Z相の設定はZModeにより指定）
void InterfaceCNTBoard::InitMode(int nCh, int CountMode, int PulseMode, int ZPhaseMode){
//    assert(nCh < ChannelNum);       // nCHで指定されるチャンネルは、初期化したチャンネル数よりも小さい必要がある。
    int CountDirection = 0;
    int EqualMode = 0;
    int LatchMode = 0;
    int InputPulseMode = 0x00;

    if((CountMode & 0xC0) == CountUp){
        CountDirection = 0;         // Up方向にカウント
    }else if((CountMode & 0xC0) == CountDown){
        CountDirection = 1;         // Down方向にカウント
    }
    if((CountMode & 0x21) == UseEqual){
        EqualMode = 1;          // 一致検出有効
    }else if((CountMode & 0x21) == NotUseEqual){
        EqualMode = 0;          // 一致検出無効
    }


    if((CountMode & 0x22) == SoftLatch){
        LatchMode = 0;          // ソフトラッチ
    }else if((CountMode & 0x24) == ExtLatch){
        LatchMode = 1;          // 外部ラッチ
    }

    if((PulseMode & 0xF0) == SinglePulse){
        InputPulseMode = 0x00;              // 単相パルスモードのとき
        if((PulseMode & 0x01) == Multi_1){
            InputPulseMode += 0x00;         //標準
        }else{
            InputPulseMode += 0x01;         // ２逓倍。単相パルスモードのときは２逓倍まで
        }
    }else if((PulseMode & 0xF0 ) == DiffPhasePulse){
        InputPulseMode = 0x04;              // 位相差パルスモードのとき
        if((PulseMode & 0x03) == Multi_1){
            InputPulseMode += 0x00;         //標準
        }else if((PulseMode & 0x03) == Multi_2){
            InputPulseMode += 0x01;         // ２逓倍
        }else if((PulseMode & 0x03) == Multi_4){
            InputPulseMode += 0x02;         // 4逓倍
        }
        if((PulseMode & 0x0C) == SyncClear){
            InputPulseMode += 0x08;         // 同期クリア
        }else if((PulseMode & 0x0C) == ASyncClear){
            InputPulseMode += 0x00;         //　非同期クリア
        }
    }else if((PulseMode & 0xF0 ) == TwoPulse){
        InputPulseMode = 0x08;
    }

    if((nRet=PencReset(DeviceHandle,nCh+1)) != 0) DisplayErrorMessage();
    if((nRet=PencSetMode(DeviceHandle, nCh+1, InputPulseMode, CountDirection, EqualMode, LatchMode)) != 0) DisplayErrorMessage();
    if((nRet=PencSetZMode(DeviceHandle, nCh+1, ZPhaseMode) != 0)) DisplayErrorMessage();
    if((nRet=PencSetCounter(DeviceHandle,nCh+1,0) != 0)) DisplayErrorMessage();
}

// チャンネルの初期化設定(全チャンネルをCountModeで設定されたモードにより、
// PulseModeで設定されたパルスを読み込む.Z相の設定はZModeにより指定）
void InterfaceCNTBoard::InitMode(int CountMode, int PulseMode, int ZPhaseMode)
{
    int i;
    for(i=0;i<ChannelNum;i++)
        InitMode(i,CountMode,PulseMode,ZPhaseMode);
}

//ボード状態表示
void InterfaceCNTBoard::Show(void)
{
    int n;
    int multi;
    int Mode, Direction, Equal, Latch, ZMode;
    cout << "PCI-6204 compatible CNT Board" << endl;

    for(n=0;n<ChannelNum;n++){
        cout << "チャンネル" << n << "  " ;
        PencGetMode(DeviceHandle, n+1, &Mode, &Direction, &Equal, &Latch);
        PencGetZMode(DeviceHandle, n+1, &ZMode);
        multi = (int)pow(2.0,(Mode & 0x03));
        switch(Mode & 0x0C){
        case 0x00:
            cout << "ゲートつき単相パルス  " << multi << "逓倍  " << "非同期クリア" << endl;
            break;
        case 0x04:
            cout << "位相差パルス　　　　　" << multi << "逓倍  " << "非同期クリア" << endl;
            break;
        case 0x0C:
            cout << "位相差パルス　　　　　" << multi << "逓倍  " << "  同期クリア" << endl;
            break;
        case 0x08:
            cout << "２パルス　　　　　　　" << multi << "逓倍  " << "非同期クリア" << endl;
            break;
        }
        if(Direction == 0)
            cout << "   カウントアップ  " ;
        else
            cout << "   カウントダウン  " ;

        if(Equal == 0)
            cout << "一致検出無効  ";
        else
            cout << "一致検出有効  ";

        if(Latch == 0)
            cout << "ソフトラッチ  ";
        else
            cout << "外部ラッチ    ";

        if((ZMode & 0x10) == 0x00)
            cout << "Z相通常  ";
        else
            cout << "Z相反転  ";

        switch(ZMode & 0x03){
        case 0x00:
            cout << "Z相クリア無効" << endl;
            break;
        case 0x01:
            cout << "Z相クリア有効" << endl;
            break;
        case 0x02:
            cout << "外部ラッチ&Z相によるクリア有効" << endl;
            break;
        }

    }

}

//エンコーダ値を返す関数二つ
//あるチャネルからの値を読む。チャネル指定は0～
DWORD InterfaceCNTBoard::Get(int Channel)
{
    static DWORD dwCounter;
    if((nRet=PencGetCounter(DeviceHandle,Channel+1,&dwCounter))!=0)DisplayErrorMessage();
    return dwCounter;

}
//すべてのチャネルからの値を同時に読み、引数の配列に格納
void InterfaceCNTBoard::Get(DWORD *EncValue)
{
    int bit = (1<<ChannelNum) - 1; //チャネル数分のビットを立てる
    if((nRet=PencGetCounterEx(DeviceHandle, bit, &EncValue[0]))!=0)
		DisplayErrorMessage();
}

//現在のすべてのエンコーダ値を同時に0に設定。
//オプションとしてオフセット値も引数として設定可・

void InterfaceCNTBoard::SetCount(DWORD Offset)
{
    //エンコーダのオフセット値を設定
    DWORD *dw= new DWORD[ChannelNum];

    for(int n=0;n<ChannelNum;n++){
        dw[n] = Offset;
    }
    //リセット
    int bit = (1<<ChannelNum) - 1; //チャネル数分のビットを立てる
    if((nRet=PencSetCounterEx(DeviceHandle, bit, dw))!=0)DisplayErrorMessage();
}

//指定チャネルの値をOffset値に設定。チャネル番号指定は０～
void InterfaceCNTBoard::SetCount(int Channel, DWORD Offset)
{
    //リセット
    if((nRet=PencSetCounter(DeviceHandle,Channel+1,Offset) != 0)) DisplayErrorMessage();
}



InterfaceDIOBoard::InterfaceDIOBoard(char *name)
{	
	strcpy(DeviceName,name);
	
    DeviceHandle = DioOpen((LPCTSTR)DeviceName, 0);
    if (DeviceHandle == INVALID_HANDLE_VALUE) {
		DioClose(DeviceHandle);
        printf("デバイス名 FBIDIO1は使用できません\n");
        exit(-1);             //プログラム終了
    }
}

InterfaceDIOBoard::~InterfaceDIOBoard(void)
{
	DioClose(DeviceHandle);
}

void InterfaceDIOBoard::DInput(int *Value, int offset, int num)
{

    int buffer[64];

	DioInputPoint(DeviceHandle, buffer, offset, num);

	for(int i=0; i<num; i++) Value[i] = buffer[i];

}

void InterfaceDIOBoard::DOutput(BYTE value)
{

}
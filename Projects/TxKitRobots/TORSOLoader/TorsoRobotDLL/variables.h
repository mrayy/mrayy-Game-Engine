#ifndef __VARIABLES
#define __VARIABLES

#include <stdio.h>
#include <iostream>
using namespace std;

#include "Interfaceboard.h"
extern InterfaceDABoard  PCIDA;			// インタフェースDAボードクラス(大域定義)
extern InterfaceCNTBoard PCICNT;		// インタフェースカウンタボードクラス(大域定義)
extern InterfaceDIOBoard PCIDIO;		// インタフェースDIOボードクラス(大域定義)

#include "CPUTimer.h"
extern CPUTimer MainTimer;				// CPUタイマクラス(大域定義)

extern FILE *fp_torso;					// データチェック用ファイルハンドル(トルソクラス用)
extern FILE *fp_link;					// データチェック用ファイルハンドル(リンククラス用)
extern FILE *fp_routine;				// データチェック用ファイルハンドル(サブルーチン用)
extern FILE *fp_board;					// データチェック用ファイルハンドル(インタフェースボード用)

#include "RealTorso.h"
extern CRealTorso	realTorso;			// トルソオブジェクト


extern int LoadData(ifstream& data_file, char partition,...);
extern int FirstMoving(void);
extern int FinishMoving(void);
extern int MainControl_task(int CalibSelect);
extern int Debug_task(void);

#endif
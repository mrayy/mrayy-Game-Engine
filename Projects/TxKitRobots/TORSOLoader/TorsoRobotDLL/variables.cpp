#include "stdafx.h"
#include "variables.h"

InterfaceDABoard	PCIDA("FBIDA1");		// インタフェースDAボードクラス(大域定義)
InterfaceCNTBoard	PCICNT("FBIPENC17", 8);	// インタフェースカウンタボードクラス(大域定義)
InterfaceDIOBoard	PCIDIO("FBIDIO1");		// インタフェースDIOボードクラス(大域定義)

CPUTimer			MainTimer;			// CPUタイマクラス(大域定義)

FILE				*fp_torso;			// データチェック用ファイルハンドル(トルソクラス用)
FILE				*fp_link;			// データチェック用ファイルハンドル(リンククラス用)
FILE				*fp_routine;		// データチェック用ファイルハンドル(サブルーチン用)
FILE				*fp_board;			// データチェック用ファイルハンドル(インタフェースボード用)

CRealTorso			realTorso;			// トルソオブジェクト

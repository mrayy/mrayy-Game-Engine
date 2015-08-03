#ifndef ___DEFINES
#define ___DEFINES


#define Torso_DOF	6
#define TIMESPAN	3					// 初期追従駆動/終了駆動にかける時間

//#define GLOUTPUT
#define GL_OUTFILE  "C:\\torso\\Programs\\TORSO\\TorsoUtilities\\TORSO_GLOUT\\streamdata.dat"
#define BUF_NUM		3					//マルチスレッドのバッファの数　最低3

// キャリブレーション識別子
#define AUTOCALIBMODE	0
#define MANUALCALIBMODE	1

// 随時変更して使うもの
#define USE_ADL1	1					// 0 ADL1と通信しない。
#define SELECT_ADL1	1					// 0->旧ADL1、1->新ADL1

#endif
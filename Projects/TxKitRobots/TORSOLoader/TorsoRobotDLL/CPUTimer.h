//====================================================================
//  タイマクラスヘッダ
//  2007/7/27 Pana  梶本さん作成オリジナルから大きく変更
//                  Core2Duo等のSpeedStepにより適切な周波数が取得できない問題を解消
//====================================================================

#ifndef __CPUTimer
#define __CPUTimer
#include<windows.h>

///-----[CPUTimer]-----------------------------------------------------
class CPUTimer
{
protected:
  __int64       CPU_Freq; // CPU周波数
  __int64		StartTime; // カウント初期値
  __int64		LastTime; // 前回カウント時のカウント値

public:
  CPUTimer(void); // コンストラクタ，作成時に周波数読み込み
  ~CPUTimer(); // デストラクタ，何もしない

  void Start(void); // Elapsed関数のためのカウント初期値の更新
  __int64 Elapsed(void); // カウント初期値からの経過時間を返す
  __int64 Count(void); // 前回カウント(Count or Wait関数)からの経過時間を返す

  void Wait(__int64 time); // 引数指定によりtime[usec]だけ待つ
  void CountAndWait(__int64 time); // 引数指定により前回カウントからの経過時間を取得し全体でtime[usec]だけ待つ

  __int64 ReadCPU_Freq(void); // コンストラクタ時に取得したCPU周波数を返す
  void SetCPU_Freq(__int64 freq); // CPU周波数を手動で設定

};
#endif
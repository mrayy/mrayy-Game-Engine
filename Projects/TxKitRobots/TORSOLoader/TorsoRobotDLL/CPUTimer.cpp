//====================================================================
//  タイマクラスファイル
//  2007/7/27 Pana  梶本さん作成オリジナルから大きく変更
//                  Core2Duo等のSpeedStepにより適切な周波数が取得できない問題を解消
//====================================================================
#include "stdafx.h"
#include "CPUTimer.h"


CPUTimer::CPUTimer(void)
{
	::QueryPerformanceFrequency( (LARGE_INTEGER*)&CPU_Freq ); // 高分解能ハードウェアカウンタの周波数を取得

	::QueryPerformanceCounter( (LARGE_INTEGER*)&StartTime); // 高分解能ハードウェアカウンタを読む(カウンタ初期値に設定)

	LastTime = StartTime; // Count関数のためのカウント値の保存
}


CPUTimer::~CPUTimer(){}


void CPUTimer::Start(void)
{
	::QueryPerformanceCounter( (LARGE_INTEGER*)&StartTime); // 高分解能ハードウェアカウンタを読む(カウンタ初期値の更新)
}


__int64 CPUTimer::Elapsed(void)
{
	__int64 PresentTime;

	::QueryPerformanceCounter( (LARGE_INTEGER*)&PresentTime); // 高分解能ハードウェアカウンタを読む

	return (PresentTime - StartTime)*1000000/CPU_Freq; // 周波数を用いて計算されたカウンタ初期値からの経過時間[μsec]を返す
}


__int64 CPUTimer::Count(void)
{
	__int64 PresentTime;
	__int64 retval;

	::QueryPerformanceCounter( (LARGE_INTEGER*)&PresentTime); // 高分解能ハードウェアカウンタを読む

	retval = (PresentTime - LastTime)*1000000/CPU_Freq; // 周波数を用いて計算された前回Countからの経過時間[μsec]を返す
	LastTime = PresentTime; // Count関数のためのカウント値の保存

	return retval;
}


void CPUTimer::Wait(__int64 time)
{
	__int64 PresentTime;

	::QueryPerformanceCounter( (LARGE_INTEGER*)&PresentTime); // 高分解能ハードウェアカウンタを読む
	LastTime = PresentTime + (__int64)time*CPU_Freq/1000000; // 現在のカウンタ値にユーザ指定待ち時間timeを足しこむ

	do{
		::QueryPerformanceCounter( (LARGE_INTEGER*)&PresentTime); // 高分解能ハードウェアカウンタを読む
	}while(LastTime > PresentTime); // 現在のカウンタ値が指定待ち時間になるまで繰り返し

}


void CPUTimer::CountAndWait(__int64 time)
{
	__int64 TempTime;

	TempTime = Count(); // 経過時間をカウントする
	if(TempTime > time) return;	// 経過時間が指定時間よりも長ければ待たずに処理を返す
	Wait(time - TempTime); // 指定時間－経過時間だけ待つ
}


__int64 CPUTimer::ReadCPU_Freq(void)
{
	return CPU_Freq; // 現在クラスメンバに保存されている周波数を返す
}


void CPUTimer::SetCPU_Freq(__int64 freq)
{
	CPU_Freq = freq; // 周波数をクラスメンバに設定する
}
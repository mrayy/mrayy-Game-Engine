/*============================================================================

                  TexART utility calss [TexART_MothorCLK]

                       file name : TexART_MothorCLK.h
                 original author : I.Kawabuchi
                                   (info@kawabuchi-lab.com)
               first publication : 2003.8/15(Fri)
                     version 2.0 : 2012.8/6(Mon)

============================================================================*/
#ifndef ___TexART_MothorCLK
#define ___TexART_MothorCLK
#include<windows.h>

// [Memo]
// LARGE_INTEGER は，8bytes(64bit) の符号付整数型である．
// 大きすぎて扱いづらいため，以下の三つのメンバからなる構造体になっている．
// HighPart : 上位ダブルワード(DWORD)を格納
// LowPart  : 下位ダブルワード(DWORD)を格納
// QuadPart : 全体(LONGLONG)を格納
// QuadPart は大きすぎるため， printf("%d",xx.QuadPart);のように「%d」や「%x」を利用することはできない．
// 「%I64d」や「%I64x」のように「I64」指定をしなければいけない．

// LARGE_INTEGER と __int64 との違いについて．
// 前者は HighPart と LowPart をもつ構造体であり，__int64 は非構造体である．
// 両者間の cast は問題なく行える．
// なお，64bit 整数の型の名前は，理想的には long long である．
// ところがその標準化が遅かったために，
// LONGLONG，LARGE_INTEGER，__int64 が暫定的に(コンパイラ製造会社の都合で)定義された経緯がある．
// そこで，
// LARGE_INTEGER は関数 QueryPerformanceFrequency と QueryPerformanceCounter の引数としてのみ用い，
// その他は long long を用いることにする．

///-----[TexART_MothorCLK]----------------------------------------------------
class TexART_MothorCLK
{
protected:
  long long CLK_Frequency,  // Frequency of the MothorCLK (clock counter).  [Hz] Default value is 1 GHz.
            Count_start,    // Count of the MothorCLK at starting.          [count]
            Count_last,     // Count of the MothorCLK at last call.         [count]
            Count_lastLap0, // Count of the MothorCLK at last call of Lap0. [count]
            Count_lastLap1, // Count of the MothorCLK at last call of Lap1. [count]
            Count_lastLap2, // Count of the MothorCLK at last call of Lap2. [count]
            Count_lastLap3, // Count of the MothorCLK at last call of Lap3. [count]
            Count_lastLap4, // Count of the MothorCLK at last call of Lap4. [count]
            Count_lastLap5, // Count of the MothorCLK at last call of Lap5. [count]
            Count_lastLap6, // Count of the MothorCLK at last call of Lap6. [count]
            Count_lastLap7, // Count of the MothorCLK at last call of Lap7. [count]
            Count_lastLap8, // Count of the MothorCLK at last call of Lap8. [count]
            Count_lastLap9; // Count of the MothorCLK at last call of Lap9. [count]

public:
  // Default constructor.
  TexART_MothorCLK(void);                      // Default constructor.
  TexART_MothorCLK(const long long frequency); // Convert constructor.

  // Destructor.
  ~TexART_MothorCLK();

  // Acquire Frequency.
  long long Acquire_Frequency(void);

  // Start the timer counting from 0.
  void Start(void);

  // Pause the timer counting.
  void Pause(void);

  // Restart the timer counting from the count value at the paused time.
  void Restart(void);

  // Wait in a designated time span [s].
  void Wait(double wait_second);

  // Periodic wait for keeping a cycle [s] of a program loop.
  void PeriodicWait(double cycle_second);

  // Get elapsed time [s] form the starting time.
  double Elapsed(void);

  // Get lap time [s] between last and present calls of the same command.
  double Lap0(void);
  double Lap1(void);
  double Lap2(void);
  double Lap3(void);
  double Lap4(void);
  double Lap5(void);
  double Lap6(void);
  double Lap7(void);
  double Lap8(void);
  double Lap9(void);

///-----[Utility Functions]---------------------------------------------------
  // Read Frequency.
  long long Read_Frequency(void);

  // Set CLK_Frequency.
  void Set_Frequency(long long frequency);

};
#endif
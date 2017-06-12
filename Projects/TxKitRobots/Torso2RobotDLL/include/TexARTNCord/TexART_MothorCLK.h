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
// LARGE_INTEGER �́C8bytes(64bit) �̕����t�����^�ł���D
// �傫�����Ĉ����Â炢���߁C�ȉ��̎O�̃����o����Ȃ�\���̂ɂȂ��Ă���D
// HighPart : ��ʃ_�u�����[�h(DWORD)���i�[
// LowPart  : ���ʃ_�u�����[�h(DWORD)���i�[
// QuadPart : �S��(LONGLONG)���i�[
// QuadPart �͑傫�����邽�߁C printf("%d",xx.QuadPart);�̂悤�Ɂu%d�v��u%x�v�𗘗p���邱�Ƃ͂ł��Ȃ��D
// �u%I64d�v��u%I64x�v�̂悤�ɁuI64�v�w������Ȃ���΂����Ȃ��D

// LARGE_INTEGER �� __int64 �Ƃ̈Ⴂ�ɂ��āD
// �O�҂� HighPart �� LowPart �����\���̂ł���C__int64 �͔�\���̂ł���D
// ���ҊԂ� cast �͖��Ȃ��s����D
// �Ȃ��C64bit �����̌^�̖��O�́C���z�I�ɂ� long long �ł���D
// �Ƃ��낪���̕W�������x���������߂ɁC
// LONGLONG�CLARGE_INTEGER�C__int64 ���b��I��(�R���p�C��������Ђ̓s����)��`���ꂽ�o�܂�����D
// �����ŁC
// LARGE_INTEGER �͊֐� QueryPerformanceFrequency �� QueryPerformanceCounter �̈����Ƃ��Ă̂ݗp���C
// ���̑��� long long ��p���邱�Ƃɂ���D

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
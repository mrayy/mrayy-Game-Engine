/*============================================================================

                  TexART utility calss [TexART_MothorCLK]


                       file name : TexART_MothorCLK.cpp
                 original author : I.Kawabuchi
                                   (info@kawabuchi-lab.com)
               first publication : 2003.8/15(Fri)
                     version 2.0 : 2012.8/6(Mon)

============================================================================*/
#include "stdafx.h"
#include "TexART_MothorCLK.h"

///-----[TexART_MothorCLK::TexART_MothorCLK]----------------------------------
  //----- Default construntor.
TexART_MothorCLK::TexART_MothorCLK(void)
{
  // Initialize parameters.
  CLK_Frequency  = 1000000000;
  Count_start    = 0;
  Count_last     = 0;
  Count_lastLap0 = 0;
  Count_lastLap1 = 0;
  Count_lastLap2 = 0;
  Count_lastLap3 = 0;
  Count_lastLap4 = 0;
  Count_lastLap5 = 0;
  Count_lastLap6 = 0;
  Count_lastLap7 = 0;
  Count_lastLap8 = 0;
  Count_lastLap9 = 0;
}

  //----- Convert construntor.
TexART_MothorCLK::TexART_MothorCLK(const long long frequency)
{
  // Initialize parameters.
  CLK_Frequency  = frequency;
  Count_start    = 0;
  Count_last     = 0;
  Count_lastLap0 = 0;
  Count_lastLap1 = 0;
  Count_lastLap2 = 0;
  Count_lastLap3 = 0;
  Count_lastLap4 = 0;
  Count_lastLap5 = 0;
  Count_lastLap6 = 0;
  Count_lastLap7 = 0;
  Count_lastLap8 = 0;
  Count_lastLap9 = 0;
}

///-----[TexART_MothorCLK::~TexART_MothorCLK]---------------------------------
TexART_MothorCLK::~TexART_MothorCLK(){}

///-----[TexART_MothorCLK::Acquire_Frequency]---------------------------------
long long TexART_MothorCLK::Acquire_Frequency(void)
{
  LARGE_INTEGER Frequency; // Frequency [Hz]

  // Get CLK_Frequency.
  QueryPerformanceFrequency(&Frequency);

  CLK_Frequency = Frequency.QuadPart;

  return CLK_Frequency;
}

///-----[TexART_MothorCLK::Start]---------------------------------------------
void TexART_MothorCLK::Start(void)
{
  LARGE_INTEGER Count_present; // Count of the CLK at present call. [count]

  QueryPerformanceCounter(&Count_present);

  Count_start = Count_present.QuadPart;
}

///-----[TexART_MothorCLK::Pause]---------------------------------------------
void TexART_MothorCLK::Pause(void)
{
  LARGE_INTEGER Count_present; // Count of the CLK at present call. [count]

  QueryPerformanceCounter(&Count_present);

  Count_last = Count_present.QuadPart;
}

///-----[TexART_MothorCLK::Restart]-------------------------------------------
void TexART_MothorCLK::Restart(void)
{
  LARGE_INTEGER Count_present; // Count of the CLK at present call. [count]

  QueryPerformanceCounter(&Count_present);

  Count_start += (Count_present.QuadPart - Count_last);
}

///-----[TexART_MothorCLK::Wait]----------------------------------------------
void TexART_MothorCLK::Wait(double wait_second)
{
  LARGE_INTEGER Count_present; // Count of the CLK at present call. [count]
  long long     wait_count;    // [count]

  wait_count = (long long)(wait_second * CLK_Frequency);

  QueryPerformanceCounter(&Count_present);

  Count_last = Count_present.QuadPart;

  while(Count_present.QuadPart - Count_last < wait_count)
    QueryPerformanceCounter(&Count_present);
}

///-----[TexART_MothorCLK::PeriodicWait]------------------------------------
void TexART_MothorCLK::PeriodicWait(double cycle_second)
{
  LARGE_INTEGER Count_present;      // Count of the CLK at present call. [count]
  long long     period_count,       // Total count during a cycle. [count]
                last_period_number;

  period_count = (long long)(cycle_second * CLK_Frequency);

  QueryPerformanceCounter(&Count_present);

  Count_last = Count_present.QuadPart;
  last_period_number = (long long)(Count_last / period_count);

  while((long long)(Count_present.QuadPart / period_count) == last_period_number)
    QueryPerformanceCounter(&Count_present);
}

///-----[TexART_MothorCLK::Elapsed]-----------------------------------------
double TexART_MothorCLK::Elapsed(void)
{
  LARGE_INTEGER Count_present; // Count of the CLK at present call. [count]

  QueryPerformanceCounter(&Count_present);

  return (double)(Count_present.QuadPart - Count_start) / CLK_Frequency;
}

///-----[TexART_MothorCLK::Lap]---------------------------------------------
double TexART_MothorCLK::Lap0(void)
{
  LARGE_INTEGER Count_present; // Count of the CLK at present call. [count]
  long long     lap_count;     // [count]

  QueryPerformanceCounter(&Count_present);
  lap_count      = Count_present.QuadPart - Count_lastLap0;
  Count_lastLap0 = Count_present.QuadPart;

  return (double)lap_count / CLK_Frequency;
}
double TexART_MothorCLK::Lap1(void)
{
  LARGE_INTEGER Count_present; // Count of the CLK at present call. [count]
  long long     lap_count;     // [count]

  QueryPerformanceCounter(&Count_present);
  lap_count      = Count_present.QuadPart - Count_lastLap1;
  Count_lastLap1 = Count_present.QuadPart;

  return (double)lap_count / CLK_Frequency;
}
double TexART_MothorCLK::Lap2(void)
{
  LARGE_INTEGER Count_present; // Count of the CLK at present call. [count]
  long long     lap_count;     // [count]

  QueryPerformanceCounter(&Count_present);
  lap_count      = Count_present.QuadPart - Count_lastLap2;
  Count_lastLap2 = Count_present.QuadPart;

  return (double)lap_count / CLK_Frequency;
}
double TexART_MothorCLK::Lap3(void)
{
  LARGE_INTEGER Count_present; // Count of the CLK at present call. [count]
  long long     lap_count;     // [count]

  QueryPerformanceCounter(&Count_present);
  lap_count      = Count_present.QuadPart - Count_lastLap3;
  Count_lastLap3 = Count_present.QuadPart;

  return (double)lap_count / CLK_Frequency;
}
double TexART_MothorCLK::Lap4(void)
{
  LARGE_INTEGER Count_present; // Count of the CLK at present call. [count]
  long long     lap_count;     // [count]

  QueryPerformanceCounter(&Count_present);
  lap_count      = Count_present.QuadPart - Count_lastLap4;
  Count_lastLap4 = Count_present.QuadPart;

  return (double)lap_count / CLK_Frequency;
}
double TexART_MothorCLK::Lap5(void)
{
  LARGE_INTEGER Count_present; // Count of the CLK at present call. [count]
  long long     lap_count;     // [count]

  QueryPerformanceCounter(&Count_present);
  lap_count      = Count_present.QuadPart - Count_lastLap5;
  Count_lastLap5 = Count_present.QuadPart;

  return (double)lap_count / CLK_Frequency;
}
double TexART_MothorCLK::Lap6(void)
{
  LARGE_INTEGER Count_present; // Count of the CLK at present call. [count]
  long long     lap_count;     // [count]

  QueryPerformanceCounter(&Count_present);
  lap_count      = Count_present.QuadPart - Count_lastLap6;
  Count_lastLap6 = Count_present.QuadPart;

  return (double)lap_count / CLK_Frequency;
}
double TexART_MothorCLK::Lap7(void)
{
  LARGE_INTEGER Count_present; // Count of the CLK at present call. [count]
  long long     lap_count;     // [count]

  QueryPerformanceCounter(&Count_present);
  lap_count      = Count_present.QuadPart - Count_lastLap7;
  Count_lastLap7 = Count_present.QuadPart;

  return (double)lap_count / CLK_Frequency;
}
double TexART_MothorCLK::Lap8(void)
{
  LARGE_INTEGER Count_present; // Count of the CLK at present call. [count]
  long long     lap_count;     // [count]

  QueryPerformanceCounter(&Count_present);
  lap_count      = Count_present.QuadPart - Count_lastLap8;
  Count_lastLap8 = Count_present.QuadPart;

  return (double)lap_count / CLK_Frequency;
}
double TexART_MothorCLK::Lap9(void)
{
  LARGE_INTEGER Count_present; // Count of the CLK at present call. [count]
  long long     lap_count;     // [count]

  QueryPerformanceCounter(&Count_present);
  lap_count      = Count_present.QuadPart - Count_lastLap9;
  Count_lastLap9 = Count_present.QuadPart;

  return (double)lap_count / CLK_Frequency;
}

///-----[TexART_MothorCLK::Read_Frequency]-------------------------------
long long TexART_MothorCLK::Read_Frequency(void)
{
  return CLK_Frequency;
}

///-----[TexART_MothorCLK::Set_Frequency]--------------------------------
void TexART_MothorCLK::Set_Frequency(long long frequency)
{
  CLK_Frequency = frequency;
}



#include "stdafx.h"
#include "Benchmarks.h"
#include "Engine.h"
#include "ITimer.h"


#include <windows.h>
#include <psapi.h>

#include <Pdh.h>
#include <TChar.h>

namespace mray
{
	class Average
	{
		double samples[30];
		int current;
		int length;
	public:
		Average(int len=30)
		{
			length = 30;
			current = 0;
			for (int i = 0; i < length; ++i)
				samples[i] = 0;
		}
		double Add(double v)
		{
			samples[current] = v;
			current = (current + 1) % length;
			double ret = 0;

			for (int i = 0; i < length; ++i)
				ret += samples[i];
			return ret / (double)length;
		}

	};

	class BVirtualMemoryUsage :public IBenchmark
	{
		Average v;
		double getCurrentValue(){



			PROCESS_MEMORY_COUNTERS_EX pmc;
			GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
			SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;

			return v.Add(virtualMemUsedByMe);
		}
	};
	class BPhysicalMemoryUsage :public IBenchmark
	{
		Average v;
		double getCurrentValue(){

			PROCESS_MEMORY_COUNTERS_EX pmc;
			GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
			SIZE_T virtualMemUsedByMe = pmc.WorkingSetSize;

			return v.Add(virtualMemUsedByMe);
		}
	};
	class BCPUCurrentUsage :public IBenchmark
	{
		 PDH_HQUERY cpuQuery;
		 PDH_HCOUNTER cpuTotal;
		 Average v;

	public:
		void init(){
			PdhOpenQuery(NULL, NULL, &cpuQuery);
			// You can also use L"\\Processor(*)\\% Processor Time" and get individual CPU values with PdhGetFormattedCounterArray()
			PdhAddEnglishCounter(cpuQuery, "\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
			PdhCollectQueryData(cpuQuery);
		}

		double getCurrentValue(){
			PDH_FMT_COUNTERVALUE counterVal;

			PdhCollectQueryData(cpuQuery);
			PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
			return v.Add(counterVal.doubleValue);
		}

	};

	class BCPUProcessUsage :public IBenchmark
	{
		ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
		int numProcessors;
		HANDLE self;


		Average _value;
		double lastV;

		ulong lastT;

	public:
		void init(){
			SYSTEM_INFO sysInfo;
			FILETIME ftime, fsys, fuser;

			lastT = gEngine.getTimer()->getMilliseconds();

			GetSystemInfo(&sysInfo);
			numProcessors = sysInfo.dwNumberOfProcessors;

			GetSystemTimeAsFileTime(&ftime);
			memcpy(&lastCPU, &ftime, sizeof(FILETIME));

			self = GetCurrentProcess();
			GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
			memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
			memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));

			lastV = 0;
		}

		double getCurrentValue(){

			ulong t=gEngine.getTimer()->getMilliseconds();
			if (t - lastT < 1000)
				return lastV;
			lastT = t;

			FILETIME ftime, fsys, fuser;
			ULARGE_INTEGER now, sys, user;
			double percent;

			GetSystemTimeAsFileTime(&ftime);
			memcpy(&now, &ftime, sizeof(FILETIME));

			GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
			memcpy(&sys, &fsys, sizeof(FILETIME));
			memcpy(&user, &fuser, sizeof(FILETIME));
			percent = (sys.QuadPart - lastSysCPU.QuadPart) +
				(user.QuadPart - lastUserCPU.QuadPart);
			percent /= (now.QuadPart - lastCPU.QuadPart);
			percent /= numProcessors;
			lastCPU = now;
			lastUserCPU = user;
			lastSysCPU = sys;
			lastV = percent * 100;
			return lastV;
			return _value.Add(percent * 100);
		}

	};
	Benchmarks::Benchmarks(){
		this->CPUCurrentUsage = new BCPUCurrentUsage();
		this->CPUProcessUsage = new BCPUProcessUsage();
		this->VirtualMemoryUsage = new BVirtualMemoryUsage();
		this->PhysicalMemoryUsage = new BPhysicalMemoryUsage();

		CPUCurrentUsage->init();
		CPUProcessUsage->init();
		VirtualMemoryUsage->init();
		PhysicalMemoryUsage->init();
	}
	Benchmarks::~Benchmarks()
	{
		delete CPUCurrentUsage;
		delete CPUProcessUsage;
		delete VirtualMemoryUsage;
		delete PhysicalMemoryUsage;
	}

}


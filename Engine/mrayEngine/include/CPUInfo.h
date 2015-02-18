// Camel - CPU Identifying Tool
// Copyright (C) 2002, Iain Chesworth
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef _CPUINFO_H_
#define _CPUINFO_H_

#include "common.h"
#include "ISingleton.h"
#include "GCPtr.h"

#ifdef _WIN32
// Include Windows header files.
#include <windows.h>
#endif // _WIN32

// Include generic C / C++ header files.
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

namespace mray{


#define VENDOR_STRING_LENGTH		(12 + 1)
#define CHIPNAME_STRING_LENGTH		(48 + 1)
#define SERIALNUMBER_STRING_LENGTH	(29 + 1)
#define TLBCACHE_INFO_UNITS			(15)

#define CPUID_AWARE_COMPILER
#ifdef CPUID_AWARE_COMPILER
	#define CPUID_INSTRUCTION		cpuid
#else
	#define CPUID_INSTRUCTION		_asm _emit 0x0f _asm _emit 0xa2
#endif

enum ECPUFeature{

	MMX_FEATURE					=BIT(0)
	,MMX_PLUS_FEATURE			=BIT(1)
	,SSE_FEATURE				=BIT(2)
	,SSE2_FEATURE				=BIT(3)
	,AMD_3DNOW_FEATURE			=BIT(4)
	,AMD_3DNOW_PLUS_FEATURE		=BIT(5)
	,IA64_FEATURE				=BIT(6)
	,MP_CAPABLE					=BIT(7)
	,HYPERTHREAD_FEATURE		=BIT(8)
	,SERIALNUMBER_FEATURE		=BIT(9)
	,APIC_FEATURE				=BIT(10)
	,SSE_FP_FEATURE				=BIT(11)
	,SSE_MMX_FEATURE			=BIT(12)
	,CMOV_FEATURE				=BIT(13)
	,MTRR_FEATURE				=BIT(14)
	,L1CACHE_FEATURE			=BIT(15)
	,L2CACHE_FEATURE			=BIT(16)
	,L3CACHE_FEATURE			=BIT(17)
	,ACPI_FEATURE				=BIT(18)
	,THERMALMONITOR_FEATURE		=BIT(19)
	,TEMPSENSEDIODE_FEATURE		=BIT(20)
	,FREQUENCYID_FEATURE		=BIT(21)
	,VOLTAGEID_FREQUENCY		=BIT(22)

};

typedef	void (*DELAY_FUNC)(unsigned int uiMS);


class CPUInfo:public ISingleton<CPUInfo> {
public:
	CPUInfo ();
	~CPUInfo ();

	char * GetVendorString ();
	char * GetVendorID ();
	char * GetTypeID ();
	char * GetFamilyID ();
	char * GetModelID ();
	char * GetSteppingCode ();
	char * GetExtendedProcessorName ();
	char * GetProcessorSerialNumber ();
	int GetLogicalProcessorsPerPhysical ();
	int GetProcessorClockFrequency ();
	int GetProcessorAPICID ();
	int GetProcessorCacheXSize (DWORD);
	bool DoesCPUSupportFeature (DWORD);

	bool __cdecl DoesCPUSupportCPUID ();


private:
	class CPUSpeed {
	public:
		CPUSpeed ();
		~CPUSpeed ();

		// Variables.
		int CPUSpeedInMHz;

		// Functions.
		__int64 __cdecl GetCyclesDifference (DELAY_FUNC, unsigned int);

	private:
		// Functions.
		static void Delay (unsigned int);
		static void DelayOverhead (unsigned int);

	protected:

	};

	typedef struct tagID {
		int Type;
		int Family;
		int Model;
		int Revision;
		int ExtendedFamily;
		int ExtendedModel;
		char ProcessorName[CHIPNAME_STRING_LENGTH];
		char Vendor[VENDOR_STRING_LENGTH];
		char SerialNumber[SERIALNUMBER_STRING_LENGTH];
	} ID;

	typedef struct tagCPUPowerManagement {
		bool HasVoltageID;
		bool HasFrequencyID;
		bool HasTempSenseDiode;
	} CPUPowerManagement;

	typedef struct tagCPUExtendedFeatures {
		bool Has3DNow;
		bool Has3DNowPlus;
		bool SupportsMP;
		bool HasMMXPlus;
		bool HasSSEMMX;
		bool SupportsHyperthreading;
		int LogicalProcessorsPerPhysical;
		int APIC_ID;
		CPUPowerManagement PowerManagement;
	} CPUExtendedFeatures;	
	
	typedef struct CPUtagFeatures {
		bool HasFPU;
		bool HasTSC;
		bool HasMMX;
		bool HasSSE;
		bool HasSSEFP;
		bool HasSSE2;
		bool HasIA64;
		bool HasAPIC;
		bool HasCMOV;
		bool HasMTRR;
		bool HasACPI;
		bool HasSerial;
		bool HasThermal;
		int CPUSpeed;
		int L1CacheSize;
		int L2CacheSize;
		int L3CacheSize;
		CPUExtendedFeatures ExtendedFeatures;
	} CPUFeatures;
    
	enum Manufacturer {
		AMD, Intel, NSC, UMC, Cyrix, NexGen, IDT, Rise, Transmeta, UnknownManufacturer
	};

	// Functions.
	bool __cdecl RetrieveCPUFeatures ();
	bool __cdecl RetrieveCPUIdentity ();
	bool __cdecl RetrieveCPUCacheDetails ();
	bool __cdecl RetrieveClassicalCPUCacheDetails ();
	bool __cdecl RetrieveCPUClockSpeed ();
	bool __cdecl RetrieveClassicalCPUClockSpeed ();
	bool __cdecl RetrieveCPUExtendedLevelSupport (int);
	bool __cdecl RetrieveExtendedCPUFeatures ();
	bool __cdecl RetrieveProcessorSerialNumber ();
	bool __cdecl RetrieveCPUPowerManagement ();
	bool __cdecl RetrieveClassicalCPUIdentity ();
	bool __cdecl RetrieveExtendedCPUIdentity ();
	
	// Variables.
	Manufacturer ChipManufacturer;
	CPUFeatures Features;
	GCPtr<CPUSpeed>  Speed;
	ID ChipID;
	
protected:

};

}

#endif // _CPUINFO_H_
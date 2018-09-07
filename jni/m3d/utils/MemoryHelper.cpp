#include "m3d/M3D.h"
#include "m3d/utils/MemoryHelper.h"
#include <limits>

#ifdef  __IOS__
#include <sys/sysctl.h>
#import <mach/mach.h> 
#endif
#ifdef _WIN32 
#include "Log4Native.h"
using Log4Native::Logger;
#endif
using namespace std;

#ifdef _WIN32
#include <iostream>
#include <windows.h>



//宏定义，解决在XP系统上出现无法定位程序输入点 K32GetProcessMemoryInfo
#ifndef PSAPI_VERSION
#define PSAPI_VERSION 1
#endif

#include <psapi.h>
#pragma comment(lib,"psapi.lib")

typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
#endif

#ifdef  __ANDROID__
#include <sys/sysinfo.h>

#include <stdio.h>

#include <unistd.h>

#include <sys/syscall.h>

#endif

namespace M3D {
#define MB (1024ULL*1024ULL)
	MemoryHelper* MemoryHelper::instance = NULL;

	unsigned long long 	MemoryHelper::MAX_MEMORY_SIZE = 1024ULL * MB; //1024内存
	unsigned long long  MemoryHelper::MAX_PHYS_MEMORY_SIZE = 100ULL * 1024ULL * MB;
	unsigned long long MemoryHelper::MAX_PHYS_AVIL_MEMORY_SIZE = 100ULL * 1024ULL * MB;
	unsigned long long MemoryHelper::MAX_32_MEMORY_SIZE = 12ULL * 1024ULL * MB / 10ULL;
	unsigned long long MemoryHelper::MAX_FREE_MEMORY_SIZE = 100ULL * MB; //预留空间大小

	unsigned long long MemoryHelper::LIMIT_FREE_MEMORY_SIZE = 300ULL * MB;

#ifdef _WIN32
	unsigned long long MemoryHelper::OPTIMIZE_LIMIT_FREE_MEMORY_SIZE = 400ULL * MB;
#endif
#ifdef  __ANDROID__
	unsigned long long MemoryHelper::OPTIMIZE_LIMIT_FREE_MEMORY_SIZE = 10ULL * MB;
#endif
#ifdef  __IOS__
	unsigned long long MemoryHelper::OPTIMIZE_LIMIT_FREE_MEMORY_SIZE = 10ULL * MB;
#endif

	M3D::MemoryHelper* MemoryHelper::Instance()
	{
		if (!MemoryHelper::instance)
		{
			MemoryHelper::instance = new MemoryHelper();
			instance->InitSystemInfo();
		}
		return MemoryHelper::instance;
	}

	MemoryHelper::MemoryHelper()
	{
		init = false;
		bIsWow64 = false;
		bIsProcess64 = false;
		processFreeMemory = MAX_MEMORY_SIZE;
		processUsedMemory = 0;
		checkTickCount = 0;
		memoryStateLazzy = M3D_Memory_Rich;
		bIsUpdating = true;
	}

	MemoryHelper::~MemoryHelper()
	{

	}

	unsigned long long MemoryHelper::GetFreeMemorySize()
	{
		UpdateFreeMemoryInfo();
		return this->processFreeMemory;
	}

	string MemoryHelper::ProcessMemoryInfo()
	{
		char processInfoChar[128] = { 0 };
		string processInfo;
#ifdef _WIN32
		HANDLE handle = GetCurrentProcess();
		PROCESS_MEMORY_COUNTERS pmc;
		if (GetProcessMemoryInfo(handle, &pmc, sizeof(pmc)))
		{
			sprintf(processInfoChar, "Current MaxMun Memory:%d M  ", pmc.PeakWorkingSetSize / 1024 / 1024);
			processInfo += string(processInfoChar);
			memset(processInfoChar, 0, 128);
			sprintf(processInfoChar, "CurrentMemory:%d M  ", pmc.WorkingSetSize / 1024 / 1024);
			processInfo += string(processInfoChar);
		}
#endif
		return processInfo;
	}

	M3D_MemoryState MemoryHelper::GetMemoryStateLazzy()
	{
		if (this->bIsUpdating)
		{
			return M3D_MemoryState::M3D_Memory_Rich;
		}
		return this->memoryStateLazzy;
	}

	void MemoryHelper::SetMemoryStateLazzy(M3D_MemoryState memoryState)
	{
		this->memoryStateLazzy = memoryState;
	}

	M3D::M3D_MemoryState MemoryHelper::GetMemoryRealTimeState()
	{
		memoryStateLazzy = M3D_Memory_Rich;
		UpdateFreeMemoryInfo();
		//剩余内存超过100M
		if (processFreeMemory < LIMIT_FREE_MEMORY_SIZE)
		{
			memoryStateLazzy = M3D_Memory_Poor;
		}
 
		return memoryStateLazzy;
	}

	bool MemoryHelper::IsMemoryRichForOptimize()
	{
		bool rich = true;
		UpdateFreeMemoryInfo();
 
		if (processFreeMemory < OPTIMIZE_LIMIT_FREE_MEMORY_SIZE)
		{
			rich = false;
		}

		return rich;
	}

	void MemoryHelper::SetLimitFreeMemorySize(int memorySize)
	{
		MemoryHelper::LIMIT_FREE_MEMORY_SIZE = memorySize*MB;
	}

	void MemoryHelper::SetOptimizeLimitFreeMemorySize(int optimizeMemorySize)
	{
		MemoryHelper::OPTIMIZE_LIMIT_FREE_MEMORY_SIZE = optimizeMemorySize*MB;
	}

	bool MemoryHelper::IsMemoryRich()
	{
		bool rich = true;
		memoryStateLazzy = M3D_Memory_Rich;
		UpdateFreeMemoryInfo();
		if (processFreeMemory < LIMIT_FREE_MEMORY_SIZE)
		{
			memoryStateLazzy = M3D_Memory_Poor;
		}
		return memoryStateLazzy == M3D_Memory_Rich;
	}

	bool MemoryHelper::IsMemoryRich(int allocMemorySize)
	{
		return true;
		memoryStateLazzy = M3D_Memory_Rich;
		UpdateFreeMemoryInfo();
		if (processFreeMemory+ allocMemorySize < LIMIT_FREE_MEMORY_SIZE)
		{
			memoryStateLazzy = M3D_Memory_Poor;
		}
		return memoryStateLazzy == M3D_Memory_Rich;
	}
 
	bool MemoryHelper::PreAllocFreeMemory(int allocMemorySize)
	{
		availPhysMomory -= allocMemorySize;
		if (availPhysMomory > MAX_FREE_MEMORY_SIZE)
		{
			return true;
		}
		return false;
	}

	bool MemoryHelper::IsWow64()
	{
		return bIsWow64;
	}

	void MemoryHelper::InitSystemInfo()
	{
		if (!init)
		{
			init = true;
#ifdef _WIN32
			bIsWow64 = false;
			typedef VOID(WINAPI *LPFN_GetNativeSystemInfo)(__out LPSYSTEM_INFO lpSystemInfo);
			LPFN_GetNativeSystemInfo fnGetNativeSystemInfo = (LPFN_GetNativeSystemInfo)GetProcAddress(GetModuleHandleW(L"kernel32"), "GetNativeSystemInfo");
			if (fnGetNativeSystemInfo)
			{
				SYSTEM_INFO stInfo = { 0 };
				fnGetNativeSystemInfo(&stInfo);
				if (stInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64
					|| stInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				{
					bIsWow64 = true;
				}
			}
			//HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, GetCurrentProcessId());
			//if (hProcess)
			//{
			//	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
			//	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandleW(L"kernel32"), "IsWow64Process");
			//	if (NULL != fnIsWow64Process)
			//	{
			//		BOOL bIsWow64 = false;
			//		fnIsWow64Process(hProcess, &bIsWow64);
			//		CloseHandle(hProcess);
			//		if (bIsWow64)
			//		{
			//			bIsProcess64 = false;
			//		}
			//		else
			//		{
			//			bIsProcess64 = true;
			//		}
			//	}
			//}
			UpdateFreeMemoryInfo();
#else
			physMemory = MAX_PHYS_AVIL_MEMORY_SIZE;
			availPhysMomory = MAX_PHYS_AVIL_MEMORY_SIZE; //可用物理内存:
#endif
			LOGI("Is 64 Version %d",this->Is64BitPorcess());
			LOGI("physMemory %d processFreeMemory %d", (int)(physMemory / MB), (int)(processFreeMemory / MB));
		}
	}

	bool MemoryHelper::Is64BitPorcess()
	{
#ifdef PROCESS32
		return false;
#else 
		return true;
#endif
	}

	void MemoryHelper::UpdateFreeMemoryInfo()
	{
#ifdef _WIN32
		HANDLE handle = GetCurrentProcess();
		PROCESS_MEMORY_COUNTERS pmc;
		if (GetProcessMemoryInfo(handle, &pmc, sizeof(pmc)))
		{
			processUsedMemory = pmc.WorkingSetSize;
		}

		//系统物理内存信息
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);
		GlobalMemoryStatusEx(&statex);
		//总物理内存
		//statex.dwMemoryLoad; //物理内存使用率:
		physMemory = statex.ullTotalPhys;
		availPhysMomory = statex.ullAvailPhys; //可用物理内存:

		if (!this->Is64BitPorcess())
		{
			if (availPhysMomory > MAX_32_MEMORY_SIZE - processUsedMemory)
			{
				availPhysMomory = MAX_32_MEMORY_SIZE - processUsedMemory;
			}
		}

		processFreeMemory = availPhysMomory;
#else
        #ifdef __IOS__
        vm_statistics_data_t vmStats;
        
        mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
        
        kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);
        
        if(kernReturn != KERN_SUCCESS)
        {
            processFreeMemory = MAX_PHYS_AVIL_MEMORY_SIZE;
        }else{
            processFreeMemory =  (vm_page_size * vmStats.free_count);// / 1024.0) / 1024.0;
        }
        #endif

#ifdef  __ANDROID__
        struct sysinfo si;
        int rc;
        bzero(&si, sizeof(struct sysinfo));
        rc = syscall(__NR_sysinfo, &si);
//
//        LOGI("Totalram ->:       %d\n", si.totalram);
//        LOGI("Available ->:       %d\n", si.freeram);
//        LOGI("Available ->:     %d\n", (si.freeram/1024)/1024);

        	processFreeMemory = si.freeram;


#endif

#endif

	}

	void MemoryHelper::LogMemoryState()
	{
			UpdateFreeMemoryInfo();
			LOGI("physMemory %d processFreeMemory %d", (int)(physMemory / MB), (int)(processFreeMemory / MB));
	}

}

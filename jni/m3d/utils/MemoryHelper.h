/**@file
 *@brief	
 *@author	XL
 *@date
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_MEMORY_HELPER_H_
#define M3D_MEMORY_HELPER_H_
#include <string>
#include <iostream>

namespace M3D{
enum M3D_API M3D_MemoryState {
		M3D_Memory_Rich = 1,//内存充足
		M3D_Memory_Alloc_Bad,//内存分配失败
		M3D_Memory_Poor, //内存不足
	};
/**
 * @class TODO
 * @brief 检测内存操作帮助类
 */
class M3D_API MemoryHelper
{
public:
	static MemoryHelper* Instance();

public:
	MemoryHelper();
	virtual ~MemoryHelper();
	/**
	 * 得到系统空闲内存的大小
	 * @return
	 */
	unsigned long long GetFreeMemorySize();

	string ProcessMemoryInfo();

	/*
	* 2 内存不足
	* 3 申请内存失败
	* 4
	*/
	M3D_MemoryState GetMemoryStateLazzy();
	void SetMemoryStateLazzy(M3D_MemoryState MemoryState);

	//小于500M可用内存，默认认为内存不足
	M3D_MemoryState GetMemoryRealTimeState();

	bool IsMemoryRichForOptimize();

	void SetLimitFreeMemorySize(int memorySize);
	void SetOptimizeLimitFreeMemorySize(int optimizeMemorySize);

	/**
	* 得到的内存是否充足
	* @return
	*/
	bool IsMemoryRich();
 
	/**
	* 判断申请指定内存后，进程内存是否有剩余
	* @return
	*/
	bool IsMemoryRich(int allocMemorySize);

	//判断是否还能够申请到内存
	bool PreAllocFreeMemory(int allocMemorySize);
	/**
	 * 得到当前操作系统是32位 还是 64位
	 * @param image
	 * @param format
	 * @return
	 */
	bool IsWow64();

	bool Is64BitPorcess();

	void UpdateFreeMemoryInfo();

	void LogMemoryState();

private:
	void InitSystemInfo();
private:
	bool init; //是否初始化
	bool bIsWow64; //64位系统
	bool bIsProcess64; //64为进程
	bool bIsUpdating;
	unsigned long long processFreeMemory; //进程可用内存 Byte
	unsigned long long processUsedMemory;//当前进程已经使用的内存 Byte

	unsigned long long physMemory; //总物理内存
	unsigned long long availPhysMomory;//可用物理内存

	int checkTickCount;//
	M3D_MemoryState memoryStateLazzy;

	static unsigned long long MAX_PHYS_MEMORY_SIZE;
	static unsigned long long MAX_PHYS_AVIL_MEMORY_SIZE;
	static unsigned long long MAX_32_MEMORY_SIZE;
	static unsigned long long MAX_FREE_MEMORY_SIZE;

	static unsigned long long LIMIT_FREE_MEMORY_SIZE;
	static unsigned long long OPTIMIZE_LIMIT_FREE_MEMORY_SIZE;

	static unsigned long long MAX_MEMORY_SIZE;

	static MemoryHelper* instance;
};
}
#endif /*M3D_IMAGE_HELPER_H_*/

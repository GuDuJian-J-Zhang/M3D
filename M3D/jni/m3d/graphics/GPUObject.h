/**@file
 *@brief	GL GPU 对象基类
 *@author	XL
 *@date
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_GPU_OBJ_H_
#define M3D_GPU_OBJ_H_

#include "m3d/graphics/Resource.h"
#include "m3d/renderer/GLESHead.h"

namespace M3D
{
class ResourceManager;

/**
 * @class
 * @brief GPU对象管理类
 */
class M3D_API GPUObject: public Resource
{
public:
	static const int DISK_CACHE;//!<硬盘缓存
	static const int GPU_CACHE;//!<GPU缓存
	static const int NO_CACHE;//!<不使用缓存

public:
	/**
	 * @brief 获取预分配的内存
	 * @param size
	 * @return
	 */
	static char* GetPriAllocMemory(unsigned size);
	static void FreePriAllocMemory();
public:
	static char* priAllocMemory;//!<预分配内存指针
	static unsigned priAllocMemorySize;//!<预分配内存大小

public:
	GPUObject();
	virtual ~GPUObject();

	/**
	 * 获取GPU对象
	 * @return
	 */
	GLuint GetObject();
 
private:

	/**
	 * 初始化
	 */
	void Init();
protected:
	unsigned m_object; //!< GPU 存储对象的ID
};
}

#endif /*M3D_GPU_OBJ_H_*/

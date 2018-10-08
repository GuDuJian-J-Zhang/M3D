/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef     M3D_OBJECT_H_
#define     M3D_OBJECT_H_
#include 	"m3d/M3D.h"

namespace M3D
{
/**
 * @class 计数引用内存管理
 */
class Object
{
public:
	static int OBJID;
	static int TEST_OBJ_COUNT;
public:
	/**
	 * @brief
	 */
	Object();
	/** 
	 * @brief
	 */
	virtual ~Object();

	/**
	 * @brief 引用计数加一
	 */
	void AddRef(void);
	/**
	 * @brief 引用计数减一
	 */
	virtual void Release(void);
	/**
	 * @brief
	 */
	virtual string GetStates();
	/**
	 * @brief
	 */
	virtual bool SetStates(const string& states);

protected:
	/** 
	 * @brief
	 */
	virtual void FinalRelease(void);

protected:
	int m_cRefrenceCount;
};

}
#endif


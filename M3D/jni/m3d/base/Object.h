/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef     M3D_OBJECT_H_
#define     M3D_OBJECT_H_
#include 	"m3d/M3DExport.h"
#include    "m3d/M3DMacros.h"
namespace M3D
{
	template <typename T>
	inline void ReleaseMe(T& a)
	{
		if (a)
		{
			a->Release();
			a = NULL;
		}
	};

	template <typename T>
	inline void AddRefMe(T const& a)
	{
		if (a)
		{
			a->AddRef();
		}
	};

class M3D_API IObject
{
public:
	/**
	* @brief 引用计数加一
	*/
	virtual void AddRef(void) = 0;
	/**
	* @brief 引用计数减一
	*/
	virtual void Release(void) = 0;
};

/**
 * @class 计数引用内存管理
 */
class M3D_API Object:public IObject
{
public:
	static IDTYPE OBJID;
	static IDTYPE TEST_OBJ_COUNT;
public:
	/**
	 * @brief
	 */
	Object();
	/** 
	 * @brief
	 */
	virtual ~Object();

	Object(const Object& orig);
	Object& operator =(const Object& orig);



	/**
	 * @brief 引用计数加一
	 */
	virtual void AddRef(void);
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
    
    int GetRefrenceCount();

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


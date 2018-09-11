/**@file
 *@brief    @see M3D::GlHelper
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_GLHELPER10_H_
#define M3D_GLHELPER10_H_
#include "m3d/renderer/RenderContext.h"

namespace M3D
{
class M3D_API GL10Context: public RenderContext
{
public:
	friend class RenderContext;
	virtual ~GL10Context();
	virtual void OptimizeContext();
	/**
	 * 获取上下文类型
	 * @return
	 */
	virtual int GetContextType();

	/**
	 * 获取矩阵
	 * @param pname
	 * @param params
	 */
	virtual void GetDoublev(GLenum pname, double *params);
	/**
	 * 获取矩阵
	 * @param pname
	 * @param params
	 */
	virtual void GetFloatv(GLenum pname, float *params);
	virtual void GetDoublev(GLenum pname, Matrix4& m);
	virtual void GetFloatv(GLenum pname, Matrix4& m);

private:
	GL10Context();
	bool initialized;
	int m_deviceSupportMsaaNum;
};
}

#endif /*M3D_GLHELPER10_H_*/

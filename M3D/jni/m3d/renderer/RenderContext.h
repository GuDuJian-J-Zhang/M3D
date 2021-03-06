﻿/**@file
 *@brief    GL基本的矩阵操作封装实现
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_GLHELPER_H_
#define M3D_GLHELPER_H_

#include "m3d/M3D.h"
#include "m3d/renderer/GLESHead.h"
#include "m3d/base/Vector3.h"
#include "m3d/model/Line3D.h"

#include "m3d/base/Matrix4.h"
#ifdef _DEBUG
#define M3D_GL_ERROR_CHECK RenderContext::OgldevError();
#else
#define M3D_GL_ERROR_CHECK
#endif

namespace M3D
{
class CameraState;
class ControlInfo;

/**
 * @class
 * @brief 该类描述了渲染上下文
 */
class M3D_API RenderContext
{
public:
	static RenderContext* GetContext(int ContextType);

	RenderContext();
	virtual ~RenderContext();

	virtual int GetContextType()=0;

	/**
	* 根据当前硬件水平优化上下文
	*/
	virtual void OptimizeContext() = 0;

	/**
	 * 设置视图矩阵
	 * @param viewMatrix
	 */
	void SetViewMatrix(const Matrix4& viewMatrix);
	/**
	 * 设置投影矩阵
	 * @param projectMatrix
	 */
	void SetProjectMatrix(const Matrix4& projectMatrix);

	/************************************************************************/
	/* 检测opengl扩展函数                                                                     */
	/************************************************************************/
	static GLboolean CheckExtension(char *extName, const GLubyte *extString);
	//************************************
	// Method:    OgldevError
	// FullName:  M3D::RenderContext::OgldevError
	// Access:    public static 
	// Returns:   void
	// Qualifier: 检测opengl错误
	//************************************
	static void OgldevError();

	Matrix4& GetViewMatrix();
	Matrix4& GetProjectMatrix();

	Matrix4& GetModelMatrix();
public:
	const static int GL10 = 1;//!<ES 1.0
	const static int GL20 = 2;//!< ES 2.0

	const static int MODELVIEWMATRIX = 0;
	const static int PROJECTMATRIX = 1;

	static bool IsFramebufferSupported;

private:
	Matrix4 m_viewMatrix; //!<视图矩阵
	Matrix4 m_projectMatrix;//!<投影矩阵

	Matrix4 m_modelMatrixCache;//!<模型矩阵缓存
private:
	int GLType;
};
}

#endif /*M3D_GLHELPER_H_*/

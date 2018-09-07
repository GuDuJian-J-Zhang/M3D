/**@file
 *@brief    
 *@author	CDJ 
 *@date		2016-3-23
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_GLRENDERTARGET_H_
#define M3D_GLRENDERTARGET_H_
#include"m3d/M3D.h"
#include "m3d/renderer/GLESHead.h"
namespace M3D
{
/**
 * @brief 渲染目标类型
 */
enum RenderTargetType
{
	DEFALUT_TARGET = -1,//!< DEFALUT_TARGET
	TEXTURE_TARGET  = 0,//!< TEXTURE_TARGET
	RBO_TARGET = 1,     //!< RBO_TARGET
};

///渲染目标的抽象，确定渲染目标类型
/**
 * @class
 * @brief
 */
class M3D_API GLRenderTarget
{
public:
	GLRenderTarget();
	virtual ~GLRenderTarget();

	/**
	 * 获取类型
	 * @return
	 */
	RenderTargetType GetTargetType()
	{
		return m_targetType;
	}

	/**
	 * 设置类型
	 * @param type
	 */
	void SetTargetType(RenderTargetType type)
	{
		m_targetType = type;
	}

protected:
	RenderTargetType m_targetType;//!<渲染目标

};

} /* namespace M3D */
#endif /* M3D_GLRENDERTARGET_H_ */

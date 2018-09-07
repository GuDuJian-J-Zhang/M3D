/**@file
 *@brief
 *@author	CDJ
 *@date		2016-4-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_HARDWARE_RENDERBUFFER_H_
#define M3D_HARDWARE_RENDERBUFFER_H_

#include "m3d/graphics/GPUObject.h"
#include "m3d/renderer/gl20/GLRenderTarget.h"

namespace M3D
{
//class GLRenderTarget;
/**
 * @class
 * @brief RBO。用来当FBO的挂载对象
 */
class M3D_API HardWareRenderBuffer: public M3D::GPUObject,public GLRenderTarget
{
public:
	HardWareRenderBuffer(GLenum internalFormat,int width,int height );
	virtual ~HardWareRenderBuffer();
	/**
	 * @brief 创建
	 */
	void Create(bool useMS,int sample);
	/**
	 * @brief 绑定
	 */
	void Bind();
	/**
	 * @brief 解绑
	 */
	void Unbind();
private:
	int m_width, m_height;//!<
	 GLenum m_internalFormat; //!<内部格式
};

} /* namespace M3D */
#endif /* M3D_HARDWA_RERENDERBUFFER_H_ */

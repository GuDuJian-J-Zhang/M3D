/**@file
 *@brief
 *@author	CDJ
 *@date		2016-3-29
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_GEOMETRYBUFFER_H_
#define M3D_GEOMETRYBUFFER_H_

#include "m3d/graphics/Texture.h"
#include "m3d/renderer/RenderContext.h"
#include "m3d/renderer/gl20/GLRenderTarget.h"
namespace M3D
{

/**
 * @class
 * @brief 用来创建FBO所需的纹理对象
 */
class M3D_API GeometryBuffer: public M3D::Texture,public GLRenderTarget
{
public:
	GeometryBuffer();
	virtual ~GeometryBuffer();
	/**
	 * @brief 获取类型
	 * @return
	 */
	virtual int GetType()
	{
		return TEXTURE_GEO;
	}
	/**
	 * 加载纹理
	 * @param buffer
	 * @param buffer_length
	 * @param force_channels
	 * @param reuse_texture_ID
	 * @param flags
	 * @return
	 */
	static unsigned int LoadOGLTexture(const unsigned char *const buffer,
			int buffer_length,
			int force_channels,
			unsigned int reuse_texture_ID,
			unsigned int flags);
	virtual void GenerateMipMap();
protected:
	/**
	 * @brief 更新纹理对象
	 */
	virtual void UpdataOGLObj();
	/**
	 * @brief 初始化
	 */
	void Init();


};

} /* namespace M3D */
#endif /* M3D_GEOMETRYBUFFER_H_ */

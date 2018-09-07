/**@file
 *@brief	GL2D纹理
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_TEXTURE2D_H_
#define M3D_TEXTURE2D_H_


#include "m3d/renderer/RenderContext.h"

#include "m3d/graphics/Texture.h"

namespace M3D
{

/**
 * @class
 * @brief 2D纹理类
 */
class M3D_API Texture2D :public Texture
{
public:
	/**
		Loads an image from disk into an OpenGL texture.
		\param filename the name of the file to upload as a texture
		\param force_channels 0-image format, 1-luminous, 2-luminous/alpha, 3-RGB, 4-RGBA
		\param reuse_texture_ID 0-generate a new texture ID, otherwise reuse the texture ID (overwriting the old texture)
		\param flags can be any of SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_DDS_LOAD_DIRECT
		\return 0-failed, otherwise returns the OpenGL texture handle
	**/
	unsigned int LoadOGLTexture(const string& filename,
			int force_channels,
			unsigned int reuse_texture_ID,
			unsigned int flags);

	/**
	 * 从内存中加载纹理
	 * @param buffer
	 * @param buffer_length
	 * @param force_channels
	 * @param reuse_texture_ID
	 * @param flags
	 * @return
	 */
	unsigned int LoadOGLTexture(const unsigned char *const buffer,
			int buffer_length,
			int force_channels,
			unsigned int reuse_texture_ID,
			unsigned int flags);

	bool FreeOGLTexture(unsigned int OGLObj);

	
	//************************************
	// Method:    UpdateImage
	// FullName:  M3D::Texture2D::UpdateImage
	// Access:    public 
	// Returns:   void
	// Qualifier: 更新图片数据
	// Parameter: Image * image
	//************************************
	void UpdateImage(Image* image);
public:
	Texture2D();

	Texture2D(const string& imagePath);

	virtual int GetType()
	{
		return TEXTURE_2D;
	}

	Texture2D(const string& imagePath,int forceChannels,unsigned int flags);

	virtual ~Texture2D();


protected:
	void Init();

protected:
	virtual void UpdataOGLObj();
};

/**
* @class
* @brief 2D纹理类包装类，纹理单元在外部创建，然后通过接口传入
*/
class M3D_API Texture2DPackaging :public Texture
{
public:
	Texture2DPackaging()
	{
		this->m_textures[0] = 0;
	}

	virtual int GetType()
	{
		return TEXTURE_2D;
	}

	virtual ~Texture2DPackaging()
	{

	}

	void SetTexture2DID(GLuint textureid)
	{
		this->m_textures[0] = textureid;
	}

	GLuint GetTexture2DID()
	{
		return this->m_textures[0];
	}

private:

};
}

#endif /*M3D_TEXTURE2D_H_*/

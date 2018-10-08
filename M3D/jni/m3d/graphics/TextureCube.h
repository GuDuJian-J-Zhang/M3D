/**@file
 *@brief	GL纹理贴图封装
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_TEXTURECUBE_H_
#define M3D_TEXTURECUBE_H_

#include "m3d/graphics/Texture.h"

namespace M3D
{

/**
 * @class
 * @brief 立方体纹理类
 */
class M3D_API TextureCube :public Texture
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
	static unsigned int LoadOGLTexture(const string& filename,
			const char face_order[6],
			int force_channels,
			unsigned int reuse_texture_ID,
			unsigned int flags);

	/**
	 * 由6张图片生成纹理
	 * @param filenames
	 * @param force_channels
	 * @return
	 */
	static unsigned int LoadOGLTextureFromSixImage(vector<string> & filenames,int force_channels);

	static bool FreeOGLTexture(unsigned int OGLObj);
public:
	TextureCube();

	TextureCube(const string& imagePath);
	TextureCube(vector<string>& imagePathes);

	virtual int GetType()
	{
		return TEXTURE_CUBE;
	}

	TextureCube(const string& imagePath,int forceChannels,unsigned int flags);

	virtual ~TextureCube();


protected:
	void Init();

protected:
	virtual void UpdataOGLObj();
	vector<string> m_imagePaths;
};
}

#endif /*M3D_TEXTURECUBE_H_*/

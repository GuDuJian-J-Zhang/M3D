/**@file
 *@brief	GL纹理贴图封装
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_TEXTURE3D_H_
#define M3D_TEXTURE3D_H_

#include "m3d/model/Image.h"
#include "m3d/renderer/RenderContext.h"

namespace M3D
{

class Image;
/**@class
 * @brief
 *
 */
class M3D_API Texture
{
public:
	Texture();
	virtual ~Texture();
	void Read(const string& fileName);
	void ReadZip(const string& zipPath, const string& fileName);
	void SetData(const void* data);
public:
	GLuint m_textures[1];
	bool CreateOGLTexture();
	Image* m_image;
	static string APKPath;
};
}

#endif /*M3D_TEXTURE3D_H_*/

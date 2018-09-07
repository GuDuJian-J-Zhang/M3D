/**@file
 *@brief	GL纹理贴图封装
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_TEXTURE_H_
#define M3D_TEXTURE_H_

#include "m3d/model/Image.h"
#include "m3d/renderer/RenderContext.h"
#include "m3d/graphics/Resource.h"

namespace M3D
{
enum
{
	TEXTURE_LOAD_AUTO = 0,
	TEXTURE_LOAD_L = 1,
	TEXTURE_LOAD_LA = 2,
	TEXTURE_LOAD_RGB = 3,
	TEXTURE_LOAD_RGBA = 4
};

/**
	Passed in as reuse_texture_ID, will cause SOIL to
	register a new texture ID using glGenTextures().
	If the value passed into reuse_texture_ID > 0 then
	SOIL will just re-use that texture ID (great for
	reloading image assets in-game!)
**/
enum
{
	TEXTURE_CREATE_NEW_ID = 0
};

/**
	flags you can pass into SOIL_load_OGL_texture()
	and SOIL_create_OGL_texture().
	(note that if SOIL_FLAG_DDS_LOAD_DIRECT is used
	the rest of the flags with the exception of
	TEXTURE_FLAG_TEXTURE_REPEATS will be ignored while
	loading already-compressed DDS files.)

	SOIL_FLAG_POWER_OF_TWO: force the image to be POT
	SOIL_FLAG_MIPMAPS: generate mipmaps for the texture
	SOIL_FLAG_TEXTURE_REPEATS: otherwise will clamp
	SOIL_FLAG_MULTIPLY_ALPHA: for using (GL_ONE,GL_ONE_MINUS_SRC_ALPHA) blending
	SOIL_FLAG_INVERT_Y: flip the image vertically
	SOIL_FLAG_COMPRESS_TO_DXT: if the card can display them, will convert RGB to DXT1, RGBA to DXT5
	SOIL_FLAG_DDS_LOAD_DIRECT: will load DDS files directly without _ANY_ additional processing
	SOIL_FLAG_NTSC_SAFE_RGB: clamps RGB components to the range [16,235]
	SOIL_FLAG_CoCg_Y: Google YCoCg; RGB=>CoYCg, RGBA=>CoCgAY
	SOIL_FLAG_TEXTURE_RECTANGE: uses ARB_texture_rectangle ; pixel indexed & no repeat or MIPmaps or cubemaps
**/
enum
{
	TEXTURE_FLAG_POWER_OF_TWO = 1,
	TEXTURE_FLAG_MIPMAPS = 2,
	TEXTURE_FLAG_TEXTURE_REPEATS = 4,
	TEXTURE_FLAG_MULTIPLY_ALPHA = 8,
	TEXTURE_FLAG_INVERT_Y = 16,
	TEXTURE_FLAG_COMPRESS_TO_DXT = 32,
	TEXTURE_FLAG_DDS_LOAD_DIRECT = 64,
	TEXTURE_FLAG_NTSC_SAFE_RGB = 128,
	TEXTURE_FLAG_CoCg_Y = 256,
	TEXTURE_FLAG_TEXTURE_RECTANGLE = 512
};

enum TextureEncodingType
{
	TEXEL_ENCODING_TYPE_LINEAR,
	TEXEL_ENCODING_TYPE_SRGB,
	TEXEL_ENCODING_TYPE_RGBE,
	TEXEL_ENCODING_TYPE_RGBM7,
	TEXEL_ENCODING_TYPE_RGBM16,
	TEXEL_ENCODING_TYPE_RGBD,
	TEXEL_ENCODING_TYPE_GAMMA	
};

class Image;
/**@class
 * @brief 纹理基类
 *
 */
class M3D_API Texture:public Resource
{
public:
	const static int TEXTURE_BASE = 0;//!<基本型
	const static int TEXTURE_2D = 1;//!<2D纹理
	const static int TEXTURE_3D = 2;//!<
	const static int TEXTURE_CUBE =3;//!<立方体纹理
	const static int TEXTURE_GEO = 4;//!<几何纹理 用来作FBO的挂载纹理对象

	GLenum MinFliter() const { return m_minFliter; }
	void MinFliter(GLenum val) { m_minFliter = val; }
	GLenum MagFliter() const { return m_magFliter; }
	void MagFliter(GLenum val) { m_magFliter = val; }
	GLenum WrapS() const { return m_wrapS; }
	void WrapS(GLenum val) { m_wrapS = val; }
	GLenum WrapT() const { return m_wrapT; }
	void WrapT(GLenum val) { m_wrapT = val; }
	bool MipMap() const { return m_isMipMap; }
	void MipMap(bool val) { m_isMipMap = val; }
	M3D::TextureEncodingType TextureEncoding() const { return m_textureEncoding; }
	void TextureEncoding(M3D::TextureEncodingType val) { m_textureEncoding = val; }
	bool IsGammaInput() const { return m_gammaInput; }
	void IsGammaInput(bool val) { m_gammaInput = val; }
	GLenum MinmapFliter() const { return m_minmapFliter; }
	void MinmapFliter(GLenum val) { m_minmapFliter = val; }
public:
	Texture();
	virtual ~Texture();

	virtual int GetType()
	{
		return TEXTURE_BASE;
	}

	/**
	 *
	 * @param fileName
	 */
	void SetPath(const string& fileName);

	string GetPath();

	void SetData(unsigned char* data, int length);

	/**
	 *
	 * @return
	 */
	unsigned int GetID();

	/**
	 * 得到此纹理的OGL对象id，此id为OGL管理和产生的
	 * @return
	 */
	unsigned int GetOGLObj();

	/**
	 *设置图片参数
	 * @param forceChannels
	 * @param flags
	 */
	void SetImageParameter(int forceChannels,unsigned int flags);

	/**
	 *
	 * @param name
	 */
	void SetName(const string& name);

	/**
	 *
	 * @return
	 */
	const string& GetName();

	/**
	 *设置图片路径
	 * @param imagePath
	 */
	//void SetImagePath(const string& imagePath);
	/**
	 *
	 * @return
	 */
	//const string& GetImagePath()const;

	/**
	 *设置关联的图片
	 * @param image
	 */
	void SetImage(Image* image);
	/**
	 *
	 * @return
	 */
	Image* GetImage();

	//virtual Texture* CreateEmpty();

	virtual void GenerateMipMap();
	void NeedUpdate();


protected:
	void Init();

	/**
	 * 更新texture在GL显示中的对象
	 */
	virtual void UpdataOGLObj();

	void MarkDirty();

protected:
	GLuint m_textures[1];

	Image* m_image;

	bool m_isTextureDirty;//!<标志位

	//static string APKPath;//!<

	//和 soil的定义一样
	int m_forceChannels;//!<
	//和 soil的定义一样
	unsigned int m_flags;//!<

	string m_name;//!<

	//string m_imagePath; //!<图片文件路径

	bool m_isMipMap;//!<是否启用mipmap
	GLenum m_minFliter;//!<过滤方式
	GLenum m_magFliter;
	GLenum m_minmapFliter;
	GLenum m_wrapS;
	GLenum m_wrapT;
	//TODO 纹理内部格式 格式 数据格式
	TextureEncodingType m_textureEncoding;
	bool m_gammaInput;

};
}

#endif /*M3D_TEXTURE_H_*/

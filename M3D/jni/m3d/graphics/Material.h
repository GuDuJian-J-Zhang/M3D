/**@file
 *@brief	材质
 *@author	XL
 *@date		20150617
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_MATERIAL_H_
#define M3D_MATERIAL_H_

#include "m3d/base/Color.h"

#include "m3d/model/Image.h"
#include "m3d/renderer/RenderContext.h"
#include "m3d/graphics/Resource.h"

namespace M3D
{

class Texture;
class Texture2D;
class TextureCube;

//copy from assimp

enum TextureOp
{
	/** T = T1 * T2 */
	TextureOp_Multiply = 0x0,

	/** T = T1 + T2 */
	TextureOp_Add = 0x1,

	/** T = T1 - T2 */
	TextureOp_Subtract = 0x2,

	/** T = T1 / T2 */
	TextureOp_Divide = 0x3,

	/** T = (T1 + T2) - (T1 * T2) */
	TextureOp_SmoothAdd = 0x4,

	/** T = T1 + (T2-0.5) */
	TextureOp_SignedAdd = 0x5,

};

enum TextureMapMode
{
    /** A texture coordinate u|v is translated to u%1|v%1
     */
    TextureMapMode_Wrap = 0x0,

    /** Texture coordinates outside [0...1]
     *  are clamped to the nearest valid value.
     */
    TextureMapMode_Clamp = 0x1,

	/** If the texture coordinates for a pixel are outside [0...1]
	 *  the texture is not applied to that pixel
     */
    TextureMapMode_Decal = 0x3,

    /** A texture coordinate u|v becomes u%1|v%1 if (u-(u%1))%2 is zero and
     *  1-(u%1)|1-(v%1) otherwise
     */
    TextureMapMode_Mirror = 0x2,

};

enum TextureMapping
{
    /** The mapping coordinates are taken from an UV channel.
	 *
	 *  The #AI_MATKEY_UVWSRC key specifies from which UV channel
	 *  the texture coordinates are to be taken from (remember,
	 *  meshes can have more than one UV channel).
    */
    TextureMapping_UV = 0x0,

	 /** Spherical mapping */
    TextureMapping_SPHERE = 0x1,

	 /** Cylindrical mapping */
    TextureMapping_CYLINDER = 0x2,

	 /** Cubic mapping */
    TextureMapping_BOX = 0x3,

	 /** Planar mapping */
    TextureMapping_PLANE = 0x4,

	 /** Undefined mapping. Have fun. */
    TextureMapping_OTHER = 0x5,

};

enum TextureType
{
	/** Dummy value.
	 *
	 *  No texture, but the value to be used as 'texture semantic'
	 *  (#aiMaterialProperty::mSemantic) for all material properties
	 *  *not* related to textures.
	 */
	TextureType_NONE = 0x0,



    /** The texture is combined with the result of the diffuse
	 *  lighting equation.
     */
    TextureType_DIFFUSE = 0x1,

	/** The texture is combined with the result of the specular
	 *  lighting equation.
     */
    TextureType_SPECULAR = 0x2,

	/** The texture is combined with the result of the ambient
	 *  lighting equation.
     */
    TextureType_AMBIENT = 0x3,

	/** The texture is added to the result of the lighting
	 *  calculation. It isn't influenced by incoming light.
     */
    TextureType_EMISSIVE = 0x4,

	/** The texture is a height map.
	 *
	 *  By convention, higher gray-scale values stand for
	 *  higher elevations from the base height.
     */
    TextureType_HEIGHT = 0x5,

	/** The texture is a (tangent space) normal-map.
	 *
	 *  Again, there are several conventions for tangent-space
	 *  normal maps. Assimp does (intentionally) not
	 *  distinguish here.
     */
    TextureType_NORMALS = 0x6,

	/** The texture defines the glossiness of the material.
	 *
	 *  The glossiness is in fact the exponent of the specular
	 *  (phong) lighting equation. Usually there is a conversion
	 *  function defined to map the linear color values in the
	 *  texture to a suitable exponent. Have fun.
    */
    TextureType_SHININESS = 0x7,

	/** The texture defines per-pixel opacity.
	 *
	 *  Usually 'white' means opaque and 'black' means
	 *  'transparency'. Or quite the opposite. Have fun.
    */
    TextureType_OPACITY = 0x8,

	/** Displacement texture
	 *
	 *  The exact purpose and format is application-dependent.
     *  Higher color values stand for higher vertex displacements.
    */
    TextureType_DISPLACEMENT = 0x9,

	/** Lightmap texture (aka Ambient Occlusion)
	 *
	 *  Both 'Lightmaps' and dedicated 'ambient occlusion maps' are
	 *  covered by this material property. The texture contains a
	 *  scaling value for the final color value of a pixel. Its
	 *  intensity is not affected by incoming light.
    */
    TextureType_LIGHTMAP = 0xA,

	/** Reflection texture
	 *
	 * Contains the color of a perfect mirror reflection.
	 * Rarely used, almost never for real-time applications.
    */
    TextureType_REFLECTION = 0xB,

	/** Unknown texture
	 *
	 *  A texture reference that does not match any of the definitions
	 *  above is considered to be 'unknown'. It is still imported,
	 *  but is excluded from any further postprocessing.
    */
    TextureType_UNKNOWN = 0xC,

};

enum ShadingMode
{
    /** Flat shading. Shading is done on per-face base,
     *  diffuse only. Also known as 'faceted shading'.
     */
    ShadingMode_Flat = 0x1,

    /** Simple Gouraud shading.
     */
    ShadingMode_Gouraud =	0x2,

    /** Phong-Shading -
     */
    ShadingMode_Phong = 0x3,

    /** Phong-Blinn-Shading
     */
    ShadingMode_Blinn	= 0x4,

    /** Toon-Shading per pixel
     *
	 *  Also known as 'comic' shader.
     */
    ShadingMode_Toon = 0x5,

    /** OrenNayar-Shading per pixel
     *
     *  Extension to standard Lambertian shading, taking the
     *  roughness of the material into account
     */
    ShadingMode_OrenNayar = 0x6,

    /** Minnaert-Shading per pixel
     *
     *  Extension to standard Lambertian shading, taking the
     *  "darkness" of the material into account
     */
    ShadingMode_Minnaert = 0x7,

    /** CookTorrance-Shading per pixel
	 *
	 *  Special shader for metallic surfaces.
     */
    ShadingMode_CookTorrance = 0x8,

    /** No shading at all. Constant light influence of 1.0.
    */
    ShadingMode_NoShading = 0x9,

	 /** Fresnel shading
     */
    ShadingMode_Fresnel = 0xa,

};

/**@class
 * @brief 材质类
 *
 */
class M3D_API Material:public Resource
{
public:
	Material();
	virtual ~Material();

	/**
	 *
	 * @param ambient
	 */
	void SetAmbient(const Color& ambient);

	/**
	 *
	 * @param diffuse
	 */
	void SetDiffuse(const Color& diffuse);

	/**
	 *
	 * @param diffuse
	 */
	void setSpecular(const Color& diffuse);

	/**
	 *
	 * @return
	 */
	const Color& GetAmbient() const;

	/**
	 *
	 * @return
	 */
	const Color& GetDiffuse() const;

	/**
	 *
	 * @return
	 */
	const Color& GetSpecular() const;

	/**
	 * 设置着色模式
	 * @param mode
	 */
	void SetShadingMode(ShadingMode mode);

	/**
	 *
	 * @param op
	 */
	void SetTextureOp(TextureOp op);

	/**
	 * 材质映射模式
	 * @param mode
	 */
	void SetTextureMapMode(TextureMapMode mode);

	/**
	 * 材质映射
	 * @param mode
	 */
	void SetTextureMapping(TextureMapping mode);

	/**
	 *
	 * @return
	 */
	ShadingMode GetShadingMode() const;

	/**
	 *
	 * @return
	 */
	TextureOp GetTextureOp() const;

	/**
	 *
	 * @return
	 */
	TextureMapMode GetTextureMapMode() const;

	/**
	 *
	 * @return
	 */

	TextureMapping GetTextureMapping() const;

	/**
	 * 设置纹理
	 */
	void SetTexture(Texture* texture);
	/**
	 * 获得纹理
	 * @return
	 */
	Texture* GetTexture();

	/**
	 * 设置凹凸映射纹理
	 * @param bumpmap
	 */
	void SetBumpMap(Texture* bumpmap);

	/**
	 * 获取凹凸映射纹理
	 * @return
	 */
	Texture* GetBumpMap();

	/**
	 *
	 * @param texture
	 */
	void SetAmbientTexture(Texture* texture);

	/**
	 *
	 * @return
	 */
	Texture * GetAmbientTexture();

	/**
	 *
	 * @param name
	 */
	void SetName(const string& name);

	/**
	 *
	 * @return
	 */
	const string& GetName()const;

	/**
	 * @brief 创建2D纹理坐标转换矩阵
	 */
	void CreateTexture2DTransform();

	/**
	 *
	 * @return
	 */
	Matrix4* GetTexture2DTransform();

	/**
	 *
	 * @param texture2DTransform
	 */
	void SetTexture2DTransform(Matrix4* texture2DTransform);

	/**
	 *
	 * @param shininess
	 */
	void SetShininess(float shininess);
	/**
	 *
	 * @return
	 */
	float GetShininess();

private:
	void Init();

private:
	Color m_ambient; //!<环境光系数

	Color m_Diffuse; //!<漫反射系数

	Color m_Specular;//!<镜面系数

	ShadingMode m_shadingMode;//!<着色模式

	TextureOp m_textureOp;//!<多个纹理叠加方式

	TextureMapMode m_textureMapMode;//!<纹理坐标超出范围处理方式

	TextureMapping m_textureMapping;//!<纹理映射方式

//	vector<Texture*> m_textures; //使用到的纹理

	Texture* m_texture;//!<漫反射 纹理

	Texture* m_ambientTexture;//!<环境光纹理 （此处用作环境映射纹理）

	Texture* m_bumpmap; //!<凹凸映射

	string m_name;//!<名称


	Matrix4* m_textureTransform;//!<Texture2D的变换矩阵，如果存在的话

	float m_shininess;//!<shininess
};
}

#endif /*M3D_MATERIAL_H_*/

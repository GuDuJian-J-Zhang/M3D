/**@file
 *@brief
 *@author	CDJ 
 *@date		2016-2-22
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_GRAPHICSDEFS_H_
#define M3D_GRAPHICSDEFS_H_
#include "m3d/M3D.h"
#include "m3d/base/StringHash.h"
namespace M3D
{
//VSP:vertex shader parameter
//FSP:fragment shader parameter

/**
 * 以下为着色器中的变量名字符串对应的字符串哈希值
 */
extern M3D_API const string VSP_POSITION;
extern M3D_API const string VSP_NORMAL;
extern M3D_API const string VSP_COLOR;
extern M3D_API const string VSP_UCOLOR;
extern M3D_API const string VSP_TEXCOORDS;

extern M3D_API const string VSP_MODELMAT;
extern M3D_API const string VSP_VIEWMAT;
extern M3D_API const string VSP_PROJECTIONMAT;
extern M3D_API const string VSP_NORMALMAT;
extern M3D_API const string VSP_LIGHTMAT;
extern M3D_API const string VSP_LIGHTPROJECT;
extern M3D_API const string VSP_BAIS;
extern M3D_API const string VSP_MODELVIEWMAT;
extern M3D_API const string VSP_MVPMAT;
extern M3D_API const string VSP_TEXTUREMAT;

extern M3D_API const string VSP_ISDIAMOND;

extern M3D_API const string VSP_MIRRORMAT;

extern M3D_API const string VSP_LIGHTPOSITION;
extern M3D_API const string VSP_EYEPOSITION;

extern M3D_API const string FSP_AMBIENT;
extern M3D_API const string FSP_DIFFUSE;
extern M3D_API const string FSP_SPECULAR;
extern M3D_API const string FSP_SHININESS;
extern M3D_API const string FSP_SAMPLER0;
extern M3D_API const string FSP_SAMPLER1;
extern M3D_API const string FSP_SAMPLER2;
extern M3D_API const string FSP_SAMPLERCUBE0;

extern M3D_API const string FSP_USETEX;//TODO 以后优化

//for test
extern M3D_API const string FSP_FRESNEL0;
extern M3D_API const string FSP_ROUGHNESS;

//for multi-clip
extern M3D_API const string FSP_CLIPPLANES;
extern M3D_API const string FSP_ENABLECLIPS;
extern M3D_API const string FSP_REVERSECLIP;

//for multilight
extern M3D_API const string FSP_ENABLELIGHTS;
extern M3D_API const string FSP_LIGHTMODEL_AMBIENT;
// light
extern M3D_API const string FSP_LIGHT_0_AMBIENT;
extern M3D_API const string FSP_LIGHT_0_DIFFUSE;
extern M3D_API const string FSP_LIGHT_0_SPECULAR;
extern M3D_API const string FSP_LIGHT_0_POSITION;
extern M3D_API const string FSP_LIGHT_0_SPOT_DIRECTION;
extern M3D_API const string FSP_LIGHT_0_SPOT_EXPONENT;
extern M3D_API const string FSP_LIGHT_0_SPOT_CUTOFF;
extern M3D_API const string FSP_LIGHT_0_SPOT_COSCUTOFF;
extern M3D_API const string FSP_LIGHT_0_CONST_ATTENUATION;
extern M3D_API const string FSP_LIGHT_0_LINEAR_ATTENUATION;
extern M3D_API const string FSP_LIGHT_0_QUAD_ATTENUATION;
extern M3D_API const string FSP_LIGHT_0_INTENSITY;

extern M3D_API const string FSP_LIGHT_1_AMBIENT;
extern M3D_API const string FSP_LIGHT_1_DIFFUSE;
extern M3D_API const string FSP_LIGHT_1_SPECULAR;
extern M3D_API const string FSP_LIGHT_1_POSITION;
extern M3D_API const string FSP_LIGHT_1_SPOT_DIRECTION;
extern M3D_API const string FSP_LIGHT_1_SPOT_EXPONENT;
extern M3D_API const string FSP_LIGHT_1_SPOT_CUTOFF;
extern M3D_API const string FSP_LIGHT_1_SPOT_COSCUTOFF;
extern M3D_API const string FSP_LIGHT_1_CONST_ATTENUATION;
extern M3D_API const string FSP_LIGHT_1_LINEAR_ATTENUATION;
extern M3D_API const string FSP_LIGHT_1_QUAD_ATTENUATION;
extern M3D_API const string FSP_LIGHT_1_INTENSITY;
//for material
extern M3D_API const string FSP_MATERIAL_EMISSION;
extern M3D_API const string FSP_MATERIAL_AMBIENT;
extern M3D_API const string FSP_MATERIAL_DIFFUSE;
extern M3D_API const string FSP_MATERIAL_SPECULAR;
extern M3D_API const string FSP_MATERIAL_SHININESS;

extern M3D_API const string FSP_USEAMBIENTTEX;
extern M3D_API const string FSP_USECUBEMAPTEX;

extern M3D_API const string FSP_SELECTCOLOR;

//for FBO
extern M3D_API const string FSP_SHADOW_LIGHTPOS;
extern M3D_API const string SHADOW_X_PIXEL_OFFSET;
extern M3D_API const string SHADOW_Y_PIXEL_OFFSET;
extern M3D_API const string VSP_LIGHT_NORMALMAT;

//为了增强边缘检测而设置的颜色
extern M3D_API const string FSP_EDGEDETEC;

////ubershader---------------------------------

extern M3D_API const string MIRROR_TEXTURE;
extern M3D_API const string MIRRORBACKGROUND_TEXTURE;

extern M3D_API const string MODEL_MATRXI;
extern M3D_API const string VIEW_MATRXI;
extern M3D_API const string NORMAL_MATRXI;
extern M3D_API const string PROJECTION_MATRXI;
extern M3D_API const string UV_TRANSFORM_MATRIX;
extern M3D_API const string DIFFUSE;
extern M3D_API const string SPECULAR;
extern M3D_API const string EMISSIVE;
extern M3D_API const string SHININESS;
extern M3D_API const string DIFFUSE_TEXTURE;
extern M3D_API const string SPECULAR_MAP;
extern M3D_API const string NORMAL_MAP;
extern M3D_API const string MATCAP_MAP;
extern M3D_API const string NORMAL_MAP_SCALE;
extern M3D_API const string DISPLACEMENT_MAP;
extern M3D_API const string DISPLACEMENT_SCALE;
extern M3D_API const string DISPLACEMENT_BIAS;
extern M3D_API const string EMISSIVE_MAP;
extern M3D_API const string AO_MAP;
extern M3D_API const string AO_MAP_INTENSITY;
extern M3D_API const string ENV_TEXTURE;
extern M3D_API const string ENV_DIFFUSE_TEXTURE;
extern M3D_API const string ROUGHNESS;
extern M3D_API const string METALNESS;
extern M3D_API const string METALLIC_ROUGHNESS_TEXTURE;
extern M3D_API const string GAMMA;
extern M3D_API const string ENV_MAP_INTENSITY;
extern M3D_API const string LUT;
extern M3D_API const string CAMERA_POSITION;
extern M3D_API const string AMBIENT_LIGHT_COLOR;
extern M3D_API const string SPOT_LIGHT_0_COLOR;
extern M3D_API const string SPOT_LIGHT_0_POSITION;
extern M3D_API const string SPOT_LIGHT_0_DIRECTION;
extern M3D_API const string SPOT_LIGHT_0_DISTANCE;
extern M3D_API const string SPOT_LIGHT_0_DECAY;
extern M3D_API const string SPOT_LIGHT_0_CONECOS;
extern M3D_API const string SPOT_LIGHT_0_PENUMBRACOS;
extern M3D_API const string SPOT_LIGHT_0_SHADOW;
extern M3D_API const string TONE_MAPPING_EXPOSURE;
extern M3D_API const string CLEARCOAT;
extern M3D_API const string CLEARCOATROUGHNESS;
extern M3D_API const string SPECULAR;
extern M3D_API const string OPACITY;
//extern M3D_API const StringHash SPOT_LIGHT_0_COLOR;
/// Primitive type.
enum PrimitiveType
{
	TRIANGLE_LIST = 0,
	LINE_LIST,
	POINT_LIST,
	TRIANGLE_STRIP,
	LINE_STRIP,
	TRIANGLE_FAN
};

/// %Geometry type.
enum GeometryType
{
	GEOM_STATIC = 0,
	GEOM_SKINNED = 1,
	GEOM_INSTANCED = 2,
	GEOM_BILLBOARD = 3,
	GEOM_STATIC_NOINSTANCING = 4,
	MAX_GEOMETRYTYPES = 4,
};

/// Blending mode.
enum BlendMode
{
	BLEND_REPLACE = 0,
	BLEND_ADD,
	BLEND_MULTIPLY,
	BLEND_ALPHA,
	BLEND_ADDALPHA,
	BLEND_PREMULALPHA,
	BLEND_INVDESTALPHA,
	BLEND_SUBTRACT,
	BLEND_SUBTRACTALPHA,
	MAX_BLENDMODES
};

/// Depth or stencil compare mode.
enum CompareMode
{
	CMP_ALWAYS = 0,
	CMP_EQUAL,
	CMP_NOTEQUAL,
	CMP_LESS,
	CMP_LESSEQUAL,
	CMP_GREATER,
	CMP_GREATEREQUAL,
	MAX_COMPAREMODES
};

/// Culling mode.
enum CullMode
{
	CULL_NONE = 0, CULL_CCW, CULL_CW, MAX_CULLMODES
};

/// Fill mode.
enum FillMode
{
	FILL_SOLID = 0, FILL_WIREFRAME, FILL_POINT
};

/// Stencil operation.
enum StencilOp
{
	OP_KEEP = 0, OP_ZERO, OP_REF, OP_INCR, OP_DECR
};

/// Vertex/index buffer lock state.
enum LockState
{
	LOCK_NONE = 0, LOCK_HARDWARE, LOCK_SHADOW, LOCK_SCRATCH
};

/// Vertex elements.
enum VertexElement
{
	ELEMENT_POSITION = 0,
	ELEMENT_NORMAL,
	ELEMENT_COLOR,
	ELEMENT_TEXCOORD1,
	ELEMENT_TEXCOORD2,
	ELEMENT_CUBETEXCOORD1,
	ELEMENT_CUBETEXCOORD2,
	ELEMENT_TANGENT,
	ELEMENT_BLENDWEIGHTS,
	ELEMENT_BLENDINDICES,
	ELEMENT_INSTANCEMATRIX1,
	ELEMENT_INSTANCEMATRIX2,
	ELEMENT_INSTANCEMATRIX3,
	MAX_VERTEX_ELEMENTS
};

/// Texture filtering mode.
enum TextureFilterMode
{
	FILTER_NEAREST = 0,
	FILTER_BILINEAR,
	FILTER_TRILINEAR,
	FILTER_ANISOTROPIC,
	FILTER_DEFAULT,
	MAX_FILTERMODES
};

/// Texture addressing mode.
enum TextureAddressMode
{
	ADDRESS_WRAP = 0,
	ADDRESS_MIRROR,
	ADDRESS_CLAMP,
	ADDRESS_BORDER,
	MAX_ADDRESSMODES
};

/// Texture coordinates.
enum TextureCoordinate
{
	COORD_U = 0, COORD_V, COORD_W, MAX_COORDS
};

/// Texture usage types.
enum TextureUsage
{
	TEXTURE_STATIC = 0,
	TEXTURE_DYNAMIC,
	TEXTURE_RENDERTARGET,
	TEXTURE_DEPTHSTENCIL
};

/// Cube map faces.
enum CubeMapFace
{
	FACE_POSITIVE_X = 0,
	FACE_NEGATIVE_X,
	FACE_POSITIVE_Y,
	FACE_NEGATIVE_Y,
	FACE_POSITIVE_Z,
	FACE_NEGATIVE_Z,
	MAX_CUBEMAP_FACES
};

/// Cubemap single image layout modes.
enum CubeMapLayout
{
	CML_HORIZONTAL = 0,
	CML_HORIZONTALNVIDIA,
	CML_HORIZONTALCROSS,
	CML_VERTICALCROSS,
	CML_BLENDER
};

/// Update mode for render surface viewports.
enum RenderSurfaceUpdateMode
{
	SURFACE_MANUALUPDATE = 0, SURFACE_UPDATEVISIBLE, SURFACE_UPDATEALWAYS
};

/// Shader types.
enum ShaderType
{
	VS = 0, PS,
};

/// Shader parameter groups for determining need to update.
enum ShaderParameterGroup
{
	SP_FRAME = 0,
	SP_CAMERA,
	SP_VIEWPORT,
	SP_ZONE,
	SP_LIGHT,
	SP_VERTEXLIGHTS,
	SP_MATERIAL,
	SP_OBJECTTRANSFORM,
	MAX_SHADER_PARAMETER_GROUPS
};

/// Texture units.
enum TextureUnit
{
	TU_DIFFUSE = 0,
	TU_ALBEDOBUFFER = 0,
	TU_NORMAL = 1,
	TU_NORMALBUFFER = 1,
	TU_SPECULAR = 2,
	TU_EMISSIVE = 3,
	TU_ENVIRONMENT = 4,
	MAX_MATERIAL_TEXTURE_UNITS = 5,
	TU_LIGHTRAMP = 5,
	TU_LIGHTSHAPE = 6,
	TU_SHADOWMAP = 7,
	TU_FACESELECT = 8,
	TU_INDIRECTION = 9,
	TU_DEPTHBUFFER = 10,
	TU_LIGHTBUFFER = 11,
	TU_VOLUMEMAP = 12,
	TU_ZONE = 13,
	MAX_NAMED_TEXTURE_UNITS = 14,
	MAX_TEXTURE_UNITS = 16
};

/// Billboard camera facing modes.
enum FaceCameraMode
{
	FC_NONE = 0, FC_ROTATE_XYZ, FC_ROTATE_Y, FC_LOOKAT_XYZ, FC_LOOKAT_Y
};

static const int QUALITY_LOW = 0;
static const int QUALITY_MEDIUM = 1;
static const int QUALITY_HIGH = 2;
static const int QUALITY_MAX = 15;

static const int SHADOWQUALITY_LOW_16BIT = 0;
static const int SHADOWQUALITY_LOW_24BIT = 1;
static const int SHADOWQUALITY_HIGH_16BIT = 2;
static const int SHADOWQUALITY_HIGH_24BIT = 3;

static const unsigned CLEAR_COLOR = 0x1;
static const unsigned CLEAR_DEPTH = 0x2;
static const unsigned CLEAR_STENCIL = 0x4;

static const unsigned MASK_NONE = 0x0;
static const unsigned MASK_POSITION = 0x1;
static const unsigned MASK_NORMAL = 0x2;
static const unsigned MASK_COLOR = 0x4;
static const unsigned MASK_TEXCOORD1 = 0x8;
static const unsigned MASK_TEXCOORD2 = 0x10;
static const unsigned MASK_CUBETEXCOORD1 = 0x20;
static const unsigned MASK_CUBETEXCOORD2 = 0x40;
static const unsigned MASK_TANGENT = 0x80;
static const unsigned MASK_BLENDWEIGHTS = 0x100;
static const unsigned MASK_BLENDINDICES = 0x200;
static const unsigned MASK_INSTANCEMATRIX1 = 0x400;
static const unsigned MASK_INSTANCEMATRIX2 = 0x800;
static const unsigned MASK_INSTANCEMATRIX3 = 0x1000;
static const unsigned MASK_DEFAULT = 0xffffffff;
static const unsigned NO_ELEMENT = 0xffffffff;

static const int MAX_RENDERTARGETS = 4;
static const int MAX_VERTEX_STREAMS = 4;
static const int MAX_SKIN_MATRICES = 64;
static const int MAX_CONSTANT_REGISTERS = 256;

static const int BITS_PER_COMPONENT = 8;
}

#endif /* M3D_GRAPHICSDEFS_H_ */

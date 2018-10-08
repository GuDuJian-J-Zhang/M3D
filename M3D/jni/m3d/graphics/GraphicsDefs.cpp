/*
 * GraphicsDefs.cpp
 *
 *  Created on: 2016-2-22
 *      Author: CDJ
 */
#include "m3d/graphics/GraphicsDefs.h"
namespace M3D
{

extern M3D_API const StringHash VSP_POSITION("a_position");
extern M3D_API const StringHash VSP_NORMAL("a_normal");
extern M3D_API const StringHash VSP_COLOR("a_color");
extern M3D_API const StringHash VSP_UCOLOR("u_color");
extern M3D_API const StringHash VSP_TEXCOORDS("a_texCoords");
extern M3D_API const StringHash VSP_MODELMAT("u_modelMat");
extern M3D_API const StringHash VSP_VIEWMAT("u_viewMat");
extern M3D_API const StringHash VSP_PROJECTIONMAT("u_projectionMat");
extern M3D_API const StringHash VSP_NORMALMAT("u_normalMat");
extern M3D_API const StringHash VSP_LIGHTMAT("u_lightMat");
extern M3D_API const StringHash VSP_LIGHTPROJECT("u_lightProject");
extern M3D_API const StringHash VSP_BAIS("u_bais");
extern M3D_API const StringHash VSP_MODELVIEWMAT("u_modelViewMat");
extern M3D_API const StringHash VSP_MVPMAT("u_MVPMat");
extern M3D_API const StringHash VSP_TEXTUREMAT("u_textureMat");

extern M3D_API const StringHash VSP_LIGHTPOSITION("u_lightPosition");
extern M3D_API const StringHash VSP_EYEPOSITION("u_eyePosition");

extern M3D_API const StringHash FSP_AMBIENT("u_ambient");
extern M3D_API const StringHash FSP_DIFFUSE("u_diffuse");
extern M3D_API const StringHash FSP_SPECULAR("u_specular");
extern M3D_API const StringHash FSP_SHININESS("u_shininess");
extern M3D_API const StringHash FSP_SAMPLER0("u_sampler0");
extern M3D_API const StringHash FSP_SAMPLER1("u_sampler1");
extern M3D_API const StringHash FSP_SAMPLER2("u_sampler2");
extern M3D_API const StringHash FSP_SAMPLERCUBE0("u_samplerCube0");

extern M3D_API const StringHash FSP_USETEX("u_useTex");

extern M3D_API const StringHash FSP_FRESNEL0("u_fresnel0");
extern M3D_API const StringHash FSP_ROUGHNESS("u_roughness");

extern M3D_API const StringHash FSP_CLIPPLANE("u_clipPlane");
extern M3D_API const StringHash FSP_ENABLECLIP("u_enableClip");

//for multilight
extern M3D_API const StringHash FSP_ENABLELIGHTS("u_enbleLights");
extern M3D_API const StringHash FSP_LIGHTMODEL_AMBIENT("u_lightModel.ambient");
// light
extern M3D_API const StringHash FSP_LIGHT_0_AMBIENT("u_lights[0].ambient");
extern M3D_API const StringHash FSP_LIGHT_0_DIFFUSE("u_lights[0].diffuse");
extern M3D_API const StringHash FSP_LIGHT_0_SPECULAR("u_lights[0].specular");
extern M3D_API const StringHash FSP_LIGHT_0_POSITION("u_lights[0].position");
extern M3D_API const StringHash FSP_LIGHT_0_SPOT_DIRECTION("u_lights[0].spotDirection");
extern M3D_API const StringHash FSP_LIGHT_0_SPOT_EXPONENT("u_lights[0].spotExponent");
extern M3D_API const StringHash FSP_LIGHT_0_SPOT_CUTOFF("u_lights[0].spotCutoff");
extern M3D_API const StringHash FSP_LIGHT_0_SPOT_COSCUTOFF("u_lights[0].spotCosCutoff");
extern M3D_API const StringHash FSP_LIGHT_0_CONST_ATTENUATION("u_lights[0].constant");
extern M3D_API const StringHash FSP_LIGHT_0_LINEAR_ATTENUATION("u_lights[0].linear");
extern M3D_API const StringHash FSP_LIGHT_0_QUAD_ATTENUATION("u_lights[0].quadratic");
extern M3D_API const StringHash FSP_LIGHT_0_INTENSITY("u_lights[0].intensity");

extern M3D_API const StringHash FSP_LIGHT_1_AMBIENT("u_lights[1].ambient");
extern M3D_API const StringHash FSP_LIGHT_1_DIFFUSE("u_lights[1].diffuse");
extern M3D_API const StringHash FSP_LIGHT_1_SPECULAR("u_lights[1].specular");
extern M3D_API const StringHash FSP_LIGHT_1_POSITION("u_lights[1].position");
extern M3D_API const StringHash FSP_LIGHT_1_SPOT_DIRECTION("u_lights[1].spotDirection");
extern M3D_API const StringHash FSP_LIGHT_1_SPOT_EXPONENT("u_lights[1].spotExponent");
extern M3D_API const StringHash FSP_LIGHT_1_SPOT_CUTOFF("u_lights[1].spotCutoff");
extern M3D_API const StringHash FSP_LIGHT_1_SPOT_COSCUTOFF("u_lights[1].spotCosCutoff");
extern M3D_API const StringHash FSP_LIGHT_1_CONST_ATTENUATION("u_lights[1].constant");
extern M3D_API const StringHash FSP_LIGHT_1_LINEAR_ATTENUATION("u_lights[1].linear");
extern M3D_API const StringHash FSP_LIGHT_1_QUAD_ATTENUATION("u_lights[1].quadratic");
extern M3D_API const StringHash FSP_LIGHT_1_INTENSITY("u_lights[1].intensity");
//for material
extern M3D_API const StringHash FSP_MATERIAL_EMISSION("u_materials.emission");
extern M3D_API const StringHash FSP_MATERIAL_AMBIENT("u_materials.ambient");
extern M3D_API const StringHash FSP_MATERIAL_DIFFUSE("u_materials.diffuse");
extern M3D_API const StringHash FSP_MATERIAL_SPECULAR("u_materials.specular");
extern M3D_API const StringHash FSP_MATERIAL_SHININESS("u_materials.shininess");

extern M3D_API const StringHash FSP_USEAMBIENTTEX("u_useAmbientTex");

extern M3D_API const StringHash FSP_USECUBEMAPTEX("u_useCubeMapTex");

extern M3D_API const StringHash FSP_SELECTCOLOR("u_selectColor");
extern M3D_API const StringHash FSP_SHADOW_LIGHTPOS("u_shadowLPos");
extern M3D_API const StringHash SHADOW_X_PIXEL_OFFSET("u_xPixelOffset");
extern M3D_API const StringHash SHADOW_Y_PIXEL_OFFSET("u_yPixelOffset");
extern M3D_API const StringHash VSP_LIGHT_NORMALMAT("u_lightNormalMat");


extern M3D_API const StringHash FSP_EDGEDETEC("u_edgeDetecColor");


};





/*
 * GraphicsDefs.cpp
 *
 *  Created on: 2016-2-22
 *      Author: CDJ
 */
#include "m3d/graphics/GraphicsDefs.h"
namespace M3D
{

extern M3D_API const string VSP_POSITION("a_position");
extern M3D_API const string VSP_NORMAL("a_normal");
extern M3D_API const string VSP_COLOR("a_color");
extern M3D_API const string VSP_UCOLOR("u_color");
extern M3D_API const string VSP_TEXCOORDS("a_texCoords");
extern M3D_API const string VSP_MODELMAT("u_modelMat");
extern M3D_API const string VSP_VIEWMAT("u_viewMat");
extern M3D_API const string VSP_PROJECTIONMAT("u_projectionMat");
extern M3D_API const string VSP_NORMALMAT("u_normalMat");
extern M3D_API const string VSP_LIGHTMAT("u_lightMat");
extern M3D_API const string VSP_LIGHTPROJECT("u_lightProject");
extern M3D_API const string VSP_BAIS("u_bais");
extern M3D_API const string VSP_MODELVIEWMAT("u_modelViewMat");
extern M3D_API const string VSP_MVPMAT("u_MVPMat");
extern M3D_API const string VSP_TEXTUREMAT("u_textureMat");

extern M3D_API const string VSP_LIGHTPOSITION("u_lightPosition");
extern M3D_API const string VSP_EYEPOSITION("u_eyePosition");

extern M3D_API const string FSP_AMBIENT("u_ambient");
extern M3D_API const string FSP_DIFFUSE("u_diffuse");
extern M3D_API const string FSP_SPECULAR("u_specular");
extern M3D_API const string FSP_SHININESS("u_shininess");
extern M3D_API const string FSP_SAMPLER0("u_sampler0");
extern M3D_API const string FSP_SAMPLER1("u_sampler1");
extern M3D_API const string FSP_SAMPLER2("u_sampler2");
extern M3D_API const string FSP_SAMPLERCUBE0("u_samplerCube0");

extern M3D_API const string FSP_USETEX("u_useTex");

extern M3D_API const string FSP_FRESNEL0("u_fresnel0");
extern M3D_API const string FSP_ROUGHNESS("u_roughness");

//for multi-clip
extern M3D_API const string FSP_CLIPPLANES("u_clipPlanes");
extern M3D_API const string FSP_ENABLECLIPS("u_enableClips");
extern M3D_API const string FSP_REVERSECLIP("u_reverseClip");

//for multilight
extern M3D_API const string FSP_ENABLELIGHTS("u_enbleLights");
extern M3D_API const string FSP_LIGHTMODEL_AMBIENT("u_lightModel.ambient");
// light
extern M3D_API const string FSP_LIGHT_0_AMBIENT("u_lights[0].ambient");
extern M3D_API const string FSP_LIGHT_0_DIFFUSE("u_lights[0].diffuse");
extern M3D_API const string FSP_LIGHT_0_SPECULAR("u_lights[0].specular");
extern M3D_API const string FSP_LIGHT_0_POSITION("u_lights[0].position");
extern M3D_API const string FSP_LIGHT_0_SPOT_DIRECTION("u_lights[0].spotDirection");
extern M3D_API const string FSP_LIGHT_0_SPOT_EXPONENT("u_lights[0].spotExponent");
extern M3D_API const string FSP_LIGHT_0_SPOT_CUTOFF("u_lights[0].spotCutoff");
extern M3D_API const string FSP_LIGHT_0_SPOT_COSCUTOFF("u_lights[0].spotCosCutoff");
extern M3D_API const string FSP_LIGHT_0_CONST_ATTENUATION("u_lights[0].constant");
extern M3D_API const string FSP_LIGHT_0_LINEAR_ATTENUATION("u_lights[0].linear");
extern M3D_API const string FSP_LIGHT_0_QUAD_ATTENUATION("u_lights[0].quadratic");
extern M3D_API const string FSP_LIGHT_0_INTENSITY("u_lights[0].intensity");

extern M3D_API const string FSP_LIGHT_1_AMBIENT("u_lights[1].ambient");
extern M3D_API const string FSP_LIGHT_1_DIFFUSE("u_lights[1].diffuse");
extern M3D_API const string FSP_LIGHT_1_SPECULAR("u_lights[1].specular");
extern M3D_API const string FSP_LIGHT_1_POSITION("u_lights[1].position");
extern M3D_API const string FSP_LIGHT_1_SPOT_DIRECTION("u_lights[1].spotDirection");
extern M3D_API const string FSP_LIGHT_1_SPOT_EXPONENT("u_lights[1].spotExponent");
extern M3D_API const string FSP_LIGHT_1_SPOT_CUTOFF("u_lights[1].spotCutoff");
extern M3D_API const string FSP_LIGHT_1_SPOT_COSCUTOFF("u_lights[1].spotCosCutoff");
extern M3D_API const string FSP_LIGHT_1_CONST_ATTENUATION("u_lights[1].constant");
extern M3D_API const string FSP_LIGHT_1_LINEAR_ATTENUATION("u_lights[1].linear");
extern M3D_API const string FSP_LIGHT_1_QUAD_ATTENUATION("u_lights[1].quadratic");
extern M3D_API const string FSP_LIGHT_1_INTENSITY("u_lights[1].intensity");
//for material
extern M3D_API const string FSP_MATERIAL_EMISSION("u_materials.emission");
extern M3D_API const string FSP_MATERIAL_AMBIENT("u_materials.ambient");
extern M3D_API const string FSP_MATERIAL_DIFFUSE("u_materials.diffuse");
extern M3D_API const string FSP_MATERIAL_SPECULAR("u_materials.specular");
extern M3D_API const string FSP_MATERIAL_SHININESS("u_materials.shininess");

extern M3D_API const string FSP_USEAMBIENTTEX("u_useAmbientTex");

extern M3D_API const string FSP_USECUBEMAPTEX("u_useCubeMapTex");

extern M3D_API const string FSP_SELECTCOLOR("u_selectColor");
extern M3D_API const string FSP_SHADOW_LIGHTPOS("u_shadowLPos");
extern M3D_API const string SHADOW_X_PIXEL_OFFSET("u_xPixelOffset");
extern M3D_API const string SHADOW_Y_PIXEL_OFFSET("u_yPixelOffset");
extern M3D_API const string VSP_LIGHT_NORMALMAT("u_lightNormalMat");


extern M3D_API const string FSP_EDGEDETEC("u_edgeDetecColor");

////ubershader---------------------------------
extern M3D_API const string MODEL_MATRXI("modelMatrix");
extern M3D_API const string VIEW_MATRXI("viewMatrix");
extern M3D_API const string NORMAL_MATRXI("normalMatrix");
extern M3D_API const string PROJECTION_MATRXI("projectionMatrix");
extern M3D_API const string UV_TRANSFORM_MATRIX("uvTransform");
extern M3D_API const string DIFFUSE("diffuse");
extern M3D_API const string SPECULAR("specular");
extern M3D_API const string EMISSIVE("emissive");
extern M3D_API const string SHININESS("shininess");
extern M3D_API const string DIFFUSE_TEXTURE("diffuseTexture");
extern M3D_API const string SPECULAR_MAP("specularMap");
extern M3D_API const string NORMAL_MAP("normalMap");
extern M3D_API const string MATCAP_MAP("matcapMap");
extern M3D_API const string NORMAL_MAP_SCALE("normalScale");
extern M3D_API const string DISPLACEMENT_MAP("displacementMap");
extern M3D_API const string DISPLACEMENT_SCALE("displacementScale");
extern M3D_API const string DISPLACEMENT_BIAS("displacementBias");
extern M3D_API const string EMISSIVE_MAP("emissiveMap");
extern M3D_API const string AO_MAP("aoMap");
extern M3D_API const string AO_MAP_INTENSITY("aoMapIntensity");
extern M3D_API const string ENV_TEXTURE("envTexture");
extern M3D_API const string ENV_DIFFUSE_TEXTURE("envDiffuseTexture");
extern M3D_API const string ROUGHNESS("roughness");
extern M3D_API const string METALNESS("metalness");
extern M3D_API const string METALLIC_ROUGHNESS_TEXTURE("metallicRoughnessTexture");
extern M3D_API const string GAMMA("gamma");
extern M3D_API const string ENV_MAP_INTENSITY("envMapIntensity");
extern M3D_API const string LUT("lut");
extern M3D_API const string CAMERA_POSITION("cameraPosition");
extern M3D_API const string AMBIENT_LIGHT_COLOR("ambientLightColor");
extern M3D_API const string SPOT_LIGHT_0_COLOR("spotLights[0].color");
extern M3D_API const string SPOT_LIGHT_0_POSITION("spotLights[0].position");
extern M3D_API const string SPOT_LIGHT_0_DIRECTION("spotLights[0].direction");
extern M3D_API const string SPOT_LIGHT_0_DISTANCE("spotLights[0].distance");
extern M3D_API const string SPOT_LIGHT_0_DECAY("spotLights[0].decay");
extern M3D_API const string SPOT_LIGHT_0_CONECOS("spotLights[0].coneCos");
extern M3D_API const string SPOT_LIGHT_0_PENUMBRACOS("spotLights[0].penumbraCos");
extern M3D_API const string SPOT_LIGHT_0_SHADOW("spotLights[0].shadow");
extern M3D_API const string TONE_MAPPING_EXPOSURE("toneMappingExposure");

extern M3D_API const string CLEARCOAT("clearCoat");
extern M3D_API const string CLEARCOATROUGHNESS("clearCoatRoughness");

extern M3D_API const string OPACITY("opacity");


};





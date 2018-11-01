/**@file
 *@brief
 *@author	CDJ
 *@date		2016-2-23
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_SHADERSRCCODE_H_
#define M3D_SHADERSRCCODE_H_
#include "m3d/M3D.h"
namespace M3D
{

/**
 * @class
 * @brief 该类保存了所有效果的shader源码
 */
class M3D_API ShaderSrcCode
{
public:
	ShaderSrcCode();
	virtual ~ShaderSrcCode();

	static const char * BackgroundVert();
	static const char * BackgroundFrag();
	

	static const char* XRayVert();
	static const char* XRayFrag();

	static const char* DraggerVert();
	static const char* DraggerFrag();

	static const char* DraggerEdgeVert();
	static const char* DraggerEdgeFrag();

	static const char * ModelBaseVert();
	static const char * ModelBaseFrag();

	static const char * BaseVertexVert();
	static const char * BaseVertexFrag();

	static const char * EdgeVert();
	static const char * EdgeFrag();
	static const char * EdgeFragMediump();
	static const char * NoteEdgeVert();
	static const char * NoteEdgeFrag();

	static const char * WireframeVert();
	static const char * WireframeFrag();

	static const char * BrdfVert();
	static const char * BrdfFrag();
	static const char * AxisVert();
	static const char * AxisFrag();

	static const char * ImageVert();
	static const char * ImageFrag();
	static const char * NoteImageVert();
	static const char * NoteImageFrag();

	static const char * CubeMapVert();
	static const char * CubeMapFrag();

	static const char * ClipPlaneVert();
	static const char * ClipPlaneFrag();
	static const char * ClipPlaneFragMediump();

	static const char * MultilightVert();
	static const char * MultilightFrag();

	static const char * ShadowMapVert();
	static const char * ShadowMapFrag();

	static const char * FBODebugVert();
	static const char * FBODebugFrag();

	static const char * PlaneShadowVert();
	static const char * PlaneShadowFrag();

	static const char * BlurVert();
	static const char * BlurFrag();

	static const char * SpecularVert();
	static const char * SpecularFrag();

	static const char * EdgeDetectionVert();
	static const char * EdgeDetectionFrag();

	static const char * QuadVert();
	static const char * QuadFrag();
	static const char * QuadFragMediump();

	//************************************
	// Method:    MultilightPerFragVert
	// FullName:  M3D::ShaderSrcCode::MultilightPerFragVert
	// Access:    public static 
	// Returns:   const char *
	// Qualifier: 原来用的多灯光渲染源码
	//************************************
	static const char * MultilightPerFragVert();
	static const char * MultilightPerFragFrag();
	static const char * MultilightPerFragFragMediump();

	static const char * CapPlaneVert();
	static const char * CapPlaneFrag();

	//************************************
	// Method:    NewJewelBackVert
	// FullName:  M3D::ShaderSrcCode::NewJewelFrontVert
	// Access:    public static
	// Returns:   const char*
	// Qualifier: 珠宝front Info渲染层 绘制完传递给back
	//************************************
	static const char* JewelFrontInfoVert();
	static const char* JewelFrontInfoFrag();
	//************************************
	// Method:    JewelFrontVert
	// FullName:  M3D::ShaderSrcCode::JewelFrontVert
	// Access:    public static 
	// Returns:   const char*
	// Qualifier: 钻石front渲染层
	//************************************
	static const char* DiamondFrontVert();
	static const char* DiamondFrontFrag();

	//************************************
	// Method:    JewelBackVert
	// FullName:  M3D::ShaderSrcCode::JewelBackVert
	// Access:    public static
	// Returns:   const char*
	// Qualifier: 钻石back渲染层
	//************************************
	static const char* DiamondBackVert();
	static const char* DiamondBackFrag();


	//************************************
	// Method:    JewelFrontVert
	// FullName:  M3D::ShaderSrcCode::JewelFrontVert
	// Access:    public static
	// Returns:   const char*
	// Qualifier: 除钻石外其他珠宝front渲染层
	//************************************
	static const char* JewelFrontVert();
	static const char* JewelFrontFrag();

	//************************************
	// Method:    JewelBackVert
	// FullName:  M3D::ShaderSrcCode::JewelBackVert
	// Access:    public static 
	// Returns:   const char*
	// Qualifier: 除钻石外其他珠宝back渲染层
	//************************************
	static const char* JewelBackVert();
	static const char* JewelBackFrag();

	//************************************
	// Method:    RingVert
	// FullName:  M3D::ShaderSrcCode::RingVert
	// Access:    public static 
	// Returns:   const char*
	// Qualifier: 金属指环渲染层
	//************************************
	static const char* RingVert();
	static const char* RingFrag();

	//************************************
	// Method:    JewelTypeVert
	// FullName:  M3D::ShaderSrcCode::JewelTypeVert
	// Access:    public static 
	// Returns:   const char*
	// Qualifier: 珠宝类型标记层，标记是钻石还是玉石等
	//************************************
	static const char* JewelTypeVert();
	static const char* JewelTypeFrag();

	//************************************
	// Method:    JewelHighLightVert
	// FullName:  M3D::ShaderSrcCode::JewelHighLightVert
	// Access:    public static 
	// Returns:   const char*
	// Qualifier: 珠宝高光层
	//************************************
	static const char* JewelHighLightVert();
	static const char* JewelHighLightFrag();

	//************************************
	// Method:    JewelBlendQuadVert
	// FullName:  M3D::ShaderSrcCode::JewelBlendQuadVert
	// Access:    public static 
	// Returns:   const char *
	// Qualifier: 钻石、玉石、珍珠等混合层
	//************************************
	static const char * JewelBlendQuadVert();
	static const char * JewelBlendQuadFrag();

	//************************************
	// Method:    JewelBlendQuadVert
	// FullName:  M3D::ShaderSrcCode::JewelBlendQuadVert
	// Access:    public static
	// Returns:   const char *
	// Qualifier: 钻石混合层
	//************************************
	static const char * DiamondBlendQuadVert();
	static const char * DiamondBlendQuadFrag();

	//************************************
	// Method:    JewelFinalQuadVert
	// FullName:  M3D::ShaderSrcCode::JewelFinalQuadVert
	// Access:    public static 
	// Returns:   const char *
	// Qualifier: 最终效果层
	//************************************
	static const char * JewelFinalQuadVert();
	static const char * JewelFinalQuadFrag();

	//************************************
	// Method:    OutlineVert
	// FullName:  M3D::ShaderSrcCode::OutlineVert
	// Access:    public static 
	// Returns:   const char*
	// Qualifier: 外框线选择效果
	//************************************
	static const char* OutlineVert();
	static const char* OutlineFrag();

	static const char* GaussianBlurVert();
	static const char* GaussianBlurFrag();

	static const char * GaussianBlurOutlineVert();
	static const char * GaussianBlurOutlineFrag();

	static const char * CombineOutlineVert();
	static const char * CombineOutlineFrag();

	//************************************
	// Method:    SsaoEffectVert
	// FullName:  M3D::ShaderSrcCode::SsaoEffectVert
	// Access:    public static 
	// Returns:   const char*
	// Qualifier: ssao效果，实验性
	//************************************
	static const char* SsaoEffectVert();
	static const char* SsaoEffectFrag();
	static const char* SsaoBlurVert();
	static const char* SsaoBlurFrag();

	//************************************
	// Method:    SaoReconstructCSZVert
	// FullName:  M3D::ShaderSrcCode::SaoReconstructCSZVert
	// Access:    public static 
	// Returns:   const char*
	// Qualifier: sao重构Z
	//************************************
	static const char* SaoReconstructCSZVert();
	static const char* SaoReconstructCSZFrag();
	//************************************
	// Method:    SaoMinifyCSZVert
	// FullName:  M3D::ShaderSrcCode::SaoMinifyCSZVert
	// Access:    public static 
	// Returns:   const char*
	// Qualifier: sao重构Z
	//************************************
	static const char* SaoMinifyCSZVert();
	static const char* SaoMinifyCSZFrag();
	//************************************
	// Method:    SaoBlurVert
	// FullName:  M3D::ShaderSrcCode::SaoBlurVert
	// Access:    public static 
	// Returns:   const char*
	// Qualifier: sao模糊，实验性
	//************************************
	static const char* SaoBlurVert();
	static const char* SaoBlurFrag();
	//************************************
	// Method:    SaoEffectVert
	// FullName:  M3D::ShaderSrcCode::SaoEffectVert
	// Access:    public static 
	// Returns:   const char*
	// Qualifier: sao效果，实验性
	//************************************
	static const char* SaoEffectVert();
	static const char* SaoEffectFrag();

	//************************************
	// Method:    PhongMaterialVert
	// FullName:  M3D::ShaderSrcCode::PhongMaterialVert
	// Access:    public static 
	// Returns:   const char*
	// Qualifier: phong光照模型源码
	//************************************
	static const char* PhongMaterialVert();
	static const char* PhongMaterialFrag();

	//************************************
	// Method:    PbrMaterialVert
	// FullName:  M3D::ShaderSrcCode::PbrMaterialVert
	// Access:    public static 
	// Returns:   const char*
	// Qualifier: 物理光照源码
	//************************************
	static const char* PbrMaterialVert();
	static const char* PbrMaterialFrag();

	//************************************
	// Method:    SceneGroundVert
	// FullName:  M3D::ShaderSrcCode::SceneGroundVert
	// Access:    public static 
	// Returns:   const char *
	// Qualifier: 地板着色器材质源码
	//************************************
	static const char * SceneGroundVert();
	static const char * SceneGroundFrag();

	static const char * MirrorVert();
	static const char * MirrorFrag();

	//************************************
	// Method:    MatCapMaterialVert
	// FullName:  M3D::ShaderSrcCode::MatCapMaterialVert
	// Access:    public static 
	// Returns:   const char*
	// Qualifier: matcap材质着色器源码
	//************************************
	static const char* MatCapMaterialVert();
	static const char* MatCapMaterialFrag();

	//************************************
	// Method:    DepthMaterialVert
	// FullName:  M3D::ShaderSrcCode::DepthMaterialVert
	// Access:    public static 
	// Returns:   const char*
	// Qualifier: 深度材质着色器源码，用来生成深度贴图
	//************************************
	static const char* DepthMaterialVert();
	static const char* DepthMaterialFrag();

};

} /* namespace M3D */
#endif /* M3D_SHADERSRCCODE_H_ */

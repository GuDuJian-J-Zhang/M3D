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

	static const char * ModelBaseVert();
	static const char * ModelBaseFrag();

	static const char * BaseVertexVert();
	static const char * BaseVertexFrag();

	static const char * EdgeVert();
	static const char * EdgeFrag();

	static const char * WireframeVert();
	static const char * WireframeFrag();

	static const char * BrdfVert();
	static const char * BrdfFrag();
	static const char * AxisVert();
	static const char * AxisFrag();

	static const char * ImageVert();
	static const char * ImageFrag();

	static const char * CubeMapVert();
	static const char * CubeMapFrag();

	static const char * ClipPlaneVert();
	static const char * ClipPlaneFrag();

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

	static const char * MultilightPerFragVert();
	static const char * MultilightPerFragFrag();

};

} /* namespace M3D */
#endif /* M3D_SHADERSRCCODE_H_ */

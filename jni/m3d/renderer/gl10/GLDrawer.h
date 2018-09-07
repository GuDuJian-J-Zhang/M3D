/**@file
 *@brief	GL10绘制接口
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_GLDRAWER_H_
#define M3D_GLDRAWER_H_
#include "m3d/renderer/GLESHead.h"
#include "m3d/renderer/RenderContext.h"

namespace M3D
{

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

const float DELTA_ROT = 1.0E-10;
class SceneNode;
class Edge;
class VertexSet;
class DirectionalLight;
class MeshVBO;
class ModelVBO;
class VBOData;
class RenderAction;
class ImageBoard;

/**@class
 * @brief 渲染类，只有背景等的有作用
 *
 *
 *
 */
class M3D_API GLDrawer
{
public:
//	static bool useVBO;
//	static bool firstUp;
//	static float drawLimit;
	//剖视使用标志
//	static bool clipPlaneDrawing;
//	static int CurClipPlaneCount; //当前剖视平面个数
	//initialize 
	static void	InitialGL();

	static void ClearGLState();

	//draw background color
	static void DrawBackGround(SceneNode*, RenderAction*);

	//draw axis
	static void DrawAxis(SceneNode*, RenderAction*);

	//draw measures
//	static void DrawMeasures(SceneNode*,RenderAction*);

	static void DrawFPS(SceneNode*, RenderAction*);
	static void DrawText(SceneNode*, RenderAction*);

	//camera
	static void	ApplyCamera(SceneNode*, RenderAction*);

	//camera
	static void	ApplyCamera(RenderAction*);

//	static void	PrepareInteractiveInfo(SceneNode*, RenderAction*);

//	static void	DrawInteractiveInfo( RenderAction*);

	//Section
//	static void	DrawSection(SceneNode*, RenderAction*);

	//solidPlan
//	static void DrawSolidPlan(ImageBoard*, RenderAction*);

	//lights
	static void ApplyLights(RenderAction* action);

	static void	ApplyPointLight(DirectionalLight* lightData);
	static void	ApplyDirLight(DirectionalLight* light);

	//////////////////////////////////////////////////////////////////////////
	//tools
	static void LogGLState();

//public:
//	static void ViewPortConvert();

public:
	static int numbers;
	static long triNumber;
};

}

#endif /* M3D_GLDRAWER_H_ */


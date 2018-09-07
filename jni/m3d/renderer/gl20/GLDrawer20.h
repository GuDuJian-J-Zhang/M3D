/**@file
 *@brief    GL20绘制接口
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_GLDRAWER20_H_
#define M3D_GLDRAWER20_H_
#include "m3d/renderer/GLESHead.h"
#include "m3d/renderer/RenderContext.h"
namespace M3D
{

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

const float DELTA_ROT20 = 1.0E-10;
class SceneNode;
class Model;
class VertexSet;
class LightData;
class MeshVBO;
class ModelVBO;
class VBOData;
class RenderAction;
/**
 * @class
 * @brief 此类中的功能已经迁移到 @see GLShapeDrawer20 中
 */
class M3D_API GLDrawer20
{
public:
	static bool useVBO;
	static bool firstUp;

	static float drawLimit;

	//initialize
	static void InitialGL();

	//draw background color


	//draw axis
	static void DrawAxis(SceneNode*, RenderAction*);
	static void DrawFPS(SceneNode*, RenderAction*);
	static void DrawText(SceneNode*, RenderAction*);

	//camera
	static void
	DrawCamera(SceneNode*, RenderAction*);

	//Section
	static void
	DrawSection(SceneNode*, RenderAction*);

	//////////////////////////////////////////////////////////////////////////
	//tools
	static void LogGLState();
public:
	static void
	LoadMatrix(SceneNode* sceneNode, RenderContext* gl);

	static void LoadContextStateToShader(RenderContext* gl);

public:
	static int numbers;
	static long triNumber;

	static GLuint gProgram;
	static GLuint gvPositionHandle;
	static GLuint gvModelViewHandle;
	static GLuint gvProjectionHandle;
	static GLuint gvColorHandle;
	static void checkGlError(const char*);
	static GLuint createProgram(const char*, const char*);
	static GLuint loadShader(GLenum, const char*);
	static bool InitShaders();
};
}

#endif /* M3D_GLDRAWER20_H_ */


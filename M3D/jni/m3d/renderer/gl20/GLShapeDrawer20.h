/**@file
 *@brief    提供GL20，shape类型的绘制
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef  GLSHAPEDRAWER20_H_
#define GLSHAPEDRAWER20_H_
#include "m3d/M3D.h"
#include "m3d/renderer/GL20/GLDrawer20.h"
#include "m3d/action/RenderAction.h"

namespace M3D
{
class PMIData;
class Body;
class Face;
class Model;
class Note;
class VoiceNote;
class PMIData;
class RenderQueue;
class MeshBoard;
class ImageBoard;
class HandlerPoint;
class SectionPlane;
class Handler;

class Light;

typedef vector<Light*> LightList;

class GLShapeDrawer20: public GLDrawer20
{
public:
	static void
	DrawShapeNode(SceneNode*, RenderAction* action);

	//test

	static long drawBoxTime;
	static long drawMeshTime;
	static long drawPMITime;
	/**
	 * 绘制不同的渲染状态
	 * @param action 绘制动作
	 */
	static void DrawRenderPassGroup(RenderAction* action);
	/**
	 * 绘制不透明模型
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawSolidRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);

	/**
	 * 绘制透明模型
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawTranRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);

	/**
	 * 使用索引方式绘制纹理
	 * @param OGLObj 纹理对象id
	 * @param pointsBuffer
	 * @param textureCoordsBuffer
	 * @param indexBuffer
	 * @param num
	 */
	static void DrawTexture2DWithIndex(GLuint OGLObj, HardWareVertexBuffer* vertexBuffer,
			HardWareIndexBuffer * indexBuffer, Matrix4* texture2DTransform, int dataLength, M3D_OFFSET_TYPE indexOffset);

	/**
	 * 使用非索引方式绘制纹理
	 * @param OGLObj 纹理对象id
	 * @param pointsBuffer
	 * @param textureCoordsBuffer
	 * @param num
	 */
	static void DrawTexture2DNoIndex(GLuint OGLObj, HardWareVertexBuffer* vertexBuffer, Matrix4* texture2DTransform,
			int dataLength);

	/**
	 * 绘制三角网格模型
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawTriLineRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);

	/**
	 * 以非索引绘制三角网格边界线
	 * @param pointsBuffer
	 * @param normalBuffer
	 * @param num
	 */
	static void DrawTriLinesNoIndex(int num);

	/**
	 * 通过索引绘制三角网格边界线
	 * @param pointsBuffer
	 * @param normalBuffer
	 * @param indexBuffer
	 * @param num
	 */
	static void DrawTriLinesWithIndex(M3D_INDEX_TYPE * indexBuffer, int num);

	/**
	 * 绘制边界线
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawEdgesRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);

	/**
	 * 绘制边界线
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawEdgesInTopRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);

	static void DrawPointPassGroup(RenderAction* action, RenderQueue* RenderStateArray);

	static void DrawPoint(Point* point, ShaderManager * shaderManager, CameraNode* camera);

	static void DrawPoint(Point* point, ShaderManager * shaderManager, CameraNode* camera, Matrix4 & modelMat);

	static void DrawImageBoard(ImageBoard* imageboard, CameraNode * camera, Matrix4 & modelMat,
			ShaderProgram* shaderEffect);

	static void DrawMeshBoard(MeshBoard* meshboard, CameraNode * camera, Matrix4 & modelMat,
			ShaderProgram* shaderEffect);

	static void DrawPointHandler(HandlerPoint* handlerPoint, ShaderManager * shaderManager, CameraNode* camera,
			Matrix4 &modelMat);

	static void DrawMeshHandler(Handler* handler);

	static void DrawBoxRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);

	static void DrawShapeBox(BoundingBox& box, bool isSelected, ShaderParameter * vertex, ShaderProgram * shaderEffect,
			Matrix4 & modelMat);

	static void DrawNoteRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);
	static void DrawNote(Note* pNote, CameraNode* camera, ShaderManager * shaderManager, Matrix4& modelMat);

	static void DrawHandlerRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);

	static void DrawPMISRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);

	static void DrawPMINew(PMIData* pmiData, CameraNode* camera, ShaderManager * shaderManager, Matrix4& modelMat);

	static bool DoSection(RenderAction* action, bool state);

	static void DrawSectionPlane(RenderAction* action, SectionPlane* sectionPlane);
	static void DrawFPS(SceneNode*, RenderAction*);
	static void DrawBackGround(SceneNode*, RenderAction*);

	static void ApplyCamera(RenderAction* action);
private:

	//immediate draw
	static void
	DrawTriWithIndex(HardWareVertexBuffer* vertexBuffer, HardWareIndexBuffer * indexBuffer, int dataLength,
			M3D_OFFSET_TYPE indexOffset);
	static void
	DrawTriWithIndex(M3D_INDEX_TYPE * indexBuffer, int num);
	static void
	DrawTriNoIndex(HardWareVertexBuffer* vertexBuffer, int dataLength);
	static void
	DrawTriNoIndex(int num);

	static void DrawCapPolygon(RenderAction* action);

	//得到灯光出的深度图
	static void DrawShadowPass(RenderAction* action, vector<RenderQueue *> & RenderStateArray);

	//得到MainCamera处的深度图
	static void DrawDepthMapPass(RenderAction* action);

	static void PrepareDepthMap(RenderAction* action);

	static void DrawSkyShadow(RenderAction* action);

	/**
	 * 获取颜色和深度贴图
	 */
	static void PrepareBlurSourceMap(RenderAction* action);

	/**
	 * 次表面散射通道
	 */
	static void DrawSSSPass(RenderAction* action);

	static void DrawBlur(RenderAction* action);

	static void DrawPhongPass(RenderAction* action);

	/**
	 * phong绘制主要模型
	 */
	static void DrawPhongPassGroup(RenderAction* action,	RenderQueue* RenderStateArray);

	/**
	 * brdf绘制主要模型
	 */
	static void DrawBrdfPassGroup(RenderAction* action,	RenderQueue* RenderStateArray);

	static void DrawFrameBufferDebug(RenderAction* action);

	static void DrawEdgeDetectionPass(RenderAction* action);

	static void DrawEdgeDetectionQuad(RenderAction* action);

	static void DrawSectionLines(RenderAction* action,vector<Vector3>& data);

	static void DrawCrossSection(RenderAction* action,SectionPlane* sectionPlane);

	static void DrawSampleModelPassGroug(RenderAction* action);

#ifdef HUAWEI
	static void DrawPhongPassSampleGroup(RenderAction* action,	RenderQueue* RenderStateArray);
#endif
public:
	static void SetFocalLength(RenderAction* action);
private:
	static int m_lastWidth;
};

}

#endif /*GLSHAPEDRAWER20_H_*/

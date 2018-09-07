/**@file
 *@brief    提供GL10，shape类型的绘制
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef  M3D_GLSHAPEDRAWER_H_
#define M3D_GLSHAPEDRAWER_H_
#include "m3d/M3D.h"
#include "m3d/renderer/gl10/GLDrawer.h"
#include "m3d/action/RenderAction.h"
namespace M3D
{
class Body;
class Face;
class Model;
class Note;
class VoiceNote;
class PMIData;
class RenderQueue;
class MeshBoard;
class HandlerPoint;
class SectionPlane;
class Handler;

class DirectionalLight;

typedef vector<DirectionalLight*> LightList;

/**
 * @class
 * @brief OpenGL ES 1的渲染类，主要负责最后的渲染。
 */
class M3D_API GLShapeDrawer: public GLDrawer
{
public:
	//test
	static long drawBoxTime;
	static long drawMeshTime;
	static long drawPMITime;

	const static float PolygonOffset; //!<多边形偏移量

	/**
	 * 绘制不同的渲染状态
	 * @param action 绘制动作
	 */
	static void DrawRenderPassGroup(RenderAction* action);

	/**
	 * 设置是否进行剖切
	 * @param action 渲染操作动作
	 * @param state true开始剖切，false关闭剖切
	 */
	static bool DoSection(RenderAction* action,bool state);

	/**
	 * 绘制边界线
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawEdgesInTopRenderPassGroup(RenderAction* action,
			RenderQueue* RenderStateArray);

	/**
	 * 绘制PMI
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawPMISRenderPassGroup(RenderAction* action,
			RenderQueue* RenderStateArray);

	/**
	 * 绘制包围盒
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawBoxRenderPassGroup(RenderAction* action,
			RenderQueue* RenderStateArray);

	/**
	 * 绘制剖切示意平面
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawSectionPlane(RenderAction* action,
			SectionPlane* sectionPlane);

	/**
	 * 绘制边界线
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawEdgesRenderPassGroup(RenderAction* action,
			RenderQueue* RenderStateArray);
	/**
	 * 绘制透明模型
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawTranRenderPassGroup(RenderAction* action,
			RenderQueue* RenderStateArray);
	/**
	 * 绘制不透明模型
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawSolidRenderPassGroup(RenderAction* action,
			RenderQueue* RenderStateArray);
	/**
	 * 绘制点组
	 */
	static void DrawPointPassGroup(RenderAction* action,
			RenderQueue* RenderStateArray);

	/**
	 * 开启立方体贴图
	 * @param action
	 * @param name
	 */
	static void OpenCubeMapping(RenderAction* action , string name);

	/**
	 * 关闭立方体贴图
	 * @param action
	 */
	static void CloseCubeMapping(RenderAction* action);

	/**
	 * 绘制三角网格模型
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawTriLineRenderPassGroup(RenderAction* action,
			RenderQueue* RenderStateArray);

	/**
	 * 绘制批注
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawNoteRenderPassGroup(RenderAction* action,
			RenderQueue* RenderStateArray);

	/**
	 * 绘制测量
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawMeasureRenderPassGroup(RenderAction* action,
			RenderQueue* RenderStateArray);

	/**
	 * 绘制handler
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawHandlerRenderPassGroup(RenderAction* action,
			RenderQueue* RenderStateArray);

	/**
	 * 以非索引绘制三角网格边界线
	 * @param pointsBuffer
	 * @param normalBuffer
	 * @param num
	 */
	static void DrawTriLinesNoIndex(Vector3* pointsBuffer, Vector3* normalBuffer,
			int num);

	/**
	 * 通过索引绘制三角网格
	 * @param pointsBuffer
	 * @param normalBuffer
	 * @param indexBuffer
	 * @param num
	 */
	static void DrawTriWithIndex(Vector3* pointsBuffer, Vector3* normalBuffer,
			M3D_INDEX_TYPE* indexBuffer, int num);

	static void DrawTriWithIndex(HardWareVertexBuffer* vertexBuffer, HardWareIndexBuffer * indexBuffer, int dataLength,
		M3D_OFFSET_TYPE indexOffset);

	/**
	 * 以非索引绘制三角网格
	 * @param pointsBuffer
	 * @param normalBuffer
	 * @param num
	 */
	static void DrawTriNoIndex(Vector3* pointsBuffer, Vector3* normalBuffer,
			int num);

	static void
		DrawTriNoIndex(HardWareVertexBuffer* vertexBuffer, int dataLength);

	/**
	 * 通过索引绘制点
	 * @param pointsBuffer
	 * @param normalBuffer
	 * @param indexBuffer
	 * @param num
	 */
	static void DrawPntWithIndex(Vector3* pointsBuffer, Vector3* normalBuffer,
			M3D_INDEX_TYPE* indexBuffer, int num,int stride = 0);

	/**
	 * 以非索引绘制点
	 * @param pointsBuffer
	 * @param normalBuffer
	 * @param num
	 * @param stride 通过增加跨度来减少绘制点的个数
	 */
	static void DrawPntNoIndex(Vector3* pointsBuffer, Vector3* normalBuffer,
			int num,int stride = 0);

	/**
	 * 通过索引绘制三角网格边界线
	 * @param pointsBuffer
	 * @param normalBuffer
	 * @param indexBuffer
	 * @param num
	 */
	static void DrawTriLinesWithIndex(Vector3* pointsBuffer, Vector3* normalBuffer,
			M3D_INDEX_TYPE* indexBuffer, int num);

	/**
	 * 使用索引方式绘制纹理
	 * @param OGLObj 纹理对象id
	 * @param pointsBuffer
	 * @param textureCoordsBuffer
	 * @param indexBuffer
	 * @param num
	 */
	static void DrawTexture2DWithIndex(GLuint OGLObj, Vector3* pointsBuffer, Vector3* normalsBuffer, Vector3* textureCoordsBuffer,
			M3D_INDEX_TYPE* indexBuffer, int num,Matrix4* texture2DTransform);

	static void DrawTexture2DWithIndex(GLuint OGLObj, HardWareVertexBuffer* vertexBuffer,
		HardWareIndexBuffer * indexBuffer, Matrix4* texture2DTransform, int dataLength, M3D_OFFSET_TYPE indexOffset);

	/**
	 * 使用非索引方式绘制纹理
	 * @param OGLObj 纹理对象id
	 * @param pointsBuffer
	 * @param textureCoordsBuffer
	 * @param num
	 */
	static void DrawTexture2DNoIndex(GLuint OGLObj, Vector3* pointsBuffer, Vector3* normalsBuffer, Vector3* textureCoordsBuffer,
			int num,Matrix4* texture2DTransform);

	static void DrawTexture2DNoIndex(GLuint OGLObj, HardWareVertexBuffer* vertexBuffer, Matrix4* texture2DTransform,
		int dataLength);

	/**
	 * 绘制包围盒
	 * @param box
	 * @param isSelected
	 */
	static void DrawShapeBox(BoundingBox& box, bool isSelected);

	/**
	 * 绘制PMI
	 * @param pmiData
	 * @param action
	 */
	static void DrawPMINew(PMIData* pmiData, RenderAction* action);

	/**
	 * 绘制批注
	 * @param pNote
	 * @param action
	 */
	static void DrawNote(Note* pNote, RenderAction* action);

	/**
	 * 绘制语音批注
	 * @param pNote
	 * @param action
	 */
	static void DrawImageBoard(ImageBoard* imageboard);

	/**
	 * 绘制meshBoard
	 * @param meshboard
	 */
	static void DrawMeshBoard(MeshBoard* meshboard);

	/**
	 * 绘制点
	 * @param handlerPoint
	 */
	static void DrawPointHandler(HandlerPoint* handlerPoint);

	/**
	 * 绘制Mesh表示的Handler
	 * @param handler
	 */
	static void DrawMeshHandler(Handler* handler);

	/**
	 * 绘制点
	 * @param point
	 */
	static void DrawPoint(Point* point);

	/**
	 * 计算球面映射的纹理坐标
	 * @param OGLObj
	 * @param material
	 * @param faceRenderData
	 * @param texArray
	 * @param indexArray
	 * @param normalArray
	 * @param positionArray
	 * @param rotateMatrix
	 * @param dataLength
	 * @return
	 */
	static bool ComputerSphereTexCoords(unsigned int OGLObj,BaseMaterial* material,Renderable* faceRenderData,Vector3 * texArray,const M3D_INDEX_TYPE* indexArray,
			const Vector3 * normalArray,const Vector3 * positionArray,
			Matrix3 & rotateMatrix,int dataLength);

	static void DrawSampleModelPassGroug(RenderAction* action);
	static void DrawSectionLines(RenderAction* action,vector<Vector3>& data);
	static void DrawCapPolygon(RenderAction* action);

};

}

#endif /*M3D_GLSHAPEDRAWER_H_*/

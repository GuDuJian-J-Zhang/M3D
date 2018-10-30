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
#include "m3d/renderer/gl20/GLDrawer20.h"
#include "m3d/action/RenderAction.h"

#include "ShaderParameter.h"

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

class BaseLight;
class DirectionalLight;
class Material;
class UniformMap;
class MatCapMaterial;
class PbrMaterial;
class DepthMaterial;

typedef vector<DirectionalLight*> LightList;
//-----透明物体排序
struct TransparentObject
{
	float z;
	Renderable* renderable;
};


class M3D_API GLShapeDrawer20: public GLDrawer20
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
	static void DrawDraggerSolidRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);

	/**
	 * 绘制透明模型
	 * @param action
	 * @param RenderStateArray
	 */
	static void DrawTranRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);
	static void DrawDraggerTranRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);

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
			ShaderProgram* shaderEffect,bool useNote = false);

	static void DrawImageBoard(RenderAction* action, ImageBoard* imageboard, Matrix4 & V, Matrix4 & P, Matrix4 & modelMat);

	static void DrawImageBoard(ImageBoard* imageboard, Matrix4 & V, Matrix4 & P, Matrix4 & modelMat,
		ShaderProgram* shaderEffect, bool isAllowDepthTest = false);

	static void DrawMeshBoard(MeshBoard* meshboard, CameraNode * camera, Matrix4 & modelMat,
			ShaderProgram* shaderEffect);

	static void DrawPointHandler(HandlerPoint* handlerPoint, ShaderManager * shaderManager, CameraNode* camera,
			Matrix4 &modelMat);

	static void DrawMeshHandler(Handler* handler);

	static void DrawBoxRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);

	static void DrawRenderActionBox(RenderAction * action);

	static void DrawImageBoardQueue(RenderAction * action);

	static void DrawUIQueue(RenderAction * action);

	static void DrawHUBImageQueue(RenderAction * action);

	static void DrawShapeBox(BoundingBox& box, bool isSelected, ShaderParameter * vertex, ShaderProgram * shaderEffect,
			Matrix4 & modelMat);

	static void DrawNoteRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);
	static void DrawNote(Note* pNote, CameraNode* camera, ShaderManager * shaderManager, Matrix4& modelMat);

	static void DrawAnnotationRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);
	static void DrawAnnotation(Note* pNote, CameraNode* camera, ShaderManager * shaderManager, Matrix4& modelMat);

	static void DrawHandlerRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);

	static void DrawPMISRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);

	static void DrawPMINew(PMIData* pmiData, CameraNode* camera, ShaderManager * shaderManager, Matrix4& modelMat);

	static bool DoSection(RenderAction* action, bool state);

	static void DrawSectionPlane(RenderAction* action, SectionPlane* sectionPlane);
	static void DrawFPS(SceneNode*, RenderAction*);
	static void DrawBackGround(SceneNode*, RenderAction*);
	static void DrawVRBackGround( RenderAction*);

	static void ApplyCamera(RenderAction* action);

	static void DrawINTOPRenderPassGroup(RenderAction* action, RenderQueue* RenderStateArray);

	static GLuint AllocTextureUnit();
//private:

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
	static void DrawCapPolygon(RenderAction* action, int index);

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

	static void DrawBasePass(RenderAction * action, RenderQueue * RenderStateArray);
	
	static void DrawDraggerLinePass(RenderAction * action, RenderQueue * RenderStateArray);

	/**
	*绘制拖拽器
	*/
	static void DrawDraggerPass(RenderAction* action);


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

	static void DrawSectionLines(RenderAction* action,vector<Vector3>& data,int id);

	static void DrawCrossSection(RenderAction* action,SectionPlane* sectionPlane);

	static void DrawSampleModelPassGroug(RenderAction* action);

	static void DrawOutlinePassGroup(RenderAction* action, RenderQueue* RenderStateArray);

	static void DrawSSAOPassGroup(RenderAction* action, RenderQueue* RenderStateArray);


#ifdef HUAWEI
	static void DrawPhongPassSampleGroup(RenderAction* action,	RenderQueue* RenderStateArray);
#endif
	static void DrawGroundNode(SceneNode* node, RenderAction* action);

	static void DrawMirrorGroundNode(SceneNode* node, RenderAction* action, const Matrix4 & mirrorMat, GLuint mirrorTexture, GLuint mirrorBackground);
public:
	static void SetFocalLength(RenderAction* action);
	static GLuint _usedTextureUnits;
private:
	static int m_lastWidth;


public:
	//************************************
	// Method:    RenderFaces
	// FullName:  M3D::GLShapeDrawer20::RenderFaces
	// Access:    public static 
	// Returns:   void
	// Qualifier: 渲染非透明物体
	// Parameter: RenderAction * action
	// Parameter: vector<Renderable * > & renderableArray
	// Parameter: BaseMaterial * material
	//************************************
	static void RenderFaces(RenderAction* action, vector<Renderable*>& renderableArray,BaseMaterial* material = nullptr);
	//************************************
	// Method:    RenderFaces
	// FullName:  M3D::GLShapeDrawer20::RenderFaces
	// Access:    public static 
	// Returns:   void
	// Qualifier: 渲染透明物体
	// Parameter: RenderAction * action
	// Parameter: vector<TransparentObject> & renderableArray
	// Parameter: BaseMaterial * material
	//************************************
	static void RenderFaces(RenderAction* action, vector<TransparentObject>& renderableArray, BaseMaterial* material = nullptr);
	//************************************
	// Method:    RenderFace
	// FullName:  M3D::GLShapeDrawer20::RenderFace
	// Access:    public static 
	// Returns:   void
	// Qualifier: 渲染单个face
	// Parameter: RenderAction * action
	// Parameter: Renderable * renderable
	// Parameter: CameraNode * camera
	// Parameter: BaseMaterial * material
	//************************************
	static void RenderFace(RenderAction* action,Renderable*renderable, CameraNode*camera = nullptr, BaseMaterial* material = nullptr);
	//************************************
	// Method:    SetProgram
	// FullName:  M3D::GLShapeDrawer20::SetProgram
	// Access:    public static 
	// Returns:   ShaderProgram*
	// Qualifier: 设置shaderprogram uniform变量
	// Parameter: RenderAction * action
	// Parameter: Renderable * renderable
	// Parameter: map<int
	// Parameter: GLenum> & textureBindingTargetMap
	// Parameter: CameraNode * camera
	// Parameter: BaseMaterial * material
	//************************************
	static ShaderProgram* SetProgram(RenderAction* action, Renderable* renderable, map<int, GLenum>& textureBindingTargetMap,CameraNode*camera = nullptr, BaseMaterial* material = nullptr);
	//************************************
	// Method:    InitMaterial
	// FullName:  M3D::GLShapeDrawer20::InitMaterial
	// Access:    public static 
	// Returns:   void
	// Qualifier: 初始化Material
	// Parameter: RenderAction * action
	// Parameter: BaseMaterial * material
	// Parameter: ShaderManager * shaderManager
	//************************************
	static void InitMaterial(RenderAction* action, BaseMaterial* material, ShaderManager* shaderManager);
	static BaseMaterial* GetDepthMaterial(BaseLight* light, ResourceManager*resourceManager, bool isPointLight);
	//************************************
	// Method:    ShadowRender
	// FullName:  M3D::GLShapeDrawer20::ShadowRender
	// Access:    public static 
	// Returns:   void
	// Qualifier: 阴影渲染pass，//TODO完成了平行光相关
	// Parameter: RenderAction * action
	// Parameter: RenderQueue * RenderStateArray
	//************************************
	static void ShadowRender(RenderAction* action, RenderQueue* RenderStateArray);
	static void ShadowRenderPass(RenderAction* action);
	//************************************
	// Method:    RefreshUniformsPhong
	// FullName:  M3D::GLShapeDrawer20::RefreshUniformsPhong
	// Access:    public static 
	// Returns:   void
	// Qualifier: 刷新phong材质uniform
	// Parameter: RenderAction * action
	// Parameter: UniformMap * unifomValueList
	// Parameter: Material * material
	//************************************
	static void RefreshUniformsPhong(RenderAction*action, UniformMap* unifomValueList,Material* material);
	//************************************
	// Method:    RefreshUniformsPbr
	// FullName:  M3D::GLShapeDrawer20::RefreshUniformsPbr
	// Access:    public static 
	// Returns:   void
	// Qualifier: 刷新pbr材质uniform
	// Parameter: RenderAction * action
	// Parameter: UniformMap * unifomValueList
	// Parameter: PbrMaterial * material
	//************************************
	static void RefreshUniformsPbr(RenderAction*action, UniformMap* unifomValueList, PbrMaterial* material);
	//************************************
	// Method:    RefreshUniformsMatCap
	// FullName:  M3D::GLShapeDrawer20::RefreshUniformsMatCap
	// Access:    public static 
	// Returns:   void
	// Qualifier: 刷新matcap材质uniform
	// Parameter: RenderAction * action
	// Parameter: UniformMap * unifomValueList
	// Parameter: MatCapMaterial * material
	//************************************
	static void RefreshUniformsMatCap(RenderAction*action, UniformMap* unifomValueList, MatCapMaterial* material);
private:
	static ShaderProgram* m_currentProgram;//当前使用的shaderprogram
	static BaseMaterial* m_currentMaterial;//当前使用的材质
	static CameraNode* m_currentCamera;//当前使用的相机
	static bool m_ssaoLastState;//当前ssao开启状态
	static bool m_shadowState;//当前阴影开启状态
	static DepthMaterial* depthMaterial;//缓存的深度贴图材质，阴影相关
	static HardWareFrameBuffer* renderTarget;//缓存的渲染目标，用来缓存阴影相关

};

}

#endif /*GLSHAPEDRAWER20_H_*/

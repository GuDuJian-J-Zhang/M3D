
/**@file
 *@brief	渲染动作，渲染过程精确控制
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */

#ifndef  M3D_RENDERACTION_H_
#define  M3D_RENDERACTION_H_

#include "m3d/action/Action.h"
#include "m3d/base/ControlInfo.h"
#include "m3d/model/MeshData.h"
#include "m3d/utils/CullerHelper.h"
#include "m3d/graphics/HardWareFrameBuffer.h"

namespace M3D
{
class DirectionalLight;
class Model;
class Body;
class Face;
class Edge;
class Note;
class RenderQueue;
class Section;
class Mesh;
class VertexSet;
class BaseMaterial;
class Renderable;
class Point;
class HandlerRotateCenter;
class HardWareFrameBuffer;
class SectionLine;
class PMIData;
class ModelShape;
class HandlerGroup;
class ShaderManager; 
class ShaderProgram;
class ImageBoard;
class GroundNode;
/**
 * @brief 剖切数据封装类
 */
struct SectionData
{
	vector< vector<Vector3> >*m_scetionLinesData;
	Section * m_section;
};

/**
 * @brief 灯光列表
 */
typedef vector<DirectionalLight*> LightList;

/**
 * @brief 渲染队列组
 */
typedef map<int, RenderQueue> RenderQueueGroup;

/**
 * @brief 小件剔除的级别
 */
class M3D_API CullerMode{
private:
	const static int ALL_LEVEL; //!<所有级别都进行剔除判断
	const static int MODEL_LEVEL ;//!<Model级别进行剔除判断
	const static int BODY_LEVEL ;//!<Body级别进行剔除判断
	const static int MESH_LEVEL ;//!<Mesh级别进行剔除判断
	const static int FACE_LEVEL ;//!<Face级别进行剔除判断
public:
	/**
	 * @brief 构造函数
	 */
	CullerMode();

	/**
	 * @brief 析构函数
	 */
	~CullerMode();
	/**
	 * @brief 设置是否开启面剔除
	 * @param isCullerFace
	 */
	void SetCullerFace(bool isCullerFace);
	/**
	 * @brief 获取是否允许面剔除
	 * @return
	 */
	bool AllowCullerFace();

	/**
	 *@brief 设置是否允许进行Model级别的剔除
	 * @param isCullerModel
	 */
	void SetCullerModel(bool isCullerModel);

	/**
	 * @brief 获取是否允许进行Model基本的剔除
	 * @return
	 */
	bool AllowCullerModel();

private:
	int m_CullerMode;//!<进行小件剔除的级别
};

/**
 *@brief 绘制类型，根据绘制类型将显示数据分组，
 *每一组数据构成了一个RenderGroup
 *比如，边界线构成了边界线显示组，半透明显示构成了半透明显示组
 */
class M3D_API RenderableType
{
public:
	const static int RGT_EDGELINE ;//!<边界线
	const static int RGT_TRILINE ;//!<三角网格
	const static int RGT_SOLID ;//!<实体不透明显示
	const static int RGT_BOX ;//!<包围盒
	const static int RGT_TRANSPARENT ;//!<半透明
	const static int RGT_TRANSLATEEDGE ;//!<半透明边
	const static int RGT_EDGELINEINTOP;//!<最前端显示的边界线 TODO
	const static int RGT_INTOP;//!<最前端显示 TODO
	const static int RGT_PMI;//!<PMI显示 TODO
	const static int RGT_POINT;//!<点显示
	const static int RGT_NOTE ;//!< NOTE显示
	const static int RGT_ANNOTATION;//!<批注显示
	const static int RGT_MEASURE ;//!<MEASURE显示
	const static int RGT_HANDLER  ;//!<HANDLER PMI显示

	const static int RGT_SOLID_TRAN ;//!<实体透明显示

	const static int RGT_SHADOW;
public:
	/**
	 *@brief 构造函数
	 *默认类型为实体不透明显示
	 */
	RenderableType()
	{
		m_type = RGT_SOLID;
	}

	/**
	 * @brief 构造函数，指定类型
	 * @param type
	 */
	RenderableType(int type)
	{
		this->m_type = type;
	}

	/**
	 * @brief 得到绘制类型
	 * @return
	 */
	int GetType()
	{
		return this->m_type;
	}
private:
	int m_type;//!<绘制类型
};


/**
 * @brief 渲染技术
 * 通过参数的配置，能够实现不同的渲染效果
 * !TODO
 */
class M3D_API RenderTech
{
public:
	/**
	 *
	 */
	RenderTech();
	/**
	 *
	 * @return
	 */
	bool IsColorMask();
	/**
	 *
	 * @param state
	 */
	void SetColorMask(bool state);

	/**
	 *
	 */
	void BeginEffect();
	/**
	 *
	 */
	void EndEffect();
	/**
	 *
	 * @param type
	 */
	void SetRenderGroupType(RenderableType type);
	/**
	 *
	 * @return
	 */
	RenderableType GetRenderGroupType();

private:
	RenderableType m_renderGroupType;//!<

	bool m_bColorMask;//!<OpenGL颜色掩码
	bool m_bDepthMask;//!<OpenGL深度测试掩码
	bool m_DepthTest;//!<OpenGL是否进行深度测试

};

/**
 *@brief  可绘制类型过滤器，通过掩码操作来控制不同类型数据的显隐
 * 在准备显示队列的过程中，通过绘制类型的过滤，来实现不同的显示效果
 * 比如，如果绘制类型中没有边界线，则不将边界线数据加入显示队列中，从而不进行边界线数据的显示
 */
class M3D_API RenderableTypeGroup
{
public:
	const static int RENDERDATA_NORMALEFFCT ; //!<  正常渲染
	const static int RENDERDATA_OPACITYEDGE  ; //!<  不透明边界线渲染
	const static int RENDERDATA_SECTION ; //!<  盖面渲
	const static int RENDERDATA_SOLIDEDGE; //!<  实体和线框
	const static int RENDERDATA_LINE  ;//!<  线框渲染
	const static int RENDERDATA_TRIANDSOLID; //!<  三角线框和实体渲染
	const static int RENDERDATA_RENDERTRISOLIDBOX;//!<  线框和实体，包围盒渲染

public:
	/**
	 *@brief 构造函数，默认指定可渲染的数据类型正常渲染
	 *可以参考正常渲染包含的显示类型
	 */
	RenderableTypeGroup()
	{
		m_renderTypes = RENDERDATA_NORMALEFFCT;
	}

	/**
	 * @brief 是否包含参数指定的渲染类型
	 * @param type 渲染类型掩码
	 * @return true包含此类型  false 不包含
	 */
	bool Contain(int renderType)
	{
		return ((m_renderTypes & renderType )== renderType);
	}

	/**
	 * @brief 得到绘制类型过滤数据
	 * @return
	 */
	int GetRenderTypes()
	{
		return this->m_renderTypes;
	}

	/**
	 * @brief 设置过滤类型数据
	 * @param types
	 */
	void SetRenderTypes(int types)
	{
		this->m_renderTypes = types;
	}

	/**
	 * @brief 允许绘制指定参数类型
	 * @param type RenderType类型
	 */
	void Open(int type){
		int currRenderFilter= GetRenderTypes();
		SetRenderTypes(currRenderFilter|type);
	}

	/**
	 * @brief 不允许绘制指定参数类型
	 * @param type  RenderType类型
	 */
	void Close(int type){
		int currRenderFilter= GetRenderTypes();
		SetRenderTypes(currRenderFilter&((type & 0xFFFF) ^ 0xFFFF));
	}

private:
	int m_renderTypes;//!< 运行渲染类型的组，每一位标识一种渲染类型
};

/**
 * @brief 渲染队列绘制顺序
 * 通过指定绘制顺序能够实现不同的绘制效果
 * TODO
 */
class M3D_API RenderQueuePriority
{
public:
	/**
	 *
	 */
	RenderQueuePriority();
	/**
	 *
	 */
	virtual~RenderQueuePriority();
	/**
	 *
	 * @return
	 */
	vector<RenderTech*>& GetData();
	/**
	 *
	 */
	void InitialOpacityLineEffect();
	/**
	 *
	 */
	void InitialNormalEffect();
	/**
	 *
	 */
	void InitialSectionEffect();
private:
	vector<RenderTech*> m_renderQueueTechs;
};


/**
 * @brief 渲染效果类
 * 通过指定绘制顺序，使用不同的绘制技术
 * 实现不同的绘制效果
 */
class  M3D_API RenderEffect
{
	
public:
	const static int RENDER_NORMALEFFCT ; //!<  正常渲染
	const static int RENDER_OPACITYEDGE ; //!<  不透明边界线渲染
	const static int RENDER_SECTION; //!<  盖面渲
public:
	/**
	 * @brief 构造函数
	 */
	RenderEffect()
	{
	  this->m_effectType = RENDER_NORMALEFFCT;
	}

	/**
	 * @brief 构造函数
	 * @param effectType 渲染效果类型
	 */
	RenderEffect(int effectType)
	{
		this->m_effectType = effectType;
	}

	/**
	 * @brief 获取渲染队列的优先级
	 * @return 渲染队列优先级
	 */
	RenderQueuePriority& GetRenderQueuePriority()
	{
		return this->m_renderQueuePriority;
	}

	/**
	 * @brief 设置渲染队列的优先级
	 * @param renderQueuePriority 渲染队列的优先级
	 */
	void SetRenderQueuePriority(const RenderQueuePriority& renderQueuePriority)
	{
		this->m_renderQueuePriority = renderQueuePriority;
	}

	/**
	 * @brief 获取可渲染对象类型过滤器
	 * @return 可渲染对象类型过滤器
	 */
	RenderableTypeGroup& GetRenderableTypeFilter()
	{
		return this->m_renderableTypeFilter;
	}

	/**
	 * @brief 设置可渲染对象类型过滤器
	 * @param typeFilter 类型过滤器
	 */
	void SetRenderableTypeFilter(const RenderableTypeGroup& typeFilter)
	{
		this->m_renderableTypeFilter = typeFilter;
	}

private:
	RenderQueuePriority m_renderQueuePriority;//!<渲染队列优先级
	RenderableTypeGroup m_renderableTypeFilter;//!<可渲染对象过滤器
	int m_effectType;//!<渲染效果类型
};

/**
 * @brief 可渲染对象数组
 */
typedef vector<Renderable*> RenderabelArray;

/**
 * @brief 渲染队列类，
 * 存储同一类型的Renderable数据，在查找可见显示数据时
 * 根据对象显示时所对应的状态，将其加入对应的队列中
 */
class M3D_API RenderQueue
{
public:
	/**
	 * @brief 构造函数
	 */
	RenderQueue();

	/**
	 * @brief 构造函数
	 * @param state
	 */
	RenderQueue(RenderableType type);

	/**
	 * @brief 将可渲染对象加入队列中
	 * @param renderable 可渲染对象
	 */
	void Push( Renderable* renderable);

	/**
	 * @brief 清空队列
	 */
	void Clear();;

	/**
	 *@brief  获取当前队列存储的可渲染对象的类型
	 * @return 可渲染对象的类型
	 */
	RenderableType GetType();

	/**
	 * @brief 设置当前队列存储的可渲染对对象的类型
	 * @param type 可渲染对象的类型
	 */
	void setType(RenderableType type);

	/**
	 * @brief 获取队列中所有的可渲染对象
	 * @return
	 */
	RenderabelArray& GetRenderableArray();

	/**
	 * @brief 设置队列中所有的可渲染对象
	 * @param statesQueue
	 */
	void SetRenderableArray(RenderabelArray& renderableArray);

	/**
	 * @brief 设置此渲染队列需要使用的渲染技术
	 * @param renderTech 渲染技术
	 */
	void SetRenderTech(RenderTech* renderTech)
	{
		this->m_renderTech = renderTech;
	}

	/**
	 * @brief 获取当前队列所使用的渲染技术
	 * @return
	 */
	RenderTech* GetRenderTech()
	{
		return this->m_renderTech;
	}

private:
	RenderTech* m_renderTech;//!< 队列所使用的渲染技术
	RenderableType  m_rednerType;//!< 队列所存储的可渲染对象的类型
	RenderabelArray m_renderableArray;//!<可渲染对象存储位置
};

class VBOData;
class CameraNode;
class CullerHelper;
class SceneManager;
class RenderAction;
class SceneNode;
class BackgroundNode;
class FPSNode;
class AxisNode;
class HandlerGroup;
class MeasureGroup;
class NoteGroup;
class RenderManager;
class Point;
class SceneGroundNode;

/**
 * @class
 * @brief 渲染动作，遍历场景节点过程中，查找可见渲染对象加入渲染队队列中
 */
class M3D_API RenderAction: public Action
{
public:
	/**
	 * @brief 构造函数
	 * @param renderMgr 渲染管理器
	 */
	RenderAction(RenderManager* renderMgr);

	/**
	 * @brief 析构函数
	 */
	virtual ~RenderAction();

	/**
	 * @brief 重置渲染动作
	 */
	virtual void Reset();

	/**
	 * @brief 将此动作应用到节点上
	 * @param node 应用此动作的节点
	 */
	virtual void Apply(SceneNode * node);

	/**
	 * @brief 获取到渲染管理对象
	 * @return 渲染管理对象
	 */
	RenderManager* GetRenderManager();

	/**
	 *@brief 开始动作
	 */
	void Begin();

	/**
	 * @brief 执行动作
	 */
	void Execute();

	/**
	 * @brief 结束动作
	 */
	void End();

	/**
	 *@brief  更新渲染参数，在每个model查找可见对象函数中调用，
	 * 通过调用此函数能够及时响应外界设置的显示时所需的参数
	 */
	void UpdataRenderPara(Body* body);

	/**
	 * @brief 将可渲染对象，加入指定类型的渲染队列中
	 * @param renderable 可渲染对象
	 * @param type 指定类型
	 */
	void PushRenderable(Renderable* renderable, int  type);

	/**
	 *@brief  是否显示旋转中心
	 * @return true显示 false 不显示
	 */
	bool ShowRotationCenter();

	/**
	 * @brief 得到渲染效果
	 * @return 渲染效果对象
	 */
	RenderEffect* GetRenderEffect();

	/**
	 * @brief 设置渲染效果
	 * @param renderEffect 渲染效果对象
	 */
	void SetRenderEffect(RenderEffect* renderEffect);

	/**
	 * @brief 设置准备显示数据所需的世界矩阵
	 * @param matrix 世界矩阵
	 */
	void SetWorldMatrix(Matrix3x4* matrix);

	/**
	 * @brief 获取当前准备显示数据所需的世界矩阵
	 * @return 世界矩阵
	 */
	Matrix3x4* GetWorldMatrix();

	/**
	 * @brief 设置准备显示数据所需的世界矩阵
	 * @param matrix 世界矩阵
	 */
	void SetGLWorldMatrix(Matrix4* matrix);

	/**
	 * @brief 获取当前准备显示数据所需的世界矩阵
	 * @return 世界矩阵
	 */
	Matrix4* GetGLWorldMatrix();

	/**
	 * @brief 准备显示边界，将Body中的Edge数据加入显示队列
	 * @param body
	 */
	void PrepareRenderEdges(Body* body);

	/**
	 * @brief 准备边界显示数据，将Face中的Edge数据加入显示队列
	 * @param face
	 */
	void PrepareRenderEdges(Face* face);

	/**
	 * @brief 将Edge加入显示队列
	 * @param edge
	 */
	void PrepareRenderEdges(Edge* edge);

	/**
	 * @brief 将Face数据加入显示队列
	 * @param face
	 */
	void PrepareRenderFace(Face* face);

	/**
	 *
	 * @brief 将Mode中的PMI数据加入显示队列
	 * @param model
	 */
	void PrepareRenderPMIS(Model* model);
	void PrepareRenderPMIS(map<int, PMIData*>* pmiDatas);
	
	/**
	 * @brief 将Mode中的BoundingBox数据加入显示队列
	 * @param model
	 */
	void PrepareRenderBox(ModelShape* modelShape);

	/**
	 * @brief 将Section数据加入显示队列
	 * @param section
	 */
	void PrepareRenderSection(Section* section);

	/**
	 * @brief 将Note数据加入显示队列
	 * @param note
	 */
	void PrepareRenderNote(Note* Note);

	void PrepareRenderAnnotation(Note* note);

	void PrepareRenderImage(ImageBoard* imageBoard);

	/**
	 * @brief 将点对象加入渲染队列
	 * @param point
	 */
	void PreapareRenderPoint(Point* point);

	/**
	 * @brief 将model中的显示数据加入渲染队列
	 * @param model
	 */
	void PrepareRenderMeasure(Model* model);

	/**
	 *
	 * @brief 开始一次面合并
	 */
	void StartMergeFace();

	/**
	 * @brief 结束一次合并面操作
	 */
	void FinishMergeFace();

	/**
	 * @brief 开始一次边界线数据合并
	 */
	void StartMergeEdge();

	/**
	 * @brief 结束一次合并边界线数据操作
	 */
	void FinishMergeEdge();

	/**
	 * @brief 合并面显示数据
	 * @param faceRenderData
	 * @param action
	 * @return true合并成功，false合并失败
	 */
	bool Merge(Face* faceRenderData);

	/**
	 * @brief 合并边界线显示数据
	 * @param edgeRenderData
	 * @param action
	 * @return true合并成功，false合并失败
	 */
	bool Merge(Edge* edgeRenderData);

	/**
	 * @brief 得到摄像机
	 * @return
	 */
	CameraNode* GetCamera();

	/**
	 * @brief 设置摄像机
	 * @param camera
	 */
	void SetCamera(CameraNode* camera);


	/**
	 * @brief 获取渲染时所需的灯光
	 * @return
	 */
	LightList* GetLights();

	/**
	 * @brief 设置渲染时所需的灯光
	 * @param lightList
	 */
	void AddLight(DirectionalLight* light);

	/**
	 * @brief 设置渲染时所需的灯光
	 * @param lightList
	 */
	void SetLights(LightList* lights);

	/**
	 * @brief 设置背景节点
	 * @param backgroundColor
	 */
	void SetBackGroundNode(BackgroundNode* backgroundColor);

	/**
	 * @brief 获取背景节点
	 * @return
	 */
	BackgroundNode* GetBackGroundNode();

	/**
	 * @brief 设置帧数信息节点
	 * @param fpsNode
	 */
	void SetFPSNode(FPSNode* fpsNode);

	/**
	 * @brief 获取帧数信息节点
	 * @return
	 */
	FPSNode* GetFPSNode();

	/**
	 * @brief 设置坐标轴节点
	 * @param axisNode
	 */
	void SetAxisNode(AxisNode* axisNode);

	/**
	 * @brief 获取坐标轴节点
	 * @return
	 */
	AxisNode* GetAxisNode();

	void FillDelayDrawRenderQueue(RenderQueue& renderQueue,int queueGroup)
	{
		m_RenderQueueGroup[queueGroup] = renderQueue;
	}

	/**
	 * @brief 获取剔除操作工具类
	 * @return
	 */
	CullerHelper& GetCullerHelper();

	/**
	 * @brief 获取显示队列组
	 * @return
	 */
	RenderQueueGroup& GetRenderQueueGroup();
	/**
	 * @brief 获取剔除方式
	 * @return
	 */
	CullerMode& GetCullerMode();
	/**
	 * @brief 设置剔除方式
	 * @param cullerMode
	 */
	void SetCullerMode(const CullerMode& cullerMode);

	/**
	 * @brief 设置使用盖面
	 * @param scetion
	 */
	void SetSection(Section* section);

	/**
	 * @brief 获取盖面数据
	 * @return
	 */
	Section* GetSection();

	/**
	 * @brief 获取当前指定的Lod级别
	 */
	int GetCurretSpecifyLod();
	/**
	 * @brief 设置当前使用的Lod级别，在遍历每一个model时进行设置，
	 * 将指定的Lod级别设置进来
	 * @param lod
	 */
	void SetCurrentSpecifyLod(int lod);

	/**
	 * @brief 设置帧率
	 * @param fps
	 */
	void SetFPS(int fps);

	/**
	 * @brief 获得当前帧率
	 * @return
	 */
	int GetFPS();

	/**
	 * @brief 设置最小帧率
	 * @param fps
	 */
	void SetMinFPS(int fps);

	/**
	 * @brief 最小帧率，如果小于此帧率
	 * 应该想办法减少绘制内容
	 * @return
	 */
	int GetMinFPS();

	/**
	 * @brief 设置批注待渲染节点
	 * @param noteGroup
	 */
	void SetNoteGroup(NoteGroup* noteGroup);

	/**
	 * @brief 得到批注节点
	 * @return
	 */
	NoteGroup* GetNoteGroup();

	/**
	 * @brief 设置测量待渲染节点
	 * @param measureGroup
	 */
	void SetMeasures(MeasureGroup* measureGroup);

	/**
	 * @brief 得到测量待渲染节点
	 * @return
	 */
	MeasureGroup* GetMeaures();

	/**
	 * @brief 设置旋转中心点，为空 不显示旋转中心点
	 * @param rotateCenter
	 */
	void SetRotateCenter(HandlerRotateCenter* rotateCenter);

	/**
	 * @brief 获取旋转中心点
	 * @return
	 */
	HandlerRotateCenter* GetRotateCenter();

	/**
	 * @brief 设置场景管理
	 * @param scene
	 */
	void SetScene(SceneManager* scene);
	/**
	 * @brief 获取场景管理节点
	 * @return 场景管理类
	 */
	SceneManager* GetScene();

	/**
	 * @brief 得到shader管理对象
	 * @return shader管理对象
	 */
	ShaderManager * GetShaderMananger();
	/**
	 * @brief 是否shader管理对象
	 */
	void ReleaseShaderManager();

	/**
	 * @brief 得到地面节点
	 * @return
	 */
	SceneGroundNode* GetSceneGroundNode();
	/**
	 * @brief 设置地面节点
	 * @param sceneGroundNode
	 */
	void SetSceneGroundNode(SceneGroundNode* sceneGroundNode);
	/**
	 * @brief 清除GL渲染资源
	 */
	void ClearGLResource();

	/**
	 * @brief 重新设置FBO 大小
	 * @param width FBO宽
	 * @param heightFBO高
	 */
	void ResizeFBOs(int width,int height);

	/**
	 *@brief 清除延迟渲染队列
	 */
	void ClearDelayDraw();

	/**
	 * @brief 设置加入延迟渲染
	 * @param delayDraw
	 */
	void SetDelayDraw(bool delayDraw);

	/**
	 * @brief 将渲染对象，加入渲染队列
	 * @param renderable
	 */
	void AddToDelayDraw(Renderable* renderable);

	/**
	* @brief 将渲染对象，加入渲染队列
	* @param renderable
	*/
	void AddFaceToDrawQueue(Face* renderable);

	/**
	 * @brief 当前是否需要延迟渲染
	 * @return true 是 false 不需要
	 */
	bool IsDelayDraw();


	/**
	 * @brief 开始延时渲染逻辑
	 * @return true 没有处理完演示渲染队列，需要在下一帧继续渲染，false 处理完成返回
	 */
	bool ProcessDelayDraw();

	/**
	 * 设置是否允许延迟渲染
	 * @param allowDelayDraw
	 */
	void SetAllowDelayDraw(bool allowDelayDraw);

	/**
	 * 优化渲染队列
	 */
	void Optimize();

	void BeginDelayDraw();

	void EndDelayDraw();

	void SetImmediateDrawBegin(bool immediateDraw);

	bool IsImmediateDrawBegin();

	/**
	* @brief 获取渲染时所需的灯光
	* @return
	*/
	LightList* GetModelLights();

	/**
	* @brief 设置渲染时所需的灯光
	* @param lightList
	*/
	void AddModelLight(DirectionalLight* light);

	/**
	* @brief 设置渲染时所需的灯光
	* @param lightList
	*/
	void SetModelLights(LightList* lights);

	void ClearRenderBox();
	void MergeRenderBox(BoundingBox& mergeBox);
	BoundingBox& GetRenderBox();

	/**
	* @brief 设置背景节点
	* @param backgroundColor
	*/
	void SetGroundNode(GroundNode* groundNode);

	/**
	* @brief 获取背景节点
	* @return
	*/
	GroundNode* GetGroundNode();

	/**
	* @brief 将Face中的数据，加入以线着色模式的渲染队列
	* @param face
	*/
	void FacePrepareLineMesh(Face* face);
	bool GetSceneBoxChanged() const;
	void SetSceneBoxChanged(bool val);
	HandlerGroup* GetHandlerGroupNode() const;
	void SetHandlerGroupNode(HandlerGroup* val);
	map<int, M3D::RenderQueue>* GetWorkingRenderQueueGroup() const;
	void SetWorkingRenderQueueGroup(map<int, M3D::RenderQueue>* val);
	map<int, M3D::RenderQueue>& GetDraggerRenderQueueGroup()  { return m_DraggerRenderQueueGroup; }
	void SetDraggerRenderQueueGroup(map<int, M3D::RenderQueue>& val) { m_DraggerRenderQueueGroup = val; }
	vector<ImageBoard *>& GetRenderImageboards()  { return m_renderImageboards; }

	vector<ImageBoard *> GetRenderUIs() const { return m_renderUIs; }
	void SetRenderUIs(vector<ImageBoard *> val) { m_renderUIs = val; }

	int GetCurrentRenderImageQueueIndex() const { return m_currentRenderImageQueueIndex; }
	void SetCurrentRenderImageQueueIndex(int val) { m_currentRenderImageQueueIndex = val; }
private:

	void InitRenderQueueGroup(map<int, M3D::RenderQueue>& renderQueueGroup);

	/**
	 * @brief 准备开始延时渲染
	 */
	void BeginPrepareDelayDraw();

	/**
	 * @brief Face中是否有显示数据
	 * @param face
	 * @return
	 */
	bool ISFaceHasDrawData(Face* face);
	/**
	 * @brief 将Face中的显示数据，加入以填充模式绘制三角形的渲染队列
	 * @param face
	 */
	void PrepareFaceMesh(Face* face);

	/**
	 * @brief 将Face中的边数据，加入边渲染队列
	 * @param face
	 */
	void FacePrepareEdgeLine(Face* face);
	/**
	 * @brief 将Face中的数据，加入最前端显示渲染队列
	 * @param face
	 */
	void PrepareRenderInTop(Face* face);
	/**
	 * @brief 初始化绘制效果
	 */
	void InitRenderEffects();
	/**
	 * @brief 释放类内的渲染效果资源
	 */
	void ReleaseRenderEffects();


public:
	HandlerGroup* m_handlerGroupCache; //!<交互节点绘制 TODO
	NoteGroup* m_noteGroupCache;//!<交互节点绘制 TODO
	ControlInfo m_Control;//!< TODO
	vector<Vector4> m_clipPlane;//!< 裁剪面列表
	vector<int> m_enableClip;//!< 开启哪些裁剪面
	bool m_bReverseClip;//!< 是否反转裁剪结果
	HardWareFrameBuffer m_frameBffer;//!< 帧缓存对象

	HardWareFrameBuffer m_teethFBO;//!< 牙齿FBO

	HardWareFrameBuffer m_delayDrawFBO;//!< 渐进显示FBO

	HardWareFrameBuffer m_depthMapFBO;//!< Maincamera出的相机深度图，用来处理边框线

	HardWareFrameBuffer m_edgeDetectionFBO;//!< 边缘检测帧缓存对象

	SectionData m_sectionData;//!< 纪录剖切对象

	static ShaderProgram* m_currentSP;//!<TODO
	static DirectionalLight m_light;//!<TODO 默认灯光

private:
	CameraNode* m_pCamera; //!< 摄像机
	RenderManager* m_renderMgr; //!<渲染管理
	Matrix4* m_glworldMatrix;//!< 世界坐标系缓存
	Matrix3x4* m_WorldMatrix; //!<世界坐标系缓存
	CullerMode m_CullerMode; //!< 微小模型剔除级别
	RenderEffect* m_renderEffect; //!< 渲染效果
	LightList m_lights; //!<渲染灯光
	BackgroundNode* m_backgroundColor; //!<背景
	FPSNode* m_fpsNode; //!<帧率显示节点
	AxisNode* m_axisNode; //!<坐标系显示节点
	SceneGroundNode* m_sceneGroundNode;//!<地面节点
	MeasureGroup* m_measureGroupCache; //!<交互节点绘制 TODO
	HandlerGroup* m_handlerGroupNode; //!<拖拽器相关节点

	Section* m_section; //!<缓存盖面操作信息
	int m_CurretSpecifyLod; //!<当前使用的LOD，如果直接使用m_iSpecifyLod，会导致在面合并的过程中使用不同的LOD面数据，导致崩溃
	int m_priTypeCache; //!<  如果状态改变了，则进行缓存
	map<int, RenderQueue>::iterator m_priTypePassGroup; //!< 渲染队列缓存，用于快速查找
	map<int, RenderQueue> m_RenderQueueGroup; //!< 正常渲染的队列
	map<int, RenderQueue> m_DraggerRenderQueueGroup; //!< 不会被裁剪的渲染队列
	map<int, RenderQueue>* m_WorkingRenderQueueGroup; //!< 当前正在起作用的渲染队列

	Renderable* m_mergeFaceCache; //!<缓存前一次面合并后的信息，供下一次面合并时比较
	Edge* m_mergeEdgeCache; //!<缓存前一次边合并后的信息，供下一次边合并比较实用
	int m_fps; //!<当前帧率
	int m_minfps; //!<最小帧率
	int m_simplifyFactor; //!<数据在显示时，简化比例
	SceneManager* m_scene;//!<
	HandlerRotateCenter* m_rotateCenter;//!< 选择中心
	ShaderManager* m_shaderManager;//!< shader管理对象

	RenderQueue m_delayDrawList;//!< 延迟渲染对象管理组

	RenderQueue m_delayOnceDrawList;//!<延迟渲染分段绘制组

	vector<ImageBoard*> m_renderImageboards; //!<渲染图片的列表

	vector<ImageBoard*> m_renderUIs; //!<渲染图片的列表

	int m_delayDrawOnceCount;//!< 一次绘制的个数

	bool m_immediatelyDrawBegin;

	bool m_delayDrawFlag;//!<是否需要延迟渲染
	bool m_allowDelayDraw;//!<是否允许延迟渲染，整体标识

	bool m_currentRenderCount; //!<当前已经加入渲染队列的总数

	BoundingBox m_selectBox; //!<纪录选中模型的包围盒

	LightList m_modellights; //!<渲染灯光

	bool m_isSceneBoxChanged; //!< 场景的包围盒改变了

	bool m_clearDelayDrawFlag;

	GroundNode* m_groundNode;

	int m_currentRenderImageQueueIndex;//当前场景使用绘制图片的组的索引
};
}

#endif   /*M3D_RENDERACTION_H_*/

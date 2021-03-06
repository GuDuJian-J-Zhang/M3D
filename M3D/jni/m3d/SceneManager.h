﻿/**@file
 *@brief	场景节点管理类
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *1,负责整个场景节点的管理，包括节点添加删除，节点移动旋转平移
 *2,负责控制摄像机的状态，包括漫游，指定点旋转，等外部操作
 */

#ifndef M3D_SCENEMANGER_H_
#define M3D_SCENEMANGER_H_

#include "m3d/M3D.h"
#include "m3d/utils/Trackball.h"
#include "m3d/graphics/MovedMatrixInfo.h"
#include "m3d/Handler/HandlerGroup.h"
#include "m3d/action/Action.h"
#include "m3d/graphics/Renderable.h"



namespace M3D
{
class Model;
class SceneNode;
class Model;
class GroupNode;
class RenderManager;
class ResourceManager;
class Shape;
class Action;
class HandlerGroup;
class Handler;
class Section;
class GlueObj;
class ModelShape;
class ModelView;
class DirectionalLight;
class SectionPlane;
class Octree;
class ExtendInfoManager;
class RenderAction;
class NoteGroup;
class AnnotationGroup;
class MeasureGroup;
class Ray;
class SectionNode;
typedef vector<DirectionalLight*> LightList;
class LightManager;
class CameraGroup;
class CameraManager;
class LightGroup;
class ScreenUILayerGroup;

#ifdef WIN32
class GUI;
#endif // WIN32


class M3D_API SceneManager
{
	friend class NodesManager;
	friend class RenderManager;
	friend class HandlerGroup;

public:
	SceneManager(void);
	virtual ~SceneManager(void);
	vector<Vector3> triglePnts;
	/**@brief 复位显示到最初加载状态
	 *
	 */
	void Reset();

	/**
	 *
	 * @param model 传入顶级Model对象指针
	 */
	void SetModel(Model* model);

	/**
	 *
	 * @param model 传入顶级Model对象指针
	 */
	void AddModel(Model* model);

	/**
	 *
	 * @return 得到顶级Model指针
	 */
	Model* GetModel();

	/**
	 * @brief 移除所有的显示模型
	 */
	void RemoveModel();
	/**
	 *
	 * @return 返回交互类组
	 * @see M3D::HandlerGroup
	 */
	HandlerGroup*GetHandlerGroup();

	/**
	 *
	 * @return 返回Note组
	 * @see M3D::HandlerGroup
	 */
	NoteGroup *GetNoteGroup();

	/**
	*
	* @return 返回Annotation组
	* @see M3D::HandlerGroup
	*/
	AnnotationGroup *GetAnnotationGroup();

	/**
	 *
	 * @return 返回测量组
	 * @see M3D::MeasureGroup
	 */
	MeasureGroup *GetMeasureGroup();

	LightGroup* GetLightGroup();

	CameraGroup* GetCameraGroup();

	/**
	 * 取得场景树的根节点
	 * @return 场景树根节点
	 */
	SceneNode * GetSceneRoot(void);

	/**
	 *
	 * @param 放入渲染管理renderMgr对象的指针
	 */
	void SetRenderManager(RenderManager * renderMgr);

	/**
	 *
	 * @return 返回渲染管理对象的指针
	 */
	RenderManager * GetRenderManager();

	/**
	 * @brief 是否显示剖面
	 * @param flag true显示，false不显示
	 */
	void setSectionEnable(bool flag);

	/**
	 * 设置剖面，后期通过重写View废弃掉
	 * @param planes
	 */
	//void SetSections(const vector<SectionPlane*> planes);
 
	SectionNode* GetSectionNode();

	/**
	 * @brief 外部添加进入的action
	 * @param action 需要异步执行的action
	 */
	void ExecuteAction(Action* action);

	/**
	 * @brief 设置旋转中心
	 * @param x 屏幕坐标x
	 * @param y 屏幕坐标y
	 *@param centerType = 0,正常模式，=1 围绕摄像机自身
	 * @return true 设置成功
	 */
	bool SetRotationCenter(float x, float y,int centerType = 0);

	/**
	 * @brief 复位旋转中心
	 * @return true表示成功
	 */
	bool RestoreRotationCenter();

	/**
	 * @brief 拾取操作，返回拾取到shape对应的ID，返回-1表示未拾取任何shape
	 * @param winx 屏幕坐标x
	 * @param winY 屏幕坐标y
	 * @param type 需要拾取的类型
	 * @return 拾取到shape的ID
	 */
	IShape* GetPickShape(float winx, float winY, int shapeType, int geoType);
	IShape* GetPickShape(M3D::Ray& ray, int shapeType, int geoType);
	IShape* GetPickShape(Vector2& screentPnt, int shapeType, int geoType);
	IShape* GetFarPickShape(M3D::Ray& ray, int shapeType, int geoType);

	//处理VR菜单事件
	bool ProcessVREvent();

	vector<IShape*> GetFramePickShape(const Vector2& leftTop, const Vector2& rightBottom,
		int shapeType,
		int geoType, int framePickType);

	/**
	 * @brief 设置能够拾取到的类型
	 * @param type 允许拾取的类型
	 */
	void SetSelectType(int type);

	///Animation Interface
	/**
	 * @brief 设置名称为plcPath节点，对应的配置矩阵为matrix
	 * @param plcPath 节点名称
	 * @param matrix  节点配置矩阵
	 */
	void SetMatrixByPlcPath(char* plcPath, float matrix[4][4]);

	/**
	 * @brief 获取名称为plcPath节点，对应的配置矩阵为matrix
	 * @param plcPath 节点名称
	 * @param matrix  节点配置矩阵
	 */
	void GetMatrixByPlcPath(char* plcPath, float matrix[4][4]);

	///Handler operator
	/**
	 * @brief 添加Handler
	 * @param x 屏幕坐标X
	 * @param y 屏幕坐标Y
	 * @param type Handler的类型
	 * @return 成功返回生成Handler的ID，失败返回-1
	 */
	int AddHandle(float x, float y, int type);
	/**
	 * @brief 获得拾取到的点坐标
	 * @param x 屏幕坐标X
	 * @param y 屏幕坐标Y
	 * @param inModel
	 * @return 空间中点三维坐标
	 */
	Vector3 GetPickPoint(float x, float y, bool inModel = true, bool inExcludeEdge = false);
	Vector3 GetPickNormal();
	Vector3 GetUILayerPickPoint(float x, float y);

	/**
	 *
	 * @param x
	 * @param y
	 * @param pnt
	 * @param inModel
	 * @return
	 */
	bool GetPickPoint(Vector2& screenPnt, Vector3& pnt, bool inModel, bool inExcludeEdge = false);
	bool GetPickPoint(Ray & ray, Vector3 & pnt, bool inModel);
	/**
	 * @brief 在空间中vec位置创建类型为Type的Handler
	 * @param vec 创建handler的位置
	 * @param Type 创建handler的类型
	 * @return 成功返回生成Handler的ID，失败返回-1
	 */
	Handler* CreateHandler(const Vector3& vec, int Type);
	/**
	 * @brief 在空间中x，y，z位置创建handler
	 * @param x 空间X
	 * @param y 空间Y
	 * @param z 空间Z
	 * @param type 创建Handler类型
	 * @return 成功返回生成Handler的ID，失败返回-1
	 */
	int AddHandle(float x, float y, float z, int type);
	/**
	 * @brief 移除ID为 id的handler
	 * @param id 移除Handler的ID
	 *
	 */
	void RemoveHandle(int id);

	/**
	 * @brief 添加shape
	 * @param x 屏幕坐标X
	 * @param y 屏幕坐标Y
	 * @param type 添加shape类型
	 * @return 添加进入shape的ID
	 */
	int AddShape(float x, float y, int type);
	/**
	 * @brief 添加shape
	 * @param vec 空间坐标
	 * @param Type 添加shape类型
	 * @return 添加成功shape的ID
	 */
	IShape* CreateShape(const Vector3& vec, int Type);
	/**
	 * @brief 添加shape
	 * @param x 空间坐标X
	 * @param y 空间坐标Y
	 * @param z 空间坐标Z
	 * @param type 添加Shape的类型
	 * @return 添加成功shape的ID
	 */
	int AddShape(float x, float y, float z, int type);
	/**
	 * @brief 移除场景中ID为 id的shape
	 * @param id 移除shape的ID
	 * @return tru表示移除成功，false表示移除失败
	 */
	bool RemoveShape(IDTYPE id);

	/**
	 * @brief 获取ID为id，类型为type的shape在空间中的当前位置
	 * @param id 待获取shape的ID
	 * @param type 待获取shape的类型
	 * @param pos  传出shape的当前位置
	 */
	void GetShapePos(IDTYPE id, int type, Vector3& pos);

	/**
	 * 获取shape的颜色
	 * @param id
	 * @param type
	 * @return
	 */
	Color GetShapeColor(IDTYPE id, int type);

	/**
	 * @brief 显示旋转中心
	 * @param flag
	 */
	void ShowRotationCenter(bool flag);

	/**
	 * 得到摄像机
	 * @return
	 */
	CameraNode* GetCamera();

	/**
	 * 设置摄像机
	 * @param camera
	 */
	void SetCamera(CameraNode* camera);

	/**
	 * 锁定场景,使线程独占场景资源
	 * 当在外部修改场景内容时调用，能够对渲染线程进行锁定
	 * 防止写冲突
	 */
	void Lock();

	/**
	 * 解锁场景,释放线程独占资源
	 * 是否Lock锁定的资源
	 */
	void UnLock();

	/**
	 * 得到路径为path的shapenode节点下挂载的shape对象
	 * @param path
	 * @return
	 */
	IShape* GetShape(const string& path);

	/**
	 * 在NodesManager中查找ID为id的shape
	 * @param shapeID 要查找shape的id
	 * @return 返回shape的地址
	 */
	IShape* GetShape(IDTYPE shapeID);
	/**
	 * 通过名称查找节点
	 * @param name
	 * @return
	 */
	Model* GetNode(const string& name);

	void AddModelCachePath(Model* model);
	/**
	 * 获取整个场景的包围盒
	 */
	BoundingBox& GetSceneBox();

	/**
	 * 设置粘合层对象
	 * @param glueObj
	 */
	void SetGluObj(GlueObj* glueObj);

	/**
	 * 获取粘合层对象
	 * @return
	 */
	GlueObj* GetGlueObj();

	/**
	 * 将shape对象从shapemap中移除
	 * @param shape
	 */
	void RemoveShapeIDFromMap(IShape* shape);
	/**
	 * 更新shape对象的id到map缓存中，方便根据shape的id进行快速查找
	 * @param shape
	 */
	void AddShapeIDToMap(IShape* shape);

	/**
	 * 重启索引map列表，在添加model对象后
	 * @param model
	 */
	void ReIndexIDMapAfterAddModel(Model* model);
 
	/**
	 * 得到资源管理器
	 * @return
	 */
	ResourceManager* GetResourceManager();

	/**
	* @brief 在场景中加载新Model后使用一次
	*/
	void OptimizeScene(bool isAsynMode);

	///更新整个场景，在每一次绘制前
	void UpdateScene();

	bool RayOctreeAction(RayPickAction* rayPickAction);

	bool FrameOctreeAction(RayPickAction* rayPickAction);

	bool FrustumOctreeAction(Action* action);

	///得到场景默认的缩放比例
	float GetDefaultZoom();
 
	///得到默认焦距大小
	float GetDefaultFocusLength();

	//----------------------------------------------------------------
	/**
	* 设置旋转中心相关操作
	*/
	bool SetRotationCenter(float x, float y, float z);
	bool SetRotationCenter(const Vector3& vec);

	/************************************************************************/
	/* 通过模型更新场景，包括扩展场景包围盒：如果传来的模型不在场景中，则扩展包围盒                                                                     */
	/************************************************************************/
	bool UpdateSceneByModel(Model* model);
	void UpdateSceneByModel(vector<Model*>models);

	/************************************************************************/
	/* 当所有的模型读取完成之后，强制更新整个场景的包围盒，来防止文件包围盒没有计算的情况                                                                     */
	/************************************************************************/
	void RequestUpdateWhenSceneBoxChanged();
 
	void OptimizeCameraView(bool fitShowView = false);
 
	void AsynUpdateModelCacheInfo(Model* mdoel,bool add, bool addSub = true);

	bool AsynAddModelToParent(Model* parentModel, Model* mdoel);
	bool AsynRemoveModelFromeScene(Model* parentModel, Model* model);

	ExtendInfoManager* GetExtendInfoManager();

	BoundingBox GetOcTreeWorldBoundingBox();

	CameraManager * GetCameraManager();

	LightManager* GetLightManager();

	ScreenUILayerGroup* GetScreenUILayerGroup();

	SectionNode* GetSectionNode(int ID); 

	void AddSectionNode(SectionNode* node);

#ifdef WIN32
	GUI* GetGUI();
#endif // WIN32

	CameraNode* GetHudCamera() const;
	void SetHudCamera(CameraNode* val);

	BoundingBox& GetHudLayerBox(){ return m_HudLayerBox; }
	void SetHudLayerBox(BoundingBox& val) { m_HudLayerBox = val; }
	//选中的面片焦点及对应的发现
	Vector3 rayIntersectNormal;
	Vector3 rayIntersectPos;
private:
	void OnRequestUpdateWhenSceneBoxChanged();


	void ReIndexIDMapAfterAddSingleModel(Model* model);

	/**
	* 重启索引map列表，在删除model对象是
	* @param model
	*/
	void ReIndexIDMapAfterDeleteModel(Model* model);

	void ReIndexIDMapAfterDeleteSingleModel(Model* model);
	/**
	* 删除名称查找节点
	* @param name
	*/
	void RemoveModelCachePath(Model* model);
	/**
	 * 计算包围盒回调函数
	 * @param data
	 * @param node
	 */
	static void ComputeBox(void* data, Model* node);

	/**
	 * 计算轻量化浏览的包围盒回调函数
	 * @param data
	 * @param node
	 */
	static void ComputeLBox(void* data, Model* node);

	static void AddNodeToOCTree(void* data, Model* node);

	void AddNodeToOCTree(Model* node);

	/**
	 * 更新硬件缓存资源，CallbackAction回调函数
	 * @param data
	 * @param node
	 */
	static void UpdataHardWareBuffer(void* data, Model* node);

	/**
	 * 复位模型回调函数
	 * @param data
	 * @param node
	 */
	static void ResetModelNodesCallback(void* data, Model* node);

	/**
	 * 更新scenenode节点路径map回调函数
	 * @param data
	 * @param node
	 */
	static void SetNodeToMap(void* data, Model* node);

	/**
	 * 更新硬件缓存
	 */
	void UpdateHardwareBuffer();

	void BeginDiskCache();

	void EndDiskCache();

	void Init();

	bool RestoreRotationCenter(SceneNode* node);

	/**
	 * 清除模型和所有的handle节点
	 */
	void ClearModelAndHandles();

	/**
	 * 更新场景所有节点路径缓存表
	 */
	void UpdataNodePathMap();

	BoundingBox GetFitViewSceneBox();

	list<SectionNode*> m_sceneNodeList;

protected:
	SceneNode *m_pSceneRoot; //!<场景根节点
	CameraNode* m_camera; //!<摄像机
	BoundingBox m_sceneBox; //!<整个场景的包围盒

	RenderManager * m_RenderMgr; //!<渲染器
	int m_iSelectType; //!<拾取类型
	Model * m_TopModel; //!<顶级模型

	mutable Mutex m_mutex; //!<线程资源同步锁，用于同步绘制线程和UI线程之间的资源竞争

	HandlerGroup* m_handlerGroup; //!<交互节点 TODO

	NoteGroup* m_noteGroup; //!<存储批注的节点组
	SectionNode* m_sectionNode;

	AnnotationGroup* m_annotationGroup;//!<存储标注的节点组

	MeasureGroup* m_measureGroup; //!< 存储测量临时对象

	LightGroup* m_lightGroup;//!挂载场景的灯光节点
	CameraGroup* m_cameraGroup;

	ResourceManager* m_resourceMgr; //!<资源管理器

	map<string, Model*> m_NodesMap; //!<string 存储node对应的path，NodesMap存储所有的路径对应的节点，
	map<IDTYPE, IShape*> m_ShapesIDMap; //!<缓存shape用于快速查找

	GlueObj* m_glueObj; //!<java或者oc粘合层对象

	bool m_isModelDirty; //!<场景内模型发生了变化

	vector<ModelShape*> m_frustumQueryResulets; //八叉树场景划分查询结果
	Octree* m_ocTree;

	ExtendInfoManager* m_extendinfoMgr; //信息扩展管理器

	bool m_SceneBoxChanged;

	CameraNode* m_hudCamera; //HubCamera

	BoundingBox m_HudLayerBox;

#ifdef WIN32

	GUI* m_gui;

#endif // WIN32

	/************************************************************************/
	/* 郑煤机项目添加                                                       */
	/************************************************************************/
	//场景单位换算 add by zhouyunpeng 
	float m_unitScale;
	//---------------------------------
	LightManager* m_lightManager;

	CameraManager* m_cameraManager;
public:
	void SetUnitScale(float scale);
};
}

#endif/*M3D_SCENEMANGER_H_*/

/**@file
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
#include "m3d/action/RenderAction.h"
#include "m3d/graphics/Renderable.h"

namespace M3D
{

class Model;
class SceneNode;
class LSceneNode;
class GroupNode;
class RenderManager;
class ResourceManager;
class Shape;
class Action;
class HandlerGroup;
class Handler;
class Section;
class GlueObj;

class ModelView;
class Light;
class SectionPlane;

typedef vector<Light*> LightList;

class SceneManager
{
	friend class NodesManager;
	friend class RenderManager;
	friend class HandlerGroup;

public:
	SceneManager(void);
	virtual ~SceneManager(void);

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
	 * @return 返回测量组
	 * @see M3D::MeasureGroup
	 */
	MeasureGroup *GetMeasureGroup();

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

	void FoucusView(BoundingBox& foucusBox, bool useAni);

	/**
	 * @brief 是否显示剖面
	 * @param flag true显示，false不显示
	 */
	void setSectionEnable(bool flag);

	/**
	 * 设置剖面，后期通过重写View废弃掉
	 * @param planes
	 */
	void SetSections(const vector<SectionPlane*> planes);

	/**
	 * @brief 获取存储剖面信息的节点  @see M3D::SectionNode
	 * @return SectionNode节点指针
	 */
	Section * GetSection();

	/**
	 * @brief 外部添加进入的action
	 * @param action 需要异步执行的action
	 */
	void ExecuteAction(Action* action);

	/**
	 * @brief 设置旋转中心
	 * @param x 屏幕坐标x
	 * @param y 屏幕坐标y
	 * @return true 设置成功
	 */
	bool SetRotationCenter(float x, float y);

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

	IShape* GetPickShape(Vector2& screentPnt, int shapeType, int geoType);

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
	 * @return 空间中点三维坐标
	 */
	Vector3 GetPickPoint(float x, float y);

	/**
	 *
	 * @param x
	 * @param y
	 * @param pnt
	 * @param inModel
	 * @return
	 */
	bool GetPickPoint(Vector2& screenPnt, Vector3& pnt, bool inModel);

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
	Shape* CreateShape(const Vector3& vec, int Type);
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
	bool RemoveShape(int id);

	/**
	 * @brief 获取ID为id，类型为type的shape在空间中的当前位置
	 * @param id 待获取shape的ID
	 * @param type 待获取shape的类型
	 * @param pos  传出shape的当前位置
	 */
	void GetShapePos(int id, int type, Vector3& pos);

	/**
	 * 获取shape的颜色
	 * @param id
	 * @param type
	 * @return
	 */
	Color GetShapeColor(int id, int type);

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
	 * 添加灯光
	 * @param light
	 */
	void AddLight(Light* light);

	/**
	 * 移除index号灯光
	 * @param index
	 */
	void RemoveLight(int index);

	/**
	 * 得到所有的灯光
	 * @return
	 */
	LightList* GetLights();

	/**
	 * 得到index号灯光
	 * @param index
	 * @return
	 */
	Light* GetLight(int index);

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
	Shape* GetShape(const string& path);

	/**
	 * 在NodesManager中查找ID为id的shape
	 * @param shapeID 要查找shape的id
	 * @return 返回shape的地址
	 */
	Shape* GetShape(int shapeID);
	/**
	 * 通过名称查找节点
	 * @param name
	 * @return
	 */
	SceneNode* GetNode(const string& name);

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
	void RemoveShapeIDFromMap(Shape* shape);
	/**
	 * 更新shape对象的id到map缓存中，方便根据shape的id进行快速查找
	 * @param shape
	 */
	void AddShapeIDToMap(Shape* shape);

	/**
	 * 创建ModelNOde节点，根据Model对象创建
	 * @param model
	 * @param currPath
	 * @param n
	 * @return
	 */
	GroupNode* CreateModelNodes(Model* model, string currPath, int n);

	/**
	 * 重启索引map列表，在添加model对象后
	 * @param model
	 */
	void ReIndexIDMapAfterAddModel(Model* model);

	/**
	 * 重启索引map列表，在删除model对象是
	 * @param model
	 */
	void ReIndexIDMapAfterDeleteModel(Model* model);

	/**
	 * 得到资源管理器
	 * @return
	 */
	ResourceManager* GetResourceManager();

	/**
	* @brief 在场景中加载新Model后使用一次
	*/
	void OptimizeScene();

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
    void UpdateSceneByModel(Model* model);
    void UpdateSceneByModel(vector<Model*>models);
private:
	/**
	 * 计算包围盒回调函数
	 * @param data
	 * @param node
	 */
	static void ComputeBox(void* data, SceneNode* node);

	/**
	 * 计算轻量化浏览的包围盒回调函数
	 * @param data
	 * @param node
	 */
	static void ComputeLBox(void* data, LSceneNode* node);

	/**
	 * 更新硬件缓存资源，CallbackAction回调函数
	 * @param data
	 * @param node
	 */
	static void UpdataHardWareBuffer(void* data, LSceneNode* node);

	/**
	 * 复位模型回调函数
	 * @param data
	 * @param node
	 */
	static void ResetModelNodesCallback(void* data, SceneNode* node);

	/**
	 * 更新scenenode节点路径map回调函数
	 * @param data
	 * @param node
	 */
	static void SetNodeToMap(void* data, SceneNode* node);

	/**
	 * 更新硬件缓存
	 */
	void UpdateHardwareBuffer();

	void Init();

	/**
	 * 根据名称和类型，获取场景节点
	 * @param type
	 * @param name
	 * @return
	 */
	SceneNode* GetNodes(int type, const string& name);

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

protected:
	SceneNode *m_pSceneRoot; //!<场景根节点
	CameraNode* m_camera; //!<摄像机
	BoundingBox m_sceneBox; //!<整个场景的包围盒

	RenderManager * m_RenderMgr; //!<渲染器
	int m_iSelectType; //!<拾取类型
	Model * m_TopModel; //!<顶级模型
	LightList lights; //!<所有灯光
	mutable Mutex m_mutex; //!<线程资源同步锁，用于同步绘制线程和UI线程之间的资源竞争

	HandlerGroup* m_handlerGroup; //!<交互节点 TODO

	NoteGroup* m_noteGroup; //!<存储批注的节点组

	MeasureGroup* m_measureGroup; //!< 存储测量临时对象

	ResourceManager* m_resourceMgr; //!<资源管理器

	map<string, SceneNode*> m_NodesMap; //!<string 存储node对应的path，NodesMap存储所有的路径对应的节点，
	map<int, Shape*> m_ShapesIDMap; //!<缓存shape用于快速查找

	GlueObj* m_glueObj; //!<java或者oc粘合层对象

	bool m_isModelDirty; //!<场景内模型发生了变化
	Section* m_section;
};
}

#endif/*M3D_SCENEMANGER_H_*/

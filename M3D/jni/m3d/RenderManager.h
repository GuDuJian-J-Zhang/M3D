/**@file
 *@brief	渲染器，负责渲染队列的最终渲染
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_RENDER_H_
#define M3D_RENDER_H_

#include "m3d/M3D.h"
#include "m3d/base/Thread.h"
#include "m3d/base/Mutex.h"
#include "m3d/base/CTimer.h"

#include "m3d/utils/CullerHelper.h"
namespace M3D
{
class Model;
class GroupNode;
class ModelVBO;
class SceneNode;
class VBOData;
class SceneManager;
class RenderContext;
class RenderAction;
class RenderEffect;
class GLDrawerFunManager;
class RenderManager;
class LShapeNode;
class LSceneNode;
class Action;
class RayPickAction;

///场景划分，简单实现按照模型从大到小排序存储，后期更多优化后的存储方法
class M3D_API SceneDivide
{
public:
	SceneDivide(SceneManager* sceneMgr);

	void AddNode(LShapeNode* node);

	void Clear();

	void Optimize();

	virtual void FindVisiableObject(RenderAction* renderAction);

	virtual void Traverse(Action* action);

	virtual void RayPick(RayPickAction * action);

	bool Dirty();

	void MarkDirty();

	static void CacheNode(void* data, LSceneNode* node);
private:
	static bool CompareByPlcWorldBoxLength(LShapeNode* node1,LShapeNode* node2);

private:
	vector<LShapeNode*> m_shapeNodesCache;//用来缓存所有可以现实的shape节点
	bool m_dirty; //场景是否准备划分完毕
	SceneManager* m_sceneMgr;
};

/**
 * 多线程准备RenderAction测试
 * TODO
 */
class RenderActionManager: public Thread
{
public:
	static const int ITEMSCOUNT;
public:

	/**
	 * Construct. Does not start the thread yet.
	 * @param renderMgr
	 */
	RenderActionManager(RenderManager* renderMgr);

	/**
	 *  Destruct. If running, stop and wait for thread to finish.
	 */
	virtual ~RenderActionManager();

	/**
	 *  The function to run in the thread.
	 */
	virtual void ThreadFunction();

	/**
	 * 获取renderaction
	 * @return
	 */
	RenderAction* GetAction();

	RenderAction* GetWorkRenderAction();

	void KeepFlag(bool finish);

private:
	mutable Mutex m_mutex;

	RenderAction* m_RenderActions[3];

	queue<RenderAction*> m_renderActionQueue; //!<缓存Action数据，同步与外界的交互

	int m_workIndex; //!<返回正在使用的Action在队列中的位置

	int m_prepareIndex; //!<正在准备的Action在队列中的位置

	bool m_finish;//!<是否完成

	RenderManager* m_renderMgr;//!<渲染管理器

	SceneManager* m_pSceneManager;
};

typedef map<int, RenderEffect*> RenderEffectsMap;

/**@class
 * @brief 渲染管理类，负责渲染参数的设置，渲染动作的管理。
 *
 */
class M3D_API RenderManager
{
	friend class RenderActionManager;
public:
	//Test
	static bool isShowTriLine;

public:
	RenderManager(SceneManager* scene);

	virtual ~RenderManager();


	/**
	 * @brief 初始化渲染上下文
	 */
	void InitialRender();

	/**
	 * @brief 激活一次渲染事件
	 */
	void OnDraw();

	/**
	 * @brief 是否需要重绘
	 */
	bool IsNeedRedraw();
	/**
	 * @brief 响应窗口大小变化
	 * @param width 窗口宽度
	 * @param height 窗口高度
	 */
	void WindowSizeChanged(int width, int height);

	/**
	 * @brief 获得显示窗口的宽度
	 * @return 窗口宽度
	 */
	int GetWindowWidth();

	/**
	 * @brief 获得显示窗口高度
	 * @return 窗口高度
	 */
	int GetWindowHeight();

	/**
	 * @brief 是否允许重绘
	 * @param true允许，false不允许
	 */
	void AllowRedraw(bool);

	/**
	 * @brief 请求重绘
	 */
	void RequestRedraw();

	/**
	 * 执行渲染
	 */
	void Render();

	/**
	 * 设置渲染效果
	 * @see RenderEffect
	 * @param renderEffect
	 */
	void SetRenderEffect(RenderEffect* renderEffect);

	/**
	 * 得到渲染效果
	 * @see RenderEffect
	 * @return
	 */
	RenderEffect* GetRenderEffect();

	/**
	 * @brief 设置是否执行小件剔除
	 */
	void ExcludeSmallModel(bool cullSmallModel);

	/**
	 * @brief 得到当前小件剔除状态
	 */
	bool IsExcludeSmallModel();

	/**
	 * @brief 得到是否显示旋转中心状态
	 */
	bool IsShowRotationCenter();

	/**
	 * @brief 设置是否显示旋转中心
	 */
	void ShowRotationCenter(bool showCenter);

	/**
	 * 设置渲染时，所使用的LOD等级
	 * @param lodLevel
	 */
	void SetLodLevel(int lodLevel);

	/**
	 * @brief得到当前渲染时，使用的LOD级别
	 */
	int GetLodLevel();

	/**
	 * 使用低质量渲染
	 * @param useLowQuality true使用低质量渲染  false 正常渲染
	 */
	void SetUseLowQuality(bool useLowQuality);

	/**
	 * 得到剔除工具类
	 * @return 剔除工具类
	 */
	CullerHelper& GetCullerHelper();

	/**
	 * @brief TODO 多线程准备渲染队列
	 */
	void CloseRenderActionThread();

	/**
	 * @brief 得到当前显示帧数
	 */
	int GetFPS();

	/**
	 * 设置OpenGL版本
	 * @param glVersion
	 */
	void SetGLVersion(int glVersion);

	/**
	 * 获取OpenGL版本
	 * @return
	 */
	int GetGLVersion();

	/**
	 * 限制最大帧数
	 * @return
	 */
	int GetMaxLimitFPS();

	/**
	 * 设置最大帧数
	 * @param num
	 */
	void SetMaxLimitFPS(int num);

	/**
	 * @brief 清除OpenGL资源
	 */
	void ClearGLResource();

	/**
	 * 获取绘制质量状态
	 * @return
	 */
	bool UseLowQuality();

	/**
	 * 得到即使帧率
	 * @return
	 */
	float GetEventFPS();

	void SceneChanged();

	void VRPrepareDrawList();

	void VRDraw();

	SceneManager* GetSceneManager();

private:
	/**
	 * 复位
	 */
	void Reset();

	/**
	 * @brief 计算FPS
	 */
	void CalculateFPS();

	/**
	 * @brief 初始化渲染效果
	 */
	void InitRenderEffect();

	/**
	 * 限制FPS
	 * @return
	 */
	bool LimitDrawFPS();


private:
	int m_maxFPS; //!<设置最大绘制帧率

	SceneManager* m_sceneMgr; //!<场景引用

	int m_avergFPS; //!<帧数统计值
	float m_evenFPS;//!<即使帧率
	long m_lastEventTime;

	bool m_allRedrawStart; //!< 整体绘制开始

	RenderEffect* m_renderEffect; //!<渲染效果

	RenderEffectsMap m_renderEffectsMap; //!< TODO  渲染效果表，用于管理多个渲染效果

	RenderActionManager* m_renderActionMgr; //!<

	RenderAction* m_renderAction; //!<渲染动作，用于遍历整个场景，准备渲染队列

	RenderContext* m_GLContext; //!<GLES上下文

	bool m_showRotateCenter; //!<显示旋转中心

	bool m_excludeSmallModel; //!<使用小模型剔除

	int m_lodLevel; //!<渲染时使用的LOD等级

	CullerHelper m_cullerHelper; //!<小模型剔除工具类

	bool m_useLowQuality; //!<使用低质量渲染

	mutable Mutex m_mutex; //!<对象锁，多线程资源同步使用

	bool m_AllowRedraw; //!<

	int m_iScreenWidth; //!<屏幕宽像素数
	int m_iScreenHeigh; //!<屏幕高像素数

	int FRAME_BUFFER_COUNT; //!<

	int m_glVersion; //!<es 版本

	int m_BufferCounter; //!<每次请求渲染时，需要刷新的次数
	//test
	bool m_allowDraw; //!<

	bool m_useDelayDraw;//!< 使用渐进显示方式绘制

	SceneDivide* m_sceneDivide;//!< 场景划分类

	static long m_LastLogTime; //!<
	static long m_FrameCount; //!<

	timeval m_startTime; //!<

	timeval m_endTime; //!<

	timeval m_fpsstartTime; //!<
	int dt;
};
}
#endif/*M3D_RENDER_H_*/

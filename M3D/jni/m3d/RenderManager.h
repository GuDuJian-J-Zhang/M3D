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
class ModelShape;
class Model;
class Action;
class RayPickAction;
class Octree;
class EffectManager;
 
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

	M3D::RenderAction* GetRenderAction() const;
	void SetRenderAction(M3D::RenderAction* val);

	M3D::CullerHelper& GetCullerHelper();

public:
	RenderManager(SceneManager* scene);

	virtual ~RenderManager();



	void ClearDelayDrawList();

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

	/************************************************************************/
	/* 当所有的模型读取完成之后，强制更新整个场景的包围盒，来防止文件包围盒没有计算的情况                                                                     */
	/************************************************************************/
	void RequestUpdateWhenSceneBoxChanged();

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

	void FindVisiableObject(RenderAction* renderAction);

	void RebindUIRenderFBO();

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

	EffectManager* GetEffectManager();

	string GetGlobalEffect();
	void SetGlobalEffect(string effectName);

	static void BeginCatchOpenGLError();
	static string EndCatchOpenGLError();

	bool IsForceNormalEffect();
	void SetISForceNormalEffect(bool effectName);

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

	/**
	* 当窗口改变时，更改hub视口大小
	* @return
	*/
	void ResizeHubViewport();

private:
	int m_maxFPS; //!<设置最大绘制帧率

	SceneManager* m_sceneMgr; //!<场景引用

	int m_avergFPS; //!<帧数统计值
	float m_evenFPS;//!<即使帧率
	long m_lastEventTime;

	bool m_allRedrawStart; //!< 整体绘制开始

	RenderEffect* m_renderEffect; //!<渲染效果

	RenderEffectsMap m_renderEffectsMap; //!< TODO  渲染效果表，用于管理多个渲染效果

	//RenderActionManager* m_renderActionMgr; //!<

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

	static long m_LastLogTime; //!<
	static long m_FrameCount; //!<

	timeval m_startTime; //!<

	timeval m_endTime; //!<

	timeval m_fpsstartTime; //!<
	int dt;

	EffectManager * m_effectManager;
	string m_globalEffect;
	bool m_isForceNormalEffect;
};
}
#endif/*M3D_RENDER_H_*/

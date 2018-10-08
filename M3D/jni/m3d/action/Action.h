/**
 *@file
 *@brief    动作基类，在执行回执的过程中能够实现一些额外的操作
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */

#ifndef  M3D_ACTION_H_
#define  M3D_ACTION_H_

#include "m3d/M3D.h"
//#include "m3d/base/ControlInfo.h"
#include "m3d/renderer/RenderContext.h"

namespace M3D
{
class SceneNode;
class LSceneNode;
class Path;
class State;
class RenderContext;


/**
 * @brief  动作执行回掉函数
 * @param data 执行数据
 * @param node 当前结点
 */
typedef void (*ActionFun)(void* data , SceneNode* node);

/**
 * @brief  简化模式下，动作执行回掉函数
 * @param data 执行数据
 * @param node 当前结点
 */
typedef void (*LActionFun)(void* data, LSceneNode* node);

/**
 * @brief 动作基类，在执行回执的过程中能够实现一些额外的操作
 * @class
 *
 */
class M3D_API Action
{
public:
	const static int KEEPING = 0; //!< 执行所有的节点
	const static int ONCE = 1; //!< 执行到第一个满足要求的节点

public:
	/**
	 *@brief 构造函数
	 */
	Action();

	/**
	 * @brief 析构函数
	 */
	virtual ~Action();

	/**
	 * @brief 获取当前动作类型
	 * @return 动作类型
	 */
	virtual int GetType();

	/**
	 * @brief 将动作作用到当前结点上
	 * @param node 当前需要应用动作的节点
	 */
	virtual void Apply(SceneNode * node);

	/**
	 * @brief 将动作作用到当前结点上
	 * @param node 当前需要应用动作的节点
	 */
	virtual void Apply(LSceneNode * node);

	/**
	 * @brief 仅执行一次动作
	 */
	virtual void ApplyOnce();

	/**
	 * @brief 动作是否需要结束
	 * @return true结束 false 正在执行
	 */
	bool IsFinish();

	/**
	 * @brief 设置动作结束标识
	 * @param state true结束 false 继续执行
	 */
	void SetFinish(bool state);

	/**
	 * @brief 获取渲染上下文
	 * @return 渲染上下文
	 */
	RenderContext* GetGLContext();

	/**
	 * @brief 设置渲染上下文
	 * @param glContext
	 */
	void SetGLContext(RenderContext* glContext);

	/**
	 * @brief 设置动作执行类型
	 * @param type   KEEPING = 0  执行所有的节点  ONCE = 1 执行到第一个满足要求的节点
	 */
	void SetApplyType(int type);

	/**
	 * @brief 获取当前动作执行类型
	 * @return KEEPING = 0  执行所有的节点  ONCE = 1 执行到第一个满足要求的节点
	 */
	int GetApplyType();

protected:

	/**
	 * @brief 执行响应函数
	 * @param node 作用节点
	 */
	virtual void OnExecute(SceneNode* node);

	/**
	 * @brief 简化浏览模式，执行响应函数
	 * @param node 作用节点
	 */
	virtual void OnExecute(LSceneNode* node);

protected:
	RenderContext* m_GLContext; //!< 渲染上下文
	int m_iActionType; //!<动作类型
	bool m_bKeepingFinished; //!< 执行结束标识
	bool m_bOnceFinished; //!< 执行结束标识
};

}

#endif   /*M3D_ACTION_H_*/

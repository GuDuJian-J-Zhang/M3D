/**
 *@file
 *@brief	在执行绘制的流程中，能够执行指定的回调函数
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */

#ifndef	M3D_CALLBACKACTION_H_
#define	M3D_CALLBACKACTION_H_

#include "m3d/action/Action.h"

namespace M3D
{
class SceneNode;
class Path;
class State;

/**
 * @class
 * @brief 执行指定的回调函数
 */
class M3D_API CallBackAction: public Action
{
public:
	const static int CALLBACK_ACTION = 2;//!< 类型值

public:
	/**
	 * @brief 构造函数
	 */
	CallBackAction();
	/**
	 * @brief 析构函数
	 */
	virtual ~CallBackAction();

	/**
	 * @brief Action类型
	 * @return Action类型
	 */
	virtual int GetType();

	/**
	 * @brief 设置简化浏览模式回调执行函数
	 * @param fun
	 */
	void SetActionFun(ActionFun fun);
	/**
	 * @brief 设置回掉执行数据
	 * @param data 回调数据
	 */
	void SetActionData(void* data);

private:
	virtual void OnExecute(Model* node);

private:
	void* m_pData; //!< 回调数据
	ActionFun m_pfun;//!<简化浏览模式回调函数
};

}

#endif   /*M3D_CALLBACKACTION_H_*/

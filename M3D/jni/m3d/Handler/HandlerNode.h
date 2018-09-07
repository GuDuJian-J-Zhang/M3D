/**@file
 *@brief    TODO  handler类，实现类似把手的相关功能
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef HANDLER_NODE_H
#define  HANDLER_NODE_H

#include "m3d/M3D.h"
#include "m3d/scene/GroupNode.h"
namespace M3D
{
class Handler;


/**
 *
 */
class M3D_API HandlerNode: public GroupNode
{
public:
	HandlerNode();
	virtual ~HandlerNode();

	virtual int GetType(void);
	/**
	 *
	 * @return
	 */
	bool InTop();
	/**
	 *
	 * @return
	 */
	bool IsRotate();
	/**
	 *
	 * @return
	 */
	bool IsTranslate();
	/**
	 *
	 * @return
	 */
	bool IsScale();
	/**
	 *
	 * @param state
	 */
	void AllowRotate(bool state);
	/**
	 *
	 * @param state
	 */
	void AllowTranslate(bool state);
	/**
	 *
	 * @param state
	 */
	void AllowScale(bool state);
	/**
	 *
	 * @param activeHandler
	 */
	void SetActiveHandler(Handler* activeHandler);

	/**
	 * 获取当前激活的handler
	 * @return
	 */
	Handler* GetActiveHandler();

protected:
	/**
	 *
	 */
	virtual void UpdataActiveHandler();

protected:
	bool m_bRotate; //!<
	bool m_bTranslate; //!<
	bool m_bScaleState; //!<
	bool m_bInTop; //!<

private:
	Handler* m_ActiveHandler; //当前激活Handler
	bool m_HandlerDirty;//Handler脏标识，当脏时，需要更新
};
}

#endif /*HANDLER_NODE_H*/

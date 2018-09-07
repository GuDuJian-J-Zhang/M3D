/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef     M3D_COMMAND_HISTORY_MANAGER_H
#define     M3D_OPERATION_HISTORY_MANAGER_H
#include 	"m3d/M3D.h"

namespace M3D
{
class Operation;
/**
 * @class
 * @brief 操作历史记录
 */
class M3D_API OperationHistoryManager:public Object
{
public:
	OperationHistoryManager();
	virtual ~OperationHistoryManager();

	/**
	 * @brief 将操作压入栈
	 * @param operation 操作
	 * @return 成功返回true 失败false
	 */
	bool Push(Operation* operation);
	/**
	 * @brief 弹出操作
	 * @return 操作
	 */
	Operation* Pop();

	/**
	 * @brief 从历史记录中移除
	 * @param operation 操作
	 * @return 成功true 失败 false
	 */
	bool RemoveFromHistory(Operation* operation);

	/**
	 * @brief 获取操作列表
	 * @return 返回操作列表
	 */
	const list<Operation*>& GetOperationList();

	/**
	 * @brief 清空操作历史
	 * @return 成功 true  失败 false
	 */
	bool Clear();

private:
	list<Operation*> m_historyList; //!<纪录操作历史 实现stack的特性
};

}
#endif


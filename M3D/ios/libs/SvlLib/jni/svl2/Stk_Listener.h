// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Listenter.h
 *
 *	@brief	事件监听
 *
 *	@par	历史:
 *		2016/08/16	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _STK_LISTENER_H_
#define _STK_LISTENER_H_

#include "DataManager.h"
#include "Stk_Define.h"
#include "Stk_Enum.h"

class Stk_DocumentManager;

/**************************************************************************************
 *	@class	Stk_Listenter
 *	@brief	事件监听器类
 *
 *	监听外部回调请求。<br>
 ***************************************************************************************/
class DATAMANAGER_API Stk_Listener
{
private:

public:
	/****************************************************************************
	 *	@brief		Stk_Listener构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Listener();

	/****************************************************************************
	 *	@brief		Stk_Listener析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Listener();
public:
	/****************************************************************************
	 *	@brief		加载进度
	 *	@param[in]	i_fLoadingPercent	加载百分比
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	virtual void LoadingProcess(STK_FLOAT32 i_fLoadingPercent);
};

/**************************************************************************************
 *	@class	Stk_ReaderListener
 *	@brief	读取监听器类
 *
 *	监听外部回调请求。<br>
 ***************************************************************************************/
class DATAMANAGER_API Stk_ReaderListener
{
private:
	Stk_DocumentManager*	m_stkDocP;
private:
	STK_FLOAT32				m_LoadPercent;			//!< ProtoType加载百分比
	STK_UINT32				m_TotalProtoTypeNum;	//!< SVL文件中ProtoType总数量
	STK_UINT32				m_ProtoTypeCount;		//!< ProtoType计数器
public:
	/****************************************************************************
	 *	@brief		Stk_ReaderListener构造函数
	 *	@param[in]	i_stkDocP		DocP管理对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_ReaderListener(Stk_DocumentManager*	i_stkDocP);

	/****************************************************************************
	 *	@brief		Stk_ReaderListener析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_ReaderListener();
public:
	/************************************************************************//**
	 *	@brief		实时更新SVL加载百分比
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		UpdataSVLLoadingPercent();

	/************************************************************************//**
	 *	@brief		SVL加载百分比数据
	 *	@param[in]	无
	 *	@param[out]	百分比
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32		GetSVLLoadingPercent();

	/************************************************************************//**
	 *	@brief		获取ProtoType总数量
	 *	@param[in]	无
	 *	@param[out]	ProtoType总数量
	 *	@retval		无
	 *	@note		无
	 *	@attention	
	 ****************************************************************************/
	STK_UINT32		GetTotalProtoTypeNum();

	/************************************************************************//**
	 *	@brief		设置ProtoType总数
	 *	@param[in]	ProtoType总数
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetTotalProtoTypeNum(STK_UINT32 i_nuPrototypeNum);

	/************************************************************************//**
	 *	@brief		加载进度
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				清空成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS		ReaderProcess();
};

#endif
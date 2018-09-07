// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Listener.cpp
 *
 *	@brief	事件监听器类
 *
 *	@par	历史:
 *		2016/08/10	创建。WangY@HOTEAMSOFT
****************************************************************************/

#include "Stk_Listener.h"
#include "Stk_DocumentManager.h"

/************************************************************************//**
 *	@brief		Stk_Listener构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Listener::Stk_Listener()
{
	
}

/************************************************************************//**
 *	@brief		Stk_Listener析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Listener::~Stk_Listener()
{
}

/************************************************************************//**
 *	@brief		加载进度
 *	@param[in]	i_fLoadingPercent	加载百分比
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
void Stk_Listener::LoadingProcess(STK_FLOAT32 i_fLoadingPercent)
{
	
}

/****************************************************************************
 *	@brief		Stk_ReaderListener构造函数
 *	@param[in]	i_stkDocP		DocP管理对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_ReaderListener::Stk_ReaderListener(Stk_DocumentManager*	i_stkDocP)
{
	m_stkDocP = i_stkDocP;
	m_LoadPercent = 0;
	m_TotalProtoTypeNum = 0;
	m_ProtoTypeCount = 0;
}

/************************************************************************//**
 *	@brief		Stk_ReaderListener析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_ReaderListener::~Stk_ReaderListener()
{
}

/************************************************************************//**
 *	@brief		实时更新SVL加载百分比
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ReaderListener::UpdataSVLLoadingPercent()
{
	m_ProtoTypeCount++;
	if (m_TotalProtoTypeNum != 0)
	{
		m_LoadPercent =  m_ProtoTypeCount/(STK_FLOAT32)m_TotalProtoTypeNum;
	}
	else
	{
		m_LoadPercent = -1;
	}
	
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		SVL加载百分比数据
 *	@param[in]	无
 *	@param[out]	百分比
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_ReaderListener::GetSVLLoadingPercent()
{
	return m_LoadPercent;
}

/************************************************************************//**
 *	@brief		获取ProtoType总数量
 *	@param[in]	无
 *	@param[out]	ProtoType总数量
 *	@retval		无
 *	@note		无
 *	@attention	
 ****************************************************************************/
STK_UINT32 Stk_ReaderListener::GetTotalProtoTypeNum()
{
	return m_TotalProtoTypeNum;
}

/************************************************************************//**
 *	@brief		设置ProtoType总数
 *	@param[in]	ProtoType总数
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ReaderListener::SetTotalProtoTypeNum(STK_UINT32 i_prototypenum)
{
	m_TotalProtoTypeNum = i_prototypenum;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		读取进度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_SUCCESS				清空成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_ReaderListener::ReaderProcess()
{
	Stk_Listener*	stkListenerP = NULL;
	stkListenerP = m_stkDocP->GetListener();
	if (stkListenerP == NULL)
	{
		return STK_ERROR;
	}
	stkListenerP->LoadingProcess(m_LoadPercent);

	return STK_SUCCESS;
}



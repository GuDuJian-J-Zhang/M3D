// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Listenter.h
 *
 *	@brief	�¼�����
 *
 *	@par	��ʷ:
 *		2016/08/16	������WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _STK_LISTENER_H_
#define _STK_LISTENER_H_

#include "DataManager.h"
#include "Stk_Define.h"
#include "Stk_Enum.h"

class Stk_DocumentManager;

/**************************************************************************************
 *	@class	Stk_Listenter
 *	@brief	�¼���������
 *
 *	�����ⲿ�ص�����<br>
 ***************************************************************************************/
class DATAMANAGER_API Stk_Listener
{
private:

public:
	/****************************************************************************
	 *	@brief		Stk_Listener���캯��
	 *	@param[in]	��
	 *	@param[out]	��
	 *	@retval		��
	 *	@note		��
	 *	@attention	��
	 ****************************************************************************/
	Stk_Listener();

	/****************************************************************************
	 *	@brief		Stk_Listener��������
	 *	@param[in]	��
	 *	@param[out]	��
	 *	@retval		��
	 *	@note		��
	 *	@attention	��
	 ****************************************************************************/
	~Stk_Listener();
public:
	/****************************************************************************
	 *	@brief		���ؽ���
	 *	@param[in]	i_fLoadingPercent	���ذٷֱ�
	 *	@param[out]	��
	 *	@retval		��
	 *	@note		��
	 *	@attention	��
	 ****************************************************************************/
	virtual void LoadingProcess(STK_FLOAT32 i_fLoadingPercent);
};

/**************************************************************************************
 *	@class	Stk_ReaderListener
 *	@brief	��ȡ��������
 *
 *	�����ⲿ�ص�����<br>
 ***************************************************************************************/
class DATAMANAGER_API Stk_ReaderListener
{
private:
	Stk_DocumentManager*	m_stkDocP;
private:
	STK_FLOAT32				m_LoadPercent;			//!< ProtoType���ذٷֱ�
	STK_UINT32				m_TotalProtoTypeNum;	//!< SVL�ļ���ProtoType������
	STK_UINT32				m_ProtoTypeCount;		//!< ProtoType������
public:
	/****************************************************************************
	 *	@brief		Stk_ReaderListener���캯��
	 *	@param[in]	i_stkDocP		DocP�������ָ��
	 *	@param[out]	��
	 *	@retval		��
	 *	@note		��
	 *	@attention	��
	 ****************************************************************************/
	Stk_ReaderListener(Stk_DocumentManager*	i_stkDocP);

	/****************************************************************************
	 *	@brief		Stk_ReaderListener��������
	 *	@param[in]	��
	 *	@param[out]	��
	 *	@retval		��
	 *	@note		��
	 *	@attention	��
	 ****************************************************************************/
	~Stk_ReaderListener();
public:
	/************************************************************************//**
	 *	@brief		ʵʱ����SVL���ذٷֱ�
	 *	@param[in]	��
	 *	@param[out]	��
	 *	@retval		��
	 *	@note		��
	 *	@attention	��
	 ****************************************************************************/
	STK_STATUS		UpdataSVLLoadingPercent();

	/************************************************************************//**
	 *	@brief		SVL���ذٷֱ�����
	 *	@param[in]	��
	 *	@param[out]	�ٷֱ�
	 *	@retval		��
	 *	@note		��
	 *	@attention	��
	 ****************************************************************************/
	STK_FLOAT32		GetSVLLoadingPercent();

	/************************************************************************//**
	 *	@brief		��ȡProtoType������
	 *	@param[in]	��
	 *	@param[out]	ProtoType������
	 *	@retval		��
	 *	@note		��
	 *	@attention	
	 ****************************************************************************/
	STK_UINT32		GetTotalProtoTypeNum();

	/************************************************************************//**
	 *	@brief		����ProtoType����
	 *	@param[in]	ProtoType����
	 *	@param[out]	��
	 *	@retval		��
	 *	@note		��
	 *	@attention	��
	 ****************************************************************************/
	STK_STATUS		SetTotalProtoTypeNum(STK_UINT32 i_nuPrototypeNum);

	/************************************************************************//**
	 *	@brief		���ؽ���
	 *	@param[in]	��
	 *	@param[out]	��
	 *	@retval		STK_SUCCESS				��ճɹ�
	 *	@note		��
	 *	@attention	�ڲ��ӿ�
	 ****************************************************************************/
	STK_STATUS		ReaderProcess();
};

#endif
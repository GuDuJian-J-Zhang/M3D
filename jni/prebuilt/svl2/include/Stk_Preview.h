// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Preview.h
 *
 *	@brief	预览图类
 *
 *	@par	历史:
 *		2013/10/06	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#ifndef _STK_PREVIEW_H_
#define _STK_PREVIEW_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Define.h"
#include "Stk_File.h"


/**************************************************************************************
	 *	@class	Stk_Preview
	 *	@brief	预览图类
	 *
***************************************************************************************/

class DATAMANAGER_API Stk_Preview
{
private:
	StkPreviewTypeEnum			m_eFiletype;
	STK_UINT32					m_nuSize;
	STK_CHAR*					m_chStreamP;

public:
	/****************************************************************************
	 *	@brief		Stk_Preview构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Preview(void);

	/****************************************************************************
	 *	@brief		Stk_Preview析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Preview(void);

public:
	/****************************************************************************
	 *	@brief		设置预览图类型
	 *	@param[in]	i_eType				预览图类型
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetType(StkPreviewTypeEnum i_eType);

	/****************************************************************************
	 *	@brief		获取预览图类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eFiletype			预览图类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkPreviewTypeEnum			GetType(void);

	/****************************************************************************
	 *	@brief		设置预览图流数据大小
	 *	@param[in]	i_nuSize			流数据大小
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetStreamSize(STK_UINT32 i_nuSize);

	/****************************************************************************
	 *	@brief		获取预览图流数据大小
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_nuSize			流数据大小
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32					GetStreamSize();

	/****************************************************************************
	 *	@brief		设置预览图流数据
	 *	@param[in]	i_chStreamP			预览图流数据
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetStream(STK_CHAR* i_chStreamP);

	/****************************************************************************
	 *	@brief		获取预览图流数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_chStreamP			预览图流数据
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_CHAR*					GetStream();

	/****************************************************************************
	 *	@brief		读取文件中的预览图数据
	 *	@param[in]	i_FileP				文件对象指针
	 *	@param[in]	i_PrevHeadP			预览图记录头指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadFile(Stk_File *i_FileP, StkSegmentHead *i_TiHeadP);
	
	/*****************************************************************************
	 *	@brief		写入预览图数据
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		STK_ERROR			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteFile(Stk_File *i_stkFileP);
};

#endif
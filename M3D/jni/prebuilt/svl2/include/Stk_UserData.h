// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_UserData.h
 *
 *	@brief	Stk_UserData用户数据类
 *
 *	@par	历史:
 *		2014/08/27	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#ifndef _Stk_USERDATA_H_
#define _Stk_USERDATA_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "DataManager.h"
#include "Stk_File.h"

/**************************************************************************************
	 *	@class	Stk_UserData
	 *	@brief	用户数据类
	 *
	 *	用户数据。<br>
	 *	继承自Stk_Entity类。
***************************************************************************************/

class DATAMANAGER_API  Stk_UserData 
{
private:
	Stk_ProtoType*				m_stkProtoP;					//!< 所属零件指针
	wstring						m_wstrFileName;					//!< 物理文件名
	wstring						m_wstrDataName;					//!< 用户数据名称
	STK_ID						m_id;							//!< 用户数据ID
	STK_CHAR					*m_dataP;						//!< 用户数据
	STK_UINT32					m_nudataSize;					//!< 用户数据长度

public:
	/****************************************************************************
	 *	@brief		Stk_UserData构造函数
	 *	@param[in]	i_stkProtoP	所属ProtoType指针
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_UserData(Stk_ProtoType* i_stkProtoP);

	/****************************************************************************
	 *	@brief		Stk_UserData构造函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_UserData();

public:

	/****************************************************************************
	 *	@brief		设置用户数据
	 *	@param[in]	i_wstrDataName	数据名称
	 *	@param[in]	i_id			数据ID
	 *	@param[in]	i_dataP			数据流
	 *	@param[in]	i_dataSize		数据流长度
	 *	@param[out]	i_wstrFileName	物理文件名(建议包括后缀)
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		成功
	 *	@retval		其它			失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetUserData( wstring i_wstrDataName, STK_ID i_id, STK_CHAR *i_dataP, STK_SIZE_T i_dataSize, wstring i_wstrFileName);

	/****************************************************************************
	 *	@brief		获取用户数据
	 *	@param[in]	无
	 *	@param[out]	o_wstrDataName	数据名称
	 *	@param[out]	o_id			数据ID
	 *	@param[out]	o_dataP			数据流
	 *	@param[out]	o_dataSize		数据流长度
	 *	@param[out]	o_wstrFileName	物理文件名
	 *	@retval		STK_SUCCESS		成功
	 *	@retval		其它			失败
	 *	@note		参数o_dataP是为用户分配的内存副本，使用完请用free释放内存
	 *	@attention	
	 ****************************************************************************/
	STK_STATUS					GetUserData( wstring &o_wstrDataName, STK_ID &o_id, STK_CHAR* &o_dataP, STK_SIZE_T &o_dataSize, wstring &o_wstrFileName);

	/****************************************************************************
	 *	@brief		往SVL文件读取用户数据
	 *	@param[in]	i_stkFileP		SVL文件指针
	 *	@param[in]	i_propHead		用户数据段头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		填写成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					WriteUserData(Stk_File *i_stkFileP, StkSegmentHead* i_userHead );
};

#endif
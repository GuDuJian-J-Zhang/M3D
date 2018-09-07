// All Rights Reserved. Copyright (C) 2017 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_UserData.h
*
*	@brief	用户数据类
*
*	@par	历史:
*		2017/06/10	创建。qinyp@HOTEAMSOFT
****************************************************************************/

#ifndef _SVLLIB_USERDATA_H_
#define _SVLLIB_USERDATA_H_

#pragma once
#include "Stk_Object.h"
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_UserData : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_UserData)
		public:
			Stk_UserData();
			virtual ~Stk_UserData();
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
			STK_STATUS					SetUserData(wstring i_wstrDataName, STK_ID i_id, 
				STK_CHAR *i_dataP, STK_SIZE_T i_dataSize, wstring i_wstrFileName);

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
			STK_STATUS					GetUserData(wstring &o_wstrDataName, STK_ID &o_id, 
				STK_CHAR* &o_dataP, STK_SIZE_T &o_dataSize, wstring &o_wstrFileName);

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* pDoc);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP);
		};
	}
}
#endif

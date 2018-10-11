// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Utility.h
 *
 *	@brief	公用接口声明
 *
 *	@par	历史:
 *		2014/2/27	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _Stk_UTILITY_H_
#define _Stk_UTILITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "DataManager.h"
#include "Stk_Object.h"
#include "Stk_Define.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
using std::string;
using std::wstring;
using std::vector;

class DATAMANAGER_API Stk_Utility:
	public Stk_Object
{
public:
	/****************************************************************************
	 *	@brief		替换名称中含有形如"<"、">"的字符为"_"的形式
	 *	@param[in]	i_wstrName				文件/ProtoType名称
	 *	@param[out]	无
	 *	@retval		文件/ProtoType名称
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	static wstring		ReplaceImproperCode(wstring& i_wstrName);

	/****************************************************************************
	 *	@brief		判断SVL版本是否是V2.2之前的版本
	 *	@param[in]	i_wstrVersion		版本号
	 *	@param[out]	无
	 *	@retval		 = TRUE				V2.2之前版本
					 = FALSE			V2.2及以后版本
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	static STK_BOOL		IsOldVersion(wstring i_wstrVersion);

	/****************************************************************************
	 *	@brief		转换矩阵到UV方向向量的形式
	 *	@param[in]	i_dMatrixAA		旋转平移矩阵
	 *	@param[out]	o_dirUp			Up方向向量
	 *	@param[out]	o_dirUp			Target方向向量
	 *	@retval		STK_SUCCESS		正常结束
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	static STK_STATUS	TransMatrixToUV(STK_DOUBLE64 i_dMatrixAA[4][4], STK_DIR32& o_dirUp, STK_DIR32& o_dirTarget);

	/*****************************************************************************
	 *	@brief		压缩记录
	 *	@param[in]	i_chRecordP			压缩对象记录
	 *	@param[in]	i_nuBuffSize		压缩对象所占的Buff大小
	 *	@param[in]	i_nCompressionLevel	压缩等级=9：高压缩比
	 *	@param[out]	o_chStreamP			压缩后的流数据
	 *	@param[out]	o_StreamSize		压缩后的流数据所占的
	 *	@retval		压缩后的流数据大小
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	static STK_INT32 LZ4_CompressRecord(const STK_CHAR* i_chRecordP, STK_INT32 i_nuBuffSize, STK_INT32 i_nCompressionLevel, char*& o_chStreamP);

	/*****************************************************************************
	 *	@brief		解压缩记录
	 *	@param[in]	i_chCompressStreamP			准备解压的流数据
	 *	@param[in]	i_nuCompressBuffSize		待解压的流数据所占内存大小
	 *	@param[in]	i_nuDeCompressBuffSize		解压后的流数据所占内存大小
	 *	@param[out]	o_chDeCompressStreamP		解压后的流数据
	 *	@retval		解压后的流数据所占内存大小
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	static STK_INT32 LZ4_DeCompressRecord(const STK_CHAR* i_chCompressStreamP, STK_INT32 i_nuCompressBuffSize, STK_INT32 i_nuDeCompressBuffSize, STK_CHAR*& o_chDeCompressStreamP);

	/*****************************************************************************
	 *	@brief		压缩记录
	 *	@param[in]	i_chRecordP			压缩对象记录
	 *	@param[in]	i_nuBuffSize		压缩对象所占的Buff大小
	 *	@param[in]	i_nCompressionLevel	压缩等级=5
	 *	@param[out]	o_chStreamP			压缩后的流数据
	 *	@param[out]	o_StreamSize		压缩后的流数据所占的
	 *	@retval		压缩后的流数据大小
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	static STK_INT32 Zip7_CompressRecord(const STK_CHAR* i_chRecordP, STK_INT32 i_nuBuffSize, STK_INT32 i_nCompressionLevel, char*& o_chStreamP);

	/*****************************************************************************
	 *	@brief		解压缩记录
	 *	@param[in]	i_chCompressStreamP			准备解压的流数据
	 *	@param[in]	i_nuCompressBuffSize		待解压的流数据所占内存大小
	 *	@param[in]	i_nuDeCompressBuffSize		解压后的流数据所占内存大小
	 *	@param[out]	o_chDeCompressStreamP		解压后的流数据
	 *	@retval		解压后的流数据所占内存大小
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	static STK_INT32 Zip7_DeCompressRecord(const STK_CHAR* i_chCompressStreamP, STK_INT32 i_nuCompressBuffSize, STK_INT32 i_nuDeCompressBuffSize, STK_CHAR*& o_chDeCompressStreamP);
};

#endif
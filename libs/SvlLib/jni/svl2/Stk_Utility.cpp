// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Utility.cpp
 *
 *	@brief	公用接口函数实现
 *
 *	@par	历史:
 *		2014/2/27	创建。WangY@HOTEAMSOFT
****************************************************************************/

#include "StdAfx.h"
#include "Stk_Utility.h"
#include "Stk_Platform.h"
#include "LZ4.h"
#include "LZ4hc.h"
#include "LzmaLib.h"

/************************************************************************//**
 *	@brief		替换名称中含有形如"<"、">"的字符为"_"的形式
 *	@param[in]	i_wstrName				文件/ProtoType名称
 *	@param[out]	无
 *	@retval		文件/ProtoType名称
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_Utility::ReplaceImproperCode(wstring& i_wstrName)
{
	STK_STATUS	stat = STK_SUCCESS;
	STK_UINT32	nuIndex = -1;
	wstring		wstrName = L"";
	STK_UINT32	nuSize = i_wstrName.length();
	for (int ii = 0; ii < nuSize; ii++)
	{
		if (i_wstrName.at(ii) == L'<' || i_wstrName.at(ii) == L'>'
			|| i_wstrName.at(ii) == L':' || i_wstrName.at(ii) == L'*'
			|| i_wstrName.at(ii) == L'?' || i_wstrName.at(ii) == L'\"'
			|| i_wstrName.at(ii) == L'/' || i_wstrName.at(ii) == L'\\'
			|| i_wstrName.at(ii) == L'|'
			)
		{
			i_wstrName.at(ii) = L'_';
		}
	}
	wstrName = i_wstrName;

	return wstrName;
}


/************************************************************************//**
 *	@brief		判断SVL版本是否是V2.2之前的版本
 *	@param[in]	i_wstrVersion		版本号
 *	@param[out]	无
 *	@retval		 = TRUE				V2.2之前版本
				 = FALSE			V2.2及以后版本
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Utility::IsOldVersion(wstring i_wstrVersion)
{
	STK_FLOAT32	fVersion = 0;
	string		strVersion = "";

	strVersion = Stk_Platform::WStringToString(i_wstrVersion);
	fVersion = atof(strVersion.c_str());
	if (fVersion >= 2.20f)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

/************************************************************************//**
 *	@brief		转换矩阵到UV方向向量的形式
 *	@param[in]	i_dMatrixAA		旋转平移矩阵
 *	@param[out]	o_dirUp			Up方向向量
 *	@param[out]	o_dirUp			Target方向向量
 *	@retval		STK_SUCCESS		正常结束
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Utility::TransMatrixToUV(STK_DOUBLE64 i_dMatrixAA[4][4], STK_DIR32& o_dirUp, STK_DIR32& o_dirTarget)
{
	/*
	 *	变量定义
	 */
	STK_STATUS	stat;
	/*
	 *	初始化
	 */
	stat = STK_SUCCESS;

	o_dirUp.X = i_dMatrixAA[1][0];
	o_dirUp.Y = i_dMatrixAA[1][1];
	o_dirUp.Z = i_dMatrixAA[1][2];

	o_dirTarget.X = -i_dMatrixAA[2][0];
	o_dirTarget.Y = -i_dMatrixAA[2][1];
	o_dirTarget.Z = -i_dMatrixAA[2][2];

	return stat;
}

/*************************************************************************//**
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
STK_INT32 Stk_Utility::LZ4_CompressRecord(const STK_CHAR* i_chRecordP, STK_INT32 i_nuBuffSize, STK_INT32 i_nCompressionLevel, char*& o_chStreamP)
{
	char*			out_buff = NULL;
	unsigned int	outSize = 0;

	out_buff = (char*)calloc(1, LZ4_compressBound(i_nuBuffSize));
	if (out_buff == NULL)
	{
		return -1;
	}

	if (i_nCompressionLevel <= 3)
	{
		outSize = LZ4_compress(i_chRecordP, out_buff, i_nuBuffSize);
	}
	else if (i_nCompressionLevel > 4 && i_nCompressionLevel <= 16)
	{
		outSize = LZ4_compressHC2(i_chRecordP, out_buff, i_nuBuffSize, i_nCompressionLevel);
	}
	else
	{
		if (out_buff == NULL)
		{
			free(out_buff);
			out_buff = NULL;
		}
		return outSize;
	}
	o_chStreamP = out_buff;

	return outSize;
}


/*************************************************************************//**
 *	@brief		解压缩记录
 *	@param[in]	i_chCompressStreamP			准备解压的流数据
 *	@param[in]	i_nuCompressBuffSize		待解压的流数据所占内存大小
 *	@param[in]	i_nuDeCompressBuffSize		解压后的流数据所占内存大小
 *	@param[out]	o_chDeCompressStreamP		解压后的流数据
 *	@retval		解压后的流数据所占内存大小
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_INT32 Stk_Utility::LZ4_DeCompressRecord(const STK_CHAR* i_chCompressStreamP, STK_INT32 i_nuCompressBuffSize, STK_INT32 i_nuDeCompressBuffSize, STK_CHAR*& o_chDeCompressStreamP)
{
	char*			out_buff = NULL;
	int				decodeSize = 0;

	out_buff = (char*)calloc(1, LZ4_compressBound(i_nuDeCompressBuffSize));
	if (out_buff == NULL)
	{
		return -1;
	}

	decodeSize = LZ4_decompress_safe(i_chCompressStreamP, out_buff, i_nuCompressBuffSize, i_nuDeCompressBuffSize + 1024);
	if (decodeSize > 0)
	{
		o_chDeCompressStreamP = out_buff;
	}

	return decodeSize;
}

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
STK_INT32 Stk_Utility::Zip7_CompressRecord(const STK_CHAR* i_chRecordP, STK_INT32 i_nuBuffSize, STK_INT32 i_nCompressionLevel, char*& o_chStreamP)
{
	int				state = 0;
	unsigned char*	out_buff = NULL;
	unsigned int	outSize = 0;
	size_t			destLen = 0;					// (io)
	size_t			unPropsSize = LZMA_PROPS_SIZE;
	unsigned		dictSize = 1 << 24;
	int				lc = 3;
	int				lp = 0;
	int				pb = 2;
	int				fb = 32;
	int				numThreads = 2;
	Byte			outProps[LZMA_PROPS_SIZE];

	out_buff = (unsigned char*)calloc(1, i_nuBuffSize);
	if (out_buff == NULL)
	{
		return -1;
	}
#ifdef __MOBILE__
	destLen = 1*i_nuBuffSize;
#else
	destLen = _msize(out_buff);
#endif

	state = LzmaCompress(out_buff, &destLen, (unsigned char *)i_chRecordP, i_nuBuffSize, outProps, &unPropsSize, i_nCompressionLevel, dictSize, lc, lp, pb, fb, numThreads);
	if (state != 0)
	{
		return STK_ERROR;
	}
	if (destLen > 0 && destLen < i_nuBuffSize)
	{
		o_chStreamP = (char *)out_buff;
	}
	
	return destLen;
}

/*************************************************************************//**
 *	@brief		解压缩记录
 *	@param[in]	i_chCompressStreamP			准备解压的流数据
 *	@param[in]	i_nuCompressBuffSize		待解压的流数据所占内存大小
 *	@param[in]	i_nuDeCompressBuffSize		解压后的流数据所占内存大小
 *	@param[out]	o_chDeCompressStreamP		解压后的流数据
 *	@retval		解压后的流数据所占内存大小
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_INT32 Stk_Utility::Zip7_DeCompressRecord(const STK_CHAR* i_chCompressStreamP, STK_INT32 i_nuCompressBuffSize, STK_INT32 i_nuDeCompressBuffSize, STK_CHAR*& o_chDeCompressStreamP)
{
	int				state = 0;
	unsigned char*	out_buff = NULL;
	size_t			destLen = 0;
	size_t			unPropsSize = LZMA_PROPS_SIZE;
	Byte			outProps[LZMA_PROPS_SIZE];
	
	out_buff = (unsigned char*)calloc(1, i_nuDeCompressBuffSize);
	if (out_buff == NULL)
	{
		return -1;
	}
	outProps[0] = 0x5d;
	outProps[1] = 0x00;
	outProps[2] = 0x00;
	outProps[3] = 0x00;
	outProps[4] = 0x01;

#ifdef __MOBILE__
	destLen = 1* i_nuDeCompressBuffSize;
#else
	destLen = _msize(out_buff);
#endif

	state = LzmaUncompress(out_buff, &destLen, (unsigned char *)i_chCompressStreamP, (size_t *)&i_nuCompressBuffSize, outProps, unPropsSize);
	if (state != 0)
	{
		return STK_ERROR;
	}
	o_chDeCompressStreamP = (char *)out_buff;

	return destLen;
}
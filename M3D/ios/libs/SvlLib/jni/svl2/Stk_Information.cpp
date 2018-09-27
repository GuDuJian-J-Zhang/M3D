// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Information.cpp
 *
 *	@brief	Node基本信息类
 *
 *	@par	历史:
 *		2014/05/16	创建。WangY@HOTEAMSOFT
****************************************************************************/

#include "StdAfx.h"
#include "Stk_Information.h"
#include "Stk_Manager.h"
#include "Stk_Enum.h"

/************************************************************************//**
 *	@brief		Stk_Information构造函数
 *	@param[in]	i_stkPrototypeP			ProtoType对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Information::Stk_Information(Stk_ProtoType* i_stkPrototypeP)
{
	m_stkPrototypeP = i_stkPrototypeP;
	m_nLayer = -1;
}

/****************************************************************************
 *	@brief		Stk_Information拷贝构造函数
 *	@param[in]	i_stkInfo	基本信息类
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

//Stk_Information::Stk_Information(const Stk_Information& i_stkInfo)
//{
//	m_stkPrototypeP = new Stk_ProtoType();
//	memcpy(m_stkPrototypeP, i_stkInfo.m_stkPrototypeP, sizeof(*i_stkInfo.m_stkPrototypeP));
//	m_nuLayer = i_stkInfo.m_nuLayer;
//}

/************************************************************************//**
 *	@brief		Stk_Information析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Information::~Stk_Information()
{

}

/****************************************************************************
 *	@brief		Stk_Information赋值操作符
 *	@param[in]	i_stkInfo	基本信息类
 *	@param[out]	无
 *	@retval		*this		基本信息类
 *	@note		无
 *	@attention	无
 ****************************************************************************/

//Stk_Information& Stk_Information::operator = (const Stk_Information& i_stkInfo)
//{
//	if (this == &i_stkInfo)
//	{
//		return *this;
//	}
//	m_stkPrototypeP = new Stk_ProtoType();
//	memcpy(m_stkPrototypeP, i_stkInfo.m_stkPrototypeP, sizeof(*i_stkInfo.m_stkPrototypeP));
//	m_nuLayer = i_stkInfo.m_nuLayer;
//	
//	return *this;
//}

/************************************************************************//**
 *	@brief		设置节点所在图层
 *	@param[in]	i_nLayer		节点所在图层
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Information::SetLayer(STK_INT32 i_nLayer)
{
	m_nLayer = i_nLayer;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取节点所在图层
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_nuLayer		节点所在图层
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_INT32 Stk_Information::GetLayer()
{
	return m_nLayer;
}

/************************************************************************//**
 *	@brief		写基本信息段
 *	@param[in]	i_stkFileP		文件指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其他			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Information::WriteFile(Stk_File *i_stkFileP)
{
	STK_STATUS			ret = STK_SUCCESS;
	StkSegmentHead		InfoHead = {0};
	StkInfoRec			stkInfoRec;

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *	【第一步】基本信息段
	 */
	// 【1.1】 输出基本信息段头
	InfoHead.kind = SEG_TYPE_INFO;
	ret = i_stkFileP->WriteSegmentHeadData(&InfoHead);
	if (ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}
	// 【1.2】 输出基本信息记录
	stkInfoRec.Control.type = REC_TYPE_INFO;
	stkInfoRec.Control.length = sizeof(StkInfoRec) - sizeof(StkControl);
	stkInfoRec.Control.version = _VER_INFO_RECORD;

	stkInfoRec.nLayer = m_nLayer;

	// 【1.3】 完成基本信息记录的输出
	ret = i_stkFileP->WriteSegmentRecordData(&InfoHead, (StkControl *)&stkInfoRec);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【1.4】 完成基本信息段的输出
	ret = i_stkFileP->WriteSegmentFinishData(&InfoHead);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return ret;
}

/************************************************************************//**
 *	@brief		读基本信息段
 *	@param[in]	i_FileP			文件指针
 *	@param[in]	i_InfoHeadP		段头
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其他			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Information::ReadFile(Stk_File *i_FileP, StkSegmentHead *i_InfoHeadP)
{
	STK_STATUS		ret = STK_SUCCESS;
	StkControl**	RecordPP = NULL;
	StkInfoRec*		stkInfoRecP = NULL;
	STK_UINT32		ii;

	/*
	 *	读取基本信息段数据
	 */
	RecordPP = (StkControl **)calloc(i_InfoHeadP->recnum, sizeof(StkControl*));
	if(i_InfoHeadP->recnum !=0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	ret = i_FileP->ReadSegmentAllRecordData(i_InfoHeadP, (StkControl** )RecordPP);
	if (ret != STK_SUCCESS)
	{
		return ret;
	}
	/*
	 *	读取基本信息段各记录
	 */
	for (ii = 0; ii < i_InfoHeadP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_INFO)
		{
			if (RecordPP[ii]->version == 1)
			{
				stkInfoRecP = (StkInfoRec*)RecordPP[ii];
				m_nLayer = stkInfoRecP->nLayer;
			}
		}
		else
		{

		}
	}

	// 释放内存
	ret = i_FileP->ReadSegmentFinishData(i_InfoHeadP, (StkControl**)RecordPP);
	if (ret != STK_SUCCESS)
	{
		return ret;
	}
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	return ret;
}
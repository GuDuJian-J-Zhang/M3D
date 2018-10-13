// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_SpacialPoint.cpp
 *
 *	@brief	空间点类的实现
 *
 *	@par	历史:
 *		2014/04/17	创建。WangY@HOTEAMSOFT
****************************************************************************/

#pragma once
#include "StdAfx.h"
#include "Stk_SpacialPoint.h"
#include "Stk_File.h"
#include "Stk_ProtoType.h"
#include "Stk_Platform.h"


/************************************************************************//**
 *	@brief		Stk_SpacialPointEntity构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_SpacialPointEntity::Stk_SpacialPointEntity()
{
	m_stkProtoP = NULL;
	m_ID = 0;
	m_wstrName = L"";
	m_bDispStat = FALSE;
	m_eType = StkSpacialPointTypeEnum::STK_PT_UNSPECIFIED;
	m_rgbaPoint.Red = -1.0f;
	m_rgbaPoint.Green = -1.0f;
	m_rgbaPoint.Blue = -1.0f;
	m_rgbaPoint.Alpha = -1.0f;
	m_pntPos.X = 0.0f;
	m_pntPos.Y = 0.0f;
	m_pntPos.Z = 0.0f;
}

/************************************************************************//**
 *	@brief		Stk_SpacialPointEntity构造函数
 *	@param[in]	i_stkProtoTypeP		空间点所属的组件类对象
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_SpacialPointEntity::Stk_SpacialPointEntity(Stk_ProtoType* i_stkProtoTypeP)
{
	m_stkProtoP = i_stkProtoTypeP;
	m_ID = 0;
	m_wstrName = L"";
	m_bDispStat = FALSE;
	m_eType = StkSpacialPointTypeEnum::STK_PT_UNSPECIFIED;
	m_rgbaPoint.Red = -1.0f;
	m_rgbaPoint.Green = -1.0f;
	m_rgbaPoint.Blue = -1.0f;
	m_rgbaPoint.Alpha = -1.0f;
	m_pntPos.X = 0.0f;
	m_pntPos.Y = 0.0f;
	m_pntPos.Z = 0.0f;
}

/****************************************************************************
 *	@brief		Stk_SpacialPointEntity拷贝构造函数
 *	@param[in]	i_stkSpacialPoint	空间点类对象
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

//Stk_SpacialPointEntity::Stk_SpacialPointEntity(const Stk_SpacialPointEntity& i_stkSpacialPoint)
//{
//	m_stkProtoP = new Stk_ProtoType();
//	memcpy(m_stkProtoP, i_stkSpacialPoint.m_stkProtoP, sizeof(*i_stkSpacialPoint.m_stkProtoP));
//	m_ID = i_stkSpacialPoint.m_ID;
//	m_wstrName = i_stkSpacialPoint.m_wstrName;
//	m_bDispStat = i_stkSpacialPoint.m_bDispStat;
//	m_eType = i_stkSpacialPoint.m_eType;
//	memcpy(&m_rgbaPoint, &i_stkSpacialPoint.m_rgbaPoint, sizeof(STK_RGBA32));
//	memcpy(&m_pntPos, &i_stkSpacialPoint.m_pntPos, sizeof(STK_PNT32));
//}

/************************************************************************//**
 *	@brief		Stk_SpacialPointEntity析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_SpacialPointEntity::~Stk_SpacialPointEntity()
{

}

/****************************************************************************
 *	@brief		Stk_SpacialPointEntity赋值构造函数
 *	@param[in]	i_stkSpacialPoint	空间点类对象
 *	@param[out]	无
 *	@retval		当前空间点类对象
 *	@note		无
 *	@attention	无
 ****************************************************************************/

//Stk_SpacialPointEntity& Stk_SpacialPointEntity::operator = (const Stk_SpacialPointEntity& i_stkSpacialPoint)
//{
//	if (this == &i_stkSpacialPoint)
//	{
//		return *this;
//	}
//	m_stkProtoP = new Stk_ProtoType();
//	memcpy(m_stkProtoP, i_stkSpacialPoint.m_stkProtoP, sizeof(*i_stkSpacialPoint.m_stkProtoP));
//	m_ID = i_stkSpacialPoint.m_ID;
//	m_wstrName = i_stkSpacialPoint.m_wstrName;
//	m_bDispStat = i_stkSpacialPoint.m_bDispStat;
//	m_eType = i_stkSpacialPoint.m_eType;
//	memcpy(&m_rgbaPoint, &i_stkSpacialPoint.m_rgbaPoint, sizeof(STK_RGBA32));
//	memcpy(&m_pntPos, &i_stkSpacialPoint.m_pntPos, sizeof(STK_PNT32));
//
//	return *this;
//}

/************************************************************************//**
 *	@brief		设置空间点ID
 *	@param[in]	i_ID	空间点ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_SpacialPointEntity::SetID(STK_ID i_ID)
{
	m_ID = i_ID;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取空间点ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_ID	空间点ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_SpacialPointEntity::GetID()
{
	return m_ID;
}

/************************************************************************//**
 *	@brief		获取ProtoType ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		ProtoType ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_SpacialPointEntity::GetProtoTypeID()
{
	return m_stkProtoP->GetID();
}

/************************************************************************//**
 *	@brief		设置空间点的名字
 *	@param[in]	i_wstrName		空间点的名字
 *	@param[out]	无
 *	@retval		STK_SUCCESS
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_SpacialPointEntity::SetName(wstring i_wstrName)
{
	m_wstrName = i_wstrName;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取空间点的名字
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrName	空间点的名字
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_SpacialPointEntity::GetName()
{
	return m_wstrName;
}

/************************************************************************//**
 *	@brief		设置显隐状态
 *	@param[in]	i_bDispStat		显隐状态
 *	@param[out]	无
 *	@retval		STK_SUCCESS
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_SpacialPointEntity::SetDispStat(STK_BOOL i_bDispStat)
{
	m_bDispStat = i_bDispStat;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取显隐状态
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bDispStat		显隐状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_SpacialPointEntity::GetDispStat()
{
	return m_bDispStat;
}

/************************************************************************//**
 *	@brief		设置空间点的类型
 *	@param[in]	i_eType			空间点的类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_SpacialPointEntity::SetType(StkSpacialPointTypeEnum i_eType)
{
	m_eType = i_eType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取空间点的类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eType		空间点的类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkSpacialPointTypeEnum Stk_SpacialPointEntity::GetType()
{
	return m_eType;
}

/************************************************************************//**
 *	@brief		设置空间点的颜色
 *	@param[in]	i_rgbaSpacialPoint		空间点的颜色
 *	@param[out]	无
 *	@retval		STK_SUCCESS
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_SpacialPointEntity::SetColor(STK_RGBA32 i_rgbaSpacialPoint)
{
	m_rgbaPoint = i_rgbaSpacialPoint;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取空间点的颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_rgbaPoint		空间点的颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_RGBA32 Stk_SpacialPointEntity::GetColor()
{
	return m_rgbaPoint;
}

/************************************************************************//**
 *	@brief		设置空间点的位置
 *	@param[in]	i_pntPos		空间点的位置
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_SpacialPointEntity::SetPntPosition(STK_PNT32 i_pntPos)
{
	m_pntPos = i_pntPos;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取空间点的位置
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntPos		空间点的位置
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_PNT32 Stk_SpacialPointEntity::GetPntPosition()
{
	return m_pntPos;
}

/************************************************************************//**
 *	@brief		从SVL中读取空间点
 *	@param[in]	i_stkFileP			SVL文件指针
 *	@param[in]	i_PointHeadP		空间点段头
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_SpacialPointEntity::ReadSpacialPoint(Stk_File* i_stkFileP, StkSegmentHead* i_PointHeadP)
{
	StkSpacialPointRec*	SpacialPointRecP = NULL;
	StkControl**		RecordPP = NULL;
	STK_UINT32			nuCharNum = 0;
	STK_UINT32			ii,jj;
	/*
	 *	读取空间点数据
	 */
	RecordPP = (StkControl**)calloc(i_PointHeadP->recnum, sizeof(StkControl*));
	if (i_PointHeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	i_stkFileP->ReadSegmentAllRecordData(i_PointHeadP, (StkControl **)RecordPP);
	/*
	 *	读取空间点各记录
	 */
	for (ii = 0; ii < i_PointHeadP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_SPACIALPOINT)
		{
			if (RecordPP[ii]->version == 1)
			{
				SpacialPointRecP = (StkSpacialPointRec*)RecordPP;
				nuCharNum = SpacialPointRecP->nuCharNum;
				STK_WCHAR* wchNameP = NULL;
				wchNameP = new STK_WCHAR[nuCharNum];
				if (nuCharNum > 0 && wchNameP == NULL)
				{
					return STK_ERROR_MEMORY;
				}
				for (jj = 0; jj < nuCharNum; jj++)
				{
					wchNameP[jj] = SpacialPointRecP->atr[jj];
				}
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(wchNameP);
#else
				m_wstrName = wchNameP;
#endif
				m_ID = SpacialPointRecP->idPoint;
				m_bDispStat = SpacialPointRecP->nuDispStat;
				m_eType = (StkSpacialPointTypeEnum)SpacialPointRecP->nuType;
				m_rgbaPoint = SpacialPointRecP->rgbaPoint;
				m_pntPos = SpacialPointRecP->pntPos;
				//删除临时分配的内存
				if (wchNameP != NULL)
				{
					delete [] wchNameP;
					wchNameP = NULL;
				}
			}
		}
	}

	// 释放内存
	i_stkFileP->ReadSegmentFinishData(i_PointHeadP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		往SVL文件填写空间点
 *	@param[in]	i_PointHeadP	空间点段头
 *	@param[in]	i_stkFileP		SVL文件指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		填写成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_SpacialPointEntity::WriteSpacialPoint(StkSegmentHead* i_PointHeadP, Stk_File *i_stkFileP)
{
	StkSpacialPointRec*	SpacialPointRecP = NULL;
	STK_STATUS			ret = STK_SUCCESS;
	STK_UINT32			nuCharNum = 0;

	if (i_PointHeadP == NULL || i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
	nuCharNum = m_wstrName.size() + 1;
	SpacialPointRecP = (StkSpacialPointRec*)calloc(1, sizeof(StkSpacialPointRec) - sizeof(STK_WCHAR) + nuCharNum * sizeof(STK_WCHAR));
	if (SpacialPointRecP == NULL)
	{
		return STK_ERROR;
	}
	/*
	 *	属性填充
	 */
	SpacialPointRecP->Control.type = REC_TYPE_SPACIALPOINT;
	SpacialPointRecP->Control.length = sizeof(StkSpacialPointRec) - sizeof(STK_WCHAR) + nuCharNum * sizeof(STK_WCHAR) - sizeof(StkControl);
	SpacialPointRecP->Control.version = _VER_SPACIALPOINT_RECORD;

	SpacialPointRecP->idPoint = m_ID;
	SpacialPointRecP->nuDispStat = m_bDispStat;
	SpacialPointRecP->nuType = m_eType;
	SpacialPointRecP->rgbaPoint = m_rgbaPoint;
	SpacialPointRecP->pntPos = m_pntPos;
	SpacialPointRecP->nuCharNum = nuCharNum;
#ifdef __MOBILE__
	memcpy(SpacialPointRecP->atr,Stk_Platform::WStringToSTKWCHARStr(m_wstrName),(m_wstrName.size() +1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(SpacialPointRecP->atr, nuCharNum, m_wstrName.c_str());
#endif

	ret = i_stkFileP->WriteSegmentRecordData(i_PointHeadP, (StkControl *)SpacialPointRecP);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	if (SpacialPointRecP != NULL)
	{
		free(SpacialPointRecP);
		SpacialPointRecP = NULL;
	}

	return STK_SUCCESS;
}
// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_CurveEntity.cpp
 *
 *	@brief	PMI类成员函数的实现
 *
 *	@par	历史:
 *		2013/08/14	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "StdAfx.h"
#include "Stk_CurveEntity.h"
#include "Stk_DocumentManager.h"

#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif

/************************************************************************//**
 *	@brief		Stk_CurveEntity构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_CurveEntity::Stk_CurveEntity(void)
{
	m_ID = 0;
	m_rgbaCurve.Red = -1.0f;
	m_rgbaCurve.Green = -1.0f;
	m_rgbaCurve.Blue = -1.0f;
	m_rgbaCurve.Alpha = -1.0f;
	m_eCurveType = CURVE_TYPE_UNKNOWN;
	m_eUsageType = CURVE_USAGE_UNKNOWN;
	m_eDispType = CONTINUOUS_LINE;
	m_bDispStat = true;
}

/************************************************************************//**
 *	@brief		Stk_CurveEntity析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_CurveEntity::~Stk_CurveEntity(void)
{

}

/************************************************************************//**
 *	@brief		注册Curve的ID
 *	@param[in]	i_ID				Curve的ID
 *	@retval		STK_SUCCESS			注册成功
 *	@retval		STK_ERROR			注册失败
 *	@note		指定ID需大于等于1的整数
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_CurveEntity::RegisterID(STK_ID i_ID)
{
	STK_STATUS eState = m_stkProtoP->GetProtoIDManager()->RegCurveID(this, i_ID);
	if (eState==STK_SUCCESS)
	{
		m_ID = i_ID;
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Curve的ID
 *	@param[in]	无
 *	@retval		m_ID			Curve的ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_CurveEntity::GetID()
{
	return m_ID;
}

/************************************************************************//**
 *	@brief		设置Curve的ID
 *	@param[in]	i_ID				Curve的ID
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_CurveEntity::SetID(STK_ID i_ID)
{
	m_ID = i_ID;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Curve的类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eCurveType	Curve的类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkCurveTypeEnum Stk_CurveEntity::GetCurveType()
{
	return m_eCurveType;
}

/************************************************************************//**
 *	@brief		获取Curve的应用类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eUsageType		Curve的定义场景类型
 *	@note		用于区分Curve的定义场景类型
 *	@attention	无
 ****************************************************************************/

StkCurveUsageEnum Stk_CurveEntity::GetUsageType()
{
	return m_eUsageType;
}

/************************************************************************//**
 *	@brief		设置Curve的定义场景类型
 *	@param[in]	i_nuDefType		Curve的定义场景类型
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_CurveEntity::SetUsageType(StkCurveUsageEnum i_nuDefType)
{
	m_eUsageType = i_nuDefType;
}

/************************************************************************//**
 *	@brief		获取Curve的颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_rgbaCurve			颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_RGBA32 Stk_CurveEntity::GetColor()
{
	return m_rgbaCurve;
}

/************************************************************************//**
 *	@brief		设置Curve的颜色
 *	@param[in]	i_rgbaCurve			Curve的颜色
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_CurveEntity::SetColor(const STK_RGBA32& i_rgbaCurve)
{
	m_rgbaCurve = i_rgbaCurve;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		设置线条是否隐藏
 *	@param[in]	i_bDispStat			线条是否隐藏
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_CurveEntity::SetCurveDispStat(STK_BOOL i_bDispStat)
{
	m_bDispStat = i_bDispStat;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取线条的显隐状态
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bDispStat
 *				= TRUE		显示
 *				= FALSE		隐藏
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_CurveEntity::GetCurveDispStat()
{
	return m_bDispStat;
}

/*************************************************************************//**
 *	@brief		设置线条的显示类型
 *	@param[in]	i_eDispType				线条显示类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_CurveEntity::SetCurveDispType(StkCurveDispTypeEnum i_eDispType)
{
	m_eDispType = i_eDispType;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取线条的显示类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_nuDispType			线条显示类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkCurveDispTypeEnum Stk_CurveEntity::GetCurveDispType()
{
	return m_eDispType;
}

/************************************************************************//**
 *	@brief		Stk_PolyLine构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_PolyLine::Stk_PolyLine(void)
{
	m_eCurveType = CURVE_TYPE_POLYLINE;
	m_fUMin = 0.0f;
	m_fUMax = 0.0f;
}

/************************************************************************//**
 *	@brief		Stk_PolyLine构造函数
 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_PolyLine::Stk_PolyLine(Stk_ProtoType* i_stkProtoTypeP)
{
	m_stkProtoP = i_stkProtoTypeP;
	m_eCurveType = CURVE_TYPE_POLYLINE;
	m_fUMin = 0.0f;
	m_fUMax = 0.0f;
}

/************************************************************************//**
 *	@brief		Stk_PolyLine析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_PolyLine::~Stk_PolyLine(void)
{

}

/************************************************************************//**
 *	@brief		获取折线点列信息
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntPoints			折线点列
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const vector<STK_PNT32>& Stk_PolyLine::GetPoints()
{
	return m_pntPoints;
}

/************************************************************************//**
 *	@brief		设置折线点列信息
 *	@param[in]	i_pntPoints			折线点列
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PolyLine::SetPoints(const vector<STK_PNT32>& i_pntPoints)
{
	m_pntPoints = i_pntPoints;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取折线起始终止范围
 *	@param[in]	无
 *	@param[out]	o_fUMin				起始范围
 *	@param[out]	o_fUMax				终止范围
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_PolyLine::GetDomain(STK_FLOAT32& o_fUMin, STK_FLOAT32& o_fUMax)
{
	o_fUMin = m_fUMin;
	o_fUMax = m_fUMax;
}

/************************************************************************//**
 *	@brief		设置折线起始终止范围
 *	@param[in]	i_fUMin				起始范围
 *	@param[in]	i_fUMax				终止范围
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_PolyLine::SetDomain(STK_FLOAT32 i_fUMin, STK_FLOAT32 i_fUMax)
{
	m_fUMin = i_fUMin;
	m_fUMax = i_fUMax;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		写入折线数据
 *	@param[in]	i_head			数据段头指针
 *	@param[in]	i_stkFileP		文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其它			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_PolyLine::WritePolyline(StkSegmentHead* i_head, Stk_File* i_stkFileP)
{
	STK_STATUS		ret = STK_SUCCESS;
	StkPolyLineRec3 *PolyLineRecP3 = NULL;
#ifdef _WINDOWS_	
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif

	if(i_head == NULL || i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	/*
	 *	属性填充
	 */
	PolyLineRecP3 = (StkPolyLineRec3 *)calloc(1, sizeof(StkPolyLineRec3) - sizeof(STK_FLOAT32) + 3*m_pntPoints.size()*sizeof(STK_FLOAT32));
	if(PolyLineRecP3 == NULL){
		return STK_ERROR;
	}
	PolyLineRecP3->Control.type = REC_TYPE_POLYLINE;
	PolyLineRecP3->Control.length = sizeof(StkPolyLineRec3) - sizeof(STK_FLOAT32) + 3*m_pntPoints.size()*sizeof(STK_FLOAT32) - sizeof(StkControl);
	PolyLineRecP3->Control.version = _VER_LINEAR_RECORD /*3*/;

	PolyLineRecP3->ID = GetID();
	PolyLineRecP3->Color = GetColor();
	PolyLineRecP3->Type = GetCurveType();
	PolyLineRecP3->DefType = GetUsageType();		// add on 2013.10.8 by gaoqq
	PolyLineRecP3->nuDisp = GetCurveDispStat();		// add by wangyuan in 2013 12 12
	PolyLineRecP3->nuDispType = GetCurveDispType();	// add by wangyuan in 2013 12 12
	PolyLineRecP3->CMin = m_fUMin;
	PolyLineRecP3->CMax = m_fUMax;
	PolyLineRecP3->NumPoints = m_pntPoints.size();
	
	for(int ii = 0; ii < m_pntPoints.size(); ii++)
	{
		WriteVertex(PolyLineRecP3->atr + ii*3, m_pntPoints[ii]);
	}

	ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)PolyLineRecP3);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：PolyLine记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	if(PolyLineRecP3 != NULL)
	{
		free(PolyLineRecP3);
		PolyLineRecP3 = NULL;
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		写入顶点数据
 *	@param[in]	fatrP			顶点存储地址
 *	@param[in]	pnt				顶点数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其它			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_PolyLine::WriteVertex(STK_FLOAT32* fatrP, const STK_PNT32& pnt)
{
	if(fatrP == NULL)
	{
		return STK_ERROR;
	}

	fatrP[0] = pnt.X;
	fatrP[1] = pnt.Y;
	fatrP[2] = pnt.Z;
	return STK_SUCCESS;
}
/************************************************************************//**
 *	@brief		Stk_Ellipse构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Ellipse::Stk_Ellipse(void)
{
	m_eCurveType = CURVE_TYPE_ELLIPSE;
	m_fUMin = 0.0f;
	m_fUMax = 0.0f;
	m_pntCenter.X = 0.0f;
	m_pntCenter.Y = 0.0f;
	m_pntCenter.Z = 0.0f;
	m_fMajorRadius = 0.0f;
	m_fMinorRadius = 0.0f;
	m_dirX.X = 0.0f;
	m_dirX.Y = 0.0f;
	m_dirX.Z = 0.0f;
	m_dirY.X = 0.0f;
	m_dirY.Y = 0.0f;
	m_dirY.Z = 0.0f;
	m_dirZ.X = 0.0f;
	m_dirZ.Y = 0.0f;
	m_dirZ.Z = 0.0f;
	m_dirNormal.X = 0.0f;
	m_dirNormal.Y = 0.0f;
	m_dirNormal.Z = 0.0f;
	m_dirOrigin.X = 0.0f;
	m_dirOrigin.Y = 0.0f;
	m_dirOrigin.Z = 0.0f;
}

/************************************************************************//**
 *	@brief		Stk_Ellipse构造函数
 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Ellipse::Stk_Ellipse(Stk_ProtoType* i_stkProtoTypeP)
{
	m_stkProtoP = i_stkProtoTypeP;
	m_eCurveType = CURVE_TYPE_ELLIPSE;
	m_fUMin = 0.0f;
	m_fUMax = 0.0f;
	m_pntCenter.X = 0.0f;
	m_pntCenter.Y = 0.0f;
	m_pntCenter.Z = 0.0f;
	m_fMajorRadius = 0.0f;
	m_fMinorRadius = 0.0f;
	m_dirX.X = 0.0f;
	m_dirX.Y = 0.0f;
	m_dirX.Z = 0.0f;
	m_dirY.X = 0.0f;
	m_dirY.Y = 0.0f;
	m_dirY.Z = 0.0f;
	m_dirZ.X = 0.0f;
	m_dirZ.Y = 0.0f;
	m_dirZ.Z = 0.0f;
	m_dirNormal.X = 0.0f;
	m_dirNormal.Y = 0.0f;
	m_dirNormal.Z = 0.0f;
	m_dirOrigin.X = 0.0f;
	m_dirOrigin.Y = 0.0f;
	m_dirOrigin.Z = 0.0f;
}

/************************************************************************//**
 *	@brief		Stk_Ellipse析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Ellipse::~Stk_Ellipse(void)
{

}

/************************************************************************//**
 *	@brief		获取弧线中心点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntCenter			中心点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Ellipse::GetCenterPoint()
{
	return m_pntCenter;
}

/************************************************************************//**
 *	@brief		设置弧线中心点
 *	@param[in]	i_pntCenter			中心点
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Ellipse::SetCenterPoint(const STK_PNT32& i_pntCenter)
{
	m_pntCenter = i_pntCenter;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取弧线方向向量
 *	@param[in]	无
 *	@param[out]	o_dirX				X方向向量
 *	@param[out]	o_dirY				Y方向向量
 *	@param[out]	o_dirZ				Z方向向量
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Ellipse::GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ)
{
	o_dirX = m_dirX;
	o_dirY = m_dirY;
	o_dirZ = m_dirZ;
}

/************************************************************************//**
 *	@brief		设置弧线向量
 *	@param[in]	i_dirX				X方向向量
 *	@param[in]	i_dirY				Y方向向量
 *	@param[in]	i_dirZ				Z方向向量
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Ellipse::SetXYZDir(const STK_DIR32& i_dirX, const STK_DIR32& i_dirY, const STK_DIR32& i_dirZ)
{
	m_dirX = i_dirX;
	m_dirY = i_dirY;
	m_dirZ = i_dirZ;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取弧线半径
 *	@param[in]	无
 *	@param[out]	o_fMajorR			长半径
 *	@param[out]	o_fMinorR			短半径
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Ellipse::GetRadius(STK_FLOAT32& o_fMajorR, STK_FLOAT32& o_fMinorR)
{
	o_fMajorR = m_fMajorRadius;
	o_fMinorR = m_fMinorRadius;
}

/************************************************************************//**
 *	@brief		设置弧线半径
 *	@param[in]	i_fMajorR			长半径
 *	@param[in]	i_fMinorR			短半径
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Ellipse::SetRadius(STK_FLOAT32 i_fMajorR, STK_FLOAT32 i_fMinorR)
{
	m_fMajorRadius = i_fMajorR;
	m_fMinorRadius = i_fMinorR;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置圆弧方向
 *	@param[in]	i_dirNormal	圆弧方向
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
void Stk_Ellipse::SetNormal(const STK_DIR32& i_dirNormal)
{
	m_dirNormal = i_dirNormal;
}

/************************************************************************//**
 *	@brief		获取圆弧方向
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dirNormal	圆弧方向
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32 Stk_Ellipse::GetNormal()
{
	return m_dirNormal;
}

/************************************************************************//**
 *	@brief		设置原点方向
 *	@param[in]	i_dirOrigin	原点方向
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Ellipse::SetOriginDir(const STK_DIR32& i_dirOrigin)
{
	m_dirOrigin = i_dirOrigin;
}

/************************************************************************//**
 *	@brief		获取原点方向
 *	@param[in]	无	
 *	@param[out]	无
 *	@retval		m_dirOrigin	原点方向
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32 Stk_Ellipse::GetOriginDir()
{
	return m_dirOrigin;
}

/************************************************************************//**
 *	@brief		获取弧线起始终止范围
 *	@param[in]	无
 *	@param[out]	o_fUMin				起始范围
 *	@param[out]	o_fUMax				终止范围
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Ellipse::GetDomain(STK_FLOAT32& o_fUMin, STK_FLOAT32& o_fUMax)
{
	o_fUMin = m_fUMin;
	o_fUMax = m_fUMax;
}

/************************************************************************//**
 *	@brief		设置弧线起始终止范围
 *	@param[in]	i_fUMin				起始范围
 *	@param[in]	i_fUMax				终止范围
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Ellipse::SetDomain(STK_FLOAT32 i_fUMin, STK_FLOAT32 i_fUMax)
{
	m_fUMin = i_fUMin;
	m_fUMax = i_fUMax;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取弧线点坐标
 *	@param[in]	无
 *	@param[out]	o_pntStart			起点坐标
 *	@param[out]	o_pntEnd			终点坐标
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Ellipse::GetCoordinatePnt(STK_PNT32& o_pntStart, STK_PNT32& o_pntEnd)
{
	o_pntStart = m_pntStart;
	o_pntEnd = m_pntEnd;
}

/************************************************************************//**
 *	@brief		设置弧线点坐标
 *	@param[in]	i_pntStart			起点坐标
 *	@param[in]	i_pntEnd			终点坐标
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Ellipse::SetCoordiantePnt(const STK_PNT32& i_pntStart, const STK_PNT32& i_pntEnd)
{
	m_pntStart = i_pntStart;
	m_pntEnd = i_pntEnd;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取弧线点参数位置
 *	@param[in]	无
 *	@param[out]	o_fStartPar			起点参数位置
 *	@param[out]	o_fEndPar			终点参数位置
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Ellipse::GetParameter(STK_FLOAT32& o_fStartPar, STK_FLOAT32& o_fEndPar)
{
	o_fStartPar = m_fStartPar;
	o_fEndPar = m_fEndPar;
}

/************************************************************************//**
 *	@brief		设置弧线点参数位置
 *	@param[in]	i_fStartPar			起点参数位置
 *	@param[in]	i_fEndPar			终点参数位置
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Ellipse::SetParameter(STK_FLOAT32 i_fStartPar, STK_FLOAT32 i_fEndPar)
{
	m_fStartPar = i_fStartPar;
	m_fEndPar = i_fEndPar;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		写入椭圆弧数据
 *	@param[in]	i_head			数据段头指针
 *	@param[in]	i_stkFileP		文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其它			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Ellipse::WriteEllipse(StkSegmentHead* i_head, Stk_File* i_stkFileP)
{
	STK_STATUS		ret = STK_SUCCESS;
#ifdef _WINDOWS_
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif

	if(i_head == NULL || i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	StkEllipseRec4* EllipseRecP4 = NULL;
	EllipseRecP4 = (StkEllipseRec4 *)calloc(1, sizeof(StkEllipseRec4));
	if (EllipseRecP4 == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *	属性填充
	 */
	EllipseRecP4->Control.type = REC_TYPE_ELLIPSE;
	EllipseRecP4->Control.length = sizeof(StkEllipseRec4) - sizeof(StkControl);
	EllipseRecP4->Control.version = _VER_ELLIPSE_RECORD /*4*/;

	EllipseRecP4->ID = GetID();
	EllipseRecP4->Color = GetColor();
	EllipseRecP4->Type = GetCurveType();
	EllipseRecP4->DefType = GetUsageType();			// add on 2013.10.9 by gaoqq
	EllipseRecP4->nuDisp = GetCurveDispStat();		// add by wangyuan in 2013 12 12
	EllipseRecP4->nuDispType = GetCurveDispType();	// add by wangyuan in 2013 12 12
	EllipseRecP4->CMin = m_fUMin;
	EllipseRecP4->CMax = m_fUMax;
	EllipseRecP4->Center = m_pntCenter;
	EllipseRecP4->XDir = m_dirX;
	EllipseRecP4->YDir = m_dirY;
	EllipseRecP4->ZDir = m_dirZ;
	EllipseRecP4->MajorRadius = m_fMajorRadius;
	EllipseRecP4->MinorRadius = m_fMinorRadius;
	EllipseRecP4->BeginPnt = m_pntStart;
	EllipseRecP4->EndPnt = m_pntEnd;
	EllipseRecP4->BeginPar = m_fStartPar;
	EllipseRecP4->EndPar = m_fEndPar;

	// 记录版本2
	EllipseRecP4->Normal = m_dirNormal;
	EllipseRecP4->OriginDir = m_dirOrigin;

	ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)EllipseRecP4);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T(" ■ERROR：Ellipse记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif

		return STK_ERROR;
	}

	if(EllipseRecP4 != NULL){
		free(EllipseRecP4);
		EllipseRecP4 = NULL;
	}
	return STK_SUCCESS;
}
/************************************************************************//**
 *	@brief		Stk_Line构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Line::Stk_Line(void)
{
	m_eCurveType = CURVE_TYPE_LINE;
	m_pntStart.X = 0.0f;
	m_pntStart.Y = 0.0f;
	m_pntStart.Z = 0.0f;
	m_pntEnd.X = 0.0f;
	m_pntEnd.Y = 0.0f;
	m_pntEnd.Z = 0.0f;
}

/************************************************************************//**
 *	@brief		Stk_Line构造函数
 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Line::Stk_Line(Stk_ProtoType* i_stkProtoTypeP)
{
	m_stkProtoP = i_stkProtoTypeP;
	m_eCurveType = CURVE_TYPE_LINE;
	m_pntStart.X = 0.0f;
	m_pntStart.Y = 0.0f;
	m_pntStart.Z = 0.0f;
	m_pntEnd.X = 0.0f;
	m_pntEnd.Y = 0.0f;
	m_pntEnd.Z = 0.0f;
}

/************************************************************************//**
 *	@brief		Stk_Line析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Line::~Stk_Line(void)
{

}

/************************************************************************//**
 *	@brief		设置起始点
 *	@param[in]	i_pntStart	起始点
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Line::SetStartPoint(const STK_PNT32& i_pntStart)
{
	m_pntStart = i_pntStart;
}

/************************************************************************//**
 *	@brief		设置终止点
 *	@param[in]	i_pntEnd	终止点
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Line::SetEndPoint(const STK_PNT32& i_pntEnd)
{
	m_pntEnd = i_pntEnd;
}

/************************************************************************//**
 *	@brief		获取起始点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntStart	起始点
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_PNT32 Stk_Line::GetStartPoint()
{
	return m_pntStart;
}

/************************************************************************//**
 *	@brief		获取终止点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntEnd	终止点
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_PNT32 Stk_Line::GetEndPoint()
{
	return m_pntEnd;
}

/*************************************************************************//**
 *	@brief		写入直线线数据
 *	@param[in]	i_head			数据段头指针
 *	@param[in]	i_stkFileP		文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其它			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Line::WriteLine(StkSegmentHead* i_head, Stk_File* i_stkFileP)
{
	STK_STATUS			ret = STK_SUCCESS;
	StkLineRec3			lineRec3 = {0};
#ifdef _WINDOWS_
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif
	if(i_head == NULL || i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	/*
	 *	属性填充
	 */
	lineRec3.Control.type = REC_TYPE_LINE;
	lineRec3.Control.length = sizeof(StkLineRec3) - sizeof(StkControl);
	lineRec3.Control.version = _VER_LINE_RECORD /*3*/;

	lineRec3.ID = GetID();
	lineRec3.Color = GetColor();
	lineRec3.Type = GetCurveType();
	lineRec3.DefType = GetUsageType();			//add on 2013.10.9 by gaoqq
	lineRec3.nuDisp = GetCurveDispStat();		//add by wangyuan in 2013 12 12
	lineRec3.nuDispType = GetCurveDispType();	//add by wangyuan in 2013 12 12
	lineRec3.StartPnt = m_pntStart;
	lineRec3.EndPnt = m_pntEnd;

	ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)&lineRec3);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Line记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif

		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		Stk_NurbsCurve构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_NurbsCurve::Stk_NurbsCurve(void)
{
	m_eCurveType = CURVE_TYPE_NURBSCURVE;
	m_fUMin = 0.0f;
	m_fUMax = 0.0f;
	m_nuDegree = 0;
}

/************************************************************************//**
 *	@brief		Stk_NurbsCurve构造函数
 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_NurbsCurve::Stk_NurbsCurve(Stk_ProtoType* i_stkProtoTypeP)
{
	m_stkProtoP = i_stkProtoTypeP;
	m_eCurveType = CURVE_TYPE_NURBSCURVE;
	m_fUMin = 0.0f;
	m_fUMax = 0.0f;
	m_nuDegree = 0;
}

/*************************************************************************//**
 *	@brief		Stk_NurbsCurve析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_NurbsCurve::~Stk_NurbsCurve(void)
{

}

/*************************************************************************//**
 *	@brief		获取起始和终止范围
 *	@param[in]	无
 *	@param[out]	o_fMin					起始范围
 *	@param[out]	o_fMax					终止范围
 *	@retval		STK_SUCCESS				获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_NurbsCurve::GetDomain(STK_FLOAT32& o_fMin, STK_FLOAT32& o_fMax)
{
	o_fMin = m_fUMin;
	o_fMax = m_fUMax;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		设置起始和终止范围
 *	@param[in]	i_fMin					起始范围
 *	@param[in]	i_fMax					终止范围
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_NurbsCurve::SetDomain(STK_FLOAT32 i_fMin, STK_FLOAT32 i_fMax)
{
	m_fUMin = i_fMin;
	m_fUMax = i_fMax;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取Degree
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_nuDegree				Degree值
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_NurbsCurve::GetDegree()
{
	return m_nuDegree;
}

/*************************************************************************//**
 *	@brief		设置Degree
 *	@param[in]	i_nuDegree				Degree参数
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_NurbsCurve::SetDegree(STK_UINT32 i_nuDegree)
{
	m_nuDegree = i_nuDegree;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取Knot的多样性
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vecKnotPlicity		Knot的多样性
 *	@note		无
 *	@attention	无
 ****************************************************************************/
vector<STK_UINT32> Stk_NurbsCurve::GetKnotMultiplicity()
{
	return m_vcKnotPlicity;
}

/*************************************************************************//**
 *	@brief		添加Knot的多样性
 *	@param[in]	i_nuKnotPlicity			Knot的多样性
 *	@param[out]	无
 *	@retval		STK_SUCCESS				添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_NurbsCurve::AddKnotMultiplicity(STK_UINT32 i_nuKnotPlicity)
{
	m_vcKnotPlicity.push_back(i_nuKnotPlicity);

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取Knot节点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vecKnots				Knots节点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
vector<STK_FLOAT32> Stk_NurbsCurve::GetKnots()
{
	return m_vcKnots;
}

/*************************************************************************//**
 *	@brief		添加Knot节点
 *	@param[in]	i_fKnot					Knot节点
 *	@param[out]	无
 *	@retval		STK_SUCCESS				添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_NurbsCurve::AddKnot(STK_FLOAT32 i_fKnot)
{
	m_vcKnots.push_back(i_fKnot);

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取控制点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vecPoints				Nurbs曲线的控制点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
vector<STK_PNT32> Stk_NurbsCurve::GetPoints()
{
	return m_vcPoints;
}

/*************************************************************************//**
 *	@brief		添加Nurbs曲线控制点
 *	@param[in]	i_pntCtrl				Nurbs曲线控制点
 *	@param[out]	无
 *	@retval		STK_SUCCESS				添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_NurbsCurve::AddPoint(STK_PNT32 i_pntCtrl)
{
	m_vcPoints.push_back(i_pntCtrl);

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取Nurbs曲线控制点的权重
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vecWeights				Nurbs曲线控制点的权重
 *	@note		无
 *	@attention	无
 ****************************************************************************/
vector<STK_FLOAT32> Stk_NurbsCurve::GetWeights()
{
	return m_vcWeights;
}

/*************************************************************************//**
 *	@brief		添加Nurbs曲线控制点的权重
 *	@param[in]	i_fWeight					Nurbs曲线控制点的权重
 *	@param[out]	无
 *	@retval		STK_SUCCESS					添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_NurbsCurve::AddWeight(STK_FLOAT32 i_fWeight)
{
	m_vcWeights.push_back(i_fWeight);

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		填写Nurbs曲线控制点
 *	@param[in]	i_Point					控制点
 *	@param[out]	o_fAtr					占位符位置
 *	@retval		无
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_NurbsCurve::WriteVertex(STK_FLOAT32* o_fAtr, STK_PNT32 i_Point)
{
	o_fAtr[0] = i_Point.X;
	o_fAtr[1] = i_Point.Y;
	o_fAtr[2] = i_Point.Z;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		将Nurbs曲线数据写入SVL文件
 *	@param[in]	i_head					记录头
 *	@param[in]	i_stkFileP				文件对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_NurbsCurve::WriteNurbsCurve(StkSegmentHead* i_head, Stk_File* i_stkFileP)
{
	StkNurbsCurveRec*	NurbsCurveRecP = NULL;
	INT					ret = 0;
	STK_UINT32			nuNumKnotPlicity = 0;
	STK_UINT32			nuNumKnot = 0;
	STK_UINT32			nuNumPoint = 0;
	STK_UINT32			nuNumWeight = 0;
#ifdef _WINDOWS_	
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif

	if (i_head == NULL || i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	NurbsCurveRecP = (StkNurbsCurveRec *)calloc(1,sizeof(StkNurbsCurveRec) - sizeof(STK_FLOAT32)
		+ m_vcKnotPlicity.size() * sizeof(STK_UINT32) + m_vcKnots.size() * sizeof(STK_FLOAT32)
		+ 3 * m_vcPoints.size() * sizeof(STK_FLOAT32) + m_vcWeights.size() * sizeof(STK_FLOAT32));
	if (NurbsCurveRecP == NULL)
	{
		return STK_ERROR_FILE_MEMORY;
	}
	nuNumKnotPlicity = m_vcKnotPlicity.size();
	nuNumKnot = m_vcKnots.size();
	nuNumPoint = m_vcPoints.size();
	nuNumWeight = m_vcWeights.size();

	/*
	 *	属性填充
	 */
	NurbsCurveRecP->Control.type = REC_TYPE_NURBSCURVE;
	NurbsCurveRecP->Control.length = sizeof(StkNurbsCurveRec) - sizeof(STK_FLOAT32) + m_vcKnotPlicity.size() * sizeof(STK_UINT32)
		+ m_vcKnots.size() * sizeof(STK_FLOAT32) + 3 * m_vcPoints.size() * sizeof(STK_FLOAT32)
		+ m_vcWeights.size() * sizeof(STK_FLOAT32) - sizeof(StkControl);
	NurbsCurveRecP->Control.version = _VER_NURBSCURVE_RECORD;

	NurbsCurveRecP->ID = GetID();
	NurbsCurveRecP->Color = GetColor();
	NurbsCurveRecP->nuType = GetCurveType();
	NurbsCurveRecP->nuDefType = GetUsageType();
	NurbsCurveRecP->nuDisp = GetCurveDispStat();
	NurbsCurveRecP->nuDispType = GetCurveDispType();
	NurbsCurveRecP->fUMin = m_fUMin;
	NurbsCurveRecP->fUMax = m_fUMax;
	NurbsCurveRecP->nuDegree = m_nuDegree;
	NurbsCurveRecP->nuNumKnotPlicity = nuNumKnotPlicity;
	NurbsCurveRecP->nuNumKnot = nuNumKnot;
	NurbsCurveRecP->nuNumPoint = nuNumPoint;
	NurbsCurveRecP->nuNumWeight = nuNumWeight;

	for (int ii = 0; ii < nuNumKnotPlicity; ii++)
	{
		NurbsCurveRecP->atr[ii] = m_vcKnotPlicity[ii];
	}
	for (int ii = 0; ii < nuNumKnot; ii++)
	{
		NurbsCurveRecP->atr[nuNumKnotPlicity * sizeof(STK_UINT32) + ii] = m_vcKnots[ii];
	}
	for (int ii = 0; ii < nuNumPoint; ii++)
	{
		WriteVertex(NurbsCurveRecP->atr + nuNumKnotPlicity * sizeof(STK_UINT32) + nuNumKnot * sizeof(STK_FLOAT32) + ii * 3, m_vcPoints[ii]);
	}
	for (int ii = 0; ii < nuNumWeight; ii++)
	{
		NurbsCurveRecP->atr[nuNumKnotPlicity * sizeof(STK_UINT32) + nuNumKnot * sizeof(STK_FLOAT32) + nuNumPoint * 3 * sizeof(STK_FLOAT32) + ii] = m_vcWeights[ii];
	}

	ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)NurbsCurveRecP);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1== stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Nurbs曲线记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	if(NurbsCurveRecP != NULL)
	{
		free(NurbsCurveRecP);
		NurbsCurveRecP = NULL;
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		Stk_Hyperbola构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Hyperbola::Stk_Hyperbola(void)
{
	m_eCurveType = CURVE_TYPE_HYPERBOLA;
	m_fUMin = 0.0f;
	m_fUMax = 0.0f;
	m_pntCenter.X = 0.0f;
	m_pntCenter.Y = 0.0f;
	m_pntCenter.Z = 0.0f;
	m_dirX.X = 0.0f;
	m_dirX.Y = 0.0f;
	m_dirX.Z = 0.0f;
	m_dirY.X = 0.0f;
	m_dirY.Y = 0.0f;
	m_dirY.Z = 0.0f;
	m_dirZ.X = 0.0f;
	m_dirZ.Y = 0.0f;
	m_dirZ.Z = 0.0f;
	m_fSemiAxis = 0.0f;
	m_fSemiImgAxis = 0.0f;
	m_pntStart.X = 0.0f;
	m_pntStart.Y = 0.0f;
	m_pntStart.Z = 0.0f;
	m_pntEnd.X = 0.0f;
	m_pntEnd.Y = 0.0f;
	m_pntEnd.Z = 0.0f;
}

/************************************************************************//**
 *	@brief		Stk_Hyperbola构造函数
 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Hyperbola::Stk_Hyperbola(Stk_ProtoType* i_stkProtoTypeP)
{
	m_stkProtoP = i_stkProtoTypeP;
	m_eCurveType = CURVE_TYPE_HYPERBOLA;
	m_fUMin = 0.0f;
	m_fUMax = 0.0f;
	m_pntCenter.X = 0.0f;
	m_pntCenter.Y = 0.0f;
	m_pntCenter.Z = 0.0f;
	m_dirX.X = 0.0f;
	m_dirX.Y = 0.0f;
	m_dirX.Z = 0.0f;
	m_dirY.X = 0.0f;
	m_dirY.Y = 0.0f;
	m_dirY.Z = 0.0f;
	m_dirZ.X = 0.0f;
	m_dirZ.Y = 0.0f;
	m_dirZ.Z = 0.0f;
	m_fSemiAxis = 0.0f;
	m_fSemiImgAxis = 0.0f;
	m_pntStart.X = 0.0f;
	m_pntStart.Y = 0.0f;
	m_pntStart.Z = 0.0f;
	m_pntEnd.X = 0.0f;
	m_pntEnd.Y = 0.0f;
	m_pntEnd.Z = 0.0f;
}

/*************************************************************************//**
 *	@brief		Stk_Hyperbola析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Hyperbola::~Stk_Hyperbola(void)
{

}

/*************************************************************************//**
 *	@brief		获取起始和终止范围
 *	@param[in]	无
 *	@param[out]	o_fMin					起始范围
 *	@param[out]	o_fMax					终止范围
 *	@retval		STK_SUCCESS				获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Hyperbola::GetDomain(STK_FLOAT32& o_fMin, STK_FLOAT32& o_fMax)
{
	o_fMin = m_fUMin;
	o_fMax = m_fUMax;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		设置起始和终止范围
 *	@param[in]	i_fMin					起始范围
 *	@param[in]	i_fMax					终止范围
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Hyperbola::SetDomain(STK_FLOAT32 i_fMin, STK_FLOAT32 i_fMax)
{
	m_fUMin = i_fMin;
	m_fUMax = i_fMax;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取双曲线的中心点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntCenter				双曲线中心点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Hyperbola::GetCenterPoint()
{
	return m_pntCenter;
}

/*************************************************************************//**
 *	@brief		设置双曲线的中心点
 *	@param[in]	i_pntCenter				双曲线的中心点
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Hyperbola::SetCenterPoint(STK_PNT32& i_pntCenter)
{
	m_pntCenter = i_pntCenter;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取双曲线的XYZ方向向量
 *	@param[in]	无
 *	@param[out]	o_dirX					X方向向量
 *	@param[out]	o_dirY					Y方向向量
 *	@param[out]	o_dirZ					Z方向向量
 *	@retval		STK_SUCCESS				获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Hyperbola::GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ)
{
	o_dirX = m_dirX;
	o_dirY = m_dirY;
	o_dirZ = m_dirZ;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		设置双曲线的XYZ方向向量
 *	@param[in]	i_dirX					X方向向量
 *	@param[in]	i_dirY					Y方向向量
 *	@param[in]	i_dirZ					Z方向向量
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Hyperbola::SetXYZDir(STK_DIR32& i_dirX, STK_DIR32& i_dirY, STK_DIR32& i_dirZ)
{
	m_dirX = i_dirX;
	m_dirY = i_dirY;
	m_dirZ = i_dirZ;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取双曲线的半轴
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fSemiAxis				获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Hyperbola::GetSemiAxis()
{
	return m_fSemiAxis;
}

/*************************************************************************//**
 *	@brief		设置双曲线的半轴
 *	@param[in]	i_fSemiAxis				双曲线的半轴
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Hyperbola::SetSemiAxis(STK_FLOAT32 i_fSemiAxis)
{
	m_fSemiAxis = i_fSemiAxis;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取双曲线的SemiImgAxis
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fSemiAxis				获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Hyperbola::GetSemiImgAxis()
{
	return m_fSemiImgAxis;
}

/*************************************************************************//**
 *	@brief		设置双曲线的SemiImgAxis
 *	@param[in]	i_fSemiImgAxis			双曲线的SemiImgAxis
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Hyperbola::SetSemiImgAxis(STK_FLOAT32 i_fSemiImgAxis)
{
	m_fSemiImgAxis = i_fSemiImgAxis;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取起点坐标
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntStart				起点坐标
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Hyperbola::GetStartPoint()
{
	return m_pntStart;
}

/*************************************************************************//**
 *	@brief		设置起点坐标
 *	@param[in]	i_pntStart				起点坐标
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Hyperbola::SetStartPoint(STK_PNT32 i_pntStart)
{
	m_pntStart = i_pntStart;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取终点坐标
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntEnd				终点坐标
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Hyperbola::GetEndPoint()
{
	return m_pntEnd;
}

/*************************************************************************//**
 *	@brief		设置终点坐标
 *	@param[in]	i_pntEnd				终点坐标
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Hyperbola::SetEndPoint(STK_PNT32 i_pntEnd)
{
	m_pntEnd = i_pntEnd;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		将双曲线数据写入SVL文件
 *	@param[in]	i_head					记录头
 *	@param[in]	i_stkFileP				文件指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Hyperbola::WriteHyperbola(StkSegmentHead* i_head, Stk_File* i_stkFileP)
{
	StkHyperbolaRec		HyperbolaRec = {0};
	INT					ret = 0;
#ifdef _WINDOWS_
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif

	if(i_head == NULL || i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	/*
	 *	属性填充
	 */
	HyperbolaRec.Control.type = REC_TYPE_HYPERBOLA;
	HyperbolaRec.Control.length = sizeof(StkHyperbolaRec) - sizeof(StkControl);
	HyperbolaRec.Control.version = _VER_HYPERBOLA_RECORD;

	HyperbolaRec.ID = GetID();
	HyperbolaRec.Color = GetColor();
	HyperbolaRec.nuType = GetCurveType();
	HyperbolaRec.nuDefType = GetUsageType();
	HyperbolaRec.nuDisp = GetCurveDispStat();
	HyperbolaRec.nuDispType = GetCurveDispType();
	HyperbolaRec.fUMin = m_fUMin;
	HyperbolaRec.fUMax = m_fUMax;
	HyperbolaRec.pntCenter = m_pntCenter;
	HyperbolaRec.dirX = m_dirX;
	HyperbolaRec.dirY = m_dirY;
	HyperbolaRec.dirZ = m_dirZ;
	HyperbolaRec.fSemiAxis = m_fSemiAxis;
	HyperbolaRec.fSemiImgAxis = m_fSemiImgAxis;
	HyperbolaRec.pntStart = m_pntStart;
	HyperbolaRec.pntEnd = m_pntEnd;

	ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)&HyperbolaRec);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1== stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：双曲线数据记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		Stk_Parabola构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Parabola::Stk_Parabola(void)
{
	m_eCurveType = CURVE_TYPE_PARABOLA;
	m_fUMin = 0.0f;
	m_fUMax = 0.0f;
	m_pntStart.X = 0.0f;
	m_pntStart.Y = 0.0f;
	m_pntStart.Z = 0.0f;
	m_pntEnd.X = 0.0f;
	m_pntEnd.Y = 0.0f;
	m_pntEnd.Z = 0.0f;
	m_pntCenter.X = 0.0f;
	m_pntCenter.Y = 0.0f;
	m_pntCenter.Z = 0.0f;
	m_dirX.X = 0.0f;
	m_dirX.Y = 0.0f;
	m_dirX.Z = 0.0f;
	m_dirY.X = 0.0f;
	m_dirY.Y = 0.0f;
	m_dirY.Z = 0.0f;
	m_dirZ.X = 0.0f;
	m_dirZ.Y = 0.0f;
	m_dirZ.Z = 0.0f;
	m_fFocalDis = 0.0f;
}

/************************************************************************//**
 *	@brief		Stk_Parabola构造函数
 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Parabola::Stk_Parabola(Stk_ProtoType* i_stkProtoTypeP)
{
	m_stkProtoP = i_stkProtoTypeP;
	m_eCurveType = CURVE_TYPE_PARABOLA;
	m_fUMin = 0.0f;
	m_fUMax = 0.0f;
	m_pntStart.X = 0.0f;
	m_pntStart.Y = 0.0f;
	m_pntStart.Z = 0.0f;
	m_pntEnd.X = 0.0f;
	m_pntEnd.Y = 0.0f;
	m_pntEnd.Z = 0.0f;
	m_pntCenter.X = 0.0f;
	m_pntCenter.Y = 0.0f;
	m_pntCenter.Z = 0.0f;
	m_dirX.X = 0.0f;
	m_dirX.Y = 0.0f;
	m_dirX.Z = 0.0f;
	m_dirY.X = 0.0f;
	m_dirY.Y = 0.0f;
	m_dirY.Z = 0.0f;
	m_dirZ.X = 0.0f;
	m_dirZ.Y = 0.0f;
	m_dirZ.Z = 0.0f;
	m_fFocalDis = 0.0f;
}

/*************************************************************************//**
 *	@brief		Stk_Parabola析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Parabola::~Stk_Parabola(void)
{

}

/*************************************************************************//**
 *	@brief		获取起始和终止范围
 *	@param[in]	无
 *	@param[out]	o_fMin					起始范围
 *	@param[out]	o_fMax					终止范围
 *	@retval		STK_SUCCESS				获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Parabola::GetDomain(STK_FLOAT32& o_fMin, STK_FLOAT32& o_fMax)
{
	o_fMin = m_fUMin;
	o_fMax = m_fUMax;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		设置起始和终止范围
 *	@param[in]	i_fMin					起始范围
 *	@param[in]	i_fMax					终止范围
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Parabola::SetDomain(STK_FLOAT32 i_fMin, STK_FLOAT32 i_fMax)
{
	m_fUMin = i_fMin;
	m_fUMax = i_fMax;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取起点坐标
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntStart				起点坐标
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Parabola::GetStartPoint()
{
	return m_pntStart;
}

/*************************************************************************//**
 *	@brief		设置起点坐标
 *	@param[in]	i_pntStart				起点坐标
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Parabola::SetStartPoint(STK_PNT32 i_pntStart)
{
	m_pntStart = i_pntStart;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取终点坐标
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntEnd				终点坐标
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Parabola::GetEndPoint()
{
	return m_pntEnd;
}

/*************************************************************************//**
 *	@brief		设置终点坐标
 *	@param[in]	i_pntEnd				终点坐标
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Parabola::SetEndPoint(STK_PNT32 i_pntEnd)
{
	m_pntEnd = i_pntEnd;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取中心点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntCenter				中心点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Parabola::GetCenterPoint()
{
	return m_pntCenter;
}

/*************************************************************************//**
 *	@brief		设置中心点
 *	@param[in]	i_pntCenter				中心点
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Parabola::SetCenterPoint(STK_PNT32& i_pntCenter)
{
	m_pntCenter = i_pntCenter;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取抛物线的XYZ方向向量
 *	@param[in]	无
 *	@param[out]	o_dirX					X方向向量
 *	@param[out]	o_dirY					Y方向向量
 *	@param[out]	o_dirZ					Z方向向量
 *	@retval		STK_SUCCESS				获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Parabola::GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ)
{
	o_dirX = m_dirX;
	o_dirY = m_dirY;
	o_dirZ = m_dirZ;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		设置抛物线的XYZ方向向量
 *	@param[in]	i_dirX					X方向向量
 *	@param[in]	i_dirY					Y方向向量
 *	@param[in]	i_dirZ					Z方向向量
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Parabola::SetXYZDir(STK_DIR32& i_dirX, STK_DIR32& i_dirY, STK_DIR32& i_dirZ)
{
	m_dirX = i_dirX;
	m_dirY = i_dirY;
	m_dirZ = i_dirZ;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取Focal距离
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fFocalDis				Focal距离
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Parabola::GetFocalDistance()
{
	return m_fFocalDis;
}

/*************************************************************************//**
 *	@brief		设置Focal距离
 *	@param[in]	i_fDistance				Focal距离
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Parabola::SetFocalDistance(STK_FLOAT32 i_fDistance)
{
	m_fFocalDis = i_fDistance;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		将抛物线数据写入SVL文件
 *	@param[in]	i_head					记录头
 *	@param[in]	i_stkFileP				文件指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Parabola::WriteParabola(StkSegmentHead* i_head, Stk_File* i_stkFileP)
{
	StkParabolaRec		ParabolaRec = {0};
	INT					ret = 0;
#ifdef _WINDOWS_	
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif

	if(i_head == NULL || i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	/*
	 *	属性填充
	 */
	ParabolaRec.Control.type = REC_TYPE_PARABOLA;
	ParabolaRec.Control.length = sizeof(StkParabolaRec) - sizeof(StkControl);
	ParabolaRec.Control.version = _VER_PARABOLA_RECORD;

	ParabolaRec.ID = GetID();
	ParabolaRec.Color = GetColor();
	ParabolaRec.nuType = GetCurveType();
	ParabolaRec.nuDefType = GetUsageType();
	ParabolaRec.nuDisp = GetCurveDispStat();
	ParabolaRec.nuDispType = GetCurveDispType();
	ParabolaRec.fUMin = m_fUMin;
	ParabolaRec.fUMax = m_fUMax;
	ParabolaRec.pntCenter = m_pntCenter;
	ParabolaRec.dirX = m_dirX;
	ParabolaRec.dirY = m_dirY;
	ParabolaRec.dirZ = m_dirZ;
	ParabolaRec.pntStart = m_pntStart;
	ParabolaRec.pntEnd = m_pntEnd;
	ParabolaRec.fFocalDistance = m_fFocalDis;

	ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)&ParabolaRec);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1== stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：抛物线记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif

		return STK_ERROR;
	}

	return STK_SUCCESS;
}

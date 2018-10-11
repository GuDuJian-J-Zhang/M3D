// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_LineSetEntity.cpp
 *
 *	@brief	线集类的实现
 *
 *	@par	历史:
 *		2014/03/03	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "StdAfx.h"
#include "Stk_LineSetEntity.h"
#include "Stk_Manager.h"
#include "Stk_Platform.h"
#include "Stk_DocumentManager.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif

/************************************************************************//**
 *	@brief		Stk_LineSet构造函数
 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_LineSet::Stk_LineSet(Stk_ProtoType* i_stkProtoTypeP)
{
	m_stkProtoP = i_stkProtoTypeP;
	m_ID = 0;
	m_wstrName = L"";
	m_bDispStat = FALSE;
	m_rgbaLineSet.Red = -1;
	m_rgbaLineSet.Green = -1;
	m_rgbaLineSet.Blue = -1;
	m_rgbaLineSet.Alpha = -1;
	m_eType = LINESET_TYPE_OTHERLINE;
}

/************************************************************************//**
 *	@brief		Stk_LineSet析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_LineSet::~Stk_LineSet()
{
	for (int ii = 0; ii < m_vcLines.size(); ii++)
	{
		if (m_vcLines[ii] != NULL)
		{
			delete m_vcLines[ii];
			m_vcLines[ii] = NULL;
		}
	}
	m_stkProtoP->GetProtoIDManager()->DelLineSetByID(m_ID);
}

/************************************************************************//**
 *	@brief		注册线集ID
 *	@param[in]	i_idLineSet		线集ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		注册成功
 *	@retval		STK_ERROR		注册失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_LineSet::RegisterID(STK_ID i_idLineSet)
{
	STK_STATUS ret=m_stkProtoP->GetProtoIDManager()->RegLineSetID(this, i_idLineSet);
	if (ret==STK_SUCCESS)
	{
		m_ID = i_idLineSet;
		return STK_SUCCESS;
	} 
	else
	{
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置线集ID
 *	@param[in]	i_ID			线集ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_LineSet::SetID(STK_UINT32 i_ID)
{
	m_ID = i_ID;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取线集ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_ID			线集ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_LineSet::GetID()
{
	return m_ID;
}

/************************************************************************//**
 *	@brief		设置线集名字
 *	@param[in]	i_wstrName		线集名字
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_LineSet::SetName(wstring i_wstrName)
{
	m_wstrName = i_wstrName;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取线集名字
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrName		线集名字
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_LineSet::GetName()
{
	return m_wstrName;
}

/************************************************************************//**
 *	@brief		获取ProtoTypeID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		STK_ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_LineSet::GetProtoTypeID()
{
	return m_stkProtoP->GetID();
}

/************************************************************************//**
 *	@brief		设置线集的类型
 *	@param[in]	i_eType			线集类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_LineSet::SetType(StkLineSetTypeEnum i_eType)
{
	m_eType = i_eType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取线集的类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		线集类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkLineSetTypeEnum Stk_LineSet::GetType()
{
	return m_eType;
}

/****************************************************************************
 *	@brief		设置线集显隐状态
 *	@param[in]	i_bDispStat		线集显隐状态
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_LineSet::SetDispStat(STK_BOOL i_bDispStat)
{
	m_bDispStat = i_bDispStat;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取线集显隐状态
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bDispStat		线集显隐状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_LineSet::GetDispStat()
{
	return m_bDispStat;
}

/************************************************************************//**
 *	@brief		设置线集颜色
 *	@param[in]	i_rgbaLineSet	线集颜色
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_LineSet::SetColor(STK_RGBA32 i_rgbaLineSet)
{
	m_rgbaLineSet = i_rgbaLineSet;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取线集颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_rgbaLineSet		线集颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_RGBA32 Stk_LineSet::GetColor()
{
	return m_rgbaLineSet;
}

/************************************************************************//**
 *	@brief		往线集中添加线数据
 *	@param[in]	i_stkLine			线数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_LineSet::AddLine(Stk_CurveEntity* i_stkLine)
{
	if (i_stkLine != NULL)
	{
		m_vcLines.push_back(i_stkLine);
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取线集中的线数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcLines			线集中的线数据
 *	@note		无
 *	@attention	无
 ****************************************************************************/

vector<Stk_CurveEntity*>& Stk_LineSet::GetAllLines()
{
	return m_vcLines;
}

/************************************************************************//**
 *	@brief		从SVL中读取线集数据
 *	@param[in]	i_FileP				SVL文件指针
 *	@param[in]	i_LineSetHeadP		线集段头
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_LineSet::ReadFile(Stk_File* i_FileP, StkSegmentHead* i_LineSetHeadP)
{
	StkLineSet*			LineSetRecP = NULL;
	StkLineSetRec2*		LineSetRecP2 = NULL;
	StkLineSetRec3*		LineSetRecP3 = NULL;
	StkPolyLineRec3*	PolyLineRecP3 = NULL;
	StkEllipseRec4*		EllipseRecP4 = NULL;
	StkLineRec3*		LineRecP3 = NULL;
	StkNurbsCurveRec*	NurbsCurveRecP = NULL;
	StkHyperbolaRec*	HyperbolaRecP = NULL;
	StkParabolaRec*		ParabolaRecP = NULL;

	Stk_PolyLine*		PolyLineP = NULL;
	Stk_Ellipse	*		EllipseP = NULL;
	Stk_Line*			LineP = NULL;
	Stk_NurbsCurve*		NurbsCurveP = NULL;
	Stk_Hyperbola*		HyperbolaP = NULL;
	Stk_Parabola*		ParabolaP = NULL;

	StkControl**		recordPP = NULL;
	STK_UINT32			ii, jj;

	STK_UINT32			nuNumKnotPlicity = 0;
	STK_UINT32			nuNumKnot = 0;
	STK_UINT32			nuNumPoint = 0;;
	STK_UINT32			nuNumWeight = 0;
	STK_PNT32			pntTmp;
	STK_STATUS			eState;

	StkLineSetRec4*		LineSetRecP4 = NULL;
	StkEntityNameRec*	stkEntityNameRecP = NULL;

	RegisterID(i_LineSetHeadP->id);	//线集的注册ID，放到ReadFile里面

	/*
	 * 读取辅助线段的数据
	 */
	recordPP = (StkControl **)calloc(i_LineSetHeadP->recnum, sizeof(StkControl *));
	if (i_LineSetHeadP->recnum != 0 && recordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}

	eState = i_FileP->ReadSegmentAllRecordData(i_LineSetHeadP, (StkControl **)recordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 *	读取辅助线的各记录
	 */
	for (ii = 0; ii < i_LineSetHeadP->recnum; ii++)
	{
		switch (recordPP[ii]->type)
		{
		case REC_TYPE_LINESET:
			if (recordPP[ii]->version == 1)
			{
				LineSetRecP = (StkLineSet*)recordPP[ii];
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(( STK_WCHAR *)
						LineSetRecP->chLineSetName);
#else
				m_wstrName = LineSetRecP->chLineSetName;
#endif

				m_bDispStat = LineSetRecP->nuDispStat;
				m_rgbaLineSet = LineSetRecP->rgbaLineSet;
			}
			else if (recordPP[ii]->version == 2)
			{
				LineSetRecP2 = (StkLineSetRec2*)recordPP[ii];
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(( STK_WCHAR *)
					LineSetRecP2->chLineSetName);
#else
				m_wstrName = LineSetRecP2->chLineSetName;
#endif

				m_bDispStat = LineSetRecP2->nuDispStat;
				m_rgbaLineSet = LineSetRecP2->rgbaLineSet;
				m_eType = (StkLineSetTypeEnum)LineSetRecP2->nuType;
			}
			else if (recordPP[ii]->version == 3)
			{
				LineSetRecP3 = (StkLineSetRec3*)recordPP[ii];
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(( STK_WCHAR *)
					LineSetRecP3->chLineSetName);
#else
				m_wstrName = LineSetRecP3->chLineSetName;
#endif

				m_bDispStat = LineSetRecP3->nuDispStat;
				m_rgbaLineSet = LineSetRecP3->rgbaLineSet;
				m_eType = (StkLineSetTypeEnum)LineSetRecP3->nuType;
			}
			else if (recordPP[ii]->version == 4)
			{
				LineSetRecP4 = (StkLineSetRec4*)recordPP[ii];
				m_bDispStat = LineSetRecP4->nuDispStat;
				m_rgbaLineSet = LineSetRecP4->rgbaLineSet;
				m_eType = (StkLineSetTypeEnum)LineSetRecP4->nuType;
			}
			break;
		case REC_TYPE_ENTITY_NAME:
			if (recordPP[ii]->version == 1)
			{
				stkEntityNameRecP = (StkEntityNameRec*)recordPP[ii];
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(stkEntityNameRecP->atr); 
#else
				m_wstrName = stkEntityNameRecP->atr; 
#endif
			}
			break;
		case REC_TYPE_POLYLINE:
			if (recordPP[ii]->version == 3)
			{
				PolyLineRecP3 = (StkPolyLineRec3 *)recordPP[ii];
				PolyLineP = new Stk_PolyLine();
				/*
				 *	填充PolyLine类
				 */
				//Curve基类
				PolyLineP->SetID(PolyLineRecP3->ID);	//仅用于单独线条时
				PolyLineP->SetColor(PolyLineRecP3->Color);
				PolyLineP->SetUsageType((StkCurveUsageEnum)PolyLineRecP3->DefType);
				PolyLineP->SetCurveDispStat(PolyLineRecP3->nuDisp);
				PolyLineP->SetCurveDispType((StkCurveDispTypeEnum)PolyLineRecP3->nuDispType);

				vector<STK_PNT32> LinearPoints;
				STK_PNT32 TmpPoint;
				for (jj = 0; jj < PolyLineRecP3->NumPoints; jj++)
				{
					TmpPoint.X = PolyLineRecP3->atr[jj*3 + 0];
					TmpPoint.Y = PolyLineRecP3->atr[jj*3 + 1];
					TmpPoint.Z = PolyLineRecP3->atr[jj*3 + 2];
					LinearPoints.push_back(TmpPoint);
				}
				PolyLineP->SetPoints(LinearPoints);

				m_vcLines.push_back((Stk_CurveEntity *)PolyLineP);
			}
			break;
		case REC_TYPE_ELLIPSE:
			if (recordPP[ii]->version == 4)
			{
				EllipseRecP4 = (StkEllipseRec4 *)recordPP[ii];
				EllipseP = new Stk_Ellipse();
				/*
				 *	填充Ellipse类
				 */
				//Curve基类
				EllipseP->SetID(EllipseRecP4->ID);	//仅用于单独线条时
				EllipseP->SetColor(EllipseRecP4->Color);
				EllipseP->SetDomain(EllipseRecP4->CMin, EllipseRecP4->CMax);
				EllipseP->SetCenterPoint(EllipseRecP4->Center);
				EllipseP->SetXYZDir(EllipseRecP4->XDir, EllipseRecP4->YDir, EllipseRecP4->ZDir);
				EllipseP->SetRadius(EllipseRecP4->MajorRadius, EllipseRecP4->MinorRadius);
				EllipseP->SetCoordiantePnt(EllipseRecP4->BeginPnt, EllipseRecP4->EndPnt);
				EllipseP->SetParameter(EllipseRecP4->BeginPar, EllipseRecP4->EndPar);
				//版本2
				EllipseP->SetNormal(EllipseRecP4->Normal);
				EllipseP->SetOriginDir(EllipseRecP4->OriginDir);
				//版本3
				EllipseP->SetUsageType((StkCurveUsageEnum)EllipseRecP4->DefType);
				//版本4
				EllipseP->SetCurveDispStat(EllipseRecP4->nuDisp);
				EllipseP->SetCurveDispType((StkCurveDispTypeEnum)EllipseRecP4->nuDispType);

				m_vcLines.push_back((Stk_CurveEntity *)EllipseP);
			}
			break;
		case REC_TYPE_LINE:
			if (recordPP[ii]->version == 3)
			{
				LineRecP3 = (StkLineRec3 *)recordPP[ii];
				LineP = new Stk_Line();

				LineP->SetID( LineRecP3->ID);
				LineP->SetColor( LineRecP3->Color);
				LineP->SetStartPoint( LineRecP3->StartPnt);
				LineP->SetEndPoint( LineRecP3->EndPnt);
				// 版本2
				LineP->SetUsageType((StkCurveUsageEnum)LineRecP3->DefType);
				//版本3
				LineP->SetCurveDispStat(LineRecP3->nuDisp);
				LineP->SetCurveDispType((StkCurveDispTypeEnum)LineRecP3->nuDispType);

				m_vcLines.push_back((Stk_CurveEntity *)LineP);
			}
			break;
		case REC_TYPE_NURBSCURVE:
			if (recordPP[ii]->version == 1)
			{
				NurbsCurveRecP = (StkNurbsCurveRec *)recordPP[ii];
				NurbsCurveP = new Stk_NurbsCurve();

				NurbsCurveP->SetID(NurbsCurveRecP->ID);
				NurbsCurveP->SetColor(NurbsCurveRecP->Color);
				NurbsCurveP->SetUsageType((StkCurveUsageEnum)NurbsCurveRecP->nuDefType);
				NurbsCurveP->SetCurveDispStat(NurbsCurveRecP->nuDisp);
				NurbsCurveP->SetCurveDispType((StkCurveDispTypeEnum)NurbsCurveRecP->nuDispType);
				NurbsCurveP->SetDomain(NurbsCurveRecP->fUMin, NurbsCurveRecP->fUMax);
				NurbsCurveP->SetDegree(NurbsCurveRecP->nuDegree);

				nuNumKnotPlicity = NurbsCurveRecP->nuNumKnotPlicity;
				nuNumKnot = NurbsCurveRecP->nuNumKnot;
				nuNumPoint = NurbsCurveRecP->nuNumPoint;
				nuNumWeight = NurbsCurveRecP->nuNumWeight;

				for (jj = 0; jj < nuNumKnotPlicity; jj++)
				{
					NurbsCurveP->AddKnotMultiplicity(NurbsCurveRecP->atr[jj]);
				}

				for (jj = 0; jj < nuNumKnot; jj++)
				{
					NurbsCurveP->AddKnot(NurbsCurveRecP->atr[nuNumKnotPlicity * sizeof(STK_UINT32) + jj]);
				}

				for (jj = 0; jj < nuNumPoint; jj++)
				{
					pntTmp.X = NurbsCurveRecP->atr[nuNumKnotPlicity * sizeof(STK_UINT32) + nuNumKnot * sizeof(STK_FLOAT32) + jj * 3 + 0];
					pntTmp.X = NurbsCurveRecP->atr[nuNumKnotPlicity * sizeof(STK_UINT32) + nuNumKnot * sizeof(STK_FLOAT32) + jj * 3 + 1];
					pntTmp.X = NurbsCurveRecP->atr[nuNumKnotPlicity * sizeof(STK_UINT32) + nuNumKnot * sizeof(STK_FLOAT32) + jj * 3 + 2];
					NurbsCurveP->AddPoint(pntTmp);
				}

				for (jj = 0; jj < nuNumWeight; jj++)
				{
					NurbsCurveP->AddWeight(NurbsCurveRecP->atr[nuNumKnotPlicity * sizeof(STK_UINT32) + nuNumKnot * sizeof(STK_FLOAT32) + nuNumPoint * 3 * sizeof(STK_FLOAT32) + jj]);
				}

				m_vcLines.push_back((Stk_CurveEntity *)NurbsCurveP);
			}
			break;
		case REC_TYPE_HYPERBOLA:
			if (recordPP[ii]->version == 1)
			{
				HyperbolaRecP = (StkHyperbolaRec *)recordPP[ii];
				HyperbolaP = new Stk_Hyperbola();

				HyperbolaP->SetID(HyperbolaRecP->ID);
				HyperbolaP->SetColor(HyperbolaRecP->Color);
				HyperbolaP->SetUsageType((StkCurveUsageEnum)HyperbolaRecP->nuDefType);
				HyperbolaP->SetCurveDispStat(HyperbolaRecP->nuDisp);
				HyperbolaP->SetCurveDispType((StkCurveDispTypeEnum)HyperbolaRecP->nuDispType);
				HyperbolaP->SetDomain(HyperbolaRecP->fUMin, HyperbolaRecP->fUMax);
				HyperbolaP->SetCenterPoint(HyperbolaRecP->pntCenter);
				HyperbolaP->SetStartPoint(HyperbolaRecP->pntStart);
				HyperbolaP->SetEndPoint(HyperbolaRecP->pntEnd);

				m_vcLines.push_back((Stk_CurveEntity *)HyperbolaP);
			}
			break;
		case REC_TYPE_PARABOLA:
			if (recordPP[ii]->version == 1)
			{
				ParabolaRecP = (StkParabolaRec *)recordPP[ii];
				ParabolaP = new Stk_Parabola();

				ParabolaP->SetID(ParabolaRecP->ID);
				ParabolaP->SetColor(ParabolaRecP->Color);
				ParabolaP->SetUsageType((StkCurveUsageEnum)ParabolaRecP->nuDefType);
				ParabolaP->SetCurveDispStat(ParabolaRecP->nuDisp);
				ParabolaP->SetCurveDispType((StkCurveDispTypeEnum)ParabolaRecP->nuDispType);
				ParabolaP->SetDomain(ParabolaRecP->fUMin, ParabolaRecP->fUMax);
				ParabolaP->SetStartPoint(ParabolaRecP->pntStart);
				ParabolaP->SetEndPoint(ParabolaRecP->pntEnd);
				ParabolaP->SetXYZDir(ParabolaRecP->dirX, ParabolaRecP->dirY, ParabolaRecP->dirZ);
				ParabolaP->SetFocalDistance(ParabolaRecP->fFocalDistance);

				m_vcLines.push_back((Stk_CurveEntity *)ParabolaP);
			}
			break;
		default:
			break;
		}
	}
	i_FileP->ReadSegmentFinishData(i_LineSetHeadP,(StkControl **)recordPP);// 释放内存
	if (recordPP != NULL)
	{
		free(recordPP);
		recordPP = NULL;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		往SVL文件填写线集数据
 *	@param[in]	i_stkFileP		SVL文件指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		填写成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_LineSet::WriteLineSet(Stk_File *i_stkFileP)
{
	StkSegmentHead		LineSetHead ={0};
	StkLineSetRec4		LineSetRec4 = {0};
	StkEntityNameRec*	stkEntityNameRecP = NULL;
	STK_UINT32			nuEntityNameSize = 0;
	INT					ret = 0;
	STK_UINT32			nuNumLine;
	STK_UINT32			nuCurveType;
	STK_UINT32			ii;

#ifdef _WINDOWS_
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif

	if(i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	nuNumLine = (STK_UINT32)m_vcLines.size();
	if(nuNumLine<=0)
	{
		return STK_SUCCESS;
	}
#ifdef _WINDOWS_
	if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bBody)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("【LineSet段】输出 LineSet ID：%d"), m_ID);
		stkSVLLogFile->Log(strConvertLog);
	}
#endif

	// [第1步] 输出段头记录
	LineSetHead.id = m_ID;
	LineSetHead.kind = SEG_TYPE_LINESET;
	ret = i_stkFileP->WriteSegmentHeadData(&LineSetHead);
	if (ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bBody)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：LineSet段-头记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif

		return STK_ERROR;
	}

	// [第2步] 输出线集记录
	LineSetRec4.Control.type = REC_TYPE_LINESET;
	LineSetRec4.Control.length = sizeof(StkLineSetRec4)-sizeof(StkControl);
	LineSetRec4.Control.version = _VER_LINESET_RECORD/*4*/;
	LineSetRec4.nuNumLine = nuNumLine;
	LineSetRec4.nuDispStat = m_bDispStat;
	LineSetRec4.rgbaLineSet = m_rgbaLineSet;
	LineSetRec4.nuType = m_eType;
	LineSetRec4.dmy1 = 0;
	LineSetRec4.dmy2 = 0;

	ret = i_stkFileP->WriteSegmentRecordData(&LineSetHead, (StkControl *)&LineSetRec4);

	// [第3步] 输出线集名字记录
	nuEntityNameSize = m_wstrName.size() + 1;
	stkEntityNameRecP = (StkEntityNameRec*)calloc(1, sizeof(StkEntityNameRec) - sizeof(STK_WCHAR) + nuEntityNameSize*sizeof(STK_WCHAR));
	if(stkEntityNameRecP == NULL)
	{
		return STK_ERROR;
	}
	stkEntityNameRecP->Control.type = REC_TYPE_ENTITY_NAME;
	stkEntityNameRecP->Control.length = sizeof(StkEntityNameRec) - sizeof(STK_WCHAR) + nuEntityNameSize*sizeof(STK_WCHAR) - sizeof(StkControl);
	stkEntityNameRecP->Control.version = _VER_ENTITY_NAME_RECORD;

	stkEntityNameRecP->nuNameSize = nuEntityNameSize;
#ifdef __MOBILE__
	memcpy(stkEntityNameRecP->atr,Stk_Platform::WStringToSTKWCHARStr(m_wstrName),(m_wstrName.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(stkEntityNameRecP->atr, nuEntityNameSize, (WCHAR*)m_wstrName.c_str());
#endif

	ret = i_stkFileP->WriteSegmentRecordData(&LineSetHead, (StkControl*)stkEntityNameRecP);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	if (stkEntityNameRecP != NULL)
	{
		free(stkEntityNameRecP);
		stkEntityNameRecP = NULL;
	}

	for (ii = 0; ii < nuNumLine; ii++)
	{
		nuCurveType = m_vcLines[ii]->GetCurveType();
		switch (nuCurveType)
		{
		case CURVE_TYPE_POLYLINE:
			((Stk_PolyLine *)m_vcLines[ii])->WritePolyline(&LineSetHead, i_stkFileP);
			break;
		case CURVE_TYPE_ELLIPSE:
			((Stk_Ellipse *)m_vcLines[ii])->WriteEllipse(&LineSetHead, i_stkFileP);
			break;
		case CURVE_TYPE_LINE:
			((Stk_Line *)m_vcLines[ii])->WriteLine(&LineSetHead, i_stkFileP);
			break;
		case CURVE_TYPE_NURBSCURVE:
			((Stk_NurbsCurve *)m_vcLines[ii])->WriteNurbsCurve(&LineSetHead, i_stkFileP);
			break;
		case CURVE_TYPE_HYPERBOLA:
			((Stk_Hyperbola *)m_vcLines[ii])->WriteHyperbola(&LineSetHead, i_stkFileP);
			break;
		case CURVE_TYPE_PARABOLA:
			((Stk_Parabola *)m_vcLines[ii])->WriteParabola(&LineSetHead, i_stkFileP);
			break;
		default:
			break;
		}
	}

	// [第3步] 完成辅助线段的输出
	ret = i_stkFileP->WriteSegmentFinishData(&LineSetHead);
	if (ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bBody)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：LineSet段Finish时失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif

		return STK_ERROR;
	}
	return STK_SUCCESS;
}
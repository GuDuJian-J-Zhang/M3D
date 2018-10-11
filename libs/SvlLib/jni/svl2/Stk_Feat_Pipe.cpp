// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Feat_Pipe.cpp
 *
 *	@brief	线缆特征类的实现
 *
 *	@par	历史:
 *		2014/05/29	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "StdAfx.h"
#include "Stk_Feat_Pipe.h"
#include "Stk_Manager.h"

/************************************************************************//**
 *	@brief		Stk_Pipe_Fiber_Segment构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Pipe_Fiber_Segment::Stk_Pipe_Fiber_Segment()
{
	m_pntStart.X = 0.0;
	m_pntStart.Y = 0.0;
	m_pntStart.Z = 0.0;
	m_pntEnd.X = 0.0;
	m_pntEnd.Y = 0.0;
	m_pntEnd.Z = 0.0;
	m_pntCenter.X = 0.0;
	m_pntCenter.Y = 0.0;
	m_pntCenter.Z = 0.0;
	m_stkCurveP = NULL;
}

/************************************************************************//**
 *	@brief		Stk_Pipe_Fiber_Segment拷贝构造函数
 *	@param[in]	i_stkPipeFiber		线缆的Fiber
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Pipe_Fiber_Segment::Stk_Pipe_Fiber_Segment(Stk_Pipe_Fiber_Segment& i_stkPipeFiber)
{
	Stk_CurveEntity*	stkCurveP = NULL;
	StkCurveTypeEnum	eCurveType = StkCurveTypeEnum::CURVE_TYPE_UNKNOWN;

	m_pntStart = i_stkPipeFiber.m_pntStart;
	m_pntEnd = i_stkPipeFiber.m_pntEnd;
	m_pntCenter = i_stkPipeFiber.m_pntCenter;
	stkCurveP = i_stkPipeFiber.GetCurve();
	eCurveType = stkCurveP->GetCurveType();
	switch(eCurveType)
	{
	case StkCurveTypeEnum::CURVE_TYPE_POLYLINE:
		m_stkCurveP = new Stk_PolyLine();
		memcpy(m_stkCurveP, stkCurveP, sizeof(*(Stk_PolyLine*)stkCurveP));
		break;
	case StkCurveTypeEnum::CURVE_TYPE_ELLIPSE:
		m_stkCurveP = new Stk_Ellipse();
		memcpy(m_stkCurveP, stkCurveP, sizeof(*(Stk_Ellipse*)stkCurveP));
		break;
	case StkCurveTypeEnum::CURVE_TYPE_LINE:
		m_stkCurveP = new Stk_Line();
		memcpy(m_stkCurveP, stkCurveP, sizeof(*(Stk_Line*)stkCurveP));
		break;
	case StkCurveTypeEnum::CURVE_TYPE_NURBSCURVE:
		m_stkCurveP = new Stk_NurbsCurve();
		memcpy(m_stkCurveP, stkCurveP, sizeof(*(Stk_NurbsCurve*)stkCurveP));
		break;
	case StkCurveTypeEnum::CURVE_TYPE_HYPERBOLA:
		m_stkCurveP = new Stk_Hyperbola();
		memcpy(m_stkCurveP, stkCurveP, sizeof(*(Stk_Hyperbola*)stkCurveP));
		break;
	case StkCurveTypeEnum::CURVE_TYPE_PARABOLA:
		m_stkCurveP = new Stk_Parabola();
		memcpy(m_stkCurveP, stkCurveP, sizeof(*(Stk_Parabola*)stkCurveP));
		break;
	default:
		break;
	}
}

/************************************************************************//**
 *	@brief		Stk_Pipe_Fiber_Segment析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Pipe_Fiber_Segment::~Stk_Pipe_Fiber_Segment()
{
	if (m_stkCurveP != NULL)
	{
		delete m_stkCurveP;
		m_stkCurveP = NULL;
	}
}

/************************************************************************//**
 *	@brief		Stk_Pipe_Fiber_Segment赋值构造函数
 *	@param[in]	i_stkPipeFiber		线缆的Fiber
 *	@retval		*this				当前线缆的Fiber
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Pipe_Fiber_Segment& Stk_Pipe_Fiber_Segment::operator = (Stk_Pipe_Fiber_Segment& i_stkPipeFiber)
{
	if (this == &i_stkPipeFiber)
	{
		return *this;
	}

	Stk_CurveEntity*	stkCurveP = NULL;
	StkCurveTypeEnum	eCurveType = StkCurveTypeEnum::CURVE_TYPE_UNKNOWN;

	m_pntStart = i_stkPipeFiber.m_pntStart;
	m_pntEnd = i_stkPipeFiber.m_pntEnd;
	m_pntCenter = i_stkPipeFiber.m_pntCenter;
	stkCurveP = i_stkPipeFiber.GetCurve();
	eCurveType = stkCurveP->GetCurveType();
	switch(eCurveType)
	{
	case StkCurveTypeEnum::CURVE_TYPE_POLYLINE:
		m_stkCurveP = new Stk_PolyLine();
		memcpy(m_stkCurveP, stkCurveP, sizeof(*(Stk_PolyLine*)stkCurveP));
		break;
	case StkCurveTypeEnum::CURVE_TYPE_ELLIPSE:
		m_stkCurveP = new Stk_Ellipse();
		memcpy(m_stkCurveP, stkCurveP, sizeof(*(Stk_Ellipse*)stkCurveP));
		break;
	case StkCurveTypeEnum::CURVE_TYPE_LINE:
		m_stkCurveP = new Stk_Line();
		memcpy(m_stkCurveP, stkCurveP, sizeof(*(Stk_Line*)stkCurveP));
		break;
	case StkCurveTypeEnum::CURVE_TYPE_NURBSCURVE:
		m_stkCurveP = new Stk_NurbsCurve();
		memcpy(m_stkCurveP, stkCurveP, sizeof(*(Stk_NurbsCurve*)stkCurveP));
		break;
	case StkCurveTypeEnum::CURVE_TYPE_HYPERBOLA:
		m_stkCurveP = new Stk_Hyperbola();
		memcpy(m_stkCurveP, stkCurveP, sizeof(*(Stk_Hyperbola*)stkCurveP));
		break;
	case StkCurveTypeEnum::CURVE_TYPE_PARABOLA:
		m_stkCurveP = new Stk_Parabola();
		memcpy(m_stkCurveP, stkCurveP, sizeof(*(Stk_Parabola*)stkCurveP));
		break;
	default:
		break;
	}

	return *this;
}

/************************************************************************//**
 *	@brief		设置起始点
 *	@param[in]	i_pntStart			起始点
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Pipe_Fiber_Segment::SetStartPoint(STK_PNT32& i_pntStart)
{
	m_pntStart = i_pntStart;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取起始点
 *	@param[in]	无
 *	@retval		m_pntStart			起始点
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_PNT32 Stk_Pipe_Fiber_Segment::GetStartPoint()
{
	return m_pntStart;
}

/************************************************************************//**
 *	@brief		设置终止点
 *	@param[in]	i_pntEnd			终止点
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Pipe_Fiber_Segment::SetEndPoint(STK_PNT32& i_pntEnd)
{
	m_pntEnd = i_pntEnd;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取终止点
 *	@param[in]	无
 *	@retval		m_pntEnd			终止点
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_PNT32 Stk_Pipe_Fiber_Segment::GetEndPoint()
{
	return m_pntEnd;
}

/************************************************************************//**
 *	@brief		设置中心点
 *	@param[in]	i_pntCenter			中心点
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Pipe_Fiber_Segment::SetCenterPoint(STK_PNT32& i_pntCenter)
{
	m_pntCenter = i_pntCenter;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取中心点
 *	@param[in]	无
 *	@retval		m_pntCenter			中心点
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_PNT32 Stk_Pipe_Fiber_Segment::GetCenterPoint()
{
	return m_pntCenter;
}

/************************************************************************//**
 *	@brief		设置Curve
 *	@param[in]	i_stkCurveP			Curve指针
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其他				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Pipe_Fiber_Segment::SetCurve(Stk_CurveEntity* i_stkCurveP)
{
	if (i_stkCurveP != NULL)
	{
		m_stkCurveP = i_stkCurveP;
	}
	else
	{
		return STK_ERROR;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Curve
 *	@param[in]	无
 *	@retval		m_stkCurveP			Curve指针
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_CurveEntity* Stk_Pipe_Fiber_Segment::GetCurve()
{
	return m_stkCurveP;
}

/************************************************************************//**
 *	@brief		读取线缆的Fiber
 *	@param[in]	i_FileP				文件指针
 *	@param[in]	i_CurveHeadP		Curve记录头
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其他				设置失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Pipe_Fiber_Segment::ReadPipeFiber(Stk_File* i_FileP, StkSegmentHead* i_CurveHeadP)
{
	StkControl**		RecordPP = NULL;
	StkPipeFiberPntRec*	PipeFiberPntRecP = NULL;
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

	STK_UINT32			nuNumKnotPlicity = 0;
	STK_UINT32			nuNumKnot = 0;
	STK_UINT32			nuNumPoint = 0;;
	STK_UINT32			nuNumWeight = 0;
	STK_PNT32			pntTmp;
	STK_UINT32			ii, jj;
	STK_STATUS			eState;

	/*
	 *	读取自定义属性段的数据
	 */
	RecordPP = (StkControl**)calloc(1, sizeof(StkControl *) * i_CurveHeadP->recnum);
	if (i_CurveHeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_CurveHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 *	读取线缆Fiber各记录
	 */
	for (ii = 0; ii < i_CurveHeadP->recnum; ii++)
	{
		switch(RecordPP[ii]->type)
		{
		case REC_TYPE_PIPEFIBER_PNT:
			if (RecordPP[ii]->version == 1)
			{
				PipeFiberPntRecP = (StkPipeFiberPntRec*)RecordPP[ii];
				m_pntStart = PipeFiberPntRecP->pntStart;
				m_pntEnd = PipeFiberPntRecP->pntEnd;
				m_pntCenter = PipeFiberPntRecP->pntCenter;
			}
			break;

		case REC_TYPE_POLYLINE:
			if (RecordPP[ii]->version == 3)
			{
				PolyLineRecP3 = (StkPolyLineRec3 *)RecordPP[ii];
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

				SetCurve((Stk_CurveEntity* )PolyLineP);
			}
			break;

		case REC_TYPE_ELLIPSE:
			if (RecordPP[ii]->version == 4)
			{
				EllipseRecP4 = (StkEllipseRec4 *)RecordPP[ii];
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
				
				SetCurve((Stk_CurveEntity* )EllipseP);
			}
			break;

		case REC_TYPE_LINE:
			if (RecordPP[ii]->version == 3)
			{
				LineRecP3 = (StkLineRec3 *)RecordPP[ii];
				LineP = new Stk_Line();

				LineP->SetID(LineRecP3->ID);
				LineP->SetColor(LineRecP3->Color);
				LineP->SetStartPoint(LineRecP3->StartPnt);
				LineP->SetEndPoint(LineRecP3->EndPnt);
				// 版本2
				LineP->SetUsageType((StkCurveUsageEnum)LineRecP3->DefType);
				//版本3
				LineP->SetCurveDispStat(LineRecP3->nuDisp);
				LineP->SetCurveDispType((StkCurveDispTypeEnum)LineRecP3->nuDispType);
			
				SetCurve((Stk_CurveEntity* )LineP);
			}
			break;

		case REC_TYPE_NURBSCURVE:
			if (RecordPP[ii]->version == 1)
			{
				NurbsCurveRecP = (StkNurbsCurveRec *)RecordPP[ii];
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
					NurbsCurveP->AddKnotMultiplicity(NurbsCurveRecP->atr[jj]); //FLOAT到UINT截断警告，已测试没有问题
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
			
				SetCurve((Stk_CurveEntity* )NurbsCurveP);
			}
			break;

		case REC_TYPE_HYPERBOLA:
			if (RecordPP[ii]->version == 1)
			{
				HyperbolaRecP = (StkHyperbolaRec *)RecordPP[ii];
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

				SetCurve((Stk_CurveEntity* )HyperbolaP);
			}
			break;

		case REC_TYPE_PARABOLA:
			if (RecordPP[ii]->version == 1)
			{
				ParabolaRecP = (StkParabolaRec *)RecordPP[ii];
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
			
				SetCurve((Stk_CurveEntity* )ParabolaP);
			}
			break;
		default:
			break;
		}
	}
	i_FileP->ReadSegmentFinishData(i_CurveHeadP,(StkControl **)RecordPP);// 释放内存
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		填写线缆的Fiber
 *	@param[in]	i_stkFileP			文件指针
 *	@param[in]	i_CurveHeadP		Curve记录头
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其他				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Pipe_Fiber_Segment::WritePipeFiber(Stk_File *i_stkFileP, StkSegmentHead* i_CurveHeadP)
{
	StkPipeFiberPntRec	PipeFiberPntRec = {0};
	INT					ret = 0;
	STK_UINT32			nuCurveType;

	if(i_stkFileP == NULL || i_CurveHeadP == NULL)
	{
		return STK_ERROR;
	}

	// [第1步] 输出Fiber中有关点的记录
	PipeFiberPntRec.Control.type = REC_TYPE_PIPEFIBER_PNT;
	PipeFiberPntRec.Control.length = sizeof(StkPipeFiberPntRec)-sizeof(StkControl);
	PipeFiberPntRec.Control.version = _VER_PIPEFIBER_PNT_RECORD;
	PipeFiberPntRec.pntStart = m_pntStart;
	PipeFiberPntRec.pntEnd = m_pntEnd;
	PipeFiberPntRec.pntCenter = m_pntCenter;

	ret = i_stkFileP->WriteSegmentRecordData(i_CurveHeadP, (StkControl *)&PipeFiberPntRec);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// [第2步] 输出Stk_Pipe_Fiber_Segment中的Curve记录
	if (m_stkCurveP != NULL)
	{
		nuCurveType = m_stkCurveP->GetCurveType();
		switch (nuCurveType)
		{
		case CURVE_TYPE_POLYLINE:
			((Stk_PolyLine *)m_stkCurveP)->WritePolyline(i_CurveHeadP, i_stkFileP);
			break;
		case CURVE_TYPE_ELLIPSE:
			((Stk_Ellipse *)m_stkCurveP)->WriteEllipse(i_CurveHeadP, i_stkFileP);
			break;
		case CURVE_TYPE_LINE:
			((Stk_Line *)m_stkCurveP)->WriteLine(i_CurveHeadP, i_stkFileP);
			break;
		case CURVE_TYPE_NURBSCURVE:
			((Stk_NurbsCurve *)m_stkCurveP)->WriteNurbsCurve(i_CurveHeadP, i_stkFileP);
			break;
		case CURVE_TYPE_HYPERBOLA:
			((Stk_Hyperbola *)m_stkCurveP)->WriteHyperbola(i_CurveHeadP, i_stkFileP);
			break;
		case CURVE_TYPE_PARABOLA:
			((Stk_Parabola *)m_stkCurveP)->WriteParabola(i_CurveHeadP, i_stkFileP);
			break;
		default:
			break;
		}
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		线缆特征构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Feat_Pipe::Stk_Feat_Pipe()
{
	m_ePipeType = StkPipeTypeEnum::PIPE_TYPE_UNKNOWN;
	m_eSectionType = StkPipeSectionTypeEnum::PIPESECTION_TYPE_UNKNOWN;
	m_fDiameter = 0;
	m_fTurnRadius = 0;
	m_fHeight = 0;
	m_fWidth = 0;
}

/************************************************************************//**
 *	@brief		线缆特征拷贝构造函数
 *	@param[in]	i_stkFeatPipe		线缆特征
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Feat_Pipe::Stk_Feat_Pipe(const Stk_Feat_Pipe& i_stkFeatPipe)
{
	Stk_Pipe_Fiber_Segment*			PipeFiberP = NULL;
	vector<Stk_Pipe_Fiber_Segment*> vcPipeFibers = i_stkFeatPipe.m_vcPipeFibers;

	m_ePipeType = i_stkFeatPipe.m_ePipeType;
	m_eSectionType = i_stkFeatPipe.m_eSectionType;

	for (int ii = 0; ii < vcPipeFibers.size(); ii++)
	{
		PipeFiberP = new Stk_Pipe_Fiber_Segment();
		memcpy(PipeFiberP, vcPipeFibers[ii], sizeof(*vcPipeFibers[ii]));
		m_vcPipeFibers.push_back(PipeFiberP);
	}
}

/************************************************************************//**
 *	@brief		线缆特征析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Feat_Pipe::~Stk_Feat_Pipe()
{
	for (int ii = 0; ii < m_vcPipeFibers.size(); ii++)
	{
		if (m_vcPipeFibers[ii] != NULL)
		{
			delete m_vcPipeFibers[ii];
			m_vcPipeFibers[ii] = NULL;
		}
	}
}

/************************************************************************//**
 *	@brief		线缆特征赋值构造函数
 *	@param[in]	i_stkFeatPipe		线缆特征
 *	@retval		*this				线缆特征
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Feat_Pipe& Stk_Feat_Pipe::operator = (const Stk_Feat_Pipe& i_stkFeatPipe)
{
	if (this == &i_stkFeatPipe)
	{
		return *this;
	}

	Stk_Pipe_Fiber_Segment*			PipeFiberP = NULL;
	vector<Stk_Pipe_Fiber_Segment*> vcPipeFibers = i_stkFeatPipe.m_vcPipeFibers;

	m_ePipeType = i_stkFeatPipe.m_ePipeType;
	m_eSectionType = i_stkFeatPipe.m_eSectionType;

	for (int ii = 0; ii < vcPipeFibers.size(); ii++)
	{
		PipeFiberP = new Stk_Pipe_Fiber_Segment();
		memcpy(PipeFiberP, vcPipeFibers[ii], sizeof(*vcPipeFibers[ii]));
		m_vcPipeFibers.push_back(PipeFiberP);
	}

	return *this;
}

/************************************************************************//**
 *	@brief		设置线缆类型
 *	@param[in]	i_ePipeType			线缆类型
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Feat_Pipe::SetPipeType(StkPipeTypeEnum i_ePipeType)
{
	m_ePipeType = i_ePipeType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取线缆类型
 *	@param[in]	无
 *	@retval		m_ePipeType			线缆类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkPipeTypeEnum Stk_Feat_Pipe::GetPipeType()
{
	return m_ePipeType;
}

/************************************************************************//**
 *	@brief		设置Section类型
 *	@param[in]	i_ePipeSectionType
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Feat_Pipe::SetSectionType(StkPipeSectionTypeEnum i_ePipeSectionType)
{
	m_eSectionType = i_ePipeSectionType;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Section类型
 *	@param[in]	无
 *	@retval		m_eSectionType			Section类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkPipeSectionTypeEnum Stk_Feat_Pipe::GetSectionType()
{
	return m_eSectionType;
}

/************************************************************************//**
 *	@brief		获取Fiber数量
 *	@param[in]	无
 *	@retval		Fiber数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_SIZE_T Stk_Feat_Pipe::GetPipeFiberNum()
{
	return m_vcPipeFibers.size();
}

/************************************************************************//**
 *	@brief		添加Fiber到数组
 *	@param[in]	i_stkPipeFiber		线缆的Fiber对象指针
 *	@retval		STK_SUCCESS			添加成功
 *	@retval		其他				添加失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Feat_Pipe::AddPipeFiber(Stk_Pipe_Fiber_Segment* i_stkPipeFiber)
{
	if (i_stkPipeFiber != NULL)
	{
		m_vcPipeFibers.push_back(i_stkPipeFiber);
	}
	else
	{
		return STK_ERROR;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取指定的Fiber
 *	@param[in]	i_nuKeyPos			索引
 *	@retval		指定的Fiber
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Pipe_Fiber_Segment* Stk_Feat_Pipe::GetPipeFiber(STK_UINT32 i_nuKeyPos)
{
	return m_vcPipeFibers.at(i_nuKeyPos);
}

/************************************************************************//**
 *	@brief		读取线缆特征数据
 *	@param[in]	i_FileP				文件指针
 *	@param[in]	i_FeatPipeHeadP		线缆特征记录头
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		其他				读取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Feat_Pipe::ReadFile(Stk_File* i_FileP, StkSegmentHead* i_FeatPipeHeadP)
{
	STK_UINT32				ii, jj;
	StkControl**			RecordPP = NULL;
	STK_STATUS				eState = STK_SUCCESS;

	StkFeatPipeRec*			FeatPipeRecP = NULL;
	StkFeatPipeRec2*		FeatPipeRec2P = NULL;
	StkPipeFiberPntRec*		PipeFiberPntRecP = NULL;
	StkPolyLineRec3*		PolyLineRecP3 = NULL;
	StkEllipseRec4*			EllipseRecP4 = NULL;
	StkLineRec3*			LineRecP3 = NULL;
	StkNurbsCurveRec*		NurbsCurveRecP = NULL;
	StkHyperbolaRec*		HyperbolaRecP = NULL;
	StkParabolaRec*			ParabolaRecP = NULL;

	Stk_Pipe_Fiber_Segment*	stkPipeFiberP = NULL;
	Stk_PolyLine*			PolyLineP = NULL;
	Stk_Ellipse	*			EllipseP = NULL;
	Stk_Line*				LineP = NULL;
	Stk_NurbsCurve*			NurbsCurveP = NULL;
	Stk_Hyperbola*			HyperbolaP = NULL;
	Stk_Parabola*			ParabolaP = NULL;

	STK_UINT32				nuNumKnotPlicity = 0;
	STK_UINT32				nuNumKnot = 0;
	STK_UINT32				nuNumPoint = 0;;
	STK_UINT32				nuNumWeight = 0;
	STK_PNT32				pntTmp;

	if (i_FileP == NULL || i_FeatPipeHeadP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *	读取线缆特征的数据
	 */
	RecordPP = (StkControl **)calloc(i_FeatPipeHeadP->recnum, sizeof(StkControl *));
	if (i_FeatPipeHeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}

	eState = i_FileP->ReadSegmentAllRecordData(i_FeatPipeHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 *	读取线缆特征段的各记录
	 */
	for (ii = 0; ii < i_FeatPipeHeadP->recnum; ii++)
	{
		switch(RecordPP[ii]->type)
		{
		case REC_TYPE_FEAT_PIPE:
			if (RecordPP[ii]->version == 1)
			{
				FeatPipeRecP = (StkFeatPipeRec* )RecordPP[ii];
				m_ePipeType = (StkPipeTypeEnum)FeatPipeRecP->nuPipeType;
				m_eSectionType = (StkPipeSectionTypeEnum)FeatPipeRecP->nuPipeSectionType;
			}
			else if (RecordPP[ii]->version == 2)
			{
				FeatPipeRec2P = (StkFeatPipeRec2* )RecordPP[ii];
				m_ePipeType = (StkPipeTypeEnum)FeatPipeRec2P->nuPipeType;
				m_eSectionType = (StkPipeSectionTypeEnum)FeatPipeRec2P->nuPipeSectionType;
				m_fDiameter = FeatPipeRec2P->fDiameter;
				m_fTurnRadius = FeatPipeRec2P->fTurnRadius;
				m_fHeight = FeatPipeRec2P->fHeight;
				m_fWidth = FeatPipeRec2P->fWidth;
			}
			break;

		case REC_TYPE_PIPEFIBER_PNT:
			if (RecordPP[ii]->version == 1)
			{
				stkPipeFiberP = new Stk_Pipe_Fiber_Segment();
				PipeFiberPntRecP = (StkPipeFiberPntRec* )RecordPP[ii];
				stkPipeFiberP->SetStartPoint(PipeFiberPntRecP->pntStart);
				stkPipeFiberP->SetEndPoint(PipeFiberPntRecP->pntEnd);
				stkPipeFiberP->SetCenterPoint(PipeFiberPntRecP->pntCenter);

				m_vcPipeFibers.push_back(stkPipeFiberP);
			}
			break;

		case REC_TYPE_POLYLINE:
			if (RecordPP[ii]->version == 3)
			{
				PolyLineRecP3 = (StkPolyLineRec3 *)RecordPP[ii];
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

				if (stkPipeFiberP != NULL)
				{
					stkPipeFiberP->SetCurve((Stk_CurveEntity* )PolyLineP);
				}
			}
			break;

		case REC_TYPE_ELLIPSE:
			if (RecordPP[ii]->version == 4)
			{
				EllipseRecP4 = (StkEllipseRec4 *)RecordPP[ii];
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
				
				if (stkPipeFiberP != NULL)
				{
					stkPipeFiberP->SetCurve((Stk_CurveEntity* )EllipseP);
				}
			}
			break;

		case REC_TYPE_LINE:
			if (RecordPP[ii]->version == 3)
			{
				LineRecP3 = (StkLineRec3 *)RecordPP[ii];
				LineP = new Stk_Line();

				LineP->SetID(LineRecP3->ID);
				LineP->SetColor(LineRecP3->Color);
				LineP->SetStartPoint(LineRecP3->StartPnt);
				LineP->SetEndPoint(LineRecP3->EndPnt);
				// 版本2
				LineP->SetUsageType((StkCurveUsageEnum)LineRecP3->DefType);
				//版本3
				LineP->SetCurveDispStat(LineRecP3->nuDisp);
				LineP->SetCurveDispType((StkCurveDispTypeEnum)LineRecP3->nuDispType);
			
				if (stkPipeFiberP != NULL)
				{
					stkPipeFiberP->SetCurve((Stk_CurveEntity* )LineP);
				}
			}
			break;

		case REC_TYPE_NURBSCURVE:
			if (RecordPP[ii]->version == 1)
			{
				NurbsCurveRecP = (StkNurbsCurveRec *)RecordPP[ii];
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
					NurbsCurveP->AddKnotMultiplicity(NurbsCurveRecP->atr[jj]);//FLOAT到UINT截断警告，已测试没有问题
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
			
				if (stkPipeFiberP != NULL)
				{
					stkPipeFiberP->SetCurve((Stk_CurveEntity* )NurbsCurveP);
				}
			}
			break;

		case REC_TYPE_HYPERBOLA:
			if (RecordPP[ii]->version == 1)
			{
				HyperbolaRecP = (StkHyperbolaRec *)RecordPP[ii];
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

				if (stkPipeFiberP != NULL)
				{
					stkPipeFiberP->SetCurve((Stk_CurveEntity* )HyperbolaP);
				}
			}
			break;

		case REC_TYPE_PARABOLA:
			if (RecordPP[ii]->version == 1)
			{
				ParabolaRecP = (StkParabolaRec *)RecordPP[ii];
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
			
				if (stkPipeFiberP != NULL)
				{
					stkPipeFiberP->SetCurve((Stk_CurveEntity* )ParabolaP);
				}
			}
			break;
		default:
			break;
		}
	}

	eState = i_FileP->ReadSegmentFinishData(i_FeatPipeHeadP,(StkControl **)RecordPP);// 释放内存
	if (eState != STK_SUCCESS)
	{
		return eState;
	}
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		填写线缆特征数据
 *	@param[in]	i_stkFileP			文件指针
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其他				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Feat_Pipe::WriteFile(Stk_File *i_stkFileP)
{
	StkSegmentHead		FeatPipeHead ={0};
	StkFeatPipeRec2		FeatPipeRec2;
	STK_STATUS			ret = STK_SUCCESS;
	STK_UINT32			nuPipeFiberNum = 0;
	STK_UINT32			ii;

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	nuPipeFiberNum = (STK_UINT32)m_vcPipeFibers.size();

	/*
	 *	【第1步】 输出线缆特征段头记录
	 */
	FeatPipeHead.id = 0;
	FeatPipeHead.kind = SEG_TYPE_FEAT_PIPE;
	ret = i_stkFileP->WriteSegmentHeadData(&FeatPipeHead);
	if (ret != STK_SUCCESS)
	{
		return ret;
	}

	/*
	 *	【第2步】 输出线缆特征记录
	 */
	FeatPipeRec2.Control.type = REC_TYPE_FEAT_PIPE;
	FeatPipeRec2.Control.length = sizeof(StkFeatPipeRec2) - sizeof(StkControl);
	FeatPipeRec2.Control.version = _VER_FEAT_PIPE_RECORD; /*2*/
	FeatPipeRec2.nuPipeType = m_ePipeType;
	FeatPipeRec2.nuPipeSectionType = m_eSectionType;
	FeatPipeRec2.nuPipeFiberNum = nuPipeFiberNum;
	FeatPipeRec2.fDiameter = m_fDiameter;
	FeatPipeRec2.fTurnRadius = m_fTurnRadius;
	FeatPipeRec2.fHeight = m_fHeight;
	FeatPipeRec2.fWidth = m_fWidth;

	ret = i_stkFileP->WriteSegmentRecordData(&FeatPipeHead, (StkControl *)&FeatPipeRec2);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	/*
	 *	【第3步】 输出线缆特征的Fiber记录
	 */
	for (ii = 0; ii < nuPipeFiberNum; ii++)
	{
		if (m_vcPipeFibers[ii] != NULL)
		{
			m_vcPipeFibers[ii]->WritePipeFiber(i_stkFileP, &FeatPipeHead);
		}
	}

	/*
	 *	【第4步】 完成线缆特征段的输出
	 */
	ret = i_stkFileP->WriteSegmentFinishData(&FeatPipeHead);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取线缆、管路实体的直径/厚度
 *	@param[in]	无
 *	@retval		无
 *	@note		线缆、管路实体的直径/厚度
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Feat_Pipe::GetDiameter()
{
	return m_fDiameter;
}

/************************************************************************//**
 *	@brief		设置线缆、管路实体的直径/厚度
 *	@param[in]	i_fDiameter			线缆、管路实体的直径/厚度
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Pipe::SetDiameter(STK_FLOAT32 i_fDiameter)
{
	m_fDiameter = i_fDiameter;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取线缆的最小折弯半径
 *	@param[in]	无
 *	@retval		无
 *	@note		线缆的最小折弯半径
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Feat_Pipe::GetTurnRadius()
{
	return m_fTurnRadius;
}

/************************************************************************//**
 *	@brief		设置线缆的最小折弯半径
 *	@param[in]	i_fTurnRadius		最小折弯半径
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Pipe::SetTurnRadius(STK_FLOAT32 i_fTurnRadius)
{
	m_fTurnRadius = i_fTurnRadius;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取线缆Section的高度
 *	@param[in]	无
 *	@retval		无
 *	@note		线缆Section的高度
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Feat_Pipe::GetHeight()
{
	return m_fHeight;
}

/************************************************************************//**
 *	@brief		设置线缆Section的高度
 *	@param[in]	i_fHeight			线缆Section的高度
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Pipe::SetHeight(STK_FLOAT32 i_fHeight)
{
	m_fHeight = i_fHeight;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取线缆Section的宽度
 *	@param[in]	无
 *	@retval		无
 *	@note		倒圆角的高度
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Feat_Pipe::GetWidth()
{
	return m_fWidth;
}

/************************************************************************//**
 *	@brief		设置线缆Section的宽度
 *	@param[in]	i_fWidth			线缆Section的宽度
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Pipe::SetWidth(STK_FLOAT32 i_fWidth)
{
	m_fWidth = i_fWidth;
	return STK_SUCCESS;
}
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

#include "Stk_Feat_Pipe.h"

namespace HoteamSoft {
	namespace SVLLib {
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
			Stk_CurveEntityPtr	stkCurveP = NULL;
			StkCurveTypeEnum	eCurveType = StkCurveTypeEnum::CURVE_TYPE_UNKNOWN;

			m_pntStart = i_stkPipeFiber.m_pntStart;
			m_pntEnd = i_stkPipeFiber.m_pntEnd;
			m_pntCenter = i_stkPipeFiber.m_pntCenter;
			stkCurveP = i_stkPipeFiber.GetCurve();
			eCurveType = stkCurveP->GetCurveType();
			switch(eCurveType)
			{
			case StkCurveTypeEnum::CURVE_TYPE_POLYLINE:
				m_stkCurveP = Stk_PolyLinePtr::CreateObject();
				m_stkCurveP = stkCurveP;
				break;
			case StkCurveTypeEnum::CURVE_TYPE_ELLIPSE:
				m_stkCurveP = Stk_ArcPtr::CreateObject();
				m_stkCurveP = stkCurveP;
				break;
			case StkCurveTypeEnum::CURVE_TYPE_LINE:
				m_stkCurveP = Stk_LinePtr::CreateObject();
				m_stkCurveP = stkCurveP;
				break;
			case StkCurveTypeEnum::CURVE_TYPE_NURBSCURVE:
				m_stkCurveP = Stk_NurbsCurvePtr::CreateObject();
				m_stkCurveP = stkCurveP;
				break;
			case StkCurveTypeEnum::CURVE_TYPE_HYPERBOLA:
				m_stkCurveP = Stk_HyperbolaPtr::CreateObject();
				m_stkCurveP = stkCurveP;
				break;
			case StkCurveTypeEnum::CURVE_TYPE_PARABOLA:
				m_stkCurveP = Stk_ParabolaPtr::CreateObject();
				m_stkCurveP = stkCurveP;
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

			Stk_CurveEntityPtr	stkCurveP = nullptr;
			StkCurveTypeEnum	eCurveType = StkCurveTypeEnum::CURVE_TYPE_UNKNOWN;

			m_pntStart = i_stkPipeFiber.m_pntStart;
			m_pntEnd = i_stkPipeFiber.m_pntEnd;
			m_pntCenter = i_stkPipeFiber.m_pntCenter;
			stkCurveP = i_stkPipeFiber.GetCurve();
			eCurveType = stkCurveP->GetCurveType();
			switch(eCurveType)
			{
			case StkCurveTypeEnum::CURVE_TYPE_POLYLINE:
				m_stkCurveP = Stk_PolyLinePtr::CreateObject();
				m_stkCurveP = stkCurveP;
				break;
			case StkCurveTypeEnum::CURVE_TYPE_ELLIPSE:
				m_stkCurveP = Stk_ArcPtr::CreateObject();
				m_stkCurveP = stkCurveP;
				break;
			case StkCurveTypeEnum::CURVE_TYPE_LINE:
				m_stkCurveP = Stk_LinePtr::CreateObject();
				m_stkCurveP = stkCurveP;
				break;
			case StkCurveTypeEnum::CURVE_TYPE_NURBSCURVE:
				m_stkCurveP = Stk_NurbsCurvePtr::CreateObject();
				m_stkCurveP = stkCurveP;
				break;
			case StkCurveTypeEnum::CURVE_TYPE_HYPERBOLA:
				m_stkCurveP = Stk_HyperbolaPtr::CreateObject();
				m_stkCurveP = stkCurveP;
				break;
			case StkCurveTypeEnum::CURVE_TYPE_PARABOLA:
				m_stkCurveP = Stk_ParabolaPtr::CreateObject();
				m_stkCurveP = stkCurveP;
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

		STK_STATUS Stk_Pipe_Fiber_Segment::SetCurve(Stk_CurveEntityPtr i_stkCurveP)
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

		Stk_CurveEntityPtr Stk_Pipe_Fiber_Segment::GetCurve()
		{
			return m_stkCurveP;
		}


		STK_IMPLEMENT_CLASS(Stk_Feat_Pipe, Stk_Object)
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
	}
}
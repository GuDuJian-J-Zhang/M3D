// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Feat_Hole.cpp
 *
 *	@brief	孔特征类的实现
 *
 *	@par	历史:
 *		2014/11/19	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Feat_Hole.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif
namespace HoteamSoft {
	namespace SVLLib {
		/************************************************************************//**
		 *	@brief		构造函数
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_Feat_Axis::Stk_Feat_Axis()
		{
			m_dirAxis.X = 0;
			m_dirAxis.Y = 0;
			m_dirAxis.Z = 0;
			memset(&m_pntOrigin, 0, sizeof(STK_PNT32));
		}

		/************************************************************************//**
		 *	@brief		析构函数
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_Feat_Axis::~Stk_Feat_Axis()
		{

		}

		/************************************************************************//**
		 *	@brief		设置轴方向
		 *	@param[in]	i_dirAxis		轴方向向量
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS		设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Axis::SetDirection(STK_DIR32 i_dirAxis)
		{
			m_dirAxis = i_dirAxis;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取轴方向
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		轴方向向量
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DIR32 Stk_Feat_Axis::GetDirection()
		{
			return m_dirAxis;
		}

		/************************************************************************//**
		 *	@brief		设置原点
		 *	@param[in]	i_pntOrigin		原点
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS		设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Axis::SetOrigin(STK_PNT32 i_pntOrigin)
		{
			m_pntOrigin = i_pntOrigin;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取原点
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		原点
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_PNT32 Stk_Feat_Axis::GetOrigin()
		{
			return m_pntOrigin;
		}

		/************************************************************************//**
		 *	@brief		构造函数
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_FeatLimit::Stk_FeatLimit()
		{
			m_eLimitType = TYPE_LIMIT_DEFAULT;
			m_dValue = 0;
			m_stkNodeConnectorP = nullptr;
		}

		/************************************************************************//**
		 *	@brief		构造函数
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_FeatLimit::~Stk_FeatLimit()
		{
			
		}

		/************************************************************************//**
		 *	@brief		设置Limit类型
		 *	@param[in]	i_eLimitType		Limit类型
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_FeatLimit::SetType(StkLimitTypeEnum i_eLimitType)
		{
			m_eLimitType = i_eLimitType;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取Limit类型
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		Limit类型
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		StkLimitTypeEnum Stk_FeatLimit::GetType()
		{
			return m_eLimitType;
		}

		/************************************************************************//**
		 *	@brief		设置Limit值
		 *	@param[in]	i_dValue			Limit值
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_FeatLimit::SetValue(STK_DOUBLE64 i_dValue)
		{
			m_dValue = i_dValue;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取Limit值
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		Limit值
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_FeatLimit::GetValue()
		{
			return m_dValue;
		}

		/************************************************************************//**
		 *	@brief		设置关联对象
		 *	@param[in]	i_NodeConnectorP	关联
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_FeatLimit::SetReferenceObject(Stk_ConnectorPtr i_NodeConnectorP)
		{
			m_stkNodeConnectorP = i_NodeConnectorP;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取关联对象
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		关联
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_ConnectorPtr Stk_FeatLimit::GetReferenceObject()
		{
			return m_stkNodeConnectorP;
		}

		/************************************************************************//**
		 *	@brief		构造函数
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_FeatInnerThread::Stk_FeatInnerThread()
		{
			m_eThreadLimitType = TYPE_THREAD_DIMENSION;
			m_eThreadType = TYPE_THREAD_UNKNOWN;
			m_dDepth = 0;
			m_bHasDepthTol = false;
			m_dDepthTol = 0;
			m_dDiameter = 0;
			m_bHasDiameterTol = false;
			m_dDiameterTol = 0;
			m_dPitch = 0;
			m_bHasPitchTol = false;
			m_dPitchTol = 0;
			m_bIsRightThread = true;
			m_NodeConnectorP = NULL;
		}

		/************************************************************************//**
		 *	@brief		析构函数
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_FeatInnerThread::~Stk_FeatInnerThread()
		{
			
		}

		/************************************************************************//**
		 *	@brief		设置螺纹底部类型
		 *	@param[in]	i_eThreadLimitType	螺纹底部类型
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_FeatInnerThread::SetThreadLimitType(StkThreadLimitTypeEnum i_eThreadLimitType)
		{
			m_eThreadLimitType = i_eThreadLimitType;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取螺纹底部类型
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		螺纹底部类型
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		StkThreadLimitTypeEnum Stk_FeatInnerThread::GetThreadLimitType()
		{
			return m_eThreadLimitType;
		}

		/************************************************************************//**
		 *	@brief		设置螺纹类型
		 *	@param[in]	i_eThreadType	螺纹类型
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS		设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_FeatInnerThread::SetThreadType(StkThreadTypeEnum i_eThreadType)
		{
			m_eThreadType = i_eThreadType;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取螺纹类型
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		螺纹类型
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		StkThreadTypeEnum Stk_FeatInnerThread::GetThreadType()
		{
			return m_eThreadType;
		}

		/************************************************************************//**
		 *	@brief		设置螺纹深度
		 *	@param[in]	i_dDepth		螺纹深度
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS		设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_FeatInnerThread::SetDepth(STK_DOUBLE64 i_dDepth)
		{
			m_dDepth = i_dDepth;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取螺纹深度
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		螺纹深度
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_FeatInnerThread::GetDepth()
		{
			return m_dDepth;
		}

		/************************************************************************//**
		 *	@brief		设置是否有螺纹深度公差
		 *	@param[in]	i_bHasTol		公差标识
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS		设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_FeatInnerThread::SetDepthHasTol(STK_BOOL i_bHasTol)
		{
			m_bHasDepthTol = i_bHasTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		是否有螺纹深度公差
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		公差标识
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_BOOL Stk_FeatInnerThread::IsDepthHasTol()
		{
			return m_bHasDepthTol;
		}

		/************************************************************************//**
		 *	@brief		设置螺纹深度公差
		 *	@param[in]	i_dDepthTol		深度公差
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS		设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_FeatInnerThread::SetDepthTol(STK_DOUBLE64 i_dDepthTol)
		{
			m_dDepthTol = i_dDepthTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取螺纹深度公差
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		深度公差
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_FeatInnerThread::GetDepthTol()
		{
			return m_dDepthTol;
		}

		/************************************************************************//**
		 *	@brief		设置螺纹直径
		 *	@param[in]	i_dDiameter		螺纹直径
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS		设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_FeatInnerThread::SetDiameter(STK_DOUBLE64 i_dDiameter)
		{
			m_dDiameter = i_dDiameter;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取螺纹直径
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		螺纹直径
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_FeatInnerThread::GetDiameter()
		{
			return m_dDiameter;
		}

		/************************************************************************//**
		 *	@brief		设置是否有螺纹直径公差
		 *	@param[in]	i_bDiameterHasTol	螺纹直径公差标识
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_FeatInnerThread::SetDiameterHasTol(STK_BOOL i_bDiameterHasTol)
		{
			m_bHasDiameterTol = i_bDiameterHasTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		是否有螺纹直径公差
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		螺纹直径公差标识
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_BOOL Stk_FeatInnerThread::IsHasDiameterTol()
		{
			return m_bHasDiameterTol;
		}

		HoteamSoft::SVLLib::STK_STATUS Stk_FeatInnerThread::SetDiameterTol(STK_DOUBLE64 i_dDiameterTol)
		{
			m_dDiameterTol = i_dDiameterTol;
			return STK_SUCCESS;
		}

		HoteamSoft::SVLLib::STK_DOUBLE64 Stk_FeatInnerThread::GetDiameterTol()
		{
			return m_dDiameterTol;
		}

		/************************************************************************//**
		 *	@brief		设置螺距
		 *	@param[in]	i_dPitch		螺距
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS		设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_FeatInnerThread::SetPitch(STK_DOUBLE64 i_dPitch)
		{
			m_dPitch = i_dPitch;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取螺距
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		螺距
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_FeatInnerThread::GetPitch()
		{
			return m_dPitch;
		}

		/************************************************************************//**
		 *	@brief		设置是否有螺距公差标识
		 *	@param[in]	i_bPitchHasTol	螺距公差标识
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS		设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_FeatInnerThread::SetPitchHasTol(STK_BOOL i_bPitchHasTol)
		{
			m_bHasPitchTol = i_bPitchHasTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		是否有螺距公差标识
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		螺距公差标识
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_BOOL Stk_FeatInnerThread::IsHasPitchTol()
		{
			return m_bHasPitchTol;
		}

		/************************************************************************//**
		 *	@brief		设置螺距公差
		 *	@param[in]	i_dPitchHasTol	螺距公差
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS		设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_FeatInnerThread::SetPitchTol(STK_DOUBLE64 i_dPitchHasTol)
		{
			m_dPitchTol = i_dPitchHasTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取螺距公差
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		螺距公差
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_FeatInnerThread::GetPitchTol()
		{
			return m_dPitchTol;
		}

		/************************************************************************//**
		 *	@brief		设置是否是右旋螺纹
		 *	@param[in]	i_bRightThread	右旋螺纹标识
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS		设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_FeatInnerThread::SetRightThreaded(STK_BOOL i_bRightThread)
		{
			m_bIsRightThread = i_bRightThread;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		是否是右旋螺纹
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		右旋螺纹标识
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_BOOL Stk_FeatInnerThread::IsRightThreaded()
		{
			return m_bIsRightThread;
		}

		/************************************************************************//**
		 *	@brief		设置螺纹参考面
		 *	@param[in]	i_NodeConnectorP	关联
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS		设置成功
		 *	@note		只有当m_eThreadLimitType类型为TYPE_THREAD_UP_TO_PLANE时，才会有该属性
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_FeatInnerThread::SetThreadLimitReference(Stk_ConnectorPtr i_NodeConnectorP)
		{
			if (m_eThreadLimitType == TYPE_THREAD_UP_TO_PLANE)
			{
				m_NodeConnectorP = i_NodeConnectorP;
			}
			else
			{
				m_NodeConnectorP = NULL;
			}
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取螺纹参考面
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		关联
		 *	@note		只有当m_eThreadLimitType类型为TYPE_THREAD_UP_TO_PLANE时，才会有该属性
		 *	@attention	无
		 ****************************************************************************/
		Stk_ConnectorPtr Stk_FeatInnerThread::GetThreadLimitReference()
		{
			return m_NodeConnectorP;
		}


		STK_IMPLEMENT_CLASS(Stk_Feat_Hole, Stk_Object)
		/************************************************************************//**
		 *	@brief		构造函数
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_Feat_Hole::Stk_Feat_Hole()
		{
			m_eHoleAnchorType = TYPE_NO_ANCHOR;
			m_eHoleType = TYPE_HOLE_SIMPLE;
			m_eHoleBottomType = TYPE_BOTTOM_FLAT;
			m_FeatAxisP = NULL;
			m_FeatInnerThreadP = NULL;
			m_dDepth = 0;
			m_bHasDepthTol = false;
			m_dDepthTol = 0;
			m_dDiameter = 0;
			m_bHasDiameterTol = false;
			m_dDiamterTol = 0;
			m_dBottomAngle = 0;
			m_bHasBottomAngleTol = false;
			m_dBottomAngleTol = 0;
			m_dCounterAngle = 0;
			m_bHasCounterAngleTol = false;
			m_dCounterAngleTol = 0;
			m_dCounterDepth = 0;
			m_bHasCounterDepthTol = false;
			m_dCounterDepthTol = 0;
			m_dCounterDiamter = 0;
			m_bHasCounterDiamterTol = false;
			m_dCounterDiamterTol = 0;
		}

		/************************************************************************//**
		 *	@brief		析构函数
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_Feat_Hole::~Stk_Feat_Hole()
		{
			if (m_FeatAxisP != NULL)
			{
				delete m_FeatAxisP;
				m_FeatAxisP = NULL;
			}
			if (m_FeatInnerThreadP != NULL)
			{
				delete m_FeatInnerThreadP;
				m_FeatInnerThreadP = NULL;
			}
		}

		/************************************************************************//**
		 *	@brief		设置孔的固定类型
		 *	@param[in]	i_eAnchorType		固定类型
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetAnchorType(StkHoleAnchorTypeEnum i_eAnchorType)
		{
			m_eHoleAnchorType = i_eAnchorType;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取孔的固定类型
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		固定类型
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		StkHoleAnchorTypeEnum Stk_Feat_Hole::GetAnchorType()
		{
			return m_eHoleAnchorType;
		}

		/************************************************************************//**
		 *	@brief		设置孔的类型
		 *	@param[in]	i_eType				孔的类型
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetType(StkHoleTypeEnum i_eType)
		{
			m_eHoleType = i_eType;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取孔的类型
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔的类型
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		StkHoleTypeEnum Stk_Feat_Hole::GetType()
		{
			return m_eHoleType;
		}

		/************************************************************************//**
		 *	@brief		设置孔的底部类型
		 *	@param[in]	i_eBottomType		底部类型
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetBottomType(StkHoleBottomTypeEnum i_eBottomType)
		{
			m_eHoleBottomType = i_eBottomType;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取孔的底部类型
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔的底部类型
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		StkHoleBottomTypeEnum Stk_Feat_Hole::GetBottomType()
		{
			return m_eHoleBottomType;
		}

		/************************************************************************//**
		 *	@brief		设置轴
		 *	@param[in]	i_AxisP				轴
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetFeatAxis(Stk_Feat_Axis* i_AxisP)
		{
			m_FeatAxisP = i_AxisP;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取轴
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		轴
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_Feat_Axis* Stk_Feat_Hole::GetFeatAxis()
		{
			return m_FeatAxisP;
		}

		/************************************************************************//**
		 *	@brief		设置螺纹
		 *	@param[in]	i_InnerThreadP		螺纹
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetInnerThread(Stk_FeatInnerThread* i_InnerThreadP)
		{
			m_FeatInnerThreadP = i_InnerThreadP;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取螺纹
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		螺纹
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_FeatInnerThread* Stk_Feat_Hole::GetInnerThread()
		{
			return m_FeatInnerThreadP;
		}

		/************************************************************************//**
		 *	@brief		设置Limit特征
		 *	@param[in]	i_FeatLimitP		Limit特征
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetFeatLimit(Stk_FeatLimit* i_FeatLimitP)
		{
			m_FeatLimitP = i_FeatLimitP;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取Limit特征
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		Limit特征
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_FeatLimit* Stk_Feat_Hole::GetFeatLimit()
		{
			return m_FeatLimitP;
		}

		/************************************************************************//**
		 *	@brief		设置孔深度
		 *	@param[in]	i_dDepth			孔深度
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetDepth(STK_DOUBLE64 i_dDepth)
		{
			m_dDepth = i_dDepth;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取孔深度
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔深度
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_Feat_Hole::GetDepth()
		{
			return m_dDepth;
		}

		/************************************************************************//**
		 *	@brief		设置是否有孔深度公差
		 *	@param[in]	i_bDepthTol			孔深度公差标识
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetDepthHasTol(STK_BOOL i_bDepthTol)
		{
			m_bHasDepthTol = i_bDepthTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		是否有孔深度公差
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔深度公差标识
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_BOOL Stk_Feat_Hole::IsHasDepthTol()
		{
			return m_bHasDepthTol;
		}

		/************************************************************************//**
		 *	@brief		设置孔深度公差
		 *	@param[in]	i_dDepthTol			孔深度公差
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetDepthWithTol(STK_DOUBLE64 i_dDepthTol)
		{
			m_dDepthTol = i_dDepthTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取孔深度公差
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔深度公差
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_Feat_Hole::GetDepthWithTol()
		{
			return m_dDepthTol;
		}

		/************************************************************************//**
		 *	@brief		设置孔直径
		 *	@param[in]	i_dDiameter			孔直径
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetDiameter(STK_DOUBLE64 i_dDiameter)
		{
			m_dDiameter = i_dDiameter;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取孔直径
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔直径
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_Feat_Hole::GetDiameter()
		{
			return m_dDiameter;
		}

		/************************************************************************//**
		 *	@brief		设置孔直径公差标识
		 *	@param[in]	i_bDiameterTol		孔直径公差标识
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetDiameterHasTol(STK_BOOL i_bDiameterTol)
		{
			m_bHasDiameterTol = i_bDiameterTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取孔直径公差标识
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔直径公差标识
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_BOOL Stk_Feat_Hole::IsHasDiameterTol()
		{
			return m_bHasDiameterTol;
		}

		/************************************************************************//**
		 *	@brief		设置孔直径公差
		 *	@param[in]	i_dDiameterTol		孔直径公差
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetDiameterWithTol(STK_DOUBLE64 i_dDiameterTol)
		{
			m_dDiamterTol = i_dDiameterTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取孔直径公差
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔直径公差
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_Feat_Hole::GetDiameterWithTol()
		{
			return m_dDiamterTol;
		}

		/************************************************************************//**
		 *	@brief		设置孔底部角度
		 *	@param[in]	i_dBottomAngle		孔底部角度
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetBottomAngle(STK_DOUBLE64 i_dBottomAngle)
		{
			m_dBottomAngle = i_dBottomAngle;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取孔底部角度
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔底部角度
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_Feat_Hole::GetBottomAngle()
		{
			return m_dBottomAngle;
		}

		/************************************************************************//**
		 *	@brief		设置孔底部角度公差标识
		 *	@param[in]	i_bBottomAngleTol	孔底部角度公差标识
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetBottomAngleHasTol(STK_BOOL i_bBottomAngleTol)
		{
			m_bHasBottomAngleTol = i_bBottomAngleTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取孔底部角度公差标识
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔底部角度标识公差
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_BOOL Stk_Feat_Hole::IsHasBottomAngleTol()
		{
			return m_bHasBottomAngleTol;
		}

		/************************************************************************//**
		 *	@brief		设置孔底部角度公差
		 *	@param[in]	i_dBottomAngleTol	孔底部角度公差
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetBottomAngleTol(STK_DOUBLE64 i_dBottomAngleTol)
		{
			m_dBottomAngleTol = i_dBottomAngleTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取孔底部角度公差
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔底部角度公差
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_Feat_Hole::GetBottomAngleTol()
		{
			return m_dBottomAngleTol;
		}

		/************************************************************************//**
		 *	@brief		设置孔的反向角度
		 *	@param[in]	i_dCounterAngle		孔的反向角度
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetCounterAngle(STK_DOUBLE64 i_dCounterAngle)
		{
			m_dCounterAngle = i_dCounterAngle;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取孔的反向角度
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔反向角度
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_Feat_Hole::GetCounterAngle()
		{
			return m_dCounterAngle;
		}

		/************************************************************************//**
		 *	@brief		设置孔的反向角度公差标识
		 *	@param[in]	i_bCounterAngleTol	孔的反向角度公差标识
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetCounterAngleHasTol(STK_BOOL i_bCounterAngleTol)
		{
			m_bHasCounterAngleTol = i_bCounterAngleTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		孔的反向角度公差标识
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔反向角度公差标识
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_BOOL Stk_Feat_Hole::IsHasCounterAngleTol()
		{
			return m_bHasCounterAngleTol;
		}

		/************************************************************************//**
		 *	@brief		设置孔的反向角度公差
		 *	@param[in]	i_dCounterAngleTol	孔的反向角度公差
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetCounterAngleTol(STK_DOUBLE64 i_dCounterAngleTol)
		{
			m_dCounterAngleTol = i_dCounterAngleTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		孔的反向角度公差
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔反向角度公差
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_Feat_Hole::GetCounterAngleTol()
		{
			return m_dCounterAngleTol;
		}

		/************************************************************************//**
		 *	@brief		设置孔的反向深度
		 *	@param[in]	i_dCounterDepth		孔的反向角度深度
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetCounterDepth(STK_DOUBLE64 i_dCounterDepth)
		{
			m_dCounterDepth = i_dCounterDepth;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		孔的反向深度
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔反向深度
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_Feat_Hole::GetCounterDepth()
		{
			return m_dCounterDepth;
		}

		/************************************************************************//**
		 *	@brief		设置孔的反向深度公差标识
		 *	@param[in]	i_bCounterDepthTol	孔的反向深度公差标识
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetCounterDepthHasTol(STK_BOOL i_bCounterDepthTol)
		{
			m_bHasCounterDepthTol = i_bCounterDepthTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		孔的反向深度公差标识
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔反向深度公差标识
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_BOOL Stk_Feat_Hole::IsHasCounterDepthTol()
		{
			return m_bHasCounterDepthTol;
		}

		/************************************************************************//**
		 *	@brief		设置孔的反向深度公差
		 *	@param[in]	i_dCounterDepthTol	孔的反向深度公差
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetCounterDepthTol(STK_DOUBLE64 i_dCounterDepthTol)
		{
			m_dCounterDepthTol = i_dCounterDepthTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		孔的反向深度公差
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔反向深度公差
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_Feat_Hole::GetCounterDepthTol()
		{
			return m_dCounterDepthTol;
		}

		/************************************************************************//**
		 *	@brief		设置孔的反向直径
		 *	@param[in]	i_dCounterDiameter	孔的反向直径
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetCounterDiameter(STK_DOUBLE64 i_dCounterDiameter)
		{
			m_dCounterDiamter = i_dCounterDiameter;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		孔的反向直径
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔反向直径
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_Feat_Hole::GetCounterDiameter()
		{
			return m_dCounterDiamter;
		}

		/************************************************************************//**
		 *	@brief		设置孔的反向直径公差标识
		 *	@param[in]	i_bCounterDiameterTol	孔的反向直径公差标识
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS				设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetCounterDiameterHasTol(STK_BOOL i_bCounterDiameterTol)
		{
			m_bHasCounterDiamterTol = i_bCounterDiameterTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		孔的反向直径公差标识
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔反向直径公差标识
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_BOOL Stk_Feat_Hole::IsHasCounterDiameterTol()
		{
			return m_bHasCounterDiamterTol;
		}

		/************************************************************************//**
		 *	@brief		设置孔的反向直径公差
		 *	@param[in]	i_dCounterDiameterTol	孔的反向直径公差
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS				设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat_Hole::SetCounterDiameterTol(STK_DOUBLE64 i_dCounterDiameterTol)
		{
			m_dCounterDiamterTol = i_dCounterDiameterTol;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取孔的反向直径公差
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		孔反向直径公差
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_DOUBLE64 Stk_Feat_Hole::GetCounterDiameterTol()
		{
			return m_dCounterDiamterTol;
		}

	}
}

// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_LineSetEntity.cpp
 *
 *	@brief	线集类的实现
 *
 *	@par	历史:
 *		2017/07/14	创建。Likun@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif


#include "Stk_LineSet.h"
#include "Stk_ObjectImpl.h"


namespace HoteamSoft {
	namespace SVLLib {
		class Stk_LineSetImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_LineSet)
		public:
			STK_ID						m_iModelID;						//!< 所属模型ID
			wstring						m_wstrName;						//!< 线集名字
			StkLineSetTypeEnum			m_eType;						//!< 线集类型
			STK_BOOL					m_bDispStat;					//!< 线集是否隐藏
			STK_RGBA32					m_rgbaLineSet;					//!< 线集颜色
			vector<Stk_CurveEntityPtr>	m_vcLines;						//!< 线集
		public:
			/****************************************************************************
			*	@brief		Stk_LineSet构造函数
			*	@param[in]	i_stkProtoTypeP		ProtoType对象指针
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			Stk_LineSetImpl();

			/****************************************************************************
			*	@brief		Stk_LineSet析构函数
			*	@param[in]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			~Stk_LineSetImpl();
		

			/****************************************************************************
			*	@brief		从Json字符串读取线集数据
			*	@param[in]	i_strJson			Json字符串
			*	@param[out]	无
			*	@retval		STK_SUCCESS			读取成功
			*	@retval		STK_ERROR_MEMORY	内存申请失败
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS					FromJson(string i_strJson);

			/****************************************************************************
			*	@brief		将线集数据转换为Json字符串
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		Json字符串
			*	@note		无
			*	@attention
			****************************************************************************/
			string					ToJson();
		};


		STK_CLASS_IMPL_IMPLEMENT(Stk_LineSet)
		/************************************************************************//**
		 *	@brief		Stk_LineSet构造函数
		 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/

		Stk_LineSetImpl::Stk_LineSetImpl()
		{
			m_iModelID = 0;
			m_wstrName = L"";
			m_bDispStat = false;
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

		Stk_LineSetImpl::~Stk_LineSetImpl()
		{
			m_vcLines.clear();
		}

		STK_STATUS Stk_LineSetImpl::FromJson(string i_strJson)
		{
			
			return STK_SUCCESS;
		}

		string Stk_LineSetImpl::ToJson()
		{
			string strJson = "";
			return strJson;
		}


		STK_IMPLEMENT_CLASS(Stk_LineSet, Stk_Object)
		/************************************************************************
		*	@brief		Stk_LineSet构造函数
		*	@param[in]	i_stkProtoTypeP		ProtoType对象指针
		*	@retval		无
		*	@note		无
		*	@attention	无
		****************************************************************************/

		Stk_LineSet::Stk_LineSet():Stk_Object(new Stk_LineSetImpl())
		{
			
		}

		/************************************************************************
		*	@brief		Stk_LineSet析构函数
		*	@param[in]	无
		*	@retval		无
		*	@note		无
		*	@attention	无
		****************************************************************************/

		Stk_LineSet::~Stk_LineSet()
		{
			DELETE_STK_CLASS_IMPL
		}

		/************************************************************************
		*	@brief		设置线集名字
		*	@param[in]	i_wstrName		线集名字
		*	@param[out]	无
		*	@retval		STK_SUCCESS		设置成功
		*	@note		无
		*	@attention	无
		****************************************************************************/

		STK_STATUS Stk_LineSet::SetName(wstring i_wstrName)
		{
			((Stk_LineSetImpl*)m_pImpl)->m_wstrName = i_wstrName;

			return STK_SUCCESS;
		}

		/************************************************************************
		*	@brief		获取线集名字
		*	@param[in]	无
		*	@param[out]	无
		*	@retval		m_wstrName		线集名字
		*	@note		无
		*	@attention	无
		****************************************************************************/

		wstring Stk_LineSet::GetName()
		{
			return ((Stk_LineSetImpl*)m_pImpl)->m_wstrName;
		}

		/************************************************************************
		*	@brief		获取ModelID
		*	@param[in]	无
		*	@param[out]	无
		*	@retval		STK_ID
		*	@note		无
		*	@attention	无
		****************************************************************************/

		STK_ID Stk_LineSet::GetModelID()
		{
			return ((Stk_LineSetImpl*)m_pImpl)->m_iModelID;
		}
		/************************************************************************
		*	@brief		获取ModelID
		*	@param[in]	无
		*	@param[out]	无
		*	@retval		STK_ID
		*	@note		无
		*	@attention	无
		****************************************************************************/

		STK_STATUS Stk_LineSet::SetModelID(STK_ID i_ID)
		{
			((Stk_LineSetImpl*)m_pImpl)->m_iModelID = i_ID;
			return STK_SUCCESS;
		}

		/************************************************************************
		*	@brief		设置线集的类型
		*	@param[in]	i_eType			线集类型
		*	@param[out]	无
		*	@retval		STK_SUCCESS		设置成功
		*	@note		无
		*	@attention	无
		****************************************************************************/

		STK_STATUS Stk_LineSet::SetType(StkLineSetTypeEnum i_eType)
		{
			((Stk_LineSetImpl*)m_pImpl)->m_eType = i_eType;
			return STK_SUCCESS;
		}

		/************************************************************************
		*	@brief		获取线集的类型
		*	@param[in]	无
		*	@param[out]	无
		*	@retval		线集类型
		*	@note		无
		*	@attention	无
		****************************************************************************/

		StkLineSetTypeEnum Stk_LineSet::GetType()
		{
			return ((Stk_LineSetImpl*)m_pImpl)->m_eType;
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
			((Stk_LineSetImpl*)m_pImpl)->m_bDispStat = i_bDispStat;
			return STK_SUCCESS;
		}

		/************************************************************************
		*	@brief		获取线集显隐状态
		*	@param[in]	无
		*	@param[out]	无
		*	@retval		m_bDispStat		线集显隐状态
		*	@note		无
		*	@attention	无
		****************************************************************************/

		STK_BOOL Stk_LineSet::GetDispStat()
		{
			return ((Stk_LineSetImpl*)m_pImpl)->m_bDispStat;
		}

		/************************************************************************
		*	@brief		设置线集颜色
		*	@param[in]	i_rgbaLineSet	线集颜色
		*	@param[out]	无
		*	@retval		STK_SUCCESS		设置成功
		*	@note		无
		*	@attention	无
		****************************************************************************/

		STK_STATUS Stk_LineSet::SetColor(STK_RGBA32 i_rgbaLineSet)
		{
			((Stk_LineSetImpl*)m_pImpl)->m_rgbaLineSet = i_rgbaLineSet;
			return STK_SUCCESS;
		}

		/************************************************************************
		*	@brief		获取线集颜色
		*	@param[in]	无
		*	@param[out]	无
		*	@retval		m_rgbaLineSet		线集颜色
		*	@note		无
		*	@attention	无
		****************************************************************************/

		STK_RGBA32 Stk_LineSet::GetColor()
		{
			return ((Stk_LineSetImpl*)m_pImpl)->m_rgbaLineSet;
		}

		/************************************************************************
		*	@brief		往线集中添加线数据
		*	@param[in]	i_stkLine			线数据
		*	@param[out]	无
		*	@retval		STK_SUCCESS			添加成功
		*	@note		无
		*	@attention	无
		****************************************************************************/

		STK_STATUS Stk_LineSet::AddLine(Stk_CurveEntityPtr i_stkLine)
		{
			if (i_stkLine != NULL)
			{
				((Stk_LineSetImpl*)m_pImpl)->m_vcLines.push_back(i_stkLine);
			}
			return STK_SUCCESS;
		}

		/************************************************************************
		*	@brief		获取线集中的线数据
		*	@param[in]	无
		*	@param[out]	无
		*	@retval		m_vcLines			线集中的线数据
		*	@note		无
		*	@attention	无
		****************************************************************************/

		vector<Stk_CurveEntityPtr>& Stk_LineSet::GetAllLines()
		{
			return ((Stk_LineSetImpl*)m_pImpl)->m_vcLines;
		}


		STK_STATUS Stk_LineSet::FromJson(string i_strJson)
		{
			return ((Stk_LineSetImpl*)m_pImpl)->FromJson(i_strJson);
		}

		string Stk_LineSet::ToJson()
		{
			return ((Stk_LineSetImpl*)m_pImpl)->ToJson();
		}
	}
}
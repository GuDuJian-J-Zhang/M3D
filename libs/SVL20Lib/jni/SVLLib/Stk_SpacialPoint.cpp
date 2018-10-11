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

#include "Stk_SpacialPoint.h"
#include "Stk_File.h"
#include "Stk_Model.h"
#include "Stk_Platform.h"
#include "Stk_ObjectImpl.h"

namespace HoteamSoft {
	namespace SVLLib {

		class Stk_SpacialPointEntityImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_SpacialPointEntity)
		public:
			STK_ID						m_iModelID;						//!< 所属模型ID
			wstring							m_wstrName;						//!< 空间点名字
			STK_BOOL						m_bDispStat;					//!< 空间点是否隐藏
			StkSpacialPointTypeEnum			m_eType;						//!< 点的类型
			STK_RGBA32						m_rgbaPoint;					//!< 点颜色
			STK_PNT32						m_pntPos;						//!< 点位置
		public:
			/****************************************************************************
			*	@brief		Stk_SpacialPointEntityImpl构造函数
			*	@param[in]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			Stk_SpacialPointEntityImpl();

			/****************************************************************************
			*	@brief		Stk_SpacialPointEntityImpl析构函数
			*	@param[in]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			~Stk_SpacialPointEntityImpl();


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


		STK_CLASS_IMPL_IMPLEMENT(Stk_SpacialPointEntity)
		/************************************************************************
		*	@brief		Stk_SpacialPointEntity构造函数
		*	@param[in]	i_stkProtoTypeP		ProtoType对象指针
		*	@retval		无
		*	@note		无
		*	@attention	无
		****************************************************************************/
		Stk_SpacialPointEntityImpl::Stk_SpacialPointEntityImpl()
		{
			m_iModelID = 0;
			m_wstrName = L"";
			m_bDispStat = false;
			m_eType = StkSpacialPointTypeEnum::STK_PT_UNSPECIFIED;
			m_rgbaPoint.Red = -1.0f;
			m_rgbaPoint.Green = -1.0f;
			m_rgbaPoint.Blue = -1.0f;
			m_rgbaPoint.Alpha = -1.0f;
			m_pntPos.X = 0.0f;
			m_pntPos.Y = 0.0f;
			m_pntPos.Z = 0.0f;
		}

		/************************************************************************
		*	@brief		Stk_LineSet析构函数
		*	@param[in]	无
		*	@retval		无
		*	@note		无
		*	@attention	无
		****************************************************************************/

		Stk_SpacialPointEntityImpl::~Stk_SpacialPointEntityImpl()
		{
			
		}

		STK_STATUS Stk_SpacialPointEntityImpl::FromJson(string i_strJson)
		{

			return STK_SUCCESS;
		}

		string Stk_SpacialPointEntityImpl::ToJson()
		{
			string strJson = "";
			return strJson;
		}


		STK_IMPLEMENT_CLASS(Stk_SpacialPointEntity, Stk_Object)
		/************************************************************************//**
		 *	@brief		Stk_SpacialPointEntity构造函数
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_SpacialPointEntity::Stk_SpacialPointEntity() :Stk_Object(new Stk_SpacialPointEntityImpl())
		{
			
		}

		 /************************************************************************//**
		  *	@brief		Stk_SpacialPointEntity析构函数
		  *	@param[in]	无
		  *	@retval		无
		  *	@note		无
		  *	@attention	无
		  ****************************************************************************/

		Stk_SpacialPointEntity::~Stk_SpacialPointEntity()
		{
			DELETE_STK_CLASS_IMPL
		}

		/************************************************************************
		*	@brief		获取ModelID
		*	@param[in]	无
		*	@param[out]	无
		*	@retval		STK_ID
		*	@note		无
		*	@attention	无
		****************************************************************************/

		STK_ID Stk_SpacialPointEntity::GetModelID()
		{
			return ((Stk_SpacialPointEntityImpl*)m_pImpl)->m_iModelID;
		}
		/************************************************************************
		*	@brief		获取ModelID
		*	@param[in]	无
		*	@param[out]	无
		*	@retval		STK_ID
		*	@note		无
		*	@attention	无
		****************************************************************************/

		STK_STATUS Stk_SpacialPointEntity::SetModelID(STK_ID i_ID)
		{
			((Stk_SpacialPointEntityImpl*)m_pImpl)->m_iModelID = i_ID;
			return STK_SUCCESS;
		}

		STK_STATUS Stk_SpacialPointEntity::SetName(wstring i_wstrName)
		{
			((Stk_SpacialPointEntityImpl*)m_pImpl)->m_wstrName = i_wstrName;
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
			return ((Stk_SpacialPointEntityImpl*)m_pImpl)->m_wstrName;
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
			((Stk_SpacialPointEntityImpl*)m_pImpl)->m_bDispStat = i_bDispStat;
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
			return ((Stk_SpacialPointEntityImpl*)m_pImpl)->m_bDispStat;
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
			((Stk_SpacialPointEntityImpl*)m_pImpl)->m_eType = i_eType;
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
			return ((Stk_SpacialPointEntityImpl*)m_pImpl)->m_eType;
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
			((Stk_SpacialPointEntityImpl*)m_pImpl)->m_rgbaPoint = i_rgbaSpacialPoint;
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
			return ((Stk_SpacialPointEntityImpl*)m_pImpl)->m_rgbaPoint;
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
			((Stk_SpacialPointEntityImpl*)m_pImpl)->m_pntPos = i_pntPos;
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
			return ((Stk_SpacialPointEntityImpl*)m_pImpl)->m_pntPos;
		}

		STK_STATUS Stk_SpacialPointEntity::FromJson(string i_strJson)
		{
			return ((Stk_SpacialPointEntityImpl*)m_pImpl)->FromJson(i_strJson);
		}

		string Stk_SpacialPointEntity::ToJson()
		{
			return ((Stk_SpacialPointEntityImpl*)m_pImpl)->ToJson();
		}
	}
}
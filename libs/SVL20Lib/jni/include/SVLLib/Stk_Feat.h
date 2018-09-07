// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_Feat.h
 *
 *	@brief	特征类
 *
 *	@par	历史:
 *		2014/05/28	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _SVLLIB_FEATENTITY_H_
#define _SVLLIB_FEATENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Object.h"
#include "Stk_Enum.h"
#include "Stk_Feat_Pipe.h"
#include "Stk_Feat_Hole.h"
#include "Stk_Feat_MagneticLine.h"
#include "Stk_Feat_Mark.h"

namespace HoteamSoft {
	namespace SVLLib {
		/****************************************************************************
			 *	@class	Stk_Feat
			 *	@brief	特征类
			 *
			 *	创建用于存储Node节点的特征的类库。<br>
			 *	继承自Stk_Entity 类。
		 ****************************************************************************/

		class STK_TOOLKIT_EXPORT Stk_Feat :
			public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Feat)
		protected:
			enum { _typeID = _SVLLIB_TYPE_FEAT };	//!< 类别
		private:
			STK_ID					m_iModelID;				//!< 所属模型ID
			StkFeatTypeEnum			m_eType;				//!< 特征类型
			STK_BOOL				m_bActive;				//!< 是否活动状态
			wstring					m_wstrName;				//!< 特征名称
			Stk_ObjectPtr			m_dataP;				//!< 特征数据
		public:
			/****************************************************************************
			 *	@brief		Stk_Feat构造函数
			 *	@param[in]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_Feat();

			/****************************************************************************
			 *	@brief		Stk_Feat析构函数
			 *	@param[in]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			~Stk_Feat();
		public:
			/****************************************************************************
			*	@brief		设置所属模型ID
			*	@param[in]	i_Id			特征ID
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void			SetModeID(STK_UINT32 i_Id) { m_iModelID = i_Id; }

			/****************************************************************************
			*	@brief		获取所属模型ID
			*	@param[in]	无
			*	@retval		所属模型ID
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_UINT32				GetModeID() { return m_iModelID; }

			/****************************************************************************
			 *	@brief		设置特征类型
			 *	@param[in]	i_eType			特征类型
			 *	@retval		STK_SUCCESS		设置成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				SetFeatType(StkFeatTypeEnum i_eType);

			/****************************************************************************
			 *	@brief		获取特征类型
			 *	@param[in]	无
			 *	@retval		m_eType			特征类型
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			StkFeatTypeEnum			GetFeatType();

			/****************************************************************************
			 *	@brief		是否活动状态
			 *	@param[in]	无
			 *	@retval		m_bActive		是否活动状态
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_BOOL				IsActiveed();

			/****************************************************************************
			 *	@brief		设置活动状态
			 *	@param[in]	i_bActived		活动状态
			 *	@retval		STK_SUCCESS		设置成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				SetActiveed(STK_BOOL i_bActived);

			/****************************************************************************
			 *	@brief		设置特证名
			 *	@param[in]	i_wstrName		特证名
			 *	@retval		STK_SUCCESS		设置成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				SetFeatName(wstring i_wstrName);

			/****************************************************************************
			 *	@brief		获取特证名
			 *	@param[in]	无
			 *	@retval		m_wstrName		特证名
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			wstring					GetFeatName();

			/****************************************************************************
			 *	@brief		设置线缆特征
			 *	@param[in]	i_FeatPipe		线缆特征对象指针
			 *	@retval		STK_SUCCESS		设置成功
			 *	@retval		其他			设置失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				SetFeatPipe(Stk_Feat_PipePtr i_FeatPipe);

			/****************************************************************************
			 *	@brief		获取线缆特征
			 *	@param[in]	无
			 *	@retval		m_dataP			线缆特征
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_Feat_PipePtr			GetFeatPipe();

			/****************************************************************************
			*	@brief		设置孔特征
			*	@param[in]	i_FeatHole		孔特征对象指针
			*	@retval		STK_SUCCESS		设置成功
			*	@retval		其他			设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetFeatHole(Stk_Feat_HolePtr i_FeatHole);

			/****************************************************************************
			*	@brief		获取孔特征
			*	@param[in]	无
			*	@retval		m_dataP			孔特征
			*	@note		无
			*	@attention	无
			****************************************************************************/
			Stk_Feat_HolePtr			GetFeatHole();


			/****************************************************************************
			 *	@brief		设置磁力线特征
			 *	@param[in]	i_MagneticLineP	磁力线特征对象
			 *	@retval		STK_SUCCESS		设置成功
			 *	@retval		其他			设置失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				SetMagneticLine(Stk_Feat_MagneticLinePtr i_MagneticLineP);

			/****************************************************************************
			 *	@brief		获取磁力线特征
			 *	@param[in]	无
			 *	@retval		磁力线特征
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_Feat_MagneticLinePtr	GetMagneticLine();

			/****************************************************************************
			 *	@brief		设置标记特征
			 *	@param[in]	i_FeatMarkP		标记特征对象
			 *	@retval		STK_SUCCESS		设置成功
			 *	@retval		其他			设置失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				SetFeatMark(Stk_Feat_MarkPtr i_FeatMarkP);

			/****************************************************************************
			 *	@brief		获取标记特征
			 *	@param[in]	无
			 *	@retval		标记特征
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_Feat_MarkPtr			GetFeatMark();

		};
	}
}

#endif
// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_ComponentFeature.h
 *
 *	@brief	组件特征类，区别与DTK中有关Node下的特征，该特征目前仅指组、阵列特征、装配特征
 *
 *	@par	历史:
 *		2014/11/11	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _SVLLIB_COMPONENTFEATURE_ENTITY_H_
#define _SVLLIB_COMPONENTFEATURE_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Object.h"
#include "Stk_Enum.h"
#include "Stk_File.h"

namespace HoteamSoft {
	namespace SVLLib {
		/****************************************************************************
			 *	@class	Stk_ComponentFeature
			 *	@brief	组件特征类
			 *
			 *	区别与DTK中有关Node下的特征，该特征目前仅指组、阵列特征、装配特征。<br>
			 *	继承自Stk_Entity 类。
		 ****************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_ComponentFeature :
			public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_ComponentFeature)
		protected:
			enum { _typeID = _SVLLIB_TYPE_COMPONENT_FEATURE };	//!< 类别
		private:
			STK_ID							m_stkModelId;			//!< 所属零件ID
			wstring							m_wstrName;				//!< 特征名称
			STK_ID							m_ID;					//!< 特征ID
			STK_INT32						m_nNumber;				//!< 特征编号
			StkComponentFeatureEnum			m_eType;				//!< 组件特征类型
			vector<Stk_ComponentFeaturePtr>	m_vcChildFeats;			//!< 子特征
		public:
			/*****************************************************************************
			 *	@brief		构造函数
			 *	@param[in]	i_stkDocManagerP		Document管理器对象指针
			 *	@param[in]	i_stkProtoP				ProtoType
			 *	@param[out]	无
			 *	@retval		无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_ComponentFeature(void);

			/*****************************************************************************
			 *	@brief		析构函数
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			virtual ~Stk_ComponentFeature();
		public:
			/****************************************************************************
			 *	@brief		设置实例名
			 *	@param[in]	i_wstrName		特征名
			 *	@param[out]	无
			 *	@retval		无
			 *	@note		STK_SUCCESS		设置成功
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				SetName(wstring i_wstrName);

			/****************************************************************************
			 *	@brief		获取特征名
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		特征名
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			wstring					GetName();
			
			/****************************************************************************
			 *	@brief		获取组件特征ID
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		组件特征ID
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_ID					GetID();

			/****************************************************************************
			 *	@brief		设置特征编号
			 *	@param[in]	i_nNumber			特征编号
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS			设置成功
			 *	@retval		STK_ERROR			设置失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				SetNumber(STK_INT32 i_nNumber);

			/****************************************************************************
			 *	@brief		获取特征编号
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		特征编号
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_INT32				GetNumber();

			/****************************************************************************
			 *	@brief		设置组件特征类型
			 *	@param[in]	i_eType			组件特征类型
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS		设置成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				SetType(StkComponentFeatureEnum i_eType);

			/****************************************************************************
			 *	@brief		获取组件特征类型
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		组件特征类型
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			StkComponentFeatureEnum	GetType();

			/****************************************************************************
			 *	@brief		添加子组件特征
			 *	@param[in]	i_ComponentFeatP		子组件特征
			 *	@param[out]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				AddChildComponentFeat(Stk_ComponentFeaturePtr i_ComponentFeatP);

			/****************************************************************************
			 *	@brief		获取子组件特征的数量
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		子特征的数量
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_UINT32				GetChildFeatNum();

			/****************************************************************************
			 *	@brief		获取指定的子组件特征
			 *	@param[in]	i_nuIndex			索引
			 *	@param[out]	无
			 *	@retval		指定的子特征ID
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_ComponentFeaturePtr	GetChildComponentFeatByIndex(STK_UINT32 i_nuIndex);

			/****************************************************************************
			*	@brief		设置当前特征所从属的ModelID
			*	@param[in]	i_ModelId	ModelID
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void			SetModelID(STK_ID i_ModelId) { m_stkModelId = i_ModelId; }

			/****************************************************************************
			 *	@brief		获取当前特征所从属的ModelID
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		当前特征所从属的Model
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_ID			GetModelID() { return m_stkModelId; }
		};
	}
}
#endif
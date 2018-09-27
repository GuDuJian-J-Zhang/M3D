// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_ComponentFeature.cpp
 *
 *	@brief	组件特征类，区别与DTK中有关Node下的特征，该特征目前仅值组、阵列特征
 *
 *	@par	历史:
 *		2014/11/11	创建。WangY@HOTEAMSOFT
****************************************************************************/


#include "Stk_Define.h"
#include "Stk_ComponentFeature.h"

namespace HoteamSoft {
	namespace SVLLib {
		STK_IMPLEMENT_CLASS(Stk_ComponentFeature, Stk_Object)
		/*************************************************************************//**
		 *	@brief		构造函数
		 *	@param[in]	i_stkDocManagerP		Document管理器对象指针
		 *	@param[in]	i_stkProtoP				ProtoType
		 *	@param[out]	无
		 *	@retval		无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_ComponentFeature::Stk_ComponentFeature() : Stk_Object()
		{
			m_ID = 0;
			m_eType = TYPE_COMPONENTFEATURE_UNKNOWN;
			m_nNumber = 0;
		}

		/*************************************************************************//**
		 *	@brief		析构函数
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_ComponentFeature::~Stk_ComponentFeature()
		{
			m_vcChildFeats.clear();
		}

		/************************************************************************//**
		 *	@brief		获取组件特征ID
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		组件特征ID
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_ID Stk_ComponentFeature::GetID()
		{
			return m_ID;
		}

		/****************************************************************************
		 *	@brief		设置特征编号
		 *	@param[in]	i_nNumber			特征编号
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@retval		STK_ERROR			设置失败
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_ComponentFeature::SetNumber(STK_INT32 i_nNumber)
		{
			m_nNumber = i_nNumber;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取特征编号
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		特征编号
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_INT32 Stk_ComponentFeature::GetNumber()
		{
			return m_nNumber;
		}

		/************************************************************************//**
		 *	@brief		设置组件特征类型
		 *	@param[in]	i_eType			组件特征类型
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS		设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_ComponentFeature::SetType(StkComponentFeatureEnum i_eType)
		{
			m_eType = i_eType;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取组件特征类型
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		组件特征类型
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		StkComponentFeatureEnum Stk_ComponentFeature::GetType()
		{
			return m_eType;
		}

		/************************************************************************//**
		 *	@brief		添加子组件特征
		 *	@param[in]	i_ComponentFeatP		子组件特征
		 *	@param[out]	无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_ComponentFeature::AddChildComponentFeat(Stk_ComponentFeaturePtr i_ComponentFeatP)
		{
			if (i_ComponentFeatP != NULL)
			{
				m_vcChildFeats.push_back(i_ComponentFeatP);
				return STK_SUCCESS;
			}
			else
			{
				return STK_ERROR;
			}
		}

		/************************************************************************//**
		 *	@brief		获取子组件特征的数量
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		子特征的数量
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_UINT32 Stk_ComponentFeature::GetChildFeatNum()
		{
			return m_vcChildFeats.size();
		}

		/************************************************************************//**
		 *	@brief		获取指定的子组件特征
		 *	@param[in]	i_nuIndex			索引
		 *	@param[out]	无
		 *	@retval		指定的子特征ID
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_ComponentFeaturePtr Stk_ComponentFeature::GetChildComponentFeatByIndex(STK_UINT32 i_nuIndex)
		{
			return m_vcChildFeats.at(i_nuIndex);
		}

		/************************************************************************//**
		 *	@brief		设置实例名
		 *	@param[in]	i_wstrName		特征名
		 *	@param[out]	无
		 *	@retval		无
		 *	@note		STK_SUCCESS		设置成功
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_ComponentFeature::SetName(wstring i_wstrName)
		{
			m_wstrName = i_wstrName;
			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取特征名
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		特征名
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		wstring Stk_ComponentFeature::GetName()
		{
			return m_wstrName;
		}
	}
}
// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_AssemblyFeature.cpp
 *
 *	@brief	装配特征类
 *
 *	@par	历史:
 *		2014/11/17	创建。WangY@HOTEAMSOFT
****************************************************************************/

#include "Stk_AssemblyFeature.h"

namespace HoteamSoft {
	namespace SVLLib {
		STK_IMPLEMENT_CLASS(Stk_AssemblyFeature, Stk_ComponentFeature)
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
		Stk_AssemblyFeature::Stk_AssemblyFeature()
			: Stk_ComponentFeature()
		{

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
		Stk_AssemblyFeature::~Stk_AssemblyFeature()
		{

		}

		/****************************************************************************
		 *	@brief		添加装配特征中所包含的要素Node ID
		 *	@param[in]	i_idPlacement		配置ID
		 *	@param[out]	无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_AssemblyFeature::AddNodeID(STK_ID i_idPlacement)
		{
			m_vcNodeIds.push_back(i_idPlacement);
			return STK_SUCCESS;
		}

		/****************************************************************************
		 *	@brief		获取装配特征中所包含的要素Node数量
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		Node数量
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_UINT32 Stk_AssemblyFeature::GetNodeNum()
		{
			return m_vcNodeIds.size();
		}

		/****************************************************************************
		 *	@brief		获取指定的Node ID
		 *	@param[in]	i_nuIndex			索引
		 *	@param[out]	无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_ID Stk_AssemblyFeature::GetNodeIDByIndex(STK_UINT32 i_nuIndex)
		{
			return m_vcNodeIds.at(i_nuIndex);
		}

		/****************************************************************************
		 *	@brief		设置装配特征关联的实例的配置路径
		 *	@param[in]	i_wstrPlcPath		配置路径
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_AssemblyFeature::SetPlcPath(wstring i_wstrPlcPath)
		{
			m_wstrPlcPath = i_wstrPlcPath;
			return STK_SUCCESS;
		}

		/****************************************************************************
		 *	@brief		获取装配特征关联的实例的配置路径
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		配置路径
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		wstring Stk_AssemblyFeature::GetPlcPath()
		{
			return m_wstrPlcPath;
		}
	}
}
// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_InsAttribute.h
 *
 *	@brief	实例属性：颜色、显隐、透明度等
 *
 *	@par	历史:
 *		2015/03/24	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _SVLLIB_InsAttribute_H_
#define _SVLLIB_InsAttribute_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Define.h"
//#include "Stk_File.h"
#include "Stk_Enum.h"
#include "Stk_Object.h"
//#include "Stk_DocumentImpl.h"

namespace HoteamSoft {
	namespace SVLLib {
		/**************************************************************************************
			 *	@class	Stk_InsAttribute
			 *	@brief	实例基本属性类
			 *
			 *	实例属性：颜色、显隐、透明度等
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_InsAttribute :
			public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_InsAttribute)
		protected:
			enum { _typeID = _SVLLIB_TYPE_INSATTIBUTE };	//!< 类别
		
		public:
			/****************************************************************************
			 *	@brief		Stk_InsAttribute构造函数
			 *	@param[in]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_InsAttribute();

			/****************************************************************************
			 *	@brief		Stk_InsAttribute析构函数
			 *	@param[in]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			~Stk_InsAttribute();
		public:
			/*****************************************************************************
			 *	@brief		设置实例显隐状态
			 *	@param[in]	i_eDisplayState		实例显隐状态
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS			设置成功
			 *	@retval		其他				设置失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS			SetDisplayState(StkDisplayStateEnum i_eDisplayState);

			/*****************************************************************************
			 *	@brief		获取实例显隐状态
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		实例显隐状态
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			StkDisplayStateEnum	GetDisplayState();

			/*****************************************************************************
			 *	@brief		设置实例颜色
			 *	@param[in]	i_rgbaIns			实例颜色
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS			设置成功
			 *	@retval		其他				设置失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS			SetColor(STK_RGBA32 i_rgbaIns);

			/*****************************************************************************
			 *	@brief		获取实例颜色
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		实例颜色
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_RGBA32&			GetColor();

			/*****************************************************************************
			 *	@brief		设置实例材质
			 *	@param[in]	i_matIns			实例材质
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS			设置成功
			 *	@retval		其他				设置失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS			SetMaterial(STK_MAT32 i_matIns);

			/*****************************************************************************
			 *	@brief		获取实例材质
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		实例材质
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_MAT32&			GetMaterial();

			/****************************************************************************
			 *	@brief		设置相对于父级的配置ID，配置矩阵
			 *	@param[in]	i_idPlacement			配置ID
			 *	@param[in]	i_mtxPlcMatrix			配置矩阵
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS			SetPlacement(STK_ID i_idPlacement, const STK_MTX32 &i_mtxPlcMatrix);

			/****************************************************************************
			 *	@brief		获取相对于父级的配置ID，配置矩阵
			 *	@param[out]	o_idPlacement			配置ID
			 *	@param[out]	o_mtxPlcMatrix			配置矩阵
			 *	@retval		STK_SUCCESS				获取成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS			GetPlacement(STK_ID &o_idPlacement, STK_MTX32 &o_mtxPlcMatrix);	

			/****************************************************************************
			*	@brief		读取文件中的Body数据
			*	@param[in]	i_pJsonNode			View 父Json节点
			*	@param[in]	i_JsonDocP			Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			读取成功
			*	@retval		STK_ERROR			读取失败
			*	@retval		STK_ERROR_MEMORY	内存申请失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS					FromJson(void* i_pJsonNode, void* i_JsonDocP);

			/*****************************************************************************
			*	@brief		写View数据
			*	@param[in]	Value* i_pJsonParentNode			View 父Json节点
			*	@param[in]	i_JsonDocP			Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			写入成功
			*	@retval		其它				写入失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS					ToJson(void* i_pJsonNode, void* i_JsonDocP);
		};
	}//SVLLib
}//HoteamSoft

#endif
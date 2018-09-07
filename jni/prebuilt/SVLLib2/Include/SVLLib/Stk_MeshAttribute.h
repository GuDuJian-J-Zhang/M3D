// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_MeshAttribute
*	@brief	几何属性类
*	@par 历史记录
*				2017/07/14 创建。qinyp(Hoteamsoft)
**************************************************************************/
#pragma once

#include "Stk_Object.h"

#ifndef _SVLLIB_MESHATTRIBUTE_H_
#define _SVLLIB_MESHATTRIBUTE_H_
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_MeshAttribute : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_MeshAttribute)
		public:
			Stk_MeshAttribute();
			virtual ~Stk_MeshAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_MESHATTRIBUTE
			};
		public:
			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		设置几何属性
			*	@param[in]	i_geoAttrP		几何属性
			*	@param[out]	无
			*	@retval		STK_SUCCESS	成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS                  SetGeometryAttribute(Stk_GeometryAttributePtr i_geoAttrP);
		};
	}
}

#endif
// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_PlaneEntity.h
*
*	@brief	面要素类
*
*	@par	历史:
*
****************************************************************************/
#ifndef _SVLLIB_PLANEENTITY_H_
#define _SVLLIB_PLANEENTITY_H_
#pragma once
#include "Stk_Object.h"
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_PlaneEntity : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_PlaneEntity)
		protected:
			enum { _typeID = _SVLLIB_TYPE_PLANE };	//!< 类别
		public:
			Stk_PlaneEntity();
			virtual ~Stk_PlaneEntity();
		protected:
			StkPlaneTypeEnum	m_ePlaneType;	     //!< 面类型
		public:
			/****************************************************************************
			*	@brief		获取剖面类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_nuType		剖面类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			StkPlaneTypeEnum	GetType();

			/****************************************************************************
			*	@brief		获取剖面颜色
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_rgbaSurface	剖面颜色
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_RGBA32			GetColor();

			/****************************************************************************
			*	@brief		设置剖面颜色
			*	@param[in]	i_rgbaSurface	剖面颜色
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetColor(const STK_RGBA32& i_rgbaSurface);

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual    STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual     STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_ClipPlane
		*	@brief	Stk_ClipPlane 类
		*
		*	继承自Stk_SurfaceEntity类。
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_ClipPlane : public Stk_PlaneEntity
		{
			STK_DECLARE_CLASS(Stk_ClipPlane)
		protected:
			enum { _typeID = _SVLLIB_TYPE_PLANE_CLIP };	//!< 类别
		public:
			Stk_ClipPlane();
			virtual ~Stk_ClipPlane();
		public:
			/****************************************************************************
			*	@brief		获取剖面的某点位置
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntOrigin		剖面上某点位置
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32			GetOrigin();

			/****************************************************************************
			*	@brief		设置剖面的某点位置
			*	@param[in]	i_pntOrigin		剖面上某点位置
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void				SetOrigin(const STK_PNT32 &i_pntOrigin);

			/****************************************************************************
			*	@brief		获取剖面法向量
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_dirNormal		剖面法向量
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DIR32			GetNormal();

			/****************************************************************************
			*	@brief		设置剖面法向量
			*	@param[in]	i_dirNormal		剖面法向量
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void				SetNormal(const STK_DIR32 &i_dirNormal);

			/****************************************************************************
			*	@brief		获取剖面UV向量
			*	@param[in]	o_dirU		剖面U向量
			*	@param[in]	o_dirV		剖面V向量
			*	@param[out]	无
			*	@retval		STK_SUCCESS	获取成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			GetUVDir(STK_DIR32 &o_dirU, STK_DIR32 &o_dirV);

			/****************************************************************************
			*	@brief		设置剖面UV向量
			*	@param[in]	i_dirU		剖面U向量
			*	@param[in]	i_dirV		剖面V向量
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void				SetUVDir(const STK_DIR32 &i_dirU, const STK_DIR32 &i_dirV);

			/****************************************************************************
			*	@brief		获取剖切深度:原点与剖切面深度
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		深度
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32			GetDistance();

			/****************************************************************************
			*	@brief		设置剖切深度:原点与剖切面深度
			*	@param[in]	i_fDistance		深度
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetDistance(STK_FLOAT32 i_fDistance);

			/****************************************************************************
			*	@brief		获取剖切线对象
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		剖切线对象
			*	@note		无
			*	@attention	无
			****************************************************************************/
			Stk_ClippingCurvePtr	GetClippingCurve();

			/****************************************************************************
			*	@brief		设置剖切线对象
			*	@param[in]	i_stkClippingCurveP	剖切线对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetClippingCurve(Stk_ClippingCurvePtr i_stkClippingCurveP);

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
		};

		/**************************************************************************************
		*	@class	Stk_ClippingCurve
		*	@brief	剖切阴影线类
		*
		*	继承自Stk_Entity类。
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_ClippingCurve : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_ClippingCurve)
		protected:
			enum {      _typeID = _SVLLIB_TYPE_CLIPPINGCURVE     };	//!< 类别
		public:
			Stk_ClippingCurve();
			~Stk_ClippingCurve();
		public:
			/****************************************************************************
			*	@brief		设置剖切线显隐状态
			*	@param[in]	i_bDisplay		显隐状态
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetDisplay(STK_BOOL i_bDisplay);

			/****************************************************************************
			*	@brief		获取剖切线显隐状态
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		显隐状态
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_BOOL		GetDisplay();

			/****************************************************************************
			*	@brief		设置剖切线绘制方式
			*	@param[in]	i_eDrawStyle	绘制方式
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetDrawStyle(StkClippingCurveDrawStyleEnum i_eDrawStyle);

			/****************************************************************************
			*	@brief		获取剖切线绘制方式
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		绘制方式
			*	@note		无
			*	@attention	无
			****************************************************************************/
			StkClippingCurveDrawStyleEnum	GetDrawStyle();

			/****************************************************************************
			*	@brief		设置剖切线绘制显示类型
			*	@param[in]	i_eDrawType		剖切线绘制显示类型
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetDrawType(StkClippingCurveDrawTypeEnum i_eDrawType);

			/****************************************************************************
			*	@brief		获取剖切线绘制显示类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		剖切线绘制显示类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			StkClippingCurveDrawTypeEnum	GetDrawType();

			/****************************************************************************
			*	@brief		设置剖切线角度
			*	@param[in]	i_fAngle		剖切线角度
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetAngle(STK_FLOAT32 i_fAngle);

			/****************************************************************************
			*	@brief		获取剖切线角度
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		剖切线角度
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32		GetAngle();

			/****************************************************************************
			*	@brief		设置剖切线显示比例
			*	@param[in]	i_fRatio		剖切线显示比例
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetRatio(STK_FLOAT32 i_fRatio);

			/****************************************************************************
			*	@brief		获取剖切线显示比例
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		剖切线显示比例
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32		GetRatio();

			/****************************************************************************
			*	@brief		设置剖切线显示颜色
			*	@param[in]	i_rgbaCurve		剖切线显示颜色
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetColor(STK_RGBA32 i_rgbaCurve);

			/****************************************************************************
			*	@brief		获取剖切线显示颜色
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		剖切线显示颜色
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_RGBA32		GetColor();

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
		};
	}
}

#endif // _SVLLIB_PLANEENTITY_H_
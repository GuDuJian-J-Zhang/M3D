// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_GeometryAttribute
*	@brief	几何属性类
*	@par 历史记录
*				2017/07/14 创建。qinyp(Hoteamsoft)
**************************************************************************/
#pragma once

#include "Stk_Object.h"

#ifndef _SVLLIB_GEOMETRYATTRIBUTE_H_
#define _SVLLIB_GEOMETRYATTRIBUTE_H_
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_GeometryAttribute : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_GeometryAttribute)
		public:
			Stk_GeometryAttribute();
			virtual ~Stk_GeometryAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_GEOMETRYATTRIBUTE
			};
		public:
			/****************************************************************************
			*	@brief		获取所属要素的类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_eGeoAttrType	所属要素的类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual StkGeoAttrTypeEnum	GetGeoAttrType();

			/****************************************************************************
			*	@brief		设置所属要素的类型
			*	@param[in]	i_eAttrType		所属要素的类型
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS			SetGeoAttrType(StkGeoAttrTypeEnum i_eAttrType);

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_PlaneFaceAttribute
		*	@brief	平面属性类
		*
		*	记录平面的基本属性信息。<br>
		*	继承自Stk_GeometryAttribute 类。
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_PlaneFaceAttribute : public Stk_GeometryAttribute
		{
			STK_DECLARE_CLASS(Stk_PlaneFaceAttribute)
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_PLANEFACEATTRIBUTE
			};
		public:
			Stk_PlaneFaceAttribute();
			virtual ~Stk_PlaneFaceAttribute();
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
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		设置平面的法向
			*	@param[in]	i_dirNormal		平面法向
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置平面法向成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetNormal(const STK_DIR32 &i_dirNormal);

			/****************************************************************************
			*	@brief		获取平面的法向
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_dirNormal		平面法向
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DIR32			GetNormal();

			/****************************************************************************
			*	@brief		设置平面的原点
			*	@param[in]	i_pntOrigin		平面原点
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置平面原点成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetOrigin(const STK_PNT32 &i_pntOrigin);

			/****************************************************************************
			*	@brief		获取平面的原点
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntOrigin		平面原点
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32			GetOrigin();
		};

		/**************************************************************************************
		*	@class	Stk_RevolutionFaceAttribute
		*	@brief	旋转面属性基类
		*
		*	记录旋转面的基本属性信息。<br>
		*	继承自Stk_GeometryAttribute 类。
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_RevolutionFaceAttribute : public Stk_GeometryAttribute
		{
			STK_DECLARE_CLASS(Stk_RevolutionFaceAttribute)
		public:
			Stk_RevolutionFaceAttribute();
			virtual ~Stk_RevolutionFaceAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_REVOLUTIONATTRIBUTE
			};
		public:
			/****************************************************************************
			*	@brief		设置旋转面半径
			*	@param[in]	i_dRadius		旋转面半径
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置旋转面半径成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetOriginRadius(STK_DOUBLE64 i_dRadius);

			/****************************************************************************
			*	@brief		获取旋转面半径
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_dRadius		旋转面半径
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DOUBLE64		GetOriginRadius();

			/****************************************************************************
			*	@brief		设置旋转面旋转轴
			*	@param[in]	i_dirRevoAxis		旋转面旋转轴方向向量
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置旋转面旋转轴成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetRevoAxis(const STK_DIR32 &i_dirRevoAxis);

			/****************************************************************************
			*	@brief		获取旋转面旋转轴方向向量
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_dirRevoAxis		旋转面旋转轴方向向量
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DIR32			GetRevoAxis();

			/****************************************************************************
			*	@brief		设置旋转面的旋转轴原点
			*	@param[in]	i_pntAxisOrigin		旋转面的旋转轴原点
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置旋转轴的原点成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetAxisOrigin(const STK_PNT32 &i_pntAxisOrigin);

			/****************************************************************************
			*	@brief		获取旋转面的旋转轴原点
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntAxisOrigin			旋转轴的原点
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32			GetAxisOrigin();

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);

		};

		/**************************************************************************************
		*	@class	Stk_CylinderFaceAttribute
		*	@brief	圆柱面属性基类
		*
		*	记录圆柱面的基本属性信息。<br>
		*	继承自Stk_GeometryAttribute 类。
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_CylinderFaceAttribute : public Stk_GeometryAttribute
		{
			STK_DECLARE_CLASS(Stk_CylinderFaceAttribute)
		public:
			Stk_CylinderFaceAttribute();
			virtual ~Stk_CylinderFaceAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_CYLINDERATTRIBUTE
			};
		public:
			/****************************************************************************
			*	@brief		设置圆柱面半径
			*	@param[in]	i_dRadius		圆柱面半径
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置圆柱面半径成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetOriginRadius(STK_DOUBLE64 i_dRadius);

			/****************************************************************************
			*	@brief		获取圆柱面半径
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_dRadius		圆柱面半径
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DOUBLE64		GetOriginRadius();

			/****************************************************************************
			*	@brief		设置圆柱面旋转轴
			*	@param[in]	i_dirRevoAxis		圆柱面旋转轴方向向量
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置圆柱面旋转轴成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetRevoAxis(const STK_DIR32 &i_dirRevoAxis);

			/****************************************************************************
			*	@brief		获取圆柱面旋转轴方向向量
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_dirRevoAxis		圆柱面旋转轴方向向量
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DIR32			GetRevoAxis();

			/****************************************************************************
			*	@brief		设置圆柱面的旋转轴原点
			*	@param[in]	i_pntAxisOrigin		圆柱面的旋转轴原点
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置旋转轴的原点成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetAxisOrigin(const STK_PNT32 &i_pntAxisOrigin);

			/****************************************************************************
			*	@brief		获取圆柱面的旋转轴原点
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntAxisOrigin			旋转轴的原点
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32			GetAxisOrigin();

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);

		};

		/**************************************************************************************
		*	@class	Stk_ConeFaceAttribute
		*	@brief	圆锥面属性基类
		*
		*	记录圆锥面的基本属性信息。<br>
		*	继承自Stk_GeometryAttribute 类。
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_ConeFaceAttribute : public Stk_GeometryAttribute
		{
			STK_DECLARE_CLASS(Stk_ConeFaceAttribute)
		public:
			Stk_ConeFaceAttribute();
			virtual ~Stk_ConeFaceAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_CONEATTRIBUTE
			};
		public:
			/****************************************************************************
			*	@brief		设置圆锥面半径
			*	@param[in]	i_dRadius		圆锥面半径
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置圆锥面半径成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetOriginRadius(STK_DOUBLE64 i_dRadius);

			/****************************************************************************
			*	@brief		获取圆锥面半径
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_dRadius		圆锥面半径
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DOUBLE64		GetOriginRadius();

			/****************************************************************************
			*	@brief		设置圆锥面旋转轴
			*	@param[in]	i_dirRevoAxis		圆锥面旋转轴方向向量
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置旋转面旋转轴成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetRevoAxis(const STK_DIR32 &i_dirRevoAxis);

			/****************************************************************************
			*	@brief		获取圆锥面旋转轴方向向量
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_dirRevoAxis		圆锥面旋转轴方向向量
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DIR32			GetRevoAxis();

			/****************************************************************************
			*	@brief		设置圆锥面的旋转轴原点
			*	@param[in]	i_pntAxisOrigin		圆锥面的旋转轴原点
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置旋转轴的原点成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetAxisOrigin(const STK_PNT32 &i_pntAxisOrigin);

			/****************************************************************************
			*	@brief		获取圆锥面的旋转轴原点
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntAxisOrigin			旋转轴的原点
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32			GetAxisOrigin();

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);

		};

		/**************************************************************************************
		*	@class	Stk_SphereFaceAttribute
		*	@brief	球面属性基类
		*
		*	记录球面的基本属性信息。<br>
		*	继承自Stk_GeometryAttribute 类。
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_SphereFaceAttribute : public Stk_GeometryAttribute
		{
			STK_DECLARE_CLASS(Stk_SphereFaceAttribute)
		public:
			Stk_SphereFaceAttribute();
			virtual ~Stk_SphereFaceAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_SPHEREATTRIBUTE
			};
		public:
			/****************************************************************************
			*	@brief		设置球面半径
			*	@param[in]	i_dRadius		旋转面半径
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置旋转面半径成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetOriginRadius(STK_DOUBLE64 i_dRadius);

			/****************************************************************************
			*	@brief		获取球面半径
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_dRadius		旋转面半径
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DOUBLE64		GetOriginRadius();

			/****************************************************************************
			*	@brief		设置球面旋转轴
			*	@param[in]	i_dirRevoAxis		旋转面旋转轴方向向量
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置旋转面旋转轴成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetRevoAxis(const STK_DIR32 &i_dirRevoAxis);

			/****************************************************************************
			*	@brief		获取球面旋转轴方向向量
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_dirRevoAxis		旋转面旋转轴方向向量
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DIR32			GetRevoAxis();

			/****************************************************************************
			*	@brief		设置球面的旋转轴原点
			*	@param[in]	i_pntAxisOrigin		旋转面的旋转轴原点
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置旋转轴的原点成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS			SetAxisOrigin(const STK_PNT32 &i_pntAxisOrigin);

			/****************************************************************************
			*	@brief		获取球面的旋转轴原点
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntAxisOrigin			旋转轴的原点
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32			GetAxisOrigin();

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);

		};

		/**************************************************************************************
		*	@class	Stk_ToroidalFaceAttribute
		*	@brief	轮胎面属性类
		*
		*	记录轮胎面的基本属性信息。<br>
		*	继承自Stk_RevolutionFaceAttribute 类。
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_ToroidalFaceAttribute : public Stk_RevolutionFaceAttribute
		{
			STK_DECLARE_CLASS(Stk_ToroidalFaceAttribute)
		public:
			Stk_ToroidalFaceAttribute();
			virtual ~Stk_ToroidalFaceAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_TOROIDALFACEATTRIBUTE
			};
		public:
			/****************************************************************************
			*	@brief		获取轮胎面的外径
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fMajorRadius			轮胎面的外径
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32		GetMajorRadius();

			/****************************************************************************
			*	@brief		设置轮胎面的外径
			*	@param[in]	i_fRadius			轮胎面的外径
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetMajorRaius(STK_FLOAT32 i_fRadius);

			/****************************************************************************
			*	@brief		获取轮胎面的内径
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fMinorRadius			轮胎面的内径
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32		GetMinorRadius();

			/****************************************************************************
			*	@brief		设置轮胎面的内径
			*	@param[in]	i_fRadius			轮胎面的内径
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetMinorRadius(STK_FLOAT32 i_fRdius);

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/****************************************************************************************
		*	@class	Stk_LineAttribute
		*	@brief	Line属性类
		*
		*	记录Line的基本属性信息。<br>
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_LineAttribute : public Stk_GeometryAttribute
		{
			STK_DECLARE_CLASS(Stk_LineAttribute)
		public:
			Stk_LineAttribute();
			virtual ~Stk_LineAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_LINEATTRIBUTE
			};
		public:
			/*****************************************************************************
			*	@brief		获取直线中点
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntCenterPoint	直线中点
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32		GetCenterPoint();

			/*****************************************************************************
			*	@brief		设置直线中点
			*	@param[in]	i_pntCenter	直线中点
			*	@param[out]	无
			*	@retval		STK_SUCCESS	设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetCenterPoint(STK_PNT32 &i_pntCenter);

			/*****************************************************************************
			*	@brief		获取直线方向向量
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_dirDirection	直线方向向量
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DIR32		GetDirection();

			/*****************************************************************************
			*	@brief		设置直线方向向量
			*	@param[in]	i_dirDirect	直线方向向量
			*	@param[out]	无
			*	@retval		STK_SUCCESS	设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetDirection(STK_DIR32 &i_dirDirect);

			/*****************************************************************************
			*	@brief		获取边界线起始点
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntStartPoint	边界线起始点
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32		GetStartPoint();

			/*****************************************************************************
			*	@brief		设置边界线起始点
			*	@param[in]	i_pntStart	边界线起始点
			*	@param[out]	无
			*	@retval		STK_SUCCESS	设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetStartPoint(STK_PNT32 &i_pntStart);

			/*****************************************************************************
			*	@brief		获取边界线终点
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntEndPoint	边界线终点
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32		GetEndPoint();

			/*****************************************************************************
			*	@brief		设置边界线终点
			*	@param[in]	i_pntEnd	边界线终点
			*	@param[out]	无
			*	@retval		STK_SUCCESS	设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetEndPoint(STK_PNT32 &i_pntEnd);

			/*****************************************************************************
			*	@brief		获取边界线长度
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fLength	边界线长度
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32		GetLength();

			/*****************************************************************************
			*	@brief		设置边界线长度
			*	@param[in]	i_fLength	边界线长度
			*	@param[out]	无
			*	@retval		STK_SUCCESS	设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetLength(STK_FLOAT32 i_fLength);

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/****************************************************************************************
		*	@class	Stk_EllipseAttribute
		*	@brief	Ellipse属性类
		*
		*	记录Ellipse的基本属性信息。<br>
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_EllipseAttribute : public Stk_GeometryAttribute
		{
			STK_DECLARE_CLASS(Stk_EllipseAttribute)
		public:
			Stk_EllipseAttribute();
			virtual ~Stk_EllipseAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_ELLIPSEATTRIBUTE
			};
		public:
			/*****************************************************************************
			*	@brief		获取椭圆弧中心点
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntCenterPoint	椭圆弧中心点
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32		GetCenterPoint();

			/*****************************************************************************
			*	@brief		设置椭圆弧中心点
			*	@param[in]	i_pntCenter	椭圆弧中心点
			*	@param[out]	无
			*	@retval		STK_SUCCESS	设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetCenterPoint(STK_PNT32 &i_pntCenter);

			/*****************************************************************************
			*	@brief		获取椭圆弧长轴半径
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fMajorRadius	椭圆弧长轴半径
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32		GetMajorRadius();

			/*****************************************************************************
			*	@brief		设置椭圆弧长轴半径
			*	@param[in]	i_fRadius	椭圆弧长轴半径
			*	@param[out]	无
			*	@retval		STK_SUCCESS	设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetMajorRadius(STK_FLOAT32 i_fRadius);

			/*****************************************************************************
			*	@brief		获取椭圆弧短轴半径
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fMinorRadius	椭圆弧短轴半径
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32		GetMinorRadius();

			/*****************************************************************************
			*	@brief		设置椭圆弧短轴半径
			*	@param[in]	i_fRadius	椭圆弧短轴半径
			*	@param[out]	无
			*	@retval		STK_SUCCESS	设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetMinorRadius(STK_FLOAT32 i_fRadius);

			/*****************************************************************************
			*	@brief		获取边界线起始点
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntStartPoint	边界线起始点
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32		GetStartPoint();

			/*****************************************************************************
			*	@brief		设置边界线起始点
			*	@param[in]	i_pntStart	边界线起始点
			*	@param[out]	无
			*	@retval		STK_SUCCESS	设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetStartPoint(STK_PNT32 &i_pntStart);

			/*****************************************************************************
			*	@brief		获取边界线终点
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntEndPoint	边界线终点
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32		GetEndPoint();

			/*****************************************************************************
			*	@brief		设置边界线终点
			*	@param[in]	i_pntEnd	边界线终点
			*	@param[out]	无
			*	@retval		STK_SUCCESS	设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetEndPoint(STK_PNT32 &i_pntEnd);

			/****************************************************************************
			*	@brief		获取弧线向量
			*	@param[in]	无
			*	@param[out]	o_dirX				X方向向量
			*	@param[out]	o_dirY				Y方向向量
			*	@param[out]	o_dirZ				Z方向向量
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void			GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ);

			/****************************************************************************
			*	@brief		设置弧线向量
			*	@param[in]	i_dirX				X方向向量
			*	@param[in]	i_dirY				Y方向向量
			*	@param[in]	i_dirZ				Z方向向量
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@retval		STK_ERROR			设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS		SetXYZDir(const STK_DIR32& i_dirX, const STK_DIR32& i_dirY, const STK_DIR32& i_dirZ);

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};
	}
}
#endif
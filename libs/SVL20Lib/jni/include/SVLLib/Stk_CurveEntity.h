// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_Entity.h
*
*	@brief	要素基类
*
*	@par	历史:
*
****************************************************************************/
#ifndef _SVLLIB_CURVEENTITY_H_
#define _SVLLIB_CURVEENTITY_H_ 

#pragma once
#include "Stk_Object.h"
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_CurveEntity : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_CurveEntity)
		protected:
			enum { _typeID = _SVLLIB_TYPE_CURVE };	//!< 类别
		public:
			Stk_CurveEntity();
			virtual ~Stk_CurveEntity();
		public:
			/****************************************************************************
			*	@brief		获取Curve的颜色
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_rgbaCurve			颜色
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_RGBA32					GetColor();

			/****************************************************************************
			*	@brief		设置Curve的颜色
			*	@param[in]	i_rgbaCurve			Curve的颜色
			*	@retval		STK_SUCCESS			设置成功
			*	@retval		STK_ERROR			设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetColor(const STK_RGBA32& i_rgbaCurve);

			/****************************************************************************
			*	@brief		获取Curve的类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_eCurveType	Curve的类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual StkCurveTypeEnum			GetCurveType();

			/****************************************************************************
			*	@brief		设置Curve的类型
			*	@param[in]	i_eCurveType	Curve的类型
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void 			SetCurveType(StkCurveTypeEnum i_eCurveType);

			/****************************************************************************
			*	@brief		获取Curve的应用类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_eUsageType		Curve的定义场景类型
			*	@note		用于区分Curve的定义场景类型
			*	@attention	无
			****************************************************************************/
			StkCurveUsageEnum			GetUsageType();

			/****************************************************************************
			*	@brief		设置Curve的定义场景类型
			*	@param[in]	i_eUsageType		Curve的定义场景类型
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetUsageType(StkCurveUsageEnum i_eUsageType);

			/*****************************************************************************
			*	@brief		设置线条是否隐藏
			*	@param[in]	i_bDispStat			线条是否隐藏
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetCurveDispStat(STK_BOOL i_bDispStat);

			/*****************************************************************************
			*	@brief		获取线条的显隐状态
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_bDispStat
			*				= TRUE		显示
			*				= FALSE		隐藏
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_BOOL					GetCurveDispStat();

			/*****************************************************************************
			*	@brief		获取线条的显示类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_nuDispType			线条显示类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			StkCurveDispTypeEnum		GetCurveDispType();
			
			/*****************************************************************************
			*	@brief		设置线条的显示类型
			*	@param[in]	i_eDispType				线条显示类型
			*	@param[out]	无
			*	@retval		STK_SUCCESS				设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetCurveDispType(StkCurveDispTypeEnum i_eDispType);

			/****************************************************************************
			*	@brief		读取文件中的Stk_line数据
			*	@param[in]	i_pJsonNode			父Json节点
			*	@param[in]	i_JsonDocP			Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			读取成功
			*	@retval		STK_ERROR			读取失败
			*	@retval		STK_ERROR_MEMORY	内存申请失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			virtual     STK_STATUS		FromJson(void* i_pJsonNode, void* i_JsonDocP);

			/*****************************************************************************
			*	@brief		写Stk_line数据
			*	@param[in]	Value* i_pJsonNode			父Json节点
			*	@param[in]	i_JsonDocP			Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			写入成功
			*	@retval		其它				写入失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			virtual      STK_STATUS		ToJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_Line
		*	@brief	两点线类
		*
		*	记录PMI中由两点确定一条线的类的数据信息。<br>
		*	继承自Stk_CurveEntity类。
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_Line : public Stk_CurveEntity
		{
			STK_DECLARE_CLASS(Stk_Line)
		protected:
			enum { _typeID = _SVLLIB_TYPE_CURVE_LINE };	//!< 类别
		public:
			/****************************************************************************
			*	@brief		Stk_Line构造函数
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			Stk_Line(void);
			/****************************************************************************
			*	@brief		Stk_Line析构函数
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual ~Stk_Line(void);
		public:
			/****************************************************************************
			 *	@brief		获取起始点
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		m_pntStart	起始点
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_PNT32				GetStartPoint();
	
			/****************************************************************************
			 *	@brief		获取终止点
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		m_pntEnd	终止点
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_PNT32				GetEndPoint();

			/****************************************************************************
			 *	@brief		设置起始点
			 *	@param[in]	i_pntStart	起始点
			 *	@param[out]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			void					SetStartPoint(const STK_PNT32& i_pntStart);

			/****************************************************************************
			 *	@brief		设置终止点
			 *	@param[in]	i_pntEnd	终止点
			 *	@param[out]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			void					SetEndPoint(const STK_PNT32& i_pntEnd);
			
			/****************************************************************************
			*	@brief		获取Curve的类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_eCurveType	Curve的类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual StkCurveTypeEnum			GetCurveType();

			/****************************************************************************
			*	@brief		读取文件中的Stk_line数据
			*	@param[in]	i_pJsonNode			父Json节点
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
			*	@brief		写Stk_line数据
			*	@param[in]	Value* i_pJsonNode			父Json节点
			*	@param[in]	i_JsonDocP			Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			写入成功
			*	@retval		其它				写入失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_Polyline
		*	@brief	折线类
		*
		*	记录PMI中类型为折线的数据信息。<br>
		*	继承自Stk_CurveEntity类。
		***************************************************************************************/

		class STK_TOOLKIT_EXPORT Stk_PolyLine : public Stk_CurveEntity
		{
			STK_DECLARE_CLASS(Stk_PolyLine)
		protected:
			enum { _typeID = _SVLLIB_TYPE_CURVE_POLYLINE };	//!< 类别
		public:
			/****************************************************************************
			*	@brief		Stk_PolyLine构造函数
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			Stk_PolyLine(void);
			/****************************************************************************
			*	@brief		Stk_PolyLine析构函数
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual ~Stk_PolyLine(void);
		public:
			/****************************************************************************
			*	@brief		获取折线点列
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntPoints			折线点列
			*	@note		无
			*	@attention	无
			****************************************************************************/
			const vector<STK_PNT32>&	GetPoints();

			/****************************************************************************
			*	@brief		设置折线点列
			*	@param[in]	i_pntPoints			折线点列
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@retval		STK_ERROR			设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetPoints(const vector<STK_PNT32>& i_pntPoints);

			/****************************************************************************
			*	@brief		获取折线起始终止范围
			*	@param[in]	无
			*	@param[out]	o_fUMin				起始范围
			*	@param[out]	o_fUMax				终止范围
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						GetRange(STK_FLOAT32& o_fUMin, STK_FLOAT32& o_fUMax);

			/****************************************************************************
			*	@brief		获取Curve的类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_eCurveType	Curve的类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual StkCurveTypeEnum			GetCurveType();
			
			/****************************************************************************
			*	@brief		设置折线起始终止范围
			*	@param[in]	i_fUMin				起始范围
			*	@param[in]	i_fUMax				终止范围
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@retval		STK_ERROR			设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetRange(STK_FLOAT32 i_fUMin, STK_FLOAT32 i_fUMax);
			
			/****************************************************************************
			*	@brief		读取文件中的Stk_PolyLine数据
			*	@param[in]	i_pJsonNode			父Json节点
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
			*	@brief		写Stk_line数据
			*	@param[in]	Value* i_pJsonNode			父Json节点
			*	@param[in]	i_JsonDocP			Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			写入成功
			*	@retval		其它				写入失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_Arc
		*	@brief	弧线类
		*
		*	记录PMI中类型为弧线的数据信息。<br>
		*	继承自Stk_CurveEntity类。
		***************************************************************************************/

		class STK_TOOLKIT_EXPORT Stk_Arc
			: public Stk_CurveEntity
		{
			STK_DECLARE_CLASS(Stk_Arc)
		protected:
			enum { _typeID = _SVLLIB_TYPE_CURVE_ELLIPSE };	//!< 类别
		public:
			/****************************************************************************
			 *	@brief		Stk_Arc构造函数
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_Arc(void);
			/****************************************************************************
			*	@brief		Stk_Arc析构函数
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual ~Stk_Arc(void);
		public:
			/****************************************************************************
			*	@brief		获取弧线起始终止范围
			*	@param[in]	无
			*	@param[out]	o_fUMin				起始范围
			*	@param[out]	o_fUMax				终止范围
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void					GetRange(STK_FLOAT32& o_fUMin, STK_FLOAT32& o_fUMax);

			/****************************************************************************
			*	@brief		设置弧线起始终止范围
			*	@param[in]	i_fUMin				起始范围
			*	@param[in]	i_fUMax				终止范围
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@retval		STK_ERROR			设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetRange(STK_FLOAT32 i_fUMin, STK_FLOAT32 i_fUMax);

			/****************************************************************************
			*	@brief		获取弧线中心点
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntCenter			中心点
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32				GetCenterPoint();

			/****************************************************************************
			*	@brief		设置弧线中心点
			*	@param[in]	i_pntCenter			中心点
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@retval		STK_ERROR			设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetCenterPoint(const STK_PNT32& i_pntCenter);

			/****************************************************************************
			*	@brief		获取弧线方向向量
			*	@param[in]	无
			*	@param[out]	o_dirX				X方向向量
			*	@param[out]	o_dirY				Y方向向量
			*	@param[out]	o_dirZ				Z方向向量
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void					GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ);

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
			STK_STATUS				SetXYZDir(const STK_DIR32& i_dirX, const STK_DIR32& i_dirY, const STK_DIR32& i_dirZ);
			
			/****************************************************************************
			*	@brief		设置圆弧方向
			*	@param[in]	i_dirNormal	圆弧方向
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void					SetNormal(const STK_DIR32& i_dirNormal);

			/****************************************************************************
			*	@brief		获取圆弧方向
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_dirNormal	圆弧方向
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DIR32				GetNormal();

			/****************************************************************************
			*	@brief		获取Curve的类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_eCurveType	Curve的类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual StkCurveTypeEnum			GetCurveType();
			
			/****************************************************************************
			*	@brief		设置原点方向
			*	@param[in]	i_dirOrigin	原点方向
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void					SetOriginDir(const STK_DIR32& i_dirOrigin);

			/****************************************************************************
			*	@brief		获取原点方向
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_dirOrigin	原点方向
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_DIR32				GetOriginDir();

			/****************************************************************************
			*	@brief		获取弧线半径
			*	@param[in]	无
			*	@param[out]	o_fMajorR			长半径
			*	@param[out]	o_fMinorR			短半径
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void					GetRadius(STK_FLOAT32& o_fMajorR, STK_FLOAT32& o_fMinorR);

			/****************************************************************************
			*	@brief		设置弧线半径
			*	@param[in]	i_fMajorR			长半径
			*	@param[in]	i_fMinorR			短半径
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@retval		STK_ERROR			设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetRadius(STK_FLOAT32 i_fMajorR, STK_FLOAT32 i_fMinorR);

			/****************************************************************************
			*	@brief		获取弧线点坐标
			*	@param[in]	无
			*	@param[out]	o_pntStart			起点坐标
			*	@param[out]	o_pntEnd			终点坐标
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void					GetCoordinatePnt(STK_PNT32& o_pntStart, STK_PNT32& o_pntEnd);

			/****************************************************************************
			*	@brief		设置弧线点坐标
			*	@param[in]	i_pntStart			起点坐标
			*	@param[in]	i_pntEnd			终点坐标
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@retval		STK_ERROR			设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetCoordiantePnt(const STK_PNT32& i_pntStart, const STK_PNT32& i_pntEnd);
			
			/****************************************************************************
			*	@brief		获取弧线点参数位置
			*	@param[in]	无
			*	@param[out]	o_fStartPar			起点参数位置
			*	@param[out]	o_fEndPar			终点参数位置
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void					GetParameter(STK_FLOAT32& o_fStartPar, STK_FLOAT32& o_fEndPar);

			/****************************************************************************
			*	@brief		设置弧线点参数位置
			*	@param[in]	i_fStartPar			起点参数位置
			*	@param[in]	i_fEndPar			终点参数位置
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@retval		STK_ERROR			设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetParameter(STK_FLOAT32 i_fStartPar, STK_FLOAT32 i_fEndPar);
			/****************************************************************************
			*	@brief		读取文件中的Stk_Arc数据
			*	@param[in]	i_pJsonNode			父Json节点
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
			*	@brief		写Stk_line数据
			*	@param[in]	Value* i_pJsonNode			父Json节点
			*	@param[in]	i_JsonDocP			Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			写入成功
			*	@retval		其它				写入失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);
		};


		/****************************************************************************************
		*	@class	Stk_NurbsCurve
		*	@brief	NURBS曲线类
		*
		*	记录NURBS曲线的数据信息。<br>
		*	继承自Stk_CurveEntity类。
		***************************************************************************************/

		class STK_TOOLKIT_EXPORT Stk_NurbsCurve
			: public Stk_CurveEntity
		{
			STK_DECLARE_CLASS(Stk_NurbsCurve)
		protected:
			enum { _typeID = _SVLLIB_TYPE_CURVE_NURBS };	//!< 类别
		public:
			/****************************************************************************
			*	@brief		Stk_Arc构造函数
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			Stk_NurbsCurve(void);
			/****************************************************************************
			*	@brief		Stk_Arc析构函数
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual ~Stk_NurbsCurve(void);
		public:
			/*****************************************************************************
			*	@brief		获取起始和终止范围
			*	@param[in]	无
			*	@param[out]	o_fMin					起始范围
			*	@param[out]	o_fMax					终止范围
			*	@retval		STK_SUCCESS				获取成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				GetDomain(STK_FLOAT32& o_fMin, STK_FLOAT32& o_fMax);

			/*****************************************************************************
			*	@brief		设置起始和终止范围
			*	@param[in]	i_fMin					起始范围
			*	@param[in]	i_fMax					终止范围
			*	@param[out]	无
			*	@retval		STK_SUCCESS				设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetDomain(STK_FLOAT32 i_fMin, STK_FLOAT32 i_fMax);

			/*****************************************************************************
			*	@brief		获取Degree
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_nuDegree				Degree值
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_UINT32				GetDegree();

			/*****************************************************************************
			*	@brief		设置Degree
			*	@param[in]	i_nuDegree				Degree参数
			*	@param[out]	无
			*	@retval		STK_SUCCESS				设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetDegree(STK_UINT32 i_nuDegree);

			/*****************************************************************************
			*	@brief		获取Knot的多样性
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_vecKnotPlicity		Knot的多样性
			*	@note		无
			*	@attention	无
			****************************************************************************/
			vector<STK_UINT32>		GetKnotMultiplicity();

			/*****************************************************************************
			*	@brief		添加Knot的多样性
			*	@param[in]	i_nuKnotPlicity			Knot的多样性
			*	@param[out]	无
			*	@retval		STK_SUCCESS				添加成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				AddKnotMultiplicity(STK_UINT32 i_nuKnotPlicity);

			/*****************************************************************************
			*	@brief		获取Knot节点
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_vecKnots				Knots节点
			*	@note		无
			*	@attention	无
			****************************************************************************/
			vector<STK_FLOAT32>		GetKnots();

			/*****************************************************************************
			*	@brief		添加Knot节点
			*	@param[in]	i_fKnot					Knot节点
			*	@param[out]	无
			*	@retval		STK_SUCCESS				添加成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				AddKnot(STK_FLOAT32 i_fKnot);

			/*****************************************************************************
			*	@brief		获取控制点
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_vecPoints				Nurbs曲线的控制点
			*	@note		无
			*	@attention	无
			****************************************************************************/
			vector<STK_PNT32>		GetPoints();

			/*****************************************************************************
			*	@brief		添加Nurbs曲线控制点
			*	@param[in]	i_pntCtrl				Nurbs曲线控制点
			*	@param[out]	无
			*	@retval		STK_SUCCESS				添加成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				AddPoint(STK_PNT32 i_pntCtrl);

			/*****************************************************************************
			*	@brief		获取Nurbs曲线控制点的权重
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_vecWeights				Nurbs曲线控制点的权重
			*	@note		无
			*	@attention	无
			****************************************************************************/
			vector<STK_FLOAT32>		GetWeights();

			/*****************************************************************************
			*	@brief		添加Nurbs曲线控制点的权重
			*	@param[in]	i_fWeight					Nurbs曲线控制点的权重
			*	@param[out]	无
			*	@retval		STK_SUCCESS					添加成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				AddWeight(STK_FLOAT32 i_fWeight);

			/*****************************************************************************
			*	@brief		填写Nurbs曲线控制点
			*	@param[in]	i_Point					控制点
			*	@param[out]	o_fAtr					占位符位置
			*	@retval		无
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS				WriteVertex(STK_FLOAT32* o_fAtr, STK_PNT32 i_Point);

			/****************************************************************************
			*	@brief		读取文件中的Nurbs数据
			*	@param[in]	i_strJson			Json格式字符串
			*	@param[out]	无
			*	@retval		STK_SUCCESS			读取成功
			*	@retval		STK_ERROR			读取失败
			*	@retval		STK_ERROR_MEMORY	内存申请失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS					FromJson(string i_strJson);
			/*****************************************************************************
			*	@brief		写Stk_line数据
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		Json格式字符串
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			string					ToJson();
		};



		/****************************************************************************************
		*	@class	Stk_Hyperbola
		*	@brief	双曲线类
		*
		*	记录双曲线的数据信息。<br>
		*	继承自Stk_CurveEntity类。
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_Hyperbola : public Stk_CurveEntity
		{
			STK_DECLARE_CLASS(Stk_Hyperbola)
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_CURVE_HYPERBOLA
			};	//!< 类别

		public:
			/*****************************************************************************
			*	@brief		Stk_Hyperbola构造函数
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			Stk_Hyperbola(void);

			/*****************************************************************************
			*	@brief		Stk_Hyperbola析构函数
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			~Stk_Hyperbola(void);

			/*****************************************************************************
			*	@brief		获取起始和终止范围
			*	@param[in]	无
			*	@param[out]	o_fMin					起始范围
			*	@param[out]	o_fMax					终止范围
			*	@retval		STK_SUCCESS				获取成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				GetDomain(STK_FLOAT32& o_fMin, STK_FLOAT32& o_fMax);

			/*****************************************************************************
			*	@brief		设置起始和终止范围
			*	@param[in]	i_fMin					起始范围
			*	@param[in]	i_fMax					终止范围
			*	@param[out]	无
			*	@retval		STK_SUCCESS				设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetDomain(STK_FLOAT32 i_fMin, STK_FLOAT32 i_fMax);

			/*****************************************************************************
			*	@brief		获取双曲线的中心点
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntCenter				双曲线中心点
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32				GetCenterPoint();

			/*****************************************************************************
			*	@brief		设置双曲线的中心点
			*	@param[in]	i_pntCenter				双曲线的中心点
			*	@param[out]	无
			*	@retval		STK_SUCCESS				设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetCenterPoint(STK_PNT32& i_pntCenter);

			/*****************************************************************************
			*	@brief		获取双曲线的XYZ方向向量
			*	@param[in]	无
			*	@param[out]	o_dirX					X方向向量
			*	@param[out]	o_dirY					Y方向向量
			*	@param[out]	o_dirZ					Z方向向量
			*	@retval		STK_SUCCESS				获取成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ);

			/*****************************************************************************
			*	@brief		设置双曲线的XYZ方向向量
			*	@param[in]	i_dirX					X方向向量
			*	@param[in]	i_dirY					Y方向向量
			*	@param[in]	i_dirZ					Z方向向量
			*	@param[out]	无
			*	@retval		STK_SUCCESS				设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetXYZDir(STK_DIR32& i_dirX, STK_DIR32& i_dirY, STK_DIR32& i_dirZ);

			/*****************************************************************************
			*	@brief		获取双曲线的半轴
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fSemiAxis				获取成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32				GetSemiAxis();

			/*****************************************************************************
			*	@brief		设置双曲线的半轴
			*	@param[in]	i_fSemiAxis				双曲线的半轴
			*	@param[out]	无
			*	@retval		STK_SUCCESS				设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetSemiAxis(STK_FLOAT32 i_fSemiAxis);

			/*****************************************************************************
			*	@brief		获取双曲线的SemiImgAxis
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fSemiAxis				获取成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32				GetSemiImgAxis();

			/*****************************************************************************
			*	@brief		设置双曲线的SemiImgAxis
			*	@param[in]	i_fSemiImgAxis			双曲线的SemiImgAxis
			*	@param[out]	无
			*	@retval		STK_SUCCESS				设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetSemiImgAxis(STK_FLOAT32 i_fSemiImgAxis);

			/*****************************************************************************
			*	@brief		获取起点坐标
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntStart				起点坐标
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32				GetStartPoint();

			/*****************************************************************************
			*	@brief		设置起点坐标
			*	@param[in]	i_pntStart				起点坐标
			*	@param[out]	无
			*	@retval		STK_SUCCESS				设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetStartPoint(STK_PNT32 i_pntStart);

			/*****************************************************************************
			*	@brief		获取终点坐标
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntEnd				终点坐标
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32				GetEndPoint();

			/*****************************************************************************
			*	@brief		设置终点坐标
			*	@param[in]	i_pntEnd				终点坐标
			*	@param[out]	无
			*	@retval		STK_SUCCESS				设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetEndPoint(STK_PNT32 i_pntEnd);

			/****************************************************************************
			*	@brief		读取文件中的Stk_Hyperbola数据
			*	@param[in]	i_strJson			Json格式字符串
			*	@param[out]	无
			*	@retval		STK_SUCCESS			读取成功
			*	@retval		STK_ERROR			读取失败
			*	@retval		STK_ERROR_MEMORY	内存申请失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS					FromJson(string i_strJson);
			/*****************************************************************************
			*	@brief		写Stk_line数据
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		Json格式字符串
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			string					ToJson();
		};


		/****************************************************************************************
		*	@class	Stk_Parabola
		*	@brief	抛物线类
		*
		*	记录抛物线的数据信息。<br>
		*	继承自Stk_CurveEntity类。
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_Parabola : public Stk_CurveEntity
		{
			STK_DECLARE_CLASS(Stk_Parabola)
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_CURVE_PARABOLA
			};	//!< 类别
		public:
			/*****************************************************************************
			*	@brief		Stk_Parabola构造函数
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			Stk_Parabola(void);

			/*****************************************************************************
			*	@brief		Stk_Parabola析构函数
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			~Stk_Parabola(void);

			/*****************************************************************************
			*	@brief		获取起始和终止范围
			*	@param[in]	无
			*	@param[out]	o_fMin					起始范围
			*	@param[out]	o_fMax					终止范围
			*	@retval		STK_SUCCESS				获取成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				GetDomain(STK_FLOAT32& o_fMin, STK_FLOAT32& o_fMax);

			/*****************************************************************************
			*	@brief		设置起始和终止范围
			*	@param[in]	i_fMin					起始范围
			*	@param[in]	i_fMax					终止范围
			*	@param[out]	无
			*	@retval		STK_SUCCESS				设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetDomain(STK_FLOAT32 i_fMin, STK_FLOAT32 i_fMax);

			/*****************************************************************************
			*	@brief		获取起点坐标
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntStart				起点坐标
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32				GetStartPoint();

			/*****************************************************************************
			*	@brief		设置起点坐标
			*	@param[in]	i_pntStart				起点坐标
			*	@param[out]	无
			*	@retval		STK_SUCCESS				设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetStartPoint(STK_PNT32 i_pntStart);

			/*****************************************************************************
			*	@brief		获取终点坐标
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntEnd				终点坐标
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32				GetEndPoint();

			/*****************************************************************************
			*	@brief		设置终点坐标
			*	@param[in]	i_pntEnd				终点坐标
			*	@param[out]	无
			*	@retval		STK_SUCCESS				设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetEndPoint(STK_PNT32 i_pntEnd);

			/*****************************************************************************
			*	@brief		获取中心点
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_pntCenter				中心点
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32				GetCenterPoint();

			/*****************************************************************************
			*	@brief		设置中心点
			*	@param[in]	i_pntCenter				中心点
			*	@param[out]	无
			*	@retval		STK_SUCCESS				设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetCenterPoint(STK_PNT32& i_pntCenter);

			/*****************************************************************************
			*	@brief		获取抛物线的XYZ方向向量
			*	@param[in]	无
			*	@param[out]	o_dirX					X方向向量
			*	@param[out]	o_dirY					Y方向向量
			*	@param[out]	o_dirZ					Z方向向量
			*	@retval		STK_SUCCESS				获取成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ);

			/*****************************************************************************
			*	@brief		设置抛物线的XYZ方向向量
			*	@param[in]	i_dirX					X方向向量
			*	@param[in]	i_dirY					Y方向向量
			*	@param[in]	i_dirZ					Z方向向量
			*	@param[out]	无
			*	@retval		STK_SUCCESS				设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetXYZDir(STK_DIR32& i_dirX, STK_DIR32& i_dirY, STK_DIR32& i_dirZ);

			/*****************************************************************************
			*	@brief		获取Focal距离
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fFocalDis				Focal距离
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32				GetFocalDistance();

			/*****************************************************************************
			*	@brief		设置Focal距离
			*	@param[in]	i_fDistance				Focal距离
			*	@param[out]	无
			*	@retval		STK_SUCCESS				设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetFocalDistance(STK_FLOAT32 i_fDistance);

			/****************************************************************************
			*	@brief		读取文件中的Stk_Hyperbola数据
			*	@param[in]	i_strJson			Json格式字符串
			*	@param[out]	无
			*	@retval		STK_SUCCESS			读取成功
			*	@retval		STK_ERROR			读取失败
			*	@retval		STK_ERROR_MEMORY	内存申请失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS					FromJson(string i_strJson);
			/*****************************************************************************
			*	@brief		写Stk_line数据
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		Json格式字符串
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			string					ToJson();
		};


	}
}
#endif
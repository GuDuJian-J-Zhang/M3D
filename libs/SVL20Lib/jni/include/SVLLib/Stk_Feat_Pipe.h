// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_Feat_Pipe.h
 *
 *	@brief	线缆特征类
 *
 *	@par	历史:
 *		2014/05/28	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _SVLLIB_FEAT_PIPE_ENTITY_H_
#define _SVLLIB_FEAT_PIPE_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Object.h"
#include "Stk_CurveEntity.h"

namespace HoteamSoft {
	namespace SVLLib {
		/****************************************************************************
			 *	@class	Stk_Pipe_Fiber_Segment
			 *	@brief	线缆特征类
			 *
			 *	创建用于存储Node节点的特征的类库。<br>
			 *	继承自Stk_Entity 类。
		 ****************************************************************************/

		class STK_TOOLKIT_EXPORT Stk_Pipe_Fiber_Segment
		{
		private:
			STK_PNT32			m_pntStart;				//!< fiber 起点
			STK_PNT32			m_pntEnd;				//!< fiber 终点
			STK_PNT32			m_pntCenter;			//!< fiber 中心点
			Stk_CurveEntityPtr	m_stkCurveP;			//!< fiber Spline 线
		public:
			/****************************************************************************
			 *	@brief		Stk_Pipe_Fiber_Segment构造函数
			 *	@param[in]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_Pipe_Fiber_Segment();

			/****************************************************************************
			 *	@brief		Stk_Pipe_Fiber_Segment拷贝构造函数
			 *	@param[in]	i_stkPipeFiber		线缆的Fiber
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_Pipe_Fiber_Segment(Stk_Pipe_Fiber_Segment& i_stkPipeFiber);

			/****************************************************************************
			 *	@brief		Stk_Pipe_Fiber_Segment析构函数
			 *	@param[in]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			~Stk_Pipe_Fiber_Segment();

			/****************************************************************************
			 *	@brief		Stk_Pipe_Fiber_Segment赋值构造函数
			 *	@param[in]	i_stkPipeFiber		线缆的Fiber
			 *	@retval		*this				当前线缆的Fiber
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_Pipe_Fiber_Segment& operator = (Stk_Pipe_Fiber_Segment& i_stkPipeFiber);
		public:

			/****************************************************************************
			 *	@brief		设置起始点
			 *	@param[in]	i_pntStart			起始点
			 *	@retval		STK_SUCCESS			设置成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS			SetStartPoint(STK_PNT32& i_pntStart);

			/****************************************************************************
			 *	@brief		获取起始点
			 *	@param[in]	无
			 *	@retval		m_pntStart			起始点
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_PNT32			GetStartPoint();

			/****************************************************************************
			 *	@brief		设置终止点
			 *	@param[in]	i_pntEnd			终止点
			 *	@retval		STK_SUCCESS			设置成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS			SetEndPoint(STK_PNT32& i_pntEnd);

			/****************************************************************************
			 *	@brief		获取终止点
			 *	@param[in]	无
			 *	@retval		m_pntEnd			终止点
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_PNT32			GetEndPoint();

			/****************************************************************************
			 *	@brief		设置中心点
			 *	@param[in]	i_pntCenter			中心点
			 *	@retval		STK_SUCCESS			设置成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS			SetCenterPoint(STK_PNT32& i_pntCenter);

			/****************************************************************************
			 *	@brief		获取中心点
			 *	@param[in]	无
			 *	@retval		m_pntCenter			中心点
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_PNT32			GetCenterPoint();

			/****************************************************************************
			 *	@brief		设置Curve
			 *	@param[in]	i_stkCurveP			Curve指针
			 *	@retval		STK_SUCCESS			设置成功
			 *	@retval		其他				设置失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS			SetCurve(Stk_CurveEntityPtr i_stkCurveP);

			/****************************************************************************
			 *	@brief		获取Curve
			 *	@param[in]	无
			 *	@retval		m_stkCurveP			Curve指针
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_CurveEntityPtr	GetCurve();

		};

		/****************************************************************************
			 *	@class	Stk_Feat_Pipe
			 *	@brief	线缆特征类
			 *
			 *	创建用于存储Node节点的特征的类库。<br>
			 *	继承自Stk_Entity 类。
		 ****************************************************************************/

		class STK_TOOLKIT_EXPORT Stk_Feat_Pipe :
			public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Feat_Pipe)
		protected:
			enum { _typeID = _SVLLIB_TYPE_FEAT_PIPE };	//!< 类别
		private:
			StkPipeTypeEnum							m_ePipeType;
			StkPipeSectionTypeEnum					m_eSectionType;
			vector<Stk_Pipe_Fiber_Segment*>			m_vcPipeFibers;
			STK_FLOAT32								m_fDiameter;			//!< 线缆直径/厚度
			STK_FLOAT32								m_fTurnRadius;			//!< 最小折弯半径
			STK_FLOAT32								m_fHeight;				//!< 线缆Section的高度
			STK_FLOAT32								m_fWidth;				//!< 线缆Section的宽度
		public:
			/****************************************************************************
			 *	@brief		线缆特征构造函数
			 *	@param[in]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_Feat_Pipe();

			/****************************************************************************
			 *	@brief		线缆特征析构函数
			 *	@param[in]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			~Stk_Feat_Pipe();

			/****************************************************************************
			 *	@brief		线缆特征赋值构造函数
			 *	@param[in]	i_stkFeatPipe		线缆特征
			 *	@retval		*this				线缆特征
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_Feat_Pipe& operator = (const Stk_Feat_Pipe& i_stkFeatPipe);

		public:
			/****************************************************************************
			 *	@brief		设置线缆类型
			 *	@param[in]	i_ePipeType			线缆类型
			 *	@retval		STK_SUCCESS			设置成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				SetPipeType(StkPipeTypeEnum i_ePipeType);

			/****************************************************************************
			 *	@brief		获取线缆类型
			 *	@param[in]	无
			 *	@retval		m_ePipeType			线缆类型
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			StkPipeTypeEnum			GetPipeType();

			/****************************************************************************
			 *	@brief		设置Section类型
			 *	@param[in]	i_ePipeSectionType
			 *	@retval		STK_SUCCESS			设置成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				SetSectionType(StkPipeSectionTypeEnum i_ePipeSectionType);

			/****************************************************************************
			 *	@brief		获取Section类型
			 *	@param[in]	无
			 *	@retval		m_eSectionType			Section类型
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			StkPipeSectionTypeEnum	GetSectionType();

			/****************************************************************************
			 *	@brief		获取Fiber数量
			 *	@param[in]	无
			 *	@retval		Fiber数量
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_SIZE_T				GetPipeFiberNum();

			/****************************************************************************
			 *	@brief		获取指定的Fiber
			 *	@param[in]	i_nuKeyPos			索引
			 *	@retval		指定的Fiber
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_Pipe_Fiber_Segment*	GetPipeFiber(STK_UINT32 i_nuKeyPos);

			/****************************************************************************
			 *	@brief		添加Fiber到数组
			 *	@param[in]	i_stkPipeFiber		线缆的Fiber对象指针
			 *	@retval		STK_SUCCESS			添加成功
			 *	@retval		其他				添加失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				AddPipeFiber(Stk_Pipe_Fiber_Segment* i_stkPipeFiber);

			/****************************************************************************
			 *	@brief		获取线缆、管路实体的直径/厚度
			 *	@param[in]	无
			 *	@retval		无
			 *	@note		线缆、管路实体的直径/厚度
			 *	@attention	无
			 ****************************************************************************/
			STK_FLOAT32				GetDiameter();

			/****************************************************************************
			 *	@brief		设置线缆、管路实体的直径/厚度
			 *	@param[in]	i_fDiameter			线缆、管路实体的直径/厚度
			 *	@retval		STK_SUCCESS			设置成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				SetDiameter(STK_FLOAT32 i_fDiameter);

			/****************************************************************************
			 *	@brief		获取线缆的最小折弯半径
			 *	@param[in]	无
			 *	@retval		无
			 *	@note		线缆的最小折弯半径
			 *	@attention	无
			 ****************************************************************************/
			STK_FLOAT32				GetTurnRadius();

			/****************************************************************************
			 *	@brief		设置线缆的最小折弯半径
			 *	@param[in]	i_fTurnRadius		最小折弯半径
			 *	@retval		STK_SUCCESS			设置成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				SetTurnRadius(STK_FLOAT32 i_fTurnRadius);

			/****************************************************************************
			 *	@brief		获取线缆Section的高度
			 *	@param[in]	无
			 *	@retval		无
			 *	@note		线缆Section的高度
			 *	@attention	无
			 ****************************************************************************/
			STK_FLOAT32				GetHeight();

			/****************************************************************************
			 *	@brief		设置线缆Section的高度
			 *	@param[in]	i_fHeight			线缆Section的高度
			 *	@retval		STK_SUCCESS			设置成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				SetHeight(STK_FLOAT32 i_fHeight);

			/****************************************************************************
			 *	@brief		获取线缆Section的宽度
			 *	@param[in]	无
			 *	@retval		无
			 *	@note		线缆Section的高度
			 *	@attention	无
			 ****************************************************************************/
			STK_FLOAT32				GetWidth();

			/****************************************************************************
			 *	@brief		设置线缆Section的宽度
			 *	@param[in]	i_fWidth			线缆Section的宽度
			 *	@retval		STK_SUCCESS			设置成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS				SetWidth(STK_FLOAT32 i_fWidth);
		};
	}
}

#endif
// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_PMIEntity
*			引线类
*
**************************************************************************/
#ifndef _SVLLIB_PMIENTITY_H_
#define _SVLLIB_PMIENTITY_H_

#pragma once
#include "Stk_Object.h"
#include "Stk_CurveEntity.h"
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_Leader : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Leader)
		protected:
			enum { _typeID = _SVLLIB_TYPE_PMI_LEADER };	//!< 类别
		public:
			Stk_Leader();
			virtual ~Stk_Leader();
		public:
			friend class Stk_PMI;
		public:
			/****************************************************************************
			*	@brief		获取引出线类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_eLeaderType	引出线类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			StkLeaderTypeEnum			GetLeaderType();

			/****************************************************************************
			*	@brief		设置引出线类型
			*	@param[in]	i_eType			引出线类型
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetLeaderType(StkLeaderTypeEnum i_eType);

			/****************************************************************************
			*	@brief		获取引出线末端类型、宽高、起始位置和方向
			*	@param[in]	无
			*	@param[out]	o_eTermType		末端类型
			*	@param[out]	o_SizeA			末端宽高
			*	@param[out]	o_pntLoc		末端起始位置
			*	@param[out]	o_dirTerm		末端方向
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						GetTerminator(StkTermTypeEnum& o_eTermType, STK_FLOAT32 o_SizeA[2], STK_PNT32& o_pntLoc, STK_DIR32& o_dirTerm);

			/****************************************************************************
			*	@brief		设置引出线末端类型、宽高、起始位置和方向
			*	@param[in]	i_eTermType		末端类型
			*	@param[in]	i_SizeA			末端宽高
			*	@param[in]	i_pntLoc		末端起始位置
			*	@param[in]	i_dirTerm		末端方向
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetTerminator(StkTermTypeEnum i_eTermType, STK_FLOAT32 i_SizeA[2], const STK_PNT32& i_pntLoc, const STK_DIR32& i_dirTerm);

			/****************************************************************************
			*	@brief		设置二维折弯线标识
			*	@param[in]	i_bStub		二维折弯线标识
			*	@param[out]	无
			*	@retval		STK_SUCCESS	设置成功
			*	@retval		STK_ERROR	设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetBendLineFlag(STK_BOOL i_bIsBendLine);

			/****************************************************************************
			*	@brief		是否是二维折弯线
			*	@param[in]	i_bStub		二维折弯线标识
			*	@param[out]	无
			*	@retval		STK_SUCCESS	设置成功
			*	@retval		STK_ERROR	设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_BOOL					GetBendLineFlag();

			/****************************************************************************
			*	@brief		获取所有的引出线
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_vcCurves	所有引出线
			*	@note		无
			*	@attention	无
			****************************************************************************/
			vector<Stk_CurveEntityPtr>	GetAllCurve();

			/****************************************************************************
			*	@brief		获取的引出线
			*	@param[in]	iIndex	索引
			*	@param[out]	无
			*	@retval		Stk_CurveEntityPtr	引出线
			*	@note		无
			*	@attention	无
			****************************************************************************/
			Stk_CurveEntityPtr	GetCurve(int iIndex);

			/****************************************************************************
			*	@brief		添加引出线数据
			*	@param[in]	i_CurveP	Curve数据指针
			*	@param[out]	无
			*	@retval		STK_SUCCESS	添加成功
			*	@retval		STK_ERROR	添加失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					AddCurve(Stk_CurveEntityPtr i_CurveP);

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*   @param[in]  i_pDoc     文档	
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_pDoc);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_Extensionline
		*	@brief	延长线类
		*
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_Extensionline : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Extensionline)
		protected:
			enum { _typeID = _SVLLIB_TYPE_PMI_EXLINE };	//!< 类别
		public:
			/****************************************************************************
			*	@brief		Stk_Extensionline构造函数
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			Stk_Extensionline(void);

			/****************************************************************************
			*	@brief		Stk_Extensionline析构函数
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual ~Stk_Extensionline(void);

		public:
			/****************************************************************************
			*	@brief		获取延长线的定义类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_eExLineType	延长线的定义类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			StkExtensTypeEnum			GetExLineType();

			/****************************************************************************
			*	@brief		设置延长线的定义类型
			*	@param[in]	i_eType		延长线的定义类型
			*	@param[out]	无
			*	@retval		STK_SUCCESS	设置成功
			*	@retval		STK_SUCCESS	设置失败
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetExLineType(StkExtensTypeEnum i_nuType);

			/****************************************************************************
			*	@brief		判断延长线样式
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_uiStyle	样式
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_UINT32					GetStyle();

			/****************************************************************************
			*	@brief		设置延长线样式
			*	@param[in]	m_uiStyle	样式	
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetStyle(STK_UINT32 i_uiStyle);

			/****************************************************************************
			*	@brief		设置曲线
			*	@param[in]	i_curveP  曲线
			*	@param[out]	无
			*	@retval		成功与否
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS                  SetCurve(Stk_CurveEntityPtr i_curveP);

			/****************************************************************************
			*	@brief		获取曲线
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		
			*	@note		无
			*	@attention	无
			****************************************************************************/
			Stk_CurveEntityPtr          GetCurve();

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[in]	i_pDoc		文档
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_SpecialLine
		*	@brief	特殊线类
		*
		***************************************************************************************/

		class STK_TOOLKIT_EXPORT Stk_SpecialLine : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_SpecialLine)
		protected:
			enum { _typeID = _SVLLIB_TYPE_PMI_SPECIALLINE };	//!< 类别
		public:
			/****************************************************************************
			*	@brief		Stk_SpecialLine构造函数
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			Stk_SpecialLine(void);

			/****************************************************************************
			*	@brief		Stk_SpecialLine析构函数
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual ~Stk_SpecialLine(void);
		public:
			/****************************************************************************
			*	@brief		获取特殊线用途
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_eLineUsage	特殊线用途
			*	@note		无
			*	@attention	无
			****************************************************************************/
			StkSpecLineUsageEnum		GetSpecLinUse();

			/****************************************************************************
			*	@brief		设置特殊线用途
			*	@param[in]	i_eLineType	特殊线用途
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetSpecLinUse(StkSpecLineUsageEnum i_eLineType);

			/****************************************************************************
			*	@brief		获取所有特殊线
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_vcCurves		所有特殊线
			*	@note		无
			*	@attention	无
			****************************************************************************/
			vector<Stk_CurveEntityPtr>	GetCurves();

			/****************************************************************************
			*	@brief		添加特殊线到特殊线数组中
			*	@param[in]	i_CurveP		Curve数据
			*	@param[out]	无
			*	@retval		STK_SUCCESS
			*	@retval		STK_ERROR
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					AddCurve(Stk_CurveEntityPtr i_CurveP);

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[in]	i_pDoc		文档
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_OutFrame
		*	@brief	OutFrame外框类
		*
		*	PMI的外框
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_OutFrame : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_OutFrame)
		protected:
			enum { _typeID = _SVLLIB_TYPE_CURVE_ELLIPSE };	//!< 类别
		public:
			Stk_OutFrame();
			virtual ~Stk_OutFrame();
		public:
			/****************************************************************************
			*	@brief		获取外框的应用类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_eFrameType		外框类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			StkOutFrameUsageEnum			GetFrameType();

			/****************************************************************************
			*	@brief		设置外框的应用类型
			*	@param[in]	i_eType				外框类型
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS						SetFrameType(StkOutFrameUsageEnum i_eType);

			/****************************************************************************
			*	@brief		获取外框的定义类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_eFrameType		外框类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			StkOutFrameDefEnum				GetDefineType();

			/****************************************************************************
			*	@brief		设置外框的定义类型
			*	@param[in]	i_eType				外框类型
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS						SetDefineType(StkOutFrameDefEnum i_eType);

			/****************************************************************************
			*	@brief		获取所有的外框线数据
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_vcCurves			所有的外框线数据
			*	@note		不再建议使用,使用GetFrameLineNum()和GetCurve()替代
			*	@attention	无
			****************************************************************************/
			const vector<Stk_CurveEntityPtr>& GetFrameData();

			/****************************************************************************
			*	@brief		设置外框线数据
			*	@param[in]	i_CurveP			外框线数据
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		不再建议使用，使用AddCurve()替代
			*	@attention	无
			****************************************************************************/
			STK_STATUS						SetFrameData(Stk_CurveEntityPtr i_CurveP);

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[in]	i_pDoc		文档
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_pDoc);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};
	}
}
#endif
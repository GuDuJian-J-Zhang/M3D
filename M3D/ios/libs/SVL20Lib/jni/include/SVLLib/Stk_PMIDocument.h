// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_PMIDocument
*	@brief	PMI管理类
*	@par 历史记录
*				2017/06/14 创建。qinyp(Hoteamsoft)
**************************************************************************/

#ifndef _SVLLIB_PMIDOCUMENT_H_
#define _SVLLIB_PMIDOCUMENT_H_
#pragma once
#include "Stk_BaseDocument.h"

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_PMI;
		class Stk_PMIDocument : public Stk_BaseDocument
		{
			STK_DECLARE_CLASS(Stk_PMIDocument)
		public:
			Stk_PMIDocument();
			virtual ~Stk_PMIDocument();
		protected:
			enum { _typeID = _SVLLIB_TYPE_PMIDOCUMENT };	//!< 类别
			
		public:
			void                       SetVersion(const wstring wstrVersion);
			const   wstring            GetVersion();
			/****************************************************************************
			*	@brief		获取指定零件的PMI列表
			*	@param[in]	modelID			零件ID
			*	@param[in][out]	vcPMIs			PMI列表
			*	@note		无
			*	@attention
			****************************************************************************/
			void			GetPMIListByModelID(unsigned int modelID, vector<Stk_PMIPtr>& vcPMIs);

			/****************************************************************************
			*	@brief		根据零件ID和PMI ID获取PMI指针
			*	@param[in]	modelID			零件ID
			*	@param[in]	pmiID			PMI ID
			*	@param[out]	无
			*	@retval		Stk_PMIPtr		视图指针
			*	@note		无
			*	@attention
			****************************************************************************/
			Stk_PMIPtr			GetPMI(unsigned int modelID, unsigned int pmiID);

			/****************************************************************************
			*	@brief		添加PMI
			*	@param[in]	modelID			零件ID
			*	@param[in]	pmiID		    PMI ID
			*	@param[out]	无
			*	@retval		STK_SUCCESS			添加成功
			*	@retval		其他				添加失败
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS          AddPMI(unsigned int modelID, Stk_PMIPtr pmiP);

			/****************************************************************************
			*	@brief		添加PMI
			*	@param[in]	modelID			零件ID
			*	@param[in]	vcPMI		    PMI ID集合
			*	@param[out]	无
			*	@retval		STK_SUCCESS			添加成功
			*	@retval		其他				添加失败
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS          AddPMI(unsigned int modelID, vector<STK_ID> vcPMI);

			/****************************************************************************
			*	@brief		添加PMI
			*	@param[in]	pmiID		    PMI ID
			*	@param[out]	无
			*	@retval		STK_SUCCESS			添加成功
			*	@retval		其他				添加失败
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS          AddPMI(Stk_PMIPtr pmiP);

			/****************************************************************************
			*	@brief		根据零件ID和PMI ID删除PMI
			*	@param[in]	modelID			零件ID
			*	@param[in]	pmiID			PMI ID
			*	@param[out]	无
			*	@retval		STK_SUCCESS			删除成功
			*	@retval		其他				删除失败
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS          DelPMI(unsigned int modelID, unsigned int pmiID);
			
			/****************************************************************************
			*	@brief		卸载PMI
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		其他				删除失败
			*	@attention
			****************************************************************************/
			void UnLoad();
		};
	}
}

#endif
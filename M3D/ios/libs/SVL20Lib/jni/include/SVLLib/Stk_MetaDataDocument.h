// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_MetaDataDocument
*	@brief	自定义属性管理类
*	@par 历史记录
*				2017/06/14 创建。qinyp(Hoteamsoft)
**************************************************************************/

#ifndef _SVLLIB_METADATADOCUMENT_H_
#define _SVLLIB_METADATADOCUMENT_H_
#pragma once
#include "Stk_BaseDocument.h"

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_MetaData;
		class Stk_MetaDataDocument : public Stk_BaseDocument
		{
			STK_DECLARE_CLASS(Stk_MetaDataDocument)
		public:
			Stk_MetaDataDocument(void);
			virtual ~Stk_MetaDataDocument(void);
		protected:
			enum { _typeID = _SVLLIB_TYPE_METADATADOCUMENT };	//!< 类别

		public:
			/****************************************************************************
			*	@brief		获取指定零件的属性列表
			*	@param[in]	modelID			零件ID
			*	@param[out]	无
			*	@retval		vector<Stk_MetaDataPtr>			视图列表
			*	@note		无
			*	@attention
			****************************************************************************/
			vector<Stk_MetaDataPtr>&			GetMetaDataListByModelId(unsigned int modelID);

			/****************************************************************************
			*	@brief		根据零件ID和属性ID获取属性指针
			*	@param[in]	modelID			零件ID
			*	@param[in]	metadataD	    属性ID
			*	@param[out]	无
			*	@retval		Stk_MetaDataPtr		属性指针
			*	@note		无
			*	@attention
			****************************************************************************/
			Stk_MetaDataPtr			GetMetaData(unsigned int modelID, unsigned int metadataID);

			/****************************************************************************
			*	@brief		添加属性
			*	@param[in]	modelID			零件ID
			*	@param[in]	metadataID			属性ID
			*	@param[out]	无
			*	@retval		STK_SUCCESS			写入成功
			*	@retval		其他				写入失败
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS          AddMetaData(unsigned int modelID, Stk_MetaDataPtr metadataP);

			/****************************************************************************
			*	@brief		根据零件ID和属性ID删除属性
			*	@param[in]	modelID			零件ID
			*	@param[in]	metadataID			属性ID
			*	@param[out]	无
			*	@retval		STK_SUCCESS			写入成功
			*	@retval		其他				写入失败
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS          DelMetaData(unsigned int modelID, unsigned int metadataID);
		};
	}
}

#endif //_SVLLIB_METADATADOCUMENT_H_
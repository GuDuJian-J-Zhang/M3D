// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_ViewDocument
*	@brief	视图文档管理类
*	@par 历史记录
*				2017/06/09 创建。likun(Hoteamsoft)
**************************************************************************/
#ifndef _SVLLIB_VIEWDOCUMENT_H_
#define _SVLLIB_VIEWDOCUMENT_H_
#include "Stk_BaseDocument.h"
#include "Stk_ObjectImpl.h"
//#include "Stk_View.h"

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_View;
		class Stk_ViewDocument : public Stk_BaseDocument
		{
			STK_DECLARE_CLASS(Stk_ViewDocument)
		protected:
			enum { _typeID = _SVLLIB_TYPE_VIEWDOCUMENT };	//!< 类别
		public:
			Stk_ViewDocument();
			virtual ~Stk_ViewDocument(void);
		public:
			void                       SetVersion(const wstring wstrVersion);
			const   wstring            GetVersion();
			/****************************************************************************
			*	@brief		获取指定零件的视图列表
			*	@param[in]	modelID			零件ID
			*	@param[out]	无
			*	@retval		vector<Stk_ViewPtr>			视图列表
			*	@note		无
			*	@attention
			****************************************************************************/
			void GetViewListByModelId(unsigned int modelID, vector<Stk_ViewPtr>& vcViews);

			/****************************************************************************
			*	@brief		根据零件ID和视图ID获取视图指针
			*	@param[in]	modelID			零件ID
			*	@param[in]	viewID			视图ID
			*	@param[out]	无
			*	@retval		Stk_ViewPtr		视图指针
			*	@note		无
			*	@attention
			****************************************************************************/
			Stk_ViewPtr			GetView(unsigned int modelID, unsigned int viewID);

			/****************************************************************************
			*	@brief		添加视图
			*	@param[in]	modelID			零件ID
			*	@param[in]	viewP			视图
			*	@param[out]	无
			*	@retval		STK_SUCCESS			写入成功
			*	@retval		其他				写入失败
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS          AddView(unsigned int modelID, Stk_ViewPtr viewP);

			/****************************************************************************
			*	@brief		添加视图
			*	@param[in]	viewP			视图
			*	@param[out]	无
			*	@retval		STK_SUCCESS			写入成功
			*	@retval		其他				写入失败
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS          AddView(Stk_ViewPtr viewP);

			/****************************************************************************
			*	@brief		添加视图
			*	@param[in]	modelID			零件ID
			*	@param[in]	vcViews		    View ID集合
			*	@param[out]	无
			*	@retval		STK_SUCCESS			添加成功
			*	@retval		其他				添加失败
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS          AddViews(unsigned int modelID, vector<STK_ID> vcViews);

			/****************************************************************************
			*	@brief		根据零件ID和视图ID删除视图
			*	@param[in]	modelID			零件ID
			*	@param[in]	viewID			视图ID
			*	@param[out]	无
			*	@retval		STK_SUCCESS			写入成功
			*	@retval		其他				写入失败
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS          DelView(unsigned int modelID, unsigned int viewID);

			map<STK_ID, Stk_ViewPtr>            GetViewMap();

			/****************************************************************************
			*	@brief		卸载视图
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		其他				删除失败
			*	@attention
			****************************************************************************/
			void UnLoad();
		};
	}//svllib
}//hoteamsoft

#endif

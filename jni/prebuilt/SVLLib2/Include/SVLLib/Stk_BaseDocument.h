// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_BaseDocument
*	@brief	文档管理基类
*	@par 历史记录
*				2017/06/09 创建。likun(Hoteamsoft)
**************************************************************************/
#ifndef _SVLLIB_BASEDOCUMENT_H_
#define _SVLLIB_BASEDOCUMENT_H_

#include "Stk_API.h"


using namespace std;

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_BaseDocumentImpl;
		class Stk_BaseDocument : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_BaseDocument)
		protected:
			enum { _typeID = _SVLLIB_TYPE_DOCUMENT };	//!< 类别
		public:
			Stk_BaseDocument(Stk_BaseDocumentImpl *pImpl);
			virtual ~Stk_BaseDocument(void);
		
			
		protected:
			Stk_BaseDocumentImpl *m_pImpl;			//!< 其他数据

		public:
			/****************************************************************************
			*	@brief		获取总体文件管理对象
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		总体文件管理对象指针
			*	@note		无
			*	@attention
			****************************************************************************/
			Stk_Document* GetDocument();
			void SetDocument(Stk_Document* pDocument);
			/****************************************************************************
			*	@brief		获取文件的扩展名
			*	@param[in]	strFilePath 文件名称或路径
			*	@param[out]	无
			*	@retval		文件的扩展名
			*	@note		无
			*	@attention
			****************************************************************************/
			const std::wstring GetFileExt(const std::wstring& strFilePath);
		
			/****************************************************************************
			*	@brief		保存文件
			*	@param[in]	strFilePath	文件路径
			*	@param[out]	无
			*	@retval		STK_SUCCESS			写入成功
			*	@retval		其他				写入失败
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS	WriteFile(const std::wstring& strFilePath);

			/****************************************************************************
			*	@brief		读取文件
			*	@param[in]	strFilePath	文件路径
			*	@param[out]	无
			*	@retval		STK_SUCCESS			读取成功
			*	@retval		其他				读取失败
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS	OpenFile(const std::wstring& strFilePath);

			/****************************************************************************
			*	@brief		设置当前文件路径
			*	@param[in]	strFilePath	文件路径
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention
			****************************************************************************/
			void		SetFilePath(const std::wstring& strFilePath);

			/****************************************************************************
			*	@brief		返回Json格式字符串
			*	@param[in]	无
			*	@param[out]	strJson 字符串内容
			*	@retval		STK_SUCCESS			成功
			*	@retval		其他				失败
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS ToJsonString(string& strJson);

			/****************************************************************************
			*	@brief		获取Json 文档指针
			*	@param[out]	无
			*	@retval		void*
			*	@note		无
			*	@attention
			****************************************************************************/
			void* GetJsonDocP();

			void  unLoad();
		};
	}//svllib
}//hoteamsoft

#endif

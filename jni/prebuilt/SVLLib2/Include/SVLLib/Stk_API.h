// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_API
	*	@brief	API接口类
	*			
**************************************************************************/
#ifndef _SVLLIB_API_H_
#define _SVLLIB_API_H_

#include "Stk_Define.h"
#include "Stk_Enum.h"
#include "Stk_Tab.h"
#include "Stk_Document.h"
#ifdef _WIN32
#include <windows.h>
#endif
namespace HoteamSoft
{
	namespace SVLLib {
#define STK_APP()	Stk_API::GetAPI()

		class STK_TOOLKIT_EXPORT Stk_API
		{
		public:
			Stk_API(void);
			~Stk_API(void);
		protected:
			struct Stk_Handle;			//!< Not defined here
			Stk_Handle *_Private;		//!< Handle
			static Stk_API *_CurrentAPI;
			RequestFilePathCallback _RequestFilePathFun;
			HMODULE m_modHandle;
			bool           m_bUseSSL;
			bool           m_bComPact;
			//friend class Stk_DocumentManager;
		public:
			/**************************************************************************
			*	@brief		获取API
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		指针
			*	@note		无
			*	@attention	无
			**************************************************************************/
			static Stk_API* GetAPI();
			/**************************************************************************
			*	@brief		结束API
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	使用API后要调用此接口进行关闭
			**************************************************************************/
			static void EndAPI();
			/**************************************************************************
			*	@brief		设置日志文件
			*	@param[in]	Stk_Log_Type i_type		日志写入方式（写入文件或控制台）
			*	@param[in]	Stk_Log_Level i_level	日志级别，低于此级别则不打印，
			默认最高界别
			*	@param[in]	Stk_string i_strPath	日志路径，写入控制台时此值不需设置
			*	@param[out]	无
			*	@retval		成功:STK_SUCCESS		失败:其他
			*	@note		无
			*	@attention	无
			**************************************************************************/
			STK_STATUS SetLog(Stk_Log_Type i_type, Stk_Log_Level i_level = STK_LOG_LEVEL_HIGHEST,
				Stk_string i_strPath = "");
			/**************************************************************************
			*	@brief		打印日志
			*	@param[in]	Stk_string i_strMsg		日志信息
			*	@param[in]	Stk_Log_Level i_level	日志级别
			*	@param[in]	int i_type				日志附加信息，默认不附加信息，
			STK_LOG_ADDITION_TIME:附加时间信息
			*	@param[out]	无
			*	@retval		成功:STK_SUCCESS		失败:其他
			*	@note		无
			*	@attention	无
			**************************************************************************/
			STK_STATUS WriteLog(Stk_string i_strMsg, Stk_Log_Level i_level = STK_LOG_LEVEL_HIGHEST,
				int i_type = STK_LOG_ADDITION_NONE);
			/**************************************************************************
			*	@brief		日志中写换行
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		成功:STK_SUCCESS		失败:其他
			*	@note		无
			*	@attention	无
			**************************************************************************/
			STK_STATUS WriteLogEndl();
			/**************************************************************************
			*	@brief		打开文件并返回Stk_DocumentManagerPtr
			*	@param[in]	const Stk_wstring &i_strSvlPath			文件路径
			*	@param[out]	Stk_DocumentManagerPtr	o_pDocument		文档管理指针
			*	@retval		成功:STK_SUCCESS		失败:其他
			*	@note		无
			*	@attention	无
			**************************************************************************/
			//STK_STATUS OpenDocument(const Stk_wstring &i_strSvlPath, Stk_DocumentManagerPtr &o_pDocument);
			/**************************************************************************
			*	@brief		从API管理中删除单个文档管理
			*	@param[in]	Stk_DocumentManagerPtr i_pDocument		文档管理指针
			*	@param[out]	无
			*	@retval
			*	@note		当打开或创建一个Document时，后续不在使用该Document可以使用此
			方法，进行关闭。
			*	@attention	无
			**************************************************************************/
			//STK_STATUS CloseDocument(Stk_DocumentManagerPtr i_pDocument);
			/**************************************************************************
			*	@brief		在API管理中是否单个文档管理
			*	@param[in]	Stk_DocumentManagerPtr i_pDocument		文档管理指针
			*	@param[out]	无
			*	@retval		存在:true		不存在:false
			*	@note		无
			*	@attention	无
			**************************************************************************/
			//bool IsActiveDocument(Stk_DocumentManagerPtr i_pDocument);

			/**************************************************************************
			*	@brief		通过uuid从API管理中查找单个文档管理
			*	@param[in]	Stk_string i_strUuid		文档管理指针的uuid
			*	@retval		文档管理指针
			*	@note		无
			*	@attention	无
			**************************************************************************/
			//Stk_DocumentManagerPtr FindDocument(Stk_string i_strUuid);
			/**************************************************************************
			*	@brief		获取文件的版本号
			*	@param[in]	i_strPath		文件路径
			*	@param[out]	o_Version		版本号
			*	@retval		查找成功:STK_SUCCESS		失败:其他
			*	@note		无
			*	@attention	无
			**************************************************************************/
			//STK_STATUS GetFileVersion(const Stk_wstring &i_strPath, Stk_wstring& o_Version);
			/**************************************************************************
			*	@brief		获取所有Document
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		Stk_Tab<Stk_DocumentManagerPtr>
			*	@note		无
			*	@attention	无
			**************************************************************************/
			//Stk_Tab<Stk_DocumentManagerPtr> GetDocuments();
			Stk_Document* GetDocument(unsigned int docID);
			STK_STATUS StartupService(int inPort);
			void InitColorMap();
			void InitConfig();
			int GetColor(int colorIndex);
			void SetRequestFileFun(RequestFilePathCallback inRequestFilePathFun);
			RequestFilePathCallback GetRequestFileFun();

			void SetDllHandle(HMODULE inModHandle) { m_modHandle = inModHandle; }
			HMODULE GetDllHandle() { return m_modHandle; }

			bool IsUseSSL() { return m_bUseSSL; }
			bool IsCompact() { return m_bComPact; }
		};
		enum aiOrigin { aiOrigin_SET = 0, aiOrigin_CUR = 1, aiOrigin_END = 2 };
		class IOStream
		{
			FILE* f;
		public:
			IOStream(FILE* file) : f(file) {}
			~IOStream() { fclose(f); f = 0; }

			size_t Read(void* b, size_t sz, size_t n) { return fread(b, sz, n, f); }
			size_t Write(const void* b, size_t sz, size_t n) { return fwrite(b, sz, n, f); }
			int    Seek(size_t off, aiOrigin orig) { return fseek(f, off, int(orig)); }
			size_t Tell() const { return ftell(f); }

			size_t FileSize() {
				long p = Tell(), len = (Seek(0, aiOrigin_END), Tell());
				return size_t((Seek(p, aiOrigin_SET), len));
			}
		};
	}
}
#endif
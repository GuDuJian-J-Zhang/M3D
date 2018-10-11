// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_API
	*	@brief	API�ӿ���
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
			*	@brief		��ȡAPI
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		ָ��
			*	@note		��
			*	@attention	��
			**************************************************************************/
			static Stk_API* GetAPI();
			/**************************************************************************
			*	@brief		����API
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	ʹ��API��Ҫ���ô˽ӿڽ��йر�
			**************************************************************************/
			static void EndAPI();
			/**************************************************************************
			*	@brief		������־�ļ�
			*	@param[in]	Stk_Log_Type i_type		��־д�뷽ʽ��д���ļ������̨��
			*	@param[in]	Stk_Log_Level i_level	��־���𣬵��ڴ˼����򲻴�ӡ��
			Ĭ����߽��
			*	@param[in]	Stk_string i_strPath	��־·����д�����̨ʱ��ֵ��������
			*	@param[out]	��
			*	@retval		�ɹ�:STK_SUCCESS		ʧ��:����
			*	@note		��
			*	@attention	��
			**************************************************************************/
			STK_STATUS SetLog(Stk_Log_Type i_type, Stk_Log_Level i_level = STK_LOG_LEVEL_HIGHEST,
				Stk_string i_strPath = "");
			/**************************************************************************
			*	@brief		��ӡ��־
			*	@param[in]	Stk_string i_strMsg		��־��Ϣ
			*	@param[in]	Stk_Log_Level i_level	��־����
			*	@param[in]	int i_type				��־������Ϣ��Ĭ�ϲ�������Ϣ��
			STK_LOG_ADDITION_TIME:����ʱ����Ϣ
			*	@param[out]	��
			*	@retval		�ɹ�:STK_SUCCESS		ʧ��:����
			*	@note		��
			*	@attention	��
			**************************************************************************/
			STK_STATUS WriteLog(Stk_string i_strMsg, Stk_Log_Level i_level = STK_LOG_LEVEL_HIGHEST,
				int i_type = STK_LOG_ADDITION_NONE);
			/**************************************************************************
			*	@brief		��־��д����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		�ɹ�:STK_SUCCESS		ʧ��:����
			*	@note		��
			*	@attention	��
			**************************************************************************/
			STK_STATUS WriteLogEndl();
			/**************************************************************************
			*	@brief		���ļ�������Stk_DocumentManagerPtr
			*	@param[in]	const Stk_wstring &i_strSvlPath			�ļ�·��
			*	@param[out]	Stk_DocumentManagerPtr	o_pDocument		�ĵ�����ָ��
			*	@retval		�ɹ�:STK_SUCCESS		ʧ��:����
			*	@note		��
			*	@attention	��
			**************************************************************************/
			//STK_STATUS OpenDocument(const Stk_wstring &i_strSvlPath, Stk_DocumentManagerPtr &o_pDocument);
			/**************************************************************************
			*	@brief		��API������ɾ�������ĵ�����
			*	@param[in]	Stk_DocumentManagerPtr i_pDocument		�ĵ�����ָ��
			*	@param[out]	��
			*	@retval
			*	@note		���򿪻򴴽�һ��Documentʱ����������ʹ�ø�Document����ʹ�ô�
			���������йرա�
			*	@attention	��
			**************************************************************************/
			//STK_STATUS CloseDocument(Stk_DocumentManagerPtr i_pDocument);
			/**************************************************************************
			*	@brief		��API�������Ƿ񵥸��ĵ�����
			*	@param[in]	Stk_DocumentManagerPtr i_pDocument		�ĵ�����ָ��
			*	@param[out]	��
			*	@retval		����:true		������:false
			*	@note		��
			*	@attention	��
			**************************************************************************/
			//bool IsActiveDocument(Stk_DocumentManagerPtr i_pDocument);

			/**************************************************************************
			*	@brief		ͨ��uuid��API�����в��ҵ����ĵ�����
			*	@param[in]	Stk_string i_strUuid		�ĵ�����ָ���uuid
			*	@retval		�ĵ�����ָ��
			*	@note		��
			*	@attention	��
			**************************************************************************/
			//Stk_DocumentManagerPtr FindDocument(Stk_string i_strUuid);
			/**************************************************************************
			*	@brief		��ȡ�ļ��İ汾��
			*	@param[in]	i_strPath		�ļ�·��
			*	@param[out]	o_Version		�汾��
			*	@retval		���ҳɹ�:STK_SUCCESS		ʧ��:����
			*	@note		��
			*	@attention	��
			**************************************************************************/
			//STK_STATUS GetFileVersion(const Stk_wstring &i_strPath, Stk_wstring& o_Version);
			/**************************************************************************
			*	@brief		��ȡ����Document
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		Stk_Tab<Stk_DocumentManagerPtr>
			*	@note		��
			*	@attention	��
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
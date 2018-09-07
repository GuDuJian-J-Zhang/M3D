// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_DocumentManager
	*
	*
**************************************************************************/
#ifndef _SVLLIB_STK_FILE_H_
#define _SVLLIB_STK_FILE_H_
#include "Stk_Object.h"

namespace HoteamSoft {
//�Ƿ������浽�ڴ��ʶ
	namespace SVLLib {
		class Mutex;

		class STK_TOOLKIT_EXPORT Stk_File: public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_File)	
		protected:
				enum { _typeID = _SVLLIB_MESHPRIM };	//!< ���
		public:
			static bool ALLOW_CACHE_TOMEMORY;
		public:
			Stk_File(void);
			virtual ~Stk_File(void);
			//��ȡ��ǰ����
			STK_UINT32 GetByteLength();
			STK_UINT32 WriteBuffer(STK_CHAR*dst, STK_UINT32 bufferLength);
			STK_UINT32 ReWriteBuffer(STK_CHAR*dst, STK_UINT32 bufferLength, STK_UINT32 fileByteOffset);
			STK_BOOL ReadBuffer(STK_CHAR*dst, STK_UINT32 bufferLength, STK_UINT32 fileByteOffset);
			STK_BOOL OpenAsBin(const char* fileFlag = "rb");
			//�ڴ��ļ�
			STK_CHAR* OpenAsBuffer(STK_UINT32 buffersize);
			STK_BOOL OpenAsAssic();
			STK_BOOL Close();

			void SetName(const string& fileName);
			const string& GetName();
			STK_UINT32 FileSize();

			//�õ��ڴ��ļ����ݵ�ַ
			STK_CHAR* GetBuffer();
			//ɾ���ļ�
			STK_STATUS deleteFile();

		private:	
			STK_BOOL AllowCache();			
			void SetAllowCache(STK_BOOL allowToCache);

			void Update();

			STK_UINT32 WriteBufferToFile(STK_CHAR*dst, STK_UINT32 bufferLength);
			STK_UINT32 ReWriteBufferToFile(STK_CHAR*dst, STK_UINT32 bufferLength, STK_UINT32 fileByteOffset);
			STK_BOOL ReadBufferFromFile(STK_CHAR*dst, STK_UINT32 bufferLength, STK_UINT32 fileByteOffset);

			STK_UINT32 WriteBufferToCache(STK_CHAR*dst, STK_UINT32 bufferLength);
			STK_UINT32 ReWriteBufferToCache(STK_CHAR*dst, STK_UINT32 bufferLength, STK_UINT32 fileByteOffset);
			STK_BOOL ReadBufferFromCache(STK_CHAR*dst, STK_UINT32 bufferLength, STK_UINT32 fileByteOffset);

			bool CacheFileToMemory();
			bool MemoryToFile();

		private:
			FILE* m_file;
			string m_fileName;
			STK_UINT32 m_fileByteLength; //��¼��ǰ�ļ������Ƴ���
			STK_UINT32 m_currentIndex; //��¼��ǰ�ļ���ȡλ��

			STK_BOOL m_allowCache; //�Ƿ�������л��洦��

			vector<STK_CHAR> m_cacheMemory;
			Mutex*			m_pMutexDocRead;
		};

	}//svllib
}//hoteamsoft

#endif
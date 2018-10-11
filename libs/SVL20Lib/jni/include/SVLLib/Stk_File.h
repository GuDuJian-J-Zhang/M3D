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
//是否允许缓存到内存标识
	namespace SVLLib {
		class Mutex;

		class STK_TOOLKIT_EXPORT Stk_File: public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_File)	
		protected:
				enum { _typeID = _SVLLIB_MESHPRIM };	//!< 类别
		public:
			static bool ALLOW_CACHE_TOMEMORY;
		public:
			Stk_File(void);
			virtual ~Stk_File(void);
			//获取当前长度
			STK_UINT32 GetByteLength();
			STK_UINT32 WriteBuffer(STK_CHAR*dst, STK_UINT32 bufferLength);
			STK_UINT32 ReWriteBuffer(STK_CHAR*dst, STK_UINT32 bufferLength, STK_UINT32 fileByteOffset);
			STK_BOOL ReadBuffer(STK_CHAR*dst, STK_UINT32 bufferLength, STK_UINT32 fileByteOffset);
			STK_BOOL OpenAsBin(const char* fileFlag = "rb");
			//内存文件
			STK_CHAR* OpenAsBuffer(STK_UINT32 buffersize);
			STK_BOOL OpenAsAssic();
			STK_BOOL Close();

			void SetName(const string& fileName);
			const string& GetName();
			STK_UINT32 FileSize();

			//得到内存文件数据地址
			STK_CHAR* GetBuffer();
			//删除文件
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
			STK_UINT32 m_fileByteLength; //纪录当前文件二进制长度
			STK_UINT32 m_currentIndex; //纪录当前文件读取位置

			STK_BOOL m_allowCache; //是否允许进行缓存处理

			vector<STK_CHAR> m_cacheMemory;
			Mutex*			m_pMutexDocRead;
		};

	}//svllib
}//hoteamsoft

#endif
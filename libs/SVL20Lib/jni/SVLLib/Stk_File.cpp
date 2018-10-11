// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_ComponentFeature.cpp
 *
 *		2016/11/22	÷ÿππ°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_File.h"

namespace HoteamSoft {
	namespace SVLLib {
		bool Stk_File::ALLOW_CACHE_TOMEMORY = false;

		STK_IMPLEMENT_CLASS(Stk_File, Stk_Object)
		Stk_File::Stk_File(void) :Stk_Object()
		{
			this->m_file = NULL;
			this->m_fileName = "";
			this->m_fileByteLength = 0;
			this->SetAllowCache(ALLOW_CACHE_TOMEMORY);
		}

		Stk_File::~Stk_File(void)
		{
			//DELETE_STK_CLASS_IMPL
			this->Close();
		}

		//ªÒ»°µ±«∞≥§∂»
		STK_UINT32 Stk_File::GetByteLength()
		{
			if (this->m_file)
			{
				return m_fileByteLength;
			}
			return 0;
		}

		STK_UINT32 Stk_File::WriteBuffer(STK_CHAR*dst, STK_UINT32 bufferLength)
		{
			if (this->AllowCache())
			{
				return this->WriteBufferToCache(dst, bufferLength);
			}
			else
			{
				return this->WriteBufferToFile(dst, bufferLength);
			}
		}

		STK_UINT32 Stk_File::ReWriteBuffer(STK_CHAR*dst, STK_UINT32 bufferLength, STK_UINT32 fileByteOffset)
		{
			/*if (this->AllowCache())
			{
				return this->ReWriteBufferToCache(dst, bufferLength, fileByteOffset);
			}
			else*/
			{
				return this->ReWriteBufferToFile(dst, bufferLength, fileByteOffset);
			}
		}

		STK_BOOL Stk_File::ReadBuffer(STK_CHAR*dst, STK_UINT32 bufferLength, STK_UINT32 fileByteOffset)
		{
			if (this->AllowCache())
			{
				return this->ReadBufferFromCache(dst, bufferLength, fileByteOffset);
			}
			else
			{
				return this->ReadBufferFromFile(dst, bufferLength, fileByteOffset);
			}
		}

		STK_UINT32 Stk_File::WriteBufferToFile(STK_CHAR*dst, STK_UINT32 bufferLength)
		{
			if (this->m_file)
			{
				if (fwrite(dst, 1, bufferLength, m_file) == 0)//»Ù¥Û”⁄bufferLength‘Ú“‚Œ∂◊≈≤˙…˙¡À¥ÌŒÛ
				{
					return false;
				}
				//√ø¥Œ–¥Œƒº˛≥…π¶∏¸–¬“ªœ¬£¨ºÕ¬ºµƒŒƒº˛≥§∂»£ª
				this->m_fileByteLength += bufferLength;
			}
			return this->m_fileByteLength;
		}

		STK_UINT32 Stk_File::ReWriteBufferToFile(STK_CHAR*dst, STK_UINT32 bufferLength, STK_UINT32 fileByteOffset)
		{
			if (this->m_file)
			{

			}

			return 0;
		}

		STK_BOOL Stk_File::ReadBufferFromFile(STK_CHAR*dst, STK_UINT32 bufferLength, STK_UINT32 fileByteOffset)
		{
			if (this->m_file)
			{
				fseek(m_file, fileByteOffset, 0);
				if (fread(dst, 1, bufferLength, m_file) > bufferLength)//»Ù¥Û”⁄bufferLength‘Ú“‚Œ∂◊≈≤˙…˙¡À¥ÌŒÛ
				{
					return false;
				}
			}
			return true;
		}

		STK_UINT32 Stk_File::WriteBufferToCache(STK_CHAR*dst, STK_UINT32 bufferLength)
		{
			if (this->m_allowCache)
			{
				STK_CHAR* data = this->m_cacheMemory.data();

				if (this->m_fileByteLength + bufferLength <= this->m_cacheMemory.size())
				{
					memcpy(data + this->m_fileByteLength, dst, bufferLength);
				}

				//√ø¥Œ–¥Œƒº˛≥…π¶∏¸–¬“ªœ¬£¨ºÕ¬ºµƒŒƒº˛≥§∂»£ª
				this->m_fileByteLength += bufferLength;
			}
			return this->m_fileByteLength;
		}

		STK_UINT32 Stk_File::ReWriteBufferToCache(STK_CHAR*dst, STK_UINT32 bufferLength, STK_UINT32 fileByteOffset)
		{
			if (this->m_file)
			{

			}
			return false;
		}

		STK_BOOL Stk_File::ReadBufferFromCache(STK_CHAR*dst, STK_UINT32 bufferLength, STK_UINT32 fileByteOffset)
		{
			if (this->m_allowCache)
			{
				STK_CHAR* data = this->m_cacheMemory.data();

				if (fileByteOffset + bufferLength <= this->m_cacheMemory.size())
				{
					memcpy(dst,data + fileByteOffset,bufferLength);

					return true;
				}
			}
			return false;
		}

		bool Stk_File::CacheFileToMemory()
		{
			//Ω´Œƒº˛÷–µƒƒ⁄»›“ª¥Œ–‘∂¡»°µΩƒ⁄¥Ê÷–
			this->m_cacheMemory.reserve(this->FileSize()+8);
			this->m_cacheMemory.resize(this->FileSize());

			STK_CHAR* dst = static_cast<STK_CHAR*>(this->m_cacheMemory.data());
			STK_UINT32 bufferLength = this->m_cacheMemory.size();

			return ReadBufferFromFile(dst,bufferLength,0);
		}

		bool Stk_File::MemoryToFile() 
		{
			//Ω´Œƒº˛÷–µƒƒ⁄»›“ª¥Œ–‘∂¡»°µΩƒ⁄¥Ê÷–
			STK_CHAR* dst = static_cast<STK_CHAR*>(this->m_cacheMemory.data());
			STK_UINT32 bufferLength = this->m_cacheMemory.size();

			return WriteBufferToFile(dst, bufferLength);
		}

		void Stk_File::Update()
		{

		}

		STK_BOOL Stk_File::OpenAsBin(const char* fileFlag)
		{
			if (!this->m_file)
			{
				this->m_file = fopen(this->m_fileName.c_str(), fileFlag);
				if (!this->m_file)
				{
					return false;
				}	
				//Ω´ ˝æ›¥”Œƒº˛÷–“ª¥Œ–‘ª∫¥Êœ¬¿¥°£
				if (this->AllowCache())
				{
					this->CacheFileToMemory();
				}
			}
			return true;
		}

		STK_CHAR* Stk_File::OpenAsBuffer(STK_UINT32 buffersize)
		{
			if (buffersize>0)
			{
				this->m_cacheMemory.resize(buffersize);
				this->SetAllowCache(true);
			}

			return NULL;
		}

		STK_BOOL Stk_File::OpenAsAssic()
		{
			return true;
		}

		STK_BOOL Stk_File::Close()
		{
			m_cacheMemory.resize(0);
			m_cacheMemory.shrink_to_fit();

			if (this->m_file)
			{
				if (fclose(this->m_file) == EOF)
				{
					m_file = NULL;
					return false;
				}
			}
			m_file = NULL;

			return true;
		}

		void Stk_File::SetName(const string& fileName)
		{
            m_fileName = fileName;
        }
        
        const string& Stk_File::GetName()
		{
			return m_fileName;
		}

		STK_BOOL Stk_File::AllowCache()
		{
			return this->m_allowCache;
		}

		void Stk_File::SetAllowCache(STK_BOOL allowToCache)
		{
			this->m_allowCache = allowToCache;
		}

		STK_UINT32 Stk_File::FileSize()
		{
			if (this->AllowCache())
			{
				return this->m_cacheMemory.size();
			}

			if (this->m_file)
			{
				fseek(m_file, 0, SEEK_END);
				return ftell(m_file);
			}
			return 0;
		}

		STK_CHAR* Stk_File::GetBuffer()
		{
			if (this->AllowCache())
			{
				return this->m_cacheMemory.data();
			}

			return NULL;
		}

		STK_STATUS Stk_File::deleteFile()
		{
			return (STK_STATUS)remove(this->m_fileName.c_str());
		}

	}//svllib
}//hoteamsoft

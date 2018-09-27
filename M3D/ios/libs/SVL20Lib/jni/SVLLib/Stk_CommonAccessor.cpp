// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
*	@file	Stk_ComponentFeature.cpp
*
*		2016/11/22	÷ÿππ°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_CommonAccessor.h"
#include "JsonMacro.h"


namespace HoteamSoft {
    namespace SVLLib {
        STK_IMPLEMENT_CLASS(Stk_CommonAccessor, Stk_Accessor)
			Stk_CommonAccessor::Stk_CommonAccessor(void):Stk_Accessor()
		{
			m_byteoffset = 0;
			m_dataByteLength = 0;
		}

		Stk_CommonAccessor::~Stk_CommonAccessor(void)
		{
			//DELETE_STK_CLASS_IMPL
		}

		STK_BOOL Stk_CommonAccessor::Encode(rapidjson::Value& accessor, rapidjson::Document& bomDoc)
		{
			accessor.SetObject();
			//accessor[MESH_BIN_FILE] = StringRef(this->m_binFileName.c_str());
			Value value(StringRef(this->m_binFileName.c_str()), bomDoc.GetAllocator());
			accessor.AddMember(MESH_BIN_FILE, value, bomDoc.GetAllocator());
			accessor.AddMember(MESH_BYTEOFFSET, this->m_byteoffset, bomDoc.GetAllocator());
			accessor.AddMember(DATA_LENGTH, this->m_dataByteLength, bomDoc.GetAllocator());
 
			return true;
		}

		STK_BOOL Stk_CommonAccessor::Uncode(rapidjson::Value& accessor)
		{
			this->m_binFileName = accessor[FILENAME].GetString();
			this->m_byteoffset = accessor[OFFSET].GetInt();
			this->m_dataByteLength = accessor[LENGTH].GetInt();
			return true;
		}

		STK_BOOL Stk_CommonAccessor::Uncode(string i_strMeshFileName, STK_UINT32 i_offSet, STK_UINT32 i_length)
		{
			this->m_binFileName = i_strMeshFileName;
			this->m_byteoffset = i_offSet;
			this->m_dataByteLength = i_length;
			return true;
		}

		STK_BOOL Stk_CommonAccessor::ReadFile(STK_CHAR* dataSrc, STK_UINT32 srcByteLength,STK_UINT32 srcByteOffset,Stk_FilePtr& binFile)
		{
			if (binFile.isNotNull())
			{
				binFile->ReadBuffer(dataSrc, srcByteLength, srcByteOffset);
			}
			return true;
		}

		const string& Stk_CommonAccessor::GetBinFileName()
		{
			return this->m_binFileName;
		}

		void Stk_CommonAccessor::SetBinFileName(const string& fileName)
		{
			this->m_binFileName = fileName;
		}


		STK_UINT32 Stk_CommonAccessor::GetByteOffset()
		{
			return this->m_byteoffset;
		}

		STK_UINT32  Stk_CommonAccessor::GetByteLength()
		{
			return this->m_dataByteLength;
		}


		STK_BOOL Stk_CommonAccessor::WriteFile(STK_CHAR* dataSrc,STK_UINT32 srcByteLength, Stk_FilePtr& binFile)
		{
			if (binFile.isNotNull())
			{
				this->m_byteoffset = binFile->GetByteLength();
				binFile->WriteBuffer(dataSrc, srcByteLength);
				m_dataByteLength = srcByteLength;
			}
			return true;
		}
	}//svllib
}//hoteamsoft

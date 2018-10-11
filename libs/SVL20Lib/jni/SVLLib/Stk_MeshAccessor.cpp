// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
*	@file	Stk_ComponentFeature.cpp
*
*		2016/11/22	÷ÿππ°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_MeshAccessor.h"


namespace HoteamSoft {
	namespace SVLLib {
		STK_IMPLEMENT_CLASS(Stk_MeshAccessor, Stk_Accessor)
		Stk_MeshAccessor::Stk_MeshAccessor(void):Stk_Accessor()
        {
            m_byteoffset = 0;
			m_indexByteLength = 0;
			m_attributeLength = 0;
			m_UVsLength = 0;
		}

		Stk_MeshAccessor::~Stk_MeshAccessor(void)
		{
			//DELETE_STK_CLASS_IMPL
		}

		STK_BOOL Stk_MeshAccessor::Encode(rapidjson::Value& accessor, rapidjson::Document& bomDoc)
		{
			accessor.SetObject();
			//accessor[MESH_BIN_FILE] = StringRef(this->m_binFileName.c_str());
			Value value(StringRef(this->m_binFileName.c_str()), bomDoc.GetAllocator());
			accessor.AddMember(MESH_BIN_FILE, value, bomDoc.GetAllocator());
			if (this->GetType() == ACCESSOR_TYPE_MESH_I_P_N)
			{
				accessor.AddMember("type", ACCESSOR_TYPE_MESH_I_P_N, bomDoc.GetAllocator());
				//accessor[MESH_BYTEOFFSET] = this->m_byteoffset;
				//accessor[MESH_INDEXLENGTH] = this->m_indexByteLength;
				//accessor[MESH_ATTRIBUTE] = this->m_attributeLength;
				accessor.AddMember(MESH_BYTEOFFSET, this->m_byteoffset, bomDoc.GetAllocator());
				accessor.AddMember(MESH_INDEXLENGTH, this->m_indexByteLength, bomDoc.GetAllocator());
				accessor.AddMember(MESH_ATTRIBUTE, this->m_attributeLength, bomDoc.GetAllocator());
				accessor.AddMember(MESH_UVSLENGTH, this->m_UVsLength, bomDoc.GetAllocator());
			}
			else if (this->GetType() == ACCESSOR_TYPE_MESH_N_P)
			{

			}

			return true;
		}

		STK_BOOL Stk_MeshAccessor::Uncode(rapidjson::Value& accessor)
		{
			this->m_binFileName = accessor[MESH_BIN_FILE].GetString();
			this->SetType(( StkAccessorTypeEnum)accessor["type"].GetInt());
			if (this->GetType() == ACCESSOR_TYPE_MESH_I_P_N)
			{
				this->m_byteoffset = accessor[MESH_BYTEOFFSET].GetInt();
				this->m_indexByteLength = accessor[MESH_INDEXLENGTH].GetInt();
				this->m_attributeLength = accessor[MESH_ATTRIBUTE].GetInt();
				this->m_UVsLength = accessor[MESH_UVSLENGTH].GetInt();

			}
			else if (this->GetType() == ACCESSOR_TYPE_MESH_N_P)
			{

			}
			
			return true;
		}

		STK_BOOL Stk_MeshAccessor::ReadFile(Stk_MeshPrimPtr& meshPrim,Stk_FilePtr& binFile)
		{
			//if (binFile)
			{
				if (this->GetType() == ACCESSOR_TYPE_MESH_I_P_N)
				{
					vector<STK_FLOAT32>& vecVert = meshPrim->m_vecVert;
					vector<STK_FLOAT32>& vecNormals = meshPrim->m_vecNormals;
					vector<STK_UINT32>& vecTrians = meshPrim->m_vecTrians;
					vector<STK_FLOAT32>& vecTextureCoordinate = meshPrim->m_vecTextureCoordinate;

					vecTrians.resize(m_indexByteLength / sizeof(STK_UINT32));
					vecNormals.resize(m_attributeLength / sizeof(STK_FLOAT32) / 2);
					vecVert.resize(m_attributeLength / sizeof(STK_FLOAT32) / 2);
					vecTextureCoordinate.resize(m_UVsLength / sizeof(STK_FLOAT32));

					binFile->ReadBuffer((STK_CHAR*)vecTrians.data(), m_indexByteLength, m_byteoffset);
					binFile->ReadBuffer((STK_CHAR*)vecVert.data(), m_attributeLength / 2, m_byteoffset + m_indexByteLength);
					binFile->ReadBuffer((STK_CHAR*)vecNormals.data(), m_attributeLength / 2, m_byteoffset + m_indexByteLength+ m_attributeLength / 2);
					binFile->ReadBuffer((STK_CHAR*)vecTextureCoordinate.data(), m_UVsLength, m_byteoffset + m_indexByteLength + m_attributeLength);
				}else if (this->GetType() == ACCESSOR_TYPE_MESH_N_P)
				{

				}
			}
			return true;
		}

		const string& Stk_MeshAccessor::GetBinFileName()
		{
			return this->m_binFileName;
		}

		void Stk_MeshAccessor::SetBinFileName(const string& fileName)
		{
			this->m_binFileName = fileName;
		}

		STK_BOOL Stk_MeshAccessor::WriteFile(Stk_MeshPrimPtr& meshPrim, Stk_FilePtr& binFile)
		{
			//if (binFile))
			{
				//this->m_binFileName = binFile->GetName();
				if (this->GetType() == ACCESSOR_TYPE_MESH_I_P_N)
				{
					vector<STK_FLOAT32>& vecVert = meshPrim->m_vecVert;
					vector<STK_FLOAT32>& vecNormals = meshPrim->m_vecNormals;
					vector<STK_UINT32>& vecTrians = meshPrim->m_vecTrians;
					vector<STK_FLOAT32>& vecTextureCoordinate = meshPrim->m_vecTextureCoordinate;

					this->m_indexByteLength = vecTrians.size() * sizeof(STK_UINT32);
					this->m_attributeLength = vecNormals.size() * sizeof(STK_FLOAT32)+ vecVert.size() * sizeof(STK_FLOAT32);;
					this->m_UVsLength = vecTextureCoordinate.size() * sizeof(STK_FLOAT32);

					this->m_byteoffset = binFile->GetByteLength();

					binFile->WriteBuffer((STK_CHAR*)vecTrians.data(), m_indexByteLength);
					binFile->WriteBuffer((STK_CHAR*)vecVert.data(), m_attributeLength / 2);
					binFile->WriteBuffer((STK_CHAR*)vecNormals.data(), m_attributeLength / 2);
					binFile->WriteBuffer((STK_CHAR*)vecTextureCoordinate.data(), m_UVsLength);
				}
				else if (this->GetType() == ACCESSOR_TYPE_MESH_N_P)
				{

				}
			}
			return true;
		}
	}//svllib
}//hoteamsoft

// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_DocumentManager
*
*
**************************************************************************/
#ifndef _SVLLIB_STK_COMMONCESSOR_H_
#define _SVLLIB_STK_COMMONCESSOR_H_
#include "Stk_Accessor.h"
#include <string>
#include "Stk_MeshPrim.h"
#include "Stk_ResourceManager.h"
#include "Stk_File.h"
// #include <map>

namespace HoteamSoft {
	namespace SVLLib {
        
        class Stk_Document;
		class STK_TOOLKIT_EXPORT Stk_CommonAccessor:public Stk_Accessor
		{
			STK_DECLARE_CLASS(Stk_CommonAccessor)
		protected:
			enum {
				_typeID = _SVLLIB_COMMON_ACCESSOR
			};	//!< ¿‡±
		public:
			Stk_CommonAccessor(void);
			virtual ~Stk_CommonAccessor(void);

			virtual STK_BOOL Encode(rapidjson::Value& accessor, rapidjson::Document& bomDoc);
			virtual STK_BOOL Uncode(rapidjson::Value& accessor);
			 STK_BOOL Uncode(string i_strMeshFileName, STK_UINT32 i_offSet, STK_UINT32 i_length);

			virtual STK_BOOL ReadFile(STK_CHAR* dataSrc, STK_UINT32 srcByteLength,STK_UINT32 srcByteOffset,Stk_FilePtr& binFile);
			virtual STK_BOOL WriteFile(STK_CHAR* dataSrc, STK_UINT32 srcByteLength, Stk_FilePtr& binFile);

			const string& GetBinFileName();
			void SetBinFileName(const string& fileName);

			STK_UINT32 GetByteOffset();
			STK_UINT32 GetByteLength();

		private:
			string m_binFileName;
			unsigned int m_byteoffset;
			unsigned int m_dataByteLength;
		};

	}//svllib
}//hoteamsoft

#endif

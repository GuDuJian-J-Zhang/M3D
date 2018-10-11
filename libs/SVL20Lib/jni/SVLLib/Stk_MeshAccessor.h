// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_DocumentManager
*
*
**************************************************************************/
#ifndef _SVLLIB_STK_MESHACCESSOR_H_
#define _SVLLIB_STK_MESHACCESSOR_H_
#include "Stk_Accessor.h"
#include <string>
#include "Stk_MeshPrim.h"
#include "Stk_ResourceManager.h"
#include "Stk_File.h"
// #include <map>

namespace HoteamSoft {
	namespace SVLLib {
        
        class Stk_Document;
		class STK_TOOLKIT_EXPORT Stk_MeshAccessor:public Stk_Accessor
		{
			STK_DECLARE_CLASS(Stk_MeshAccessor)
		protected:
				enum { _typeID = _SVLLIB_MESHPRIM };	//!< ¿‡±
		public:
			Stk_MeshAccessor(void);
			virtual ~Stk_MeshAccessor(void);

			virtual STK_BOOL Encode(rapidjson::Value& accessor, rapidjson::Document& bomDoc);
			virtual STK_BOOL Uncode(rapidjson::Value& accessor);

			virtual STK_BOOL ReadFile(Stk_MeshPrimPtr& meshPrim, Stk_FilePtr& binFile);
			virtual STK_BOOL WriteFile(Stk_MeshPrimPtr& meshPrim, Stk_FilePtr& binFile);

			const string& GetBinFileName();
			void SetBinFileName(const string& fileName);

		private:
			string m_binFileName;
			unsigned int m_byteoffset;
			unsigned int m_indexByteLength;
			unsigned int m_attributeLength;
			unsigned int m_UVsLength;
		};

	}//svllib
}//hoteamsoft

#endif

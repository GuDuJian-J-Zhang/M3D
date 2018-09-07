// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_DocumentManager
	*
	*
**************************************************************************/
#ifndef _SVLLIB_STK_RESOURCEMANGER_H_
#define _SVLLIB_STK_RESOURCEMANGER_H_
#include "Stk_Object.h"
#include "Stk_File.h"
#include <map>

namespace HoteamSoft {
	namespace SVLLib {

		class STK_TOOLKIT_EXPORT Stk_ResourceManager :public Stk_Object
        {
            STK_DECLARE_CLASS(Stk_ResourceManager)
		protected:
			enum { _typeID = _SVLLIB_MESHPRIM };	//!< ¿‡±
		public:
			Stk_ResourceManager(void);
			virtual ~Stk_ResourceManager(void);
			virtual Stk_FilePtr GetFile(const string& fileUri);

			virtual Stk_FilePtr AddFile(const string& fileUri);
			virtual void RemoveFile(const string& fileUri);

			bool Clear();
			bool CloseAllFile();

			void SetWorkingDir(const string& workingDir);
			const string& GetWorkingDir();

		private:
			string m_workingDir;
		protected:
			map<string, Stk_FilePtr> m_files;
		};

	}//svllib
}//hoteamsoft

#endif

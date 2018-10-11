// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_DocumentManager
*
*
**************************************************************************/
#ifndef _SVLLIB_STRINGTOOLS_H_
#define _SVLLIB_STRINGTOOLS_H_

#include <string>
#include <vector>

using namespace std;

namespace HoteamSoft {
	namespace SVLLib {
		class CStringTools
		{
		public:
			static void StringSplit(const std::string& str, const std::string& pattern, std::vector<std::string>& result);
			static void StringSplit(const std::wstring& str, const std::wstring& pattern, std::vector<std::wstring>& result);
		};
	}
}
#endif //_SVLLIB_STRINGTOOLS_H_

#include "StringTools.h"

namespace HoteamSoft {
	namespace SVLLib {
		void CStringTools::StringSplit(const std::string& str, const std::string& pattern, std::vector<std::string>& result)
		{
			std::string::size_type pos;
			int size = str.size();

			for (int i = 0; i < size; i++)
			{
				pos = str.find(pattern, i);
				if (pos == -1)
				{
					//√ª”–’“µΩ£¨»° £”‡◊÷∑˚¥Æ
					std::string s = str.substr(i, size - i);
					result.push_back(s);
					break;
                }
                if (pos < size)
				{
					std::string s = str.substr(i, pos - i);
					result.push_back(s);
					i = pos + pattern.size() - 1;
				}
				else
				{
					break;
				}
			}
		}

		void CStringTools::StringSplit(const std::wstring& str, const std::wstring& pattern, std::vector<std::wstring>& result)
		{
			std::wstring::size_type pos;
			int size = str.size();

			for (int i = 0; i < size; i++)
			{
				pos = str.find(pattern, i);
				if (pos == -1)
				{
					//√ª”–’“µΩ£¨»° £”‡◊÷∑˚¥Æ
					std::wstring s = str.substr(i, size - i);
					result.push_back(s);
					break;
				}
				if (pos < size)
				{
					std::wstring s = str.substr(i, pos - i);
					result.push_back(s);
					i = pos + pattern.size() - 1;
				}
				else
				{
					break;
				}
			}
		}

	}
}


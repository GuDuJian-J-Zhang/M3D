#include "StringHash.h"
#include <cstdio>

//#include "../DebugNew.h"

namespace HoteamSoft
{
	namespace SVLLib {
		inline unsigned SDBMHash(unsigned hash, unsigned char c) {
			return c + (hash << 6) + (hash << 16) - hash;
		}

		const StringHash StringHash::ZERO;

		StringHash::StringHash(const char* str) :
			value_(Calculate(str))
		{
		}

		StringHash::StringHash(const string& str) :
			value_(Calculate(str.c_str()))
		{
		}

		unsigned StringHash::Calculate(const char* str)
		{
			unsigned hash = 0;

			if (!str)
				return hash;

			while (*str)
			{
				// Perform the actual hashing as case-insensitive
				char c = *str;
				hash = SDBMHash(hash, tolower(c));
				++str;
			}

			return hash;
		}
	}
 
}

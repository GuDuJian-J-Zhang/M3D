

#include "m3d/base/MathDefs.h"
#include "m3d/base/StringHash.h"

#include <cstdio>

//#include "../DebugNew.h"

namespace M3D
{

const StringHash StringHash::ZERO;

StringHash::StringHash(const char* str) :
    m_value(Calculate(str))
{
}

StringHash::StringHash(const string& str) :
    m_value(Calculate(str.c_str()))
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

string StringHash::Tostring() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%08X", m_value);
    return string(tempBuffer);
}

}


#include "m3d/base/Vector4.h"

#include <cstdio>

//#include "../DebugNew.h"

namespace M3D
{

const Vector4 Vector4::ZERO;
const Vector4 Vector4::ONE(1.0f, 1.0f, 1.0f, 1.0f);

string Vector4::Tostring() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%g %g %g %g", m_x, m_y, m_z, m_w);
    return string(tempBuffer);
}

Vector4::Vector4() :
	m_x(0.0f),
	m_y(0.0f),
	m_z(0.0f),
	m_w(0.0f)
{

}

}

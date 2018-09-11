
#include "m3d/base/Vector3.h"

#include <cstdio>

//#include "../DebugNew.h"

namespace M3D
{

const Vector3 Vector3::ZERO;
const Vector3 Vector3::LEFT(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::RIGHT(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UP(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::DOWN(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::FORWARD(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::BACK(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::ONE(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::MINIMUM(-99999.0f, -99999.0f, -9999.0f);
const Vector3 Vector3::MAXMUN(99999.0f, 99999.0f, 9999.0f);

string Vector3::Tostring() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%.7g %.7g %.7g", m_x, m_y, m_z);
    return string(tempBuffer);
}

Vector3::Vector3() :
	m_x(0.0f),
	m_y(0.0f),
	m_z(0.0f)
{

}

}

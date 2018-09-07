
#include "m3d/base/Vector2.h"

#include <cstdio>

//#include "../DebugNew.h"

namespace M3D
{

const Vector2 Vector2::ZERO;
const Vector2 Vector2::LEFT(-1.0f, 0.0f);
const Vector2 Vector2::RIGHT(1.0f, 0.0f);
const Vector2 Vector2::UP(0.0f, 1.0f);
const Vector2 Vector2::DOWN(0.0f, -1.0f);
const Vector2 Vector2::ONE(1.0f, 1.0f);

const IntVector2 IntVector2::ZERO;

string Vector2::Tostring() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%g %g", m_x, m_y);
    return string(tempBuffer);
}

Vector2::Vector2() :
	m_x(0.0f),
	m_y(0.0f)
{

}

string IntVector2::Tostring() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%d %d", m_x, m_y);
    return string(tempBuffer);
}

}

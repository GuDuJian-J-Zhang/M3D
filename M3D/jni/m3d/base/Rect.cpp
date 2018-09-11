
#include "m3d/base/Rect.h"

#include <cstdio>

namespace M3D
{

const Rect Rect::FULL(-1.0f, -1.0f, 1.0f, 1.0f);
const Rect Rect::POSITIVE(0.0f, 0.0f, 1.0f, 1.0f);
const Rect Rect::ZERO(0.0f, 0.0f, 0.0f, 0.0f);

const IntRect IntRect::ZERO(0, 0, 0, 0);

IntVector2 IntRect::Center() const
{
	return IntVector2(m_left + Width() / 2, m_top + Height()/2);
}

string Rect::Tostring() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%g %g %g %g", m_min.m_x, m_min.m_y, m_max.m_x, m_max.m_y);
    return string(tempBuffer);
}

string IntRect::Tostring() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%d %d %d %d", m_left, m_top, m_right, m_bottom);
    return string(tempBuffer);
}

void Rect::Clip(const Rect& rect)
{
    if (rect.m_min.m_x > m_min.m_x)
        m_min.m_x = rect.m_min.m_x;
    if (rect.m_max.m_x < m_max.m_x)
        m_max.m_x = rect.m_max.m_x;
    if (rect.m_min.m_y > m_min.m_y)
        m_min.m_y = rect.m_min.m_y;
    if (rect.m_max.m_y < m_max.m_y)
        m_max.m_y = rect.m_max.m_y;
    
    if (m_min.m_x > m_max.m_x)
        swap(m_min.m_x, m_max.m_x);
    if (m_min.m_y > m_max.m_y)
        swap(m_min.m_y, m_max.m_y);
}



}

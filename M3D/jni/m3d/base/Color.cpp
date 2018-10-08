
#include "m3d/base/Color.h"
#include "m3d/M3D.h"

#include <cstdio>

namespace M3D
{
Color* Color::Default = new Color(0.8, 0.8, 0.8, 1.0);
Color* Color::SelectColor = new Color(1, 0, 0, 1.0);
Color* Color::SelectorShelter = new Color(1, 0, 0, 0.2);

Color* Color::FaceDefaultColor = new Color(1, 0, 0, 1.0);
Color* Color::FaceSelectColor  = new Color(1, 0, 0, 1.0);

Color* Color::EdgeDefaultColor =  new Color(0.25, 0.25, 0.25, 1.0);
Color* Color::EdgeSelectColor = new Color(1, 0, 0, 1.0);


const Color Color::WHITE  (1.0f,1.0f,1.0f,1.0f);
const Color Color::GRAY   (0.5f, 0.5f, 0.5f);
const Color Color::BLACK  (0.0f, 0.0f, 0.0f);
const Color Color::RED    (1.0f, 0.0f, 0.0f);
const Color Color::GREEN  (0.0f, 1.0f, 0.0f);
const Color Color::BLUE   (0.0f, 0.0f, 1.0f);
const Color Color::CYAN   (0.0f, 1.0f, 1.0f);
const Color Color::MAGENTA(1.0f, 0.0f, 1.0f);
const Color Color::YELLOW (1.0f, 1.0f, 0.0f);
const Color Color::TRANSPARENT(0.0f, 0.0f, 0.0f, 0.0f);

unsigned Color::ToUInt() const
{
    unsigned r = Clamp(((int)(m_r * 255.0f)), 0, 255);
    unsigned g = Clamp(((int)(m_g * 255.0f)), 0, 255);
    unsigned b = Clamp(((int)(m_b * 255.0f)), 0, 255);
    unsigned a = Clamp(((int)(m_a * 255.0f)), 0, 255);
    return (a << 24) | (b << 16) | (g << 8) | r;
}

Vector3 Color::ToHSL() const
{
    float min, max;
    Bounds(&min, &max, true);

    float h = Hue(min, max);
    float s = SaturationHSL(min, max);
    float l = (max + min) * 0.5f;

    return Vector3(h, s, l);
}

Vector3 Color::ToHSV() const
{
    float min, max;
    Bounds(&min, &max, true);

    float h = Hue(min, max);
    float s = SaturationHSV(min, max);
    float v = max;

    return Vector3(h, s, v);
}

void Color::FromHSL(float h, float s, float l, float a)
{
    float c;
    if (l < 0.5f)
        c = (1.0f + (2.0f * l - 1.0f)) * s;
    else
        c = (1.0f - (2.0f * l - 1.0f)) * s;

    float m = l - 0.5f * c;

    FromHCM(h, c, m);

    m_a = a;
}

void Color::FromHSV(float h, float s, float v, float a)
{
    float c = v * s;
    float m = v - c;

    FromHCM(h, c, m);

    m_a = a;
}

float Color::Chroma() const
{
    float min, max;
    Bounds(&min, &max, true);

    return max - min;
}

float Color::Hue() const
{
    float min, max;
    Bounds(&min, &max, true);

    return Hue(min, max);
}

float Color::SaturationHSL() const
{
    float min, max;
    Bounds(&min, &max, true);

    return SaturationHSL(min, max);
}

float Color::SaturationHSV() const
{
    float min, max;
    Bounds(&min, &max, true);

    return SaturationHSV(min, max);
}

float Color::Lightness() const
{
    float min, max;
    Bounds(&min, &max, true);

    return (max + min) * 0.5f;
}

void Color::Bounds(float* min, float* max, bool clipped) const
{
    assert(min && max);

    if (m_r > m_g)
    {
        if (m_g > m_b) // r > g > b
        {
            *max = m_r;
            *min = m_b;
        }
        else // r > g && g <= b
        {
            *max = m_r > m_b ? m_r : m_b;
            *min = m_g;
        }
    }
    else
    {
        if (m_b > m_g) // r <= g < b
        {
            *max = m_b;
            *min = m_r;
        }
        else // r <= g && b <= g
        {
            *max = m_g;
            *min = m_r < m_b ? m_r : m_b;
        }
    }

    if (clipped)
    {
        *max = *max > 1.0f ? 1.0f : (*max < 0.0f ? 0.0f : *max);
        *min = *min > 1.0f ? 1.0f : (*min < 0.0f ? 0.0f : *min);
    }
}

float Color::MaxRGB() const
{
    if (m_r > m_g)
        return (m_r > m_b) ? m_r : m_b;
    else
        return (m_g > m_b) ? m_g : m_b;
}

float Color::MinRGB() const
{
    if (m_r < m_g)
        return (m_r < m_b) ? m_r : m_b;
    else
        return (m_g < m_b) ? m_g : m_b;
}

float Color::Range() const
{
    float min, max;
    Bounds(&min, &max);
    return max - min;
}

void Color::Clip(bool clipAlpha)
{
    m_r = (m_r > 1.0f) ? 1.0f : ((m_r < 0.0f) ? 0.0f : m_r);
    m_g = (m_g > 1.0f) ? 1.0f : ((m_g < 0.0f) ? 0.0f : m_g);
    m_b = (m_b > 1.0f) ? 1.0f : ((m_b < 0.0f) ? 0.0f : m_b);

    if (clipAlpha)
        m_a = (m_a > 1.0f) ? 1.0f : ((m_a < 0.0f) ? 0.0f : m_a);
}

void Color::Invert(bool invertAlpha)
{
    m_r = 1.0f - m_r;
    m_g = 1.0f - m_g;
    m_b = 1.0f - m_b;

    if (invertAlpha)
        m_a = 1.0f - m_a;
}

Color Color::Lerp(const Color &rhs, float t) const
{
    float invT = 1.0f - t;
    return Color(
        m_r * invT + rhs.m_r * t,
        m_g * invT + rhs.m_g * t,
        m_b * invT + rhs.m_b * t,
        m_a * invT + rhs.m_a * t
    );
}

string Color::Tostring() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%g %g %g %g", m_r, m_g, m_b, m_a);
    return string(tempBuffer);
}

float Color::Hue(float min, float max) const
{
    float chroma = max - min;

    // If chroma equals zero, hue is undefined
    if (chroma <= M_EPSILON)
        return 0.0f;

    // Calculate and return hue
    if (M3D::Equals(m_g, max))
        return (m_b + 2.0f*chroma - m_r) / (6.0f * chroma);
    else if (M3D::Equals(m_b, max))
        return (4.0f * chroma - m_g + m_r) / (6.0f * chroma);
    else
    {
        float r = (m_g - m_b) / (6.0f * chroma);
        return (r < 0.0f) ? 1.0f + r : ((r >= 1.0f) ? r - 1.0f : r);
    }

}

float Color::SaturationHSV(float min, float max) const
{
    // Avoid div-by-zero: result undefined
    if (max <= M_EPSILON)
        return 0.0f;

    // Saturation equals chroma:value ratio
    return 1.0f - (min / max);
}

float Color::SaturationHSL(float min, float max) const
{
    // Avoid div-by-zero: result undefined
    if (max <= M_EPSILON || min >= 1.0f - M_EPSILON)
        return 0.0f;

    // Chroma = max - min, lightness = (max + min) * 0.5
    float hl = (max + min);
    if (hl <= 1.0f)
        return (max - min) / hl;
    else
        return (min - max) / (hl - 2.0f);

}

void Color::FromHCM(float h, float c, float m)
{
    if (h < 0.0f || h >= 1.0f)
        h -= floorf(h);

    float hs = h * 6.0f;
    float x  = c * (1.0f - M3D::Abs(fmodf(hs, 2.0f) - 1.0f));

    // Reconstruct r', g', b' from hue
    if (hs < 2.0f)
    {
        m_b = 0.0f;
        if (hs < 1.0f)
        {
            m_g = x;
            m_r = c;
        }
        else
        {
            m_g = c;
            m_r = x;
        }
    }
    else if (hs < 4.0f)
    {
        m_r = 0.0f;
        if (hs < 3.0f)
        {
            m_g = c;
            m_b = x;
        }
        else
        {
            m_g = x;
            m_b = c;
        }
    }
    else
    {
        m_g = 0.0f;
        if (hs < 5.0f)
        {
            m_r = x;
            m_b = c;
        }
        else
        {
            m_r = c;
            m_b = x;
        }
    }

    m_r += m;
    m_g += m;
    m_b += m;
}

}

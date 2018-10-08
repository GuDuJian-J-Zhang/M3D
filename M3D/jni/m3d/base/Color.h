/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */

#ifndef M3D_COLOR_H_
#define M3D_COLOR_H_

#include "m3d/base/Vector4.h"

#undef TRANSPARENT

namespace M3D
{

/** 
 * @brief RGBA color.
 *
 */
class M3D_API Color
{
public:
	/**
	 * @brief Construct with default values (opaque white.)
	 *
	 */
	Color() :
			m_r(0.8f), m_g(0.8f), m_b(0.8f), m_a(1.0f)
	{

	}

	/**
	 * @brief Copy-construct from another color.
	 *
	 * @param color
	 */
	Color(const Color& color) :
			m_r(color.m_r), m_g(color.m_g), m_b(color.m_b), m_a(color.m_a)
	{
	}

	/**
	 * @brief Construct from another color and modify the alpha.
	 *
	 * @param color
	 * @param a
	 */
	Color(const Color& color, float a) :
			m_r(color.m_r), m_g(color.m_g), m_b(color.m_b), m_a(a)
	{
	}

	/**
	 * @brief Construct from RGB values and set alpha fully opaque.
	 *
	 * @param r
	 * @param g
	 * @param b
	 */
	Color(float r, float g, float b) :
			m_r(r), m_g(g), m_b(b), m_a(1.0f)
	{
	}

	/**
	 * @brief Construct from RGBA values.
	 *
	 * @param r
	 * @param g
	 * @param b
	 * @param a
	 */
	Color(float r, float g, float b, float a) :
			m_r(r), m_g(g), m_b(b), m_a(a)
	{
	}

	/**
	 * @brief Construct from a float array.
	 *
	 * @param data
	 */
	Color(const float* data) :
			m_r(data[0]), m_g(data[1]), m_b(data[2]), m_a(data[3])
	{
	}
	void SetColor(float r, float g, float b, float a = 1.0f)
	{
		m_r = r;
		m_g = g;
		m_b = b;
		m_a = a;
	}

	/**
	 * @brief Test for equality with another color without epsilon.
	 *
	 * @param rhs
	 * @return
	 */
	bool operator ==(const Color& rhs) const
	{
		return m_r == rhs.m_r && m_g == rhs.m_g && m_b == rhs.m_b
				&& m_a == rhs.m_a;
	}
	/**
	 * @brief Test for inequality with another color without epsilon.
	 *
	 * @param rhs
	 * @return
	 */
	bool operator !=(const Color& rhs) const
	{
		return m_r != rhs.m_r || m_g != rhs.m_g || m_b != rhs.m_b
				|| m_a != rhs.m_a;
	}
	/**
	 * @brief Multiply with a scalar.
	 *
	 * @param rhs
	 * @return
	 */
	Color operator *(float rhs) const
	{
		return Color(m_r * rhs, m_g * rhs, m_b * rhs, m_a * rhs);
	}
	/**
	 * @brief Add a color.
	 *
	 * @param rhs
	 * @return
	 */
	Color operator +(const Color& rhs) const
	{
		return Color(m_r + rhs.m_r, m_g + rhs.m_g, m_b + rhs.m_b, m_a + rhs.m_a);
	}
	/**
	 * @brief Substract a color.
	 *
	 * @param rhs
	 * @return
	 */
	Color operator -(const Color& rhs) const
	{
		return Color(m_r - rhs.m_r, m_g - rhs.m_g, m_b - rhs.m_b, m_a - rhs.m_a);
	}

	/**
	 * @brief Add-assign a color.
	 *
	 * @param rhs
	 * @return
	 */
	Color& operator +=(const Color& rhs)
	{
		m_r += rhs.m_r;
		m_g += rhs.m_g;
		m_b += rhs.m_b;
		m_a += rhs.m_a;
		return *this;
	}

	/**
	 * @brief Return float data.
	 *
	 * @return
	 */
	const float* Data() const
	{
		return &m_r;
	}

	/**
	 * @brief Return color packed to a 32-bit integer, with R component in the lowest 8 bits. Components are clamped to [0, 1] range.
	 *
	 */
	unsigned ToUInt() const;
	/**
	 * @brief Return HSL color-space representation as a Vector3; the RGB values are clipped before conversion but not changed in the process.
	 *
	 * @return
	 */
	Vector3 ToHSL() const;
	/**
	 * @brief Return HSV color-space representation as a Vector3; the RGB values are clipped before conversion but not changed in the process.
	 *
	 * @return
	 */
	Vector3 ToHSV() const;
	/**
	 * @brief Set RGBA values from specified HSL values and alpha.
	 *
	 * @param h
	 * @param s
	 * @param l
	 * @param a
	 */
	void FromHSL(float h, float s, float l, float a = 1.0f);
	/**
	 * @brief Set RGBA values from specified HSV values and alpha.
	 *
	 * @param h
	 * @param s
	 * @param v
	 * @param a
	 */
	void FromHSV(float h, float s, float v, float a = 1.0f);

	/**
	 * @brief Return RGB as a three-dimensional vector.
	 *
	 * @return
	 */
	Vector3 ToVector3() const
	{
		return Vector3(m_r, m_g, m_b);
	}
	/**
	 * @brief Return RGBA as a four-dimensional vector.
	 *
	 * @return
	 */
	Vector4 ToVector4() const
	{
		return Vector4(m_r, m_g, m_b, m_a);
	}

	/**
	 * @brief Return sum of RGB components.
	 *
	 * @return
	 */
	float SumRGB() const
	{
		return m_r + m_g + m_b;
	}
	/**
	 * @brief Return average value of the RGB channels.
	 *
	 * @return
	 */
	float Average() const
	{
		return (m_r + m_g + m_b) / 3.0f;
	}

	/**
	 * @brief Return the 'grayscale' representation of RGB values, as used by JPEG and PAL/NTSC among others.
	 *
	 * @return
	 */
	float Luma() const
	{
		return m_r * 0.299f + m_g * 0.587f + m_b * 0.114f;
	}
	/**
	 * @brief Return the colorfulness relative to the brightness of a similarly illuminated white.
	 *
	 * @return
	 */
	float Chroma() const;
	/**
	 * @brief Return hue mapped to range [0, 1.0).
	 *
	 * @return
	 */
	float Hue() const;
	/**
	 * @brief Return saturation as defined for HSL.
	 *
	 * @return
	 */
	float SaturationHSL() const;
	/**
	 * @brief Return saturation as defined for HSV.
	 *
	 * @return
	 */
	float SaturationHSV() const;
	/**
	 * @brief Return value as defined for HSV: largest value of the RGB components. Equivalent to calling MinRGB().
	 *
	 * @return
	 */
	float Value() const
	{
		return MaxRGB();
	}
	/**
	 * @brief Return lightness as defined for HSL: average of the largest and smallest values of the RGB components.
	 *
	 * @return
	 */
	float Lightness() const;

	/**
	 * @brief Stores the values of least and greatest RGB component at specified pointer addresses, optionally clipping those values to [0, 1] range.
	 *
	 * @param min
	 * @param max
	 * @param clipped
	 */
	void Bounds(float* min, float* max, bool clipped = false) const;
	/**
	 * @brief Return the largest value of the RGB components.
	 *
	 * @return
	 */
	float MaxRGB() const;
	/**
	 * @brief Return the smallest value of the RGB components.
	 *
	 * @return
	 */
	float MinRGB() const;
	/**
	 * @brief Return range, defined as the difference between the greatest and least RGB component.
	 *
	 * @return
	 */
	float Range() const;

	bool IsTransparent() const
	{
		return this->m_a < 0.99;
	}

	/**
	 * @brief Clip to [0, 1.0] range.
	 *
	 * @param clipAlpha
	 */
	void Clip(bool clipAlpha = false);
	/**
	 * @brief Inverts the RGB channels and optionally the alpha channel as well.
	 *
	 * @param invertAlpha
	 */
	void Invert(bool invertAlpha = false);
	/**
	 * @brief Return linear interpolation of this color with another color.
	 *
	 * @param rhs
	 * @param t
	 * @return
	 */
	Color Lerp(const Color& rhs, float t) const;
	/**
	 * @brief Return color with absolute components.
	 *
	 * @return
	 */
	Color Abs() const
	{
		return Color(M3D::Abs(m_r), M3D::Abs(m_g), M3D::Abs(m_b), M3D::Abs(m_a));
	}
	/**
	 * @brief Test for equality with another color with epsilon.
	 *
	 * @param rhs
	 * @return
	 */
	bool Equals(const Color& rhs) const
	{
		return M3D::Equals(m_r, rhs.m_r) && M3D::Equals(m_g, rhs.m_g)
				&& M3D::Equals(m_b, rhs.m_b) && M3D::Equals(m_a, rhs.m_a);
	}

	/**
	 * @brief Return as string.
	 *
	 * @return
	 */
	string Tostring() const;

	/**
	 * @brief Red value.
	 *
	 */
	float m_r;
	/**
	 * @brief Green value.
	 *
	 */
	float m_g;
	/**
	 * @brief Blue value.
	 *
	 */
	float m_b;
	/**
	 * @brief Alpha value.
	 *
	 */
	float m_a;

	/**
	 * @brief Opaque white color.
	 *
	 */
	static const Color WHITE;
	/**
	 * @brief Opaque gray color.
	 *
	 */
	static const Color GRAY;
	/**
	 * @brief Opaque black color.
	 *
	 */
	static const Color BLACK;
	/**
	 * @brief Opaque red color.
	 *
	 */
	static const Color RED;
	/**
	 * @brief Opaque green color.
	 *
	 */
	static const Color GREEN;
	/**
	 * @brief Opaque blue color.
	 *
	 */
	static const Color BLUE;
	/**
	 * @brief Opaque cyan color.
	 *
	 */
	static const Color CYAN;
	/**
	 * @brief Opaque magenta color.
	 *
	 */
	static const Color MAGENTA;
	/**
	 * @brief Opaque yellow color.
	 *
	 */
	static const Color YELLOW;
	/**
	 * @brief Transparent color (black with no alpha).
	 *
	 */
	static const Color TRANSPARENT;

	static Color * Default;//!<默认颜色
	static Color * SelectColor;//!< 选中颜色
	static Color* SelectorShelter; //!<选中遮挡颜色

	static Color* FaceDefaultColor;//!<face的默认颜色
	static Color* FaceSelectColor;//!<face的选中颜色
	static Color* EdgeDefaultColor;//!<edge的默认颜色
	static Color* EdgeSelectColor;//!<edge的选中颜色

protected:
	/**
	 * @brief Return hue value given greatest and least RGB component, value-wise.
	 *
	 * @param min
	 * @param max
	 * @return
	 */
	float Hue(float min, float max) const;
	/**
	 * @brief Return saturation (HSV) given greatest and least RGB component, value-wise.
	 *
	 * @param min
	 * @param max
	 * @return
	 */
	float SaturationHSV(float min, float max) const;
	/**
	 * @brief Return saturation (HSL) given greatest and least RGB component, value-wise.
	 *
	 * @param min
	 * @param max
	 * @return
	 */
	float SaturationHSL(float min, float max) const;
	/**
	 * @brief Calculate and set RGB values. Convenience function used by FromHSV and FromHSL to avoid code duplication.
	 *
	 * @param h
	 * @param c
	 * @param m
	 */
	void FromHCM(float h, float c, float m);
};

/** 
 * @brief Multiply Color with a scalar.
 *
 * @param lhs
 * @param rhs
 * @return
 */
inline Color operator *(float lhs, const Color& rhs)
{
	return rhs * lhs;
}

}
#endif //M3D_COLOR_H_

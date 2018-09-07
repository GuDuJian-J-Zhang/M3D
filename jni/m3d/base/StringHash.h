/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef M3D_STRINGHASH_H_
#define M3D_STRINGHASH_H_

#include "m3d/M3DExport.h"

namespace M3D
{

/** 
 * @brief 32-bit hash value for a string. 
 */
class M3D_API StringHash
{
public:
	/**
	 * @brief Construct with zero value.
	 */
	StringHash() :
			m_value(0)
	{
	}

	/**
	 * @brief Copy-construct from another hash.
	 */
	StringHash(const StringHash& rhs) :
			m_value(rhs.m_value)
	{
	}

	/**
	 * @brief Construct with an initial value.
	 */
	explicit StringHash(unsigned value) :
			m_value(value)
	{
	}

	/**
	 * @brief Construct from a C string case-insensitively.
	 */
	StringHash(const char* str);
	/**
	 * @brief Construct from a string case-insensitively.
	 */
	StringHash(const string& str);

	/**
	 * @brief Assign from another hash.
	 */
	StringHash& operator =(const StringHash& rhs)
	{
		m_value = rhs.m_value;
		return *this;
	}

	/**
	 * @brief Add a hash.
	 */
	StringHash operator +(const StringHash& rhs) const
	{
		StringHash ret;
		ret.m_value = m_value + rhs.m_value;
		return ret;
	}

	/**
	 * @brief Add-assign a hash.
	 */
	StringHash& operator +=(const StringHash& rhs)
	{
		m_value += rhs.m_value;
		return *this;
	}

	// Test for equality with another hash.
	bool operator ==(const StringHash& rhs) const
	{
		return m_value == rhs.m_value;
	}
	/**
	 * @brief Test for inequality with another hash.
	 */
	bool operator !=(const StringHash& rhs) const
	{
		return m_value != rhs.m_value;
	}
	/**
	 * @brief Test if less than another hash.
	 */
	bool operator <(const StringHash& rhs) const
	{
		return m_value < rhs.m_value;
	}
	/**
	 * @brief Test if greater than another hash.
	 */
	bool operator >(const StringHash& rhs) const
	{
		return m_value > rhs.m_value;
	}
	/**
	 * @brief Return true if nonzero hash value.
	 */
	operator bool() const
	{
		return m_value != 0;
	}
	/**
	 * @brief Return hash value.
	 */
	unsigned Value() const
	{
		return m_value;
	}
	/**
	 * @brief Return as string.
	 */
	string Tostring() const;
	/**
	 * @brief Return hash value for HashSet & HashMap.
	 */
	unsigned ToHash() const
	{
		return m_value;
	}

	/**
	 * @brief Calculate hash value case-insensitively from a C string.
	 */
	static unsigned Calculate(const char* str);

	/**
	 * @brief Zero hash.
	 */
	static const StringHash ZERO;

private:
	/**
	 * @brief Hash value.
	 */
	unsigned m_value;
};

}
#endif //STRINGHASH_H

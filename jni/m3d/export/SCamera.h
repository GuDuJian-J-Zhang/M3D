#pragma once
#ifndef M3D_SCAMERA_H_
#define M3D_SCAMERA_H_

#include "../base/AddressMacro.h"
class SCamera
{
public:
	SCamera();
	virtual ~SCamera();

	M3D_ADDRESSTYPE Address()
	{
		return m_address;
	}

	void SetAddress(M3D_ADDRESSTYPE memoryAddress)
	{
		m_address = memoryAddress;
	}
private:
	M3D_ADDRESSTYPE m_address;
};
#endif

#ifndef M3D_SLIGHT_H_
#define M3D_SLIGHT_H_
#include "../base/AddressMacro.h"
class SLight
{
public:
	SLight();
	virtual ~SLight();

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

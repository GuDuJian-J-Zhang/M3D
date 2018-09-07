#ifndef M3D_SMATERIAL_H_
#define M3D_SMATERIAL_H_

#include "../base/AddressMacro.h"
class SMaterial
{
public:
	SMaterial();
	virtual ~SMaterial();

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


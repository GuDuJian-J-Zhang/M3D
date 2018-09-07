#include "m3d/graphics/GPUObject.h"
#include "m3d/ResourceManager.h"

namespace M3D
{

char* GPUObject::priAllocMemory = NULL;
unsigned GPUObject::priAllocMemorySize = 0;

const int GPUObject::DISK_CACHE = 2;
const int GPUObject::GPU_CACHE = 1;
const int GPUObject::NO_CACHE = 0;

char* GPUObject::GetPriAllocMemory(unsigned size)
{
	if(size>priAllocMemorySize)
	{
		priAllocMemorySize = size;
		delete[] priAllocMemory;
		priAllocMemory = new char[size];
	}

	return GPUObject::priAllocMemory;
}

void GPUObject::FreePriAllocMemory()
{
	if(priAllocMemory)
	{
		priAllocMemorySize = 0;
		delete[] priAllocMemory;
		priAllocMemory = NULL;
	}
}

GPUObject::GPUObject() :
		Resource()
{
	Init();
}

GPUObject::~GPUObject()
{
	m_object =0;
}


void GPUObject::Init()
{
	m_object = 0;
}

GLuint GPUObject::GetObject()
{
	return this->m_object;
}

}


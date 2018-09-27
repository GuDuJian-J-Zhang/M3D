#include "CompressHelper.h"
#include "Extract.h"
#include "Alloc.h"

CCompressHelper::CCompressHelper()
{
}


CCompressHelper::~CCompressHelper()
{
}


bool CCompressHelper::CombineBuffer(void **bufP, std::vector<int>& vecBufNum, void** comBufferAddress,unsigned int* comBufferSize)
{
	size_t combineBufferSize = 0;
	int numBuf = vecBufNum.size();
	for (int i = 0; i < numBuf; i++)
	{
		combineBufferSize += vecBufNum.at(i);
	}

	*comBufferAddress = (char*)MidAlloc(combineBufferSize);

	size_t offset = 0;
	for (int i = 0; i < numBuf; i++)
	{
		memcpy(comBufferAddress+offset,bufP[i], vecBufNum.at(i)); 
		offset+= vecBufNum.at(i);
	}

	return true;
}

void CCompressHelper::FreeBuffer(void* bufP)
{
	if (bufP!= NULL)
	{
		MidFree(bufP);
	}
}

void CCompressHelper::FreeBuffer(void **bufP, std::vector<int>& vecBufNum)
{
	int numBuf = vecBufNum.size(); 
	for (int i = 0; i < numBuf; i++)
	{
		FreeBuffer(bufP[i]);
		bufP[i] = NULL;
	}
	FreeBuffer(bufP);
	bufP = NULL;
}

bool CCompressHelper::CombineBufferWithFree(void **bufP, std::vector<int>& vecBufNum, void** comBufferAddress, unsigned int* comBufferSize)
{
	if (CombineBuffer(bufP, vecBufNum, comBufferAddress, comBufferSize))
	{
		return false;
	}
	{
		size_t combineBufferSize = 0;
		int numBuf = vecBufNum.size();
		for (int i = 0; i < numBuf; i++)
		{
			combineBufferSize += vecBufNum.at(i);
		}

		*comBufferAddress = (char*)MidAlloc(combineBufferSize);

		size_t offset = 0;
		for (int i = 0; i < numBuf; i++)
		{
			memcpy(comBufferAddress + offset, bufP[i], vecBufNum.at(i));
			offset += vecBufNum.at(i);
		}

		return true;
	}
}

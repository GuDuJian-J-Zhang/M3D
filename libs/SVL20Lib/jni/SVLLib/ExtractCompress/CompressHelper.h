#ifdef __MOBILE__
#include "../AutoTchar.h"
#include "ExtractTypeDef.h"
#else
#include <atlstr.h>
#endif
#include "Stk_Object.h"
#include "7zTypes.h"

#pragma once
class CCompressHelper
{
public:
	CCompressHelper();
	~CCompressHelper();
public:
	static bool CombineBuffer(void **bufP, std::vector<int>& vecBufNum,void** comBufferAddress, unsigned int* comBufferSize);
	static void FreeBuffer(void* bufP);
	static void FreeBuffer(void **bufP,std::vector<int>& vecBufNum);

	static bool CombineBufferWithFree(void **bufP, std::vector<int>& vecBufNum, void** comBufferAddress, unsigned int* comBufferSize);
};


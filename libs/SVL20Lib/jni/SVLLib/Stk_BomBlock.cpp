// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_ComponentFeature.cpp
 *
 *		2016/11/22	÷ÿππ°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_BomBlock.h"

namespace HoteamSoft {
	namespace SVLLib {
		Stk_BomBlock::Stk_BomBlock(void)
        {
            mBlockMemory = NULL;
			mBlockByteSize = 0;
			mOffset = 0;
		}
		STK_BOOL Stk_BomBlock::AllocMamory(STK_UINT32 blockByteSize) {
			this->mBlockByteSize = blockByteSize;

			mBlockMemory = (STK_CHAR*)malloc(this->mBlockByteSize);
			if (mBlockMemory)
			{
				return false;
			}

			return true;
		}


		STK_STATUS Stk_BomBlock::WrieteFile(Stk_CommonAccessorPtr& commonAccessor, Stk_FilePtr& binFile)
		{
			this->memoryBufferOffset = 0;
			STK_UINT32 bomBlockSize = 0;
			//º∆À„À˘”– ˝æ›◊‹¥Û–°
			int numInstance = m_vecInstance.size();
			bomBlockSize += numInstance*sizeof(StkBomBlock);
			this->mBlockByteSize = bomBlockSize;
			this->AllocMamory(this->mBlockByteSize);
 
			for (int i = 0; i < numInstance; i++)
			{
				this->WriteToBuffer(m_vecInstance[i]);
			}
			//
			commonAccessor->WriteFile(this->mBlockMemory, this->mBlockByteSize, binFile);

			return STK_SUCCESS;
		}

		STK_STATUS Stk_BomBlock::ReadFile(Stk_FilePtr& bomFile)
		{
			this->mBlockByteSize = bomFile->FileSize();
			//Ω´ ˝æ›“ª¥Œ–‘∂¡»°≥ˆ¿¥
			this->AllocMamory(this->mBlockByteSize);
			bomFile->ReadBuffer(this->mBlockMemory,mBlockByteSize, 0);
			this->memoryBufferOffset = 0;
			//≈–∂œ ˝æ› «∑Ò∂¡»°ÕÍ≥…
			while (this->memoryBufferOffset<mBlockByteSize)
			{
				//ªÒ»°Model Block
				StkBomBlock bomBlock;
				this->ReadFromBuffer(bomBlock);
				this->m_vecInstance.push_back(bomBlock);
			}
			return STK_SUCCESS;
		}

		void Stk_BomBlock::AddInstance(const StkBomBlock i_stkBomBlock)
		{
			this->m_vecInstance.push_back(i_stkBomBlock);
		}

		Stk_BomBlock::~Stk_BomBlock()
		{
			if (this->mBlockMemory != NULL)
			{
				free(mBlockMemory);
				this->mBlockMemory = NULL;
			}
			m_vecInstance.clear();
		}

		void Stk_BomBlock::WriteToBuffer(StkBomBlock& bomBlock)
		{
			int writebufferLength = sizeof(StkBomBlock);
			memcpy(mBlockMemory + memoryBufferOffset, &bomBlock, writebufferLength);
			memoryBufferOffset += writebufferLength;
		}

		void Stk_BomBlock::ReadFromBuffer(StkBomBlock& bomBlock)
		{
			int writebufferLength = sizeof(StkBomBlock);
			memcpy(&bomBlock, mBlockMemory + memoryBufferOffset, writebufferLength);
			memoryBufferOffset += writebufferLength;
		}


	}//Stk_BomBlock
}//hoteamsoft

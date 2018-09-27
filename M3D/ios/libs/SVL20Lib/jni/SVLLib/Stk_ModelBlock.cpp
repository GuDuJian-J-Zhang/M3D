// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_ComponentFeature.cpp
 *
 *		2016/11/22	÷ÿππ°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_ModelBlock.h"

namespace HoteamSoft {
	namespace SVLLib {
		Stk_ModelBlock::Stk_ModelBlock(void)
        {
            mBlockMemory = NULL;
			mBlockByteSize = 0;
			mOffset = 0;
		}
		STK_BOOL Stk_ModelBlock::AllocMamory(STK_UINT32 blockByteSize) {
			this->mBlockByteSize = blockByteSize;

			mBlockMemory = (STK_CHAR*)malloc(this->mBlockByteSize);
			if (mBlockMemory)
			{
				return false;
			}

			return true;
		}

		STK_STATUS Stk_ModelBlock::WrieteFile(Stk_CommonAccessorPtr& commonAccessor, Stk_FilePtr& binFile)
		{
			this->memoryBufferOffset = 0;
			STK_UINT32 bomBlockSize = 0;
			//º∆À„À˘”– ˝æ›◊‹¥Û–°
			int numModel = m_vecModel.size();
			bomBlockSize += numModel*sizeof(StkModelBlock);
			int numAccessor = m_vecAccessor.size();
			bomBlockSize += numAccessor*sizeof(StkAccessorBlock);
			this->mBlockByteSize = bomBlockSize;
			this->AllocMamory(this->mBlockByteSize);
			int accessorOriginIndex = 0;
			for (int i = 0; i < numModel; i++)
			{
				this->WriteToBuffer(m_vecModel[i]);
				int numLod = m_vecModel[i].numLod;
				for (int j =0; j < numLod; j++)
				{
					this->WriteToBuffer(m_vecAccessor[accessorOriginIndex+j]);
				}
				accessorOriginIndex += numLod;
			}
			//
			commonAccessor->WriteFile(this->mBlockMemory, this->mBlockByteSize, binFile);

			return STK_SUCCESS;
		}

		STK_STATUS Stk_ModelBlock::ReadFile(Stk_FilePtr& binFile)
		{
			this->mBlockByteSize = binFile->FileSize();
			//Ω´ ˝æ›“ª¥Œ–‘∂¡»°≥ˆ¿¥
			this->AllocMamory(this->mBlockByteSize);
			binFile->ReadBuffer(this->mBlockMemory,mBlockByteSize, 0);
			this->memoryBufferOffset = 0;
			//≈–∂œ ˝æ› «∑Ò∂¡»°ÕÍ≥…
			while (this->memoryBufferOffset<mBlockByteSize)
			{
				//ªÒ»°Model Block
				StkModelBlock modelBlock;
				this->ReadFromBuffer(modelBlock);
				this->m_vecModel.push_back(modelBlock);
				int numLod = modelBlock.numLod;
				for (int i = 0; i < numLod; i++)
				{
					//ªÒ»°AccessorøÈ
					StkAccessorBlock accessorBlock;
					this->ReadFromBuffer(accessorBlock);
					this->m_vecAccessor.push_back(accessorBlock);
				}
			}
			return STK_SUCCESS;
		}

		void Stk_ModelBlock::AddModel(const StkModelBlock i_stkModelBlock)
		{
			this->m_vecModel.push_back(i_stkModelBlock);
		}

		void Stk_ModelBlock::AddAccessor(const StkAccessorBlock i_stkAccessorBlock)
		{
			this->m_vecAccessor.push_back(i_stkAccessorBlock);
		}

		Stk_ModelBlock::~Stk_ModelBlock()
		{
			if (this->mBlockMemory != NULL)
			{
				free(mBlockMemory);
				this->mBlockMemory = NULL;
			}
			m_vecAccessor.clear();
			m_vecModel.clear();
		}

		void Stk_ModelBlock::WriteToBuffer(StkModelBlock& modelBlock)
		{
			int writebufferLength = sizeof(StkModelBlock);
			memcpy(mBlockMemory + memoryBufferOffset, &modelBlock, writebufferLength);
			memoryBufferOffset += writebufferLength;
		}

		void Stk_ModelBlock::WriteToBuffer(StkAccessorBlock& accessorBlock)
		{
			int writebufferLength = sizeof(StkAccessorBlock);
			memcpy(mBlockMemory + memoryBufferOffset, &accessorBlock, writebufferLength);
			memoryBufferOffset += writebufferLength;
		}

		void Stk_ModelBlock::ReadFromBuffer(StkModelBlock& modelBlock)
		{
			int writebufferLength = sizeof(StkModelBlock);
			memcpy(&modelBlock, mBlockMemory + memoryBufferOffset, writebufferLength);
			memoryBufferOffset += writebufferLength;
		}


		void Stk_ModelBlock::ReadFromBuffer(StkAccessorBlock& accessorBlock)
		{
			int writebufferLength = sizeof(StkAccessorBlock);
			memcpy(&accessorBlock, mBlockMemory + memoryBufferOffset, writebufferLength);
			memoryBufferOffset += writebufferLength;
		}

	}//Stk_BomBlock
}//hoteamsoft

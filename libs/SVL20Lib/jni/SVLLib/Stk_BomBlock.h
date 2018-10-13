// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_DocumentManager
	*
	*
**************************************************************************/
#ifndef _SVLLIB_STK_BOMBLOCK_H_
#define _SVLLIB_STK_BOMBLOCK_H_
#include "Stk_Object.h"
#include "Stk_CommonAccessor.h"

namespace HoteamSoft {
// «∑Ò‘ –Ìª∫¥ÊµΩƒ⁄¥Ê±Í ∂
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_BomBlock
        {
        public:
            Stk_BomBlock();
			STK_BOOL AllocMamory(STK_UINT32 blockByteSize);
			~Stk_BomBlock();
			void AddInstance(const StkBomBlock i_stkBomBlock);
			STK_UINT32 getInstanceCount(){return m_vecInstance.size();}
			StkBomBlock getInstanceByIdx(STK_UINT32 i_idx){return m_vecInstance[i_idx];}

			STK_STATUS WrieteFile(Stk_CommonAccessorPtr& commonAccessor, Stk_FilePtr& binFile);
			STK_STATUS	 ReadFile(Stk_FilePtr& bomFile);

			vector<StkBomBlock> m_vecInstance;

			STK_UINT32 mBlockByteSize;
			STK_CHAR* mBlockMemory;
			STK_UINT32 mOffset;

		private:

			void WriteToBuffer(StkBomBlock& bomBlock);
			void ReadFromBuffer(StkBomBlock& bomBlock);

			int memoryBufferOffset;
		};
	}//Stk_MeshLodBlock
}//hoteamsoft

#endif

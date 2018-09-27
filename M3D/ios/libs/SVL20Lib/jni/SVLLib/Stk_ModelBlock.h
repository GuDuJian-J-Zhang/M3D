// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_DocumentManager
	*
	*
**************************************************************************/
#ifndef _SVLLIB_STK_MODELBLOCK_H_
#define _SVLLIB_STK_MODELBLOCK_H_
#include "Stk_Object.h"
#include "Stk_CommonAccessor.h"

namespace HoteamSoft {
// «∑Ò‘ –Ìª∫¥ÊµΩƒ⁄¥Ê±Í ∂
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_ModelBlock
        {
        public:
            Stk_ModelBlock();
			STK_BOOL AllocMamory(STK_UINT32 blockByteSize);
			~Stk_ModelBlock();
			void AddModel(const StkModelBlock i_stkBomBlock);
			void AddAccessor(const StkAccessorBlock i_stkAccessorBlock);

			STK_STATUS WrieteFile(Stk_CommonAccessorPtr& commonAccessor, Stk_FilePtr& binFile);
			STK_STATUS ReadFile(Stk_FilePtr& binFile);

			vector<StkModelBlock> m_vecModel;
			vector<StkAccessorBlock> m_vecAccessor;

			STK_UINT32 getModelBlockCount(){return m_vecModel.size();}
			StkModelBlock getModelBlockByIdx(STK_UINT32 i_index){return m_vecModel[i_index];}

			STK_UINT32 getAccessorBlockCount(){return m_vecAccessor.size();}
			StkAccessorBlock getAccessorBlockByIdx(STK_UINT32 i_index){return m_vecAccessor[i_index];}

			STK_UINT32 mBlockByteSize;
			STK_CHAR* mBlockMemory;
			STK_UINT32 mOffset;

		private:

			void WriteToBuffer(StkModelBlock& modelBlock);
			void ReadFromBuffer(StkModelBlock& meshBlock);

			void WriteToBuffer(StkAccessorBlock& accessorBlock);
			void ReadFromBuffer(StkAccessorBlock& accessorBlock);
			int memoryBufferOffset;
		};
	}//Stk_MeshLodBlock
}//hoteamsoft

#endif

// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_DocumentManager
	*
	*
**************************************************************************/
#ifndef _SVLLIB_STK_MESHLODBLOCK_H_
#define _SVLLIB_STK_MESHLODBLOCK_H_
#include "Stk_Object.h"
#include "Stk_CommonAccessor.h"

namespace HoteamSoft {
// «∑Ò‘ –Ìª∫¥ÊµΩƒ⁄¥Ê±Í ∂
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_MeshLodBlock
		{
        public:
            Stk_MeshLodBlock();
			STK_BOOL AllocMamory(STK_UINT32 blockByteSize);
			~Stk_MeshLodBlock();

			void AddMesh(Stk_MeshPrimPtr& meshPriPtr);
			STK_STATUS GetMeshes(vector<Stk_MeshPrimPtr>& i_vecMesh );

			STK_STATUS ReadFile(Stk_CommonAccessorPtr& commonAccessor, Stk_FilePtr& binFile);
			STK_STATUS WrieteFile(Stk_CommonAccessorPtr& commonAccessor, Stk_FilePtr& binFile);

			vector<Stk_MeshPrimPtr> mMeshes;

			STK_UINT32 mBlockByteSize;
			STK_CHAR* mBlockMemory;
			STK_UINT32 mOffset;

		private:
			void WriteToBuffer(vector<STK_UINT32>& srcBuffer);
			void ReadFromBuffer(vector<STK_UINT32>& srcBuffer);

			void WriteToBuffer(vector<int>& srcBuffer);
			void ReadFromBuffer(vector<int>& srcBuffer);

			void WriteToBuffer(vector<STK_FLOAT32>& srcBuffer);
			void ReadFromBuffer(vector<STK_FLOAT32>& srcBuffer);

			void WriteToBuffer(StkMeshBlock& meshBlock);
			void ReadFromBuffer(StkMeshBlock& meshBlock);

			void WriteToBuffer(StkMeshFaceBlock& meshFaceBlock);
			void ReadFromBuffer(StkMeshFaceBlock& meshFaceBlock);

			void WriteToBuffer(StkMeshEdgeBlock& meshEdgeBlock);
			void ReadFromBuffer(StkMeshEdgeBlock& meshEdgeBlock);


			int memoryBufferOffset;
		};
	}//Stk_MeshLodBlock
}//hoteamsoft

#endif

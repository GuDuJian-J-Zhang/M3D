// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_ComponentFeature.cpp
 *
 *		2016/11/22	÷ÿππ°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_MeshLodBlock.h"

namespace HoteamSoft {
	namespace SVLLib {
		Stk_MeshLodBlock::Stk_MeshLodBlock(void)
        {
            mBlockMemory = NULL;
			mBlockByteSize = 0;
			mOffset = 0;
		}
		STK_BOOL Stk_MeshLodBlock::AllocMamory(STK_UINT32 blockByteSize) {
			this->mBlockByteSize = blockByteSize;

			mBlockMemory = (STK_CHAR*)malloc(this->mBlockByteSize);
			if (mBlockMemory)
			{
				return false;
			}

			return true;
		}

		STK_STATUS Stk_MeshLodBlock::ReadFile(Stk_CommonAccessorPtr& commonAccessor, Stk_FilePtr& binFile)
		{
			mMeshes.clear();
			//ªÒ»°blockµƒ¥Û–°
			this->mBlockByteSize = commonAccessor->GetByteLength();
			STK_UINT32 meshBlockOffset = commonAccessor->GetByteOffset();

			//Ω´ ˝æ›“ª¥Œ–‘∂¡»°≥ˆ¿¥
			this->AllocMamory(this->mBlockByteSize);
			commonAccessor->ReadFile(this->mBlockMemory,mBlockByteSize,meshBlockOffset,binFile);

			this->memoryBufferOffset = 0;
			//≈–∂œ ˝æ› «∑Ò∂¡»°ÕÍ≥…
			while (this->memoryBufferOffset<mBlockByteSize)
			{
				Stk_MeshPrimPtr meshPrimPtr = Stk_MeshPrimPtr::CreateObject();

				//ªÒ»°MeshBlock
				StkMeshBlock meshBlock;
				this->ReadFromBuffer(meshBlock);
				meshPrimPtr->ReadFromBlock(meshBlock);
				meshPrimPtr->SetID(meshBlock.meshID);
				vector<STK_FLOAT32> vertexsArray(meshBlock.numVertex);
				vector<STK_FLOAT32> normalsArray(meshBlock.numNormal);
				vector<STK_FLOAT32> textuercoordinatesArray(meshBlock.numUV);

				this->ReadFromBuffer(vertexsArray);
				this->ReadFromBuffer(normalsArray);
				this->ReadFromBuffer(textuercoordinatesArray);
				//±£¥ÊMeshµƒ∂•µ„∫Õ∑®œﬂ–≈œ¢
				meshPrimPtr->SetVertexs(vertexsArray);
				meshPrimPtr->SetNormals(normalsArray);
				meshPrimPtr->SetTextuercoordinate(textuercoordinatesArray);

				STK_UINT32 faceNum = meshBlock.numFace;

				for (int j = 0; j < faceNum; j++)
				{
					//Stk_MeshFacePtr meshFacePtr = Stk_MeshFacePtr::CreateObject();
					StkMeshFaceBlock meshFaceBlock;
					this->ReadFromBuffer(meshFaceBlock);
					vector<STK_UINT32> faceIndexArray(meshFaceBlock.numIndex);
					this->ReadFromBuffer(faceIndexArray);
					vector<int> edgeIDArray(meshFaceBlock.numEdge);
					this->ReadFromBuffer(edgeIDArray);
					//¥¥Ω®√Êµƒ∂‘œÛ
					Stk_MeshFacePtr meshFacePtr = meshPrimPtr->AddMeshFace(meshFaceBlock.meshFaceID);
					meshFacePtr->ReadFromBlock(meshFaceBlock);
					meshFacePtr->SetIndex(faceIndexArray);
					meshFacePtr->SetMaterialID(meshFaceBlock.materialID);
					for (int indexEdgeID = 0; indexEdgeID < meshFaceBlock.numEdge; indexEdgeID++)
					{
						meshFacePtr->AddMeshEdgeID(edgeIDArray[indexEdgeID]);
					}
					//meshPrimPtr->AddMeshFace(meshFacePtr);
				}

				STK_UINT32 edgeNum = meshBlock.numEdge;

				for (int edgeIndex = 0; edgeIndex < edgeNum; edgeIndex++)
				{
					//Stk_MeshEdgePtr meshEdgePtr = Stk_MeshEdgePtr::CreateObject();
					//Ω‚Œˆ±ﬂµƒ–≈œ¢
					StkMeshEdgeBlock meshEdgeBlock;
					this->ReadFromBuffer(meshEdgeBlock);
					vector<STK_UINT32> edgeIndexArray(meshEdgeBlock.numIndex);
					this->ReadFromBuffer(edgeIndexArray);
					//¥¥Ω®±ﬂµƒ∂‘œÛ
					Stk_MeshEdgePtr meshEdgePtr = meshPrimPtr->AddMeshEdge(meshEdgeBlock.meshEdgeID);
					meshEdgePtr->ReadFromBlock(meshEdgeBlock);
					meshEdgePtr->SetIndex(edgeIndexArray);
				}
				mMeshes.push_back(meshPrimPtr);
			}
			return STK_SUCCESS;
		}

		STK_STATUS Stk_MeshLodBlock::WrieteFile(Stk_CommonAccessorPtr& commonAccessor, Stk_FilePtr& binFile)
		{
			this->memoryBufferOffset = 0;
			STK_UINT32 meshBlockSize = 0;
			//º∆À„À˘”– ˝æ›◊‹¥Û–°
			for (int i = 0;i<mMeshes.size();i++)
			{
				Stk_MeshPrimPtr meshPrimPtr = mMeshes.at(i);

				//ªÒ»°MeshBlock
				StkMeshBlock meshBlock;
				//meshPrimPtr->FillBlock(&meshBlock);
				meshBlockSize += sizeof(StkMeshBlock);

				vector<STK_FLOAT32>& vertexsArray = meshPrimPtr->GetVertexs();
				vector<STK_FLOAT32>& normalsArray = meshPrimPtr->GetNormals();
				vector<STK_FLOAT32>& textuercoordinatesArray = meshPrimPtr->GetTextuercoordinate();
 
				meshBlockSize += vertexsArray.size()*sizeof(STK_FLOAT32);
				meshBlockSize += normalsArray.size() * sizeof(STK_FLOAT32);
				meshBlockSize += textuercoordinatesArray.size() * sizeof(STK_FLOAT32);

				//STK_UINT32 faceNum = meshPrimPtr->GetNbMeshFace();
				map<STK_INT32, Stk_MeshFacePtr>mapMeshFace; 
				map<STK_INT32, Stk_MeshFacePtr>::iterator meshFaceIter;
				mapMeshFace = meshPrimPtr->GetMeshFaces();
				//for (int j = 0;j<faceNum;j++)
				for(meshFaceIter = mapMeshFace.begin(); meshFaceIter != mapMeshFace.end(); meshFaceIter++)
				{
					//Stk_MeshFacePtr meshFacePtr =meshPrimPtr->GetMeshFaceByIdx(j);
					Stk_MeshFacePtr meshFacePtr = meshFaceIter->second;
					StkMeshFaceBlock meshFaceBlock;
					meshBlockSize += sizeof(StkMeshFaceBlock);
					//meshFacePtr->FillBlock(&meshFaceBlock);
					//º∆À„√ÊÀ˜“˝µƒ¥Û–°
					vector<STK_UINT32> faceIndexArray;
					meshFacePtr->GetIndex(faceIndexArray); 
					meshBlockSize += faceIndexArray.size() * sizeof(STK_UINT32);
					//º∆À„√Ê÷–±ﬂµƒ∏ˆ ˝µƒ¥Û–°
					vector<int> meshEdgeIDArray;
					meshFacePtr->GetEdges(meshEdgeIDArray);
					meshBlockSize += meshEdgeIDArray.size()*sizeof(int);
				}

				//STK_UINT32 edgeNum = meshPrimPtr->GetNbMeshEdge();
				map<STK_INT32, Stk_MeshEdgePtr>mapMeshEdge; 
				map<STK_INT32, Stk_MeshEdgePtr>::iterator meshEdgeIter;
				mapMeshEdge = meshPrimPtr->GetMeshEdges();
				//for (int edgeIndex = 0; edgeIndex < edgeNum; edgeIndex++)
				for(meshEdgeIter = mapMeshEdge.begin(); meshEdgeIter != mapMeshEdge.end(); meshEdgeIter++)
				{
					//Stk_MeshEdgePtr meshEdgePtr = meshPrimPtr->GetMeshEdgeByIdx(edgeIndex);
					Stk_MeshEdgePtr meshEdgePtr = meshEdgeIter->second;

					StkMeshEdgeBlock meshEdgeBlock;
					meshBlockSize += sizeof(StkMeshEdgeBlock);
					//meshEdgePtr->FillBlock(&meshEdgeBlock);

					vector<STK_UINT32> edgeIndexArray;
					meshEdgePtr->GetIndex(edgeIndexArray);
					meshBlockSize += edgeIndexArray.size() * sizeof(STK_UINT32);
				}
			}
			this->mBlockByteSize = meshBlockSize;
			this->AllocMamory(this->mBlockByteSize);
 
			for (int i = 0; i < mMeshes.size(); i++)
			{
				Stk_MeshPrimPtr meshPrimPtr = mMeshes.at(i);

				//ªÒ»°MeshBlock
				StkMeshBlock meshBlock;
				meshPrimPtr->WriteToBlock(meshBlock);

				this->WriteToBuffer(meshBlock);

				vector<STK_FLOAT32>& vertexsArray = meshPrimPtr->GetVertexs();
				vector<STK_FLOAT32>& normalsArray = meshPrimPtr->GetNormals();
				vector<STK_FLOAT32>& textuercoordinatesArray = meshPrimPtr->GetTextuercoordinate();

				this->WriteToBuffer(vertexsArray); 
				this->WriteToBuffer(normalsArray);
				this->WriteToBuffer(textuercoordinatesArray);

				STK_UINT32 faceNum = meshPrimPtr->GetNbMeshFace();

				for (int j = 0; j < faceNum; j++)
				{
					Stk_MeshFacePtr meshFacePtr = meshPrimPtr->GetMeshFaceByIdx(j);

					StkMeshFaceBlock meshFaceBlock;
					meshFacePtr->WriteToBlock(meshFaceBlock);
					this->WriteToBuffer(meshFaceBlock);

					vector<STK_UINT32> faceIndexArray;
					meshFacePtr->GetIndex(faceIndexArray);
					this->WriteToBuffer(faceIndexArray);
					//±£¥Ê√ø∏ˆ√Ê÷–±ﬂµƒID–≈œ¢
					vector<int> vecMeshEdgeID;
					meshFacePtr->GetEdges(vecMeshEdgeID);
					this->WriteToBuffer(vecMeshEdgeID);
				}

				STK_UINT32 edgeNum = meshPrimPtr->GetNbMeshEdge();

				for (int edgeIndex = 0; edgeIndex < edgeNum; edgeIndex++)
				{
					Stk_MeshEdgePtr meshEdgePtr = meshPrimPtr->GetMeshEdgeByIdx(edgeIndex);

					StkMeshEdgeBlock meshEdgeBlock;
					meshEdgePtr->WriteToBlock(meshEdgeBlock);
					this->WriteToBuffer(meshEdgeBlock);

					vector<STK_UINT32> edgeIndexArray;
					meshEdgePtr->GetIndex(edgeIndexArray);

					this->WriteToBuffer(edgeIndexArray);
				}
			}
			//
			commonAccessor->WriteFile(this->mBlockMemory, this->mBlockByteSize, binFile);

			return STK_SUCCESS;
		}

		void Stk_MeshLodBlock::AddMesh(Stk_MeshPrimPtr& meshPriPtr)
		{
			this->mMeshes.push_back(meshPriPtr);
		}

		STK_STATUS Stk_MeshLodBlock::GetMeshes(vector<Stk_MeshPrimPtr>& i_vecMesh)
		{
			int numMesh = mMeshes.size();
			for (int i = 0; i < numMesh; i++)
			{
				i_vecMesh.push_back(mMeshes[i]);
			}
			//mMeshes.swap(i_vecMesh);
			return STK_SUCCESS;
		}

		Stk_MeshLodBlock::~Stk_MeshLodBlock()
		{
			if (this->mBlockMemory != NULL)
			{
				free(mBlockMemory);
				this->mBlockMemory = NULL;
			}
		}

		void Stk_MeshLodBlock::WriteToBuffer(vector<STK_UINT32>& srcBuffer)
		{
			int writebufferLength = srcBuffer.size() * sizeof(STK_UINT32);
			memcpy(mBlockMemory + memoryBufferOffset, srcBuffer.data(), writebufferLength);
			memoryBufferOffset += writebufferLength;
		}

		void Stk_MeshLodBlock::WriteToBuffer(vector<int>& srcBuffer)
		{
			int writebufferLength = srcBuffer.size() * sizeof(int);
			memcpy(mBlockMemory + memoryBufferOffset, srcBuffer.data(), writebufferLength);
			memoryBufferOffset += writebufferLength;
		}

		void Stk_MeshLodBlock::ReadFromBuffer(vector<STK_UINT32>& srcBuffer)
		{
			int writebufferLength = srcBuffer.size() * sizeof(STK_UINT32);
			memcpy(srcBuffer.data(),mBlockMemory + memoryBufferOffset, writebufferLength);
			memoryBufferOffset += writebufferLength;
		}

		void Stk_MeshLodBlock::ReadFromBuffer(vector<int>& srcBuffer)
		{
			int writebufferLength = srcBuffer.size() * sizeof(int);
			memcpy(srcBuffer.data(),mBlockMemory + memoryBufferOffset, writebufferLength);
			memoryBufferOffset += writebufferLength;
		}

		void Stk_MeshLodBlock::WriteToBuffer(vector<STK_FLOAT32>& srcBuffer)
		{
			int writebufferLength = srcBuffer.size() * sizeof(STK_FLOAT32);
			memcpy(mBlockMemory + memoryBufferOffset, srcBuffer.data(), writebufferLength);
			memoryBufferOffset += writebufferLength;
		}

		void Stk_MeshLodBlock::ReadFromBuffer(vector<STK_FLOAT32>& srcBuffer)
		{
			int writebufferLength = srcBuffer.size() * sizeof(STK_FLOAT32);
			memcpy(srcBuffer.data(), mBlockMemory + memoryBufferOffset, writebufferLength);
			memoryBufferOffset += writebufferLength;
		}

		void Stk_MeshLodBlock::WriteToBuffer(StkMeshBlock& meshBlock)
		{
			int writebufferLength = sizeof(StkMeshBlock);
			memcpy(mBlockMemory + memoryBufferOffset, &meshBlock, writebufferLength);
			memoryBufferOffset += writebufferLength;
		}

		void Stk_MeshLodBlock::ReadFromBuffer(StkMeshBlock& meshBlock)
		{
			int writebufferLength = sizeof(StkMeshBlock);
			memcpy(&meshBlock, mBlockMemory + memoryBufferOffset, writebufferLength);
			memoryBufferOffset += writebufferLength;
		}

		void Stk_MeshLodBlock::WriteToBuffer(StkMeshFaceBlock& meshFaceBlock)
		{
			int writebufferLength = sizeof(StkMeshFaceBlock);
			memcpy(mBlockMemory + memoryBufferOffset, &meshFaceBlock, writebufferLength);
			memoryBufferOffset += writebufferLength;
		}

		void Stk_MeshLodBlock::ReadFromBuffer(StkMeshFaceBlock& meshFaceBlock)
		{
			int writebufferLength =  sizeof(StkMeshFaceBlock);
			memcpy(&meshFaceBlock, mBlockMemory + memoryBufferOffset, writebufferLength);
			memoryBufferOffset += writebufferLength;
		}

		void Stk_MeshLodBlock::WriteToBuffer(StkMeshEdgeBlock& meshEdgeBlock)
		{
			int writebufferLength = sizeof(StkMeshEdgeBlock);
			memcpy(mBlockMemory + memoryBufferOffset, &meshEdgeBlock, writebufferLength);
			memoryBufferOffset += writebufferLength;
		}

		void Stk_MeshLodBlock::ReadFromBuffer(StkMeshEdgeBlock& meshEdgeBlock)
		{
			int writebufferLength =sizeof(StkMeshEdgeBlock);
			memcpy(&meshEdgeBlock, mBlockMemory + memoryBufferOffset, writebufferLength);
			memoryBufferOffset += writebufferLength;
		}

	}//Stk_MeshLodBlock
}//hoteamsoft

// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_ComponentFeature.cpp
 *
 *		2016/11/22	÷ÿππ°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_UnitTest.h"
#include "Stk_ResourceManager.h"
#include "Stk_MeshPrim.h"
#include "Stk_MeshAccessor.h"

namespace HoteamSoft {
	namespace SVLLib {

Stk_UnitTest::Stk_UnitTest(void)
{
    
}
        
        Stk_UnitTest::~Stk_UnitTest(void)
{
	//DELETE_STK_CLASS_IMPL
}

void Stk_UnitTest::WriteMeshToBinFile()
{
	Stk_ResourceManagerPtr resourceManagerPtr =  Stk_ResourceManagerPtr::CreateObject();

	Stk_MeshPrimPtr stk_MeshPtr = Stk_MeshPrimPtr::CreateObject();

	std::vector<STK_FLOAT32> m_vecVert;
	std::vector<STK_FLOAT32> m_vecNormals;
	std::vector<STK_UINT32> m_vecTrians;
	for (int i =0;i<3000;i++)
	{
		m_vecVert.push_back(100.0f);
		m_vecNormals.push_back(100.0f);
		m_vecTrians.push_back(100);
	}

	stk_MeshPtr->SetVertexs(m_vecVert);
	stk_MeshPtr->SetNormals(m_vecNormals);
	stk_MeshPtr->SetIndex(m_vecTrians);

	Stk_MeshAccessorPtr stk_MeshAccessorPtr = Stk_MeshAccessorPtr::CreateObject();
	string GeoFileName = "Geom_01.bin";
	Stk_FilePtr stk_filePtr = resourceManagerPtr->GetFile(GeoFileName);
	stk_filePtr->OpenAsBin("wb"); 
	stk_MeshAccessorPtr->SetType(ACCESSOR_TYPE_MESH_I_P_N);
	stk_MeshAccessorPtr->WriteFile(stk_MeshPtr,stk_filePtr);
	stk_filePtr->Close();

	resourceManagerPtr->Clear();

	//string GeoFileName = "Geom_01.bin";
	stk_MeshAccessorPtr->SetBinFileName(GeoFileName);

	stk_filePtr = resourceManagerPtr->GetFile(GeoFileName);
	stk_filePtr->OpenAsBin("rb");
	//stk_MeshAccessorPtr->FillMesh(stk_MeshPtr, resourceManagerPtr);
	stk_filePtr->Close();

	resourceManagerPtr->Clear();
}

void Stk_UnitTest::ReadAssemblyTest(wstring& filePath, bool readMesh)
{
	Stk_API::GetAPI();
	Stk_DocumentPtr m_svl2Doc = Stk_DocumentPtr::CreateObject();
	//m_svl2Doc->OpenLocal(filePath);
	m_svl2Doc->LoadBom(filePath);
	//int topNodeCount = m_svl2Doc->GetNbTopNode();
	int numTopInstnace = m_svl2Doc->GetNbTopInstance();
	if (numTopInstnace > 1)
	{
		//topModel->SetName("TopAssembly");
		for (int i = 0; i < numTopInstnace; i++)
		{
			//Stk_NodePtr topNode = m_svl2Doc->GetTopNode(i);
			Stk_InstancePtr topInstance = m_svl2Doc->GetTopInstance(i);
			FillAssembly(readMesh,&topInstance);
		}
	}
	else if (numTopInstnace == 1)
	{
		//Stk_NodePtr topNode = m_svl2Doc->GetTopNode(0);
			Stk_InstancePtr topInstance = m_svl2Doc->GetTopInstance(0);
		FillAssembly(readMesh, &topInstance);
	}
	// Õ∑≈Bom
	m_svl2Doc->UnloadBom();

	//º”‘ÿModel
	m_svl2Doc->LoadModel();
	//≤‚ ‘Model–≈œ¢
	//TestModel();
	// Õ∑≈Model
	//m_svl2Doc->UnloadModel();

}

void Stk_UnitTest::ReadMeshTest(bool readMesh)
{

}	


void Stk_UnitTest::FillPart(bool readMesh, void* pstk_ModelPtr)
{
	//Stk_PartPtr stkPart = stk_Node->GetPart();
	Stk_ModelPtr stkModel = NULL;// stk_Instance->GetModel();
	if (stkModel.isNotNull() && readMesh)
	{
		if (stkModel.isNotNull())
		{
			int meshCount = stkModel->GetNbMesh();
			if (meshCount)
			{
				for (int i = 0; i < meshCount; i++)
				{
					Stk_MeshPrimPtr meshPtr = stkModel->GetMesh(i);
					std::vector<float> vecVert;
					std::vector<float> vecNormals;
					std::vector<unsigned int> vecTrians;
					meshPtr->GetMeshBuf(vecVert, vecNormals, vecTrians);

					bool hasNormals = vecNormals.size() > 0;
					bool hasTextureCoords = false;
				}
			}
		}
	}
	//_stk_rgba32 nodeClr = stkModel->GetColor();
}

void Stk_UnitTest::FillAssembly(bool readMesh,void* pstk_NodePtr)
{
	//Stk_NodePtr stk_Node = *(Stk_NodePtr*)pstk_NodePtr;
	Stk_InstancePtr stk_Instance = *(Stk_InstancePtr*)pstk_NodePtr;
	if (stk_Instance.isNotNull())
	{
		for (int subModelIndex = 0; subModelIndex < stk_Instance->GetNbChild(); subModelIndex++)
		{
			//Model* childModel = new Model();
			//Stk_NodePtr nodePtr = stk_Node->GetChild(subModelIndex);
			Stk_InstancePtr instancePtr = stk_Instance->GetChild(subModelIndex);
			FillAssembly(readMesh, &instancePtr);
			//model->AddSubModel(childModel);
		}
	}
}


}//svllib
}//hoteamsoft

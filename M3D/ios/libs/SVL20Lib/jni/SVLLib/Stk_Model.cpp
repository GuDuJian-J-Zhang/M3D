// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_Model.cpp
 *
****************************************************************************/
#include "Stk_Model.h"
#include "Stk_ObjectImpl.h"
#include "Stk_DocumentImpl.h"
#include "Stk_Document.h"
#include "JsonMacro.h"
#include "Stk_MeshLodBlock.h"
#include "Stk_PMI.h"
#include "Stk_View.h"
#include "Stk_LineSet.h"
#include "Stk_SpacialPoint.h"
#include "Stk_LayerInfosSet.h"
#include "Stk_Feat.h"

namespace HoteamSoft {
	namespace SVLLib {
class Stk_ModelImpl : public Stk_ObjectImpl
{
	STK_CLASS_IMPL_DECLARE(Stk_Model)
public:
	Stk_ModelImpl(void);
	virtual ~Stk_ModelImpl(void);

	Stk_Document* m_pDoc;
	Stk_Document* GetDoc() { return m_pDoc; }

	Value*				m_pJsonModel;
	map<int, Stk_MeshPrimPtr> m_mapMeshIDToStkMesh;
	map<int, Stk_MeshPrimPtr> m_mapLOD1Mesh;
	vector<Stk_MeshPrimPtr> m_vecLOD1Mesh;
	map<int, Stk_MeshPrimPtr> m_mapLOD2Mesh;
	vector<Stk_MeshPrimPtr> m_vecLOD2Mesh;
	StkModelBlock m_modelBlock;
	vector<StkAccessorBlock> m_vecAccessorBlock;

	map<int, Stk_LineSetPtr> m_mapLineSet;
	map<int, Stk_CurveEntityPtr> m_mapSubLine;
	map<int, Stk_SpacialPointEntityPtr> m_mapSpacialPoint;
	map<int, Stk_LayerInfosSetPtr> m_mapLayerInfosSet;
	map<int, Stk_LayerPtr> m_mapLayer;
	map<int, Stk_FeatPtr> m_mapFeat;
};

STK_CLASS_IMPL_IMPLEMENT(Stk_Model)
Stk_ModelImpl::Stk_ModelImpl(void)
{

}

Stk_ModelImpl::~Stk_ModelImpl(void)
{
	m_mapMeshIDToStkMesh.clear();
	m_mapLOD1Mesh.clear();
	m_vecLOD1Mesh.clear();
	m_mapLOD2Mesh.clear();
	m_vecLOD2Mesh.clear();
	m_vecAccessorBlock.shrink_to_fit();
	m_vecAccessorBlock.clear();
	m_mapLineSet.clear();
	m_mapSubLine.clear();
	m_mapSpacialPoint.clear();
	m_mapLayerInfosSet.clear();
	m_mapLayer.clear();
	m_mapFeat.clear();
}


STK_IMPLEMENT_CLASS(Stk_Model, Stk_Object)

Stk_Model::Stk_Model(void):Stk_Object(new Stk_ModelImpl())
{
}

Stk_Model::~Stk_Model(void)
{
	DELETE_STK_CLASS_IMPL
}

int Stk_Model::Add()
{
	int iRet = 0;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_ModelImpl*)m_pImpl)->m_pDoc->m_pImpl;
	//添加Part节点
	Value valModelRoot;
	Value valModel;
	valModelRoot.SetArray();
	valModel.SetObject();
	if (!docImpl->m_modelDoc.HasMember(MODELS))
	{
		docImpl->m_modelDoc.AddMember(MODELS, valModelRoot, docImpl->m_modelDoc.GetAllocator());
	}
	Value::MemberIterator modelRootIter = docImpl->m_modelDoc.FindMember(MODELS);
	if (modelRootIter != docImpl->m_modelDoc.MemberEnd())
	{
		valModel.AddMember("id", this->GetID(), docImpl->m_modelDoc.GetAllocator());
		modelRootIter->value.PushBack(valModel, docImpl->m_modelDoc.GetAllocator());
		//将Json节点和Part对象绑定
		BindModel(&(modelRootIter->value[modelRootIter->value.Size() - 1]));
	}
	return iRet;
}

STK_UINT32 Stk_Model::GetLodCount()
{
	return ((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock.size();
}

HoteamSoft::SVLLib::Stk_MeshPrimPtr Stk_Model::AddMeshPrim(unsigned int uiID /*= -1*/)
{
	Value* pValPart = ((Stk_ModelImpl*)m_pImpl)->m_pJsonModel;
	Stk_MeshPrimPtr meshPtr = Stk_MeshPrimPtr::CreateObject();
	if (-1 != uiID) {
		meshPtr->SetID(uiID);
	}
	else {
		Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_ModelImpl*)m_pImpl)->m_pDoc->m_pImpl;
		docImpl->m_vecMeshes.push_back(meshPtr);
		//创建Mesh节点
		Value valMeshRoot;
		Value valMesh;
		valMeshRoot.SetObject();
		valMesh.SetObject();
		if (!pValPart->HasMember("Meshes"))
		{
			pValPart->AddMember("Meshes", valMeshRoot, docImpl->m_modelDoc.GetAllocator());
		}
		Value::MemberIterator meshRootIter = pValPart->FindMember("Meshes");
		if (meshRootIter !=pValPart->MemberEnd())
		{
			char charID[32];
			sprintf(charID,"%d",meshPtr->GetID());
			//itoa(meshPtr->GetID(), charID, 10);
			string strID = charID;
			valMesh.AddMember("materialID", 1, docImpl->m_modelDoc.GetAllocator());
			Value key(StringRef(strID), docImpl->m_modelDoc.GetAllocator());
			meshRootIter->value.AddMember(key, valMesh, docImpl->m_modelDoc.GetAllocator());
			//绑定Mesh节点
			Value::MemberIterator meshIter = meshRootIter->value.MemberEnd();
			meshIter--;
			meshPtr->BindMesh(&meshIter->value);
		}
	}
	//meshPtr->Init(pNodeImp->m_pDoc, GetID(),  - 1 != uiID ? false : true);
	return meshPtr;
}
Stk_MeshPrimPtr Stk_Model::AddLOD1Mesh(unsigned int uiID /*= -1*/)
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_ModelImpl*)m_pImpl)->m_pDoc->m_pImpl;
#if 0
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_ModelImpl*)m_pImpl)->m_pDoc->m_pImpl;
	Value* pValModel = ((Stk_ModelImpl*)m_pImpl)->m_pJsonModel;
	//创建Mesh的Json节点
	if (!pValModel->HasMember("mesh"))
	{
		Value valMeshRoot;
		valMeshRoot.SetObject();
		pValModel->AddMember("mesh", valMeshRoot, docImpl->m_modelDoc.GetAllocator());
	}
	if (!(*pValModel)[MESH].HasMember("lod1"))
	{
		Value valLOD1;
		valLOD1.SetObject();
		(*pValModel)[MESH].AddMember("lod1", valLOD1, docImpl->m_modelDoc.GetAllocator());
	}
#endif
	//创建Mesh对象
	Stk_MeshPrimPtr meshPtr = Stk_MeshPrimPtr::CreateObject();
	docImpl->m_meshCount++;
	if (-1 != uiID) {
		meshPtr->SetID(uiID);
	}
	meshPtr->Init(((Stk_ModelImpl*)m_pImpl)->m_pDoc, uiID);
	map<int, Stk_MeshPrimPtr>::iterator it;
	it = ((Stk_ModelImpl*)m_pImpl)->m_mapLOD1Mesh.find(meshPtr->GetID());
	if (((Stk_ModelImpl*)m_pImpl)->m_mapLOD1Mesh.end() == it)
		((Stk_ModelImpl*)m_pImpl)->m_mapLOD1Mesh.insert(make_pair(meshPtr->GetID(), meshPtr));
	return meshPtr;
}

STK_STATUS Stk_Model::DelViewByID(unsigned int viewID /*= -1*/)
{
	return STK_SUCCESS;
}
unsigned int Stk_Model::GetNbMesh()
{
	int numMesh = 0;
	Value* valPart = ((Stk_ModelImpl*)m_pImpl)->m_pJsonModel;
	Value::MemberIterator meshRootIter = valPart->FindMember("Meshes");
	if (meshRootIter !=valPart->MemberEnd())
	{
		numMesh = meshRootIter->value.MemberCount();
	}
	return numMesh;
}

Stk_MeshPrimPtr Stk_Model::GetMesh(int iIdx)
{
	Value* valPart = ((Stk_ModelImpl*)m_pImpl)->m_pJsonModel;
	Value::MemberIterator meshRootIter = valPart->FindMember("Meshes");
	if (meshRootIter ==valPart->MemberEnd())
	{
		return NULL;
	}
	string strMeshID;
	int meshID;
	Value::MemberIterator meshIter = meshRootIter->value.MemberBegin();
	strMeshID = (meshIter+iIdx)->name.GetString();
	meshID = atoi(strMeshID.c_str());
	//判断该Mesh对象被创建
	map<int, Stk_MeshPrimPtr>::iterator it;
	it = ((Stk_ModelImpl*)m_pImpl)->m_mapMeshIDToStkMesh.find(meshID);
	if(it != ((Stk_ModelImpl*)m_pImpl)->m_mapMeshIDToStkMesh.end())
		return it->second;
	//创建Mesh对象
	Stk_MeshPrimPtr meshPtr = Stk_MeshPrimPtr::CreateObject();
	meshPtr->SetID(meshID);
	meshPtr->Init(((Stk_ModelImpl*)m_pImpl)->m_pDoc, this->GetID(), true);
	Value* valMesh = &(meshIter+iIdx)->value;
	meshPtr->BindMesh(valMesh);
	//添加Map
	((Stk_ModelImpl*)m_pImpl)->m_mapMeshIDToStkMesh.insert(make_pair(meshID, meshPtr));
	return meshPtr;
}

unsigned int Stk_Model::GetNbLOD1Mesh()
{
	return ((Stk_ModelImpl*)m_pImpl)->m_vecLOD1Mesh.size();
}

unsigned int Stk_Model::GetNbLOD2Mesh()
{
	return ((Stk_ModelImpl*)m_pImpl)->m_vecLOD2Mesh.size();
}

Stk_MeshPrimPtr Stk_Model::GetLOD1Mesh(int iIdx)
{
	return ((Stk_ModelImpl*)m_pImpl)->m_vecLOD1Mesh[iIdx];
}

Stk_MeshPrimPtr Stk_Model::GetLOD2Mesh(int iIdx)
{
	return ((Stk_ModelImpl*)m_pImpl)->m_vecLOD2Mesh[iIdx];
}

STK_STATUS Stk_Model::GetLOD1Accessor(STK_INT32& i_fileIndex, STK_UINT32& i_fileOffset, STK_UINT32& i_fileLength)
{
	i_fileIndex = -1;
	i_fileOffset = i_fileLength = 0;
#if 0
	//解析Mesh信息
	Value* valModel = ((Stk_ModelImpl*)m_pImpl)->m_pJsonModel;
	if( !(*valModel).HasMember(MESH))
		return STK_ERROR;
	//创建MeshBlock
	Stk_MeshLodBlock* pStkMeshLodBlock = new Stk_MeshLodBlock();
	//获取文件信息
	string GeoFileName = (*valModel)[MESH][LOD1][ACCESOR][FILENAME].GetString();
	i_fileIndex = -1;
	i_fileOffset = (*valModel)[MESH][LOD1][ACCESOR][OFFSET].GetInt();
	i_fileLength = (*valModel)[MESH][LOD1][ACCESOR][LENGTH].GetInt();
#endif
	if (((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock.size() < 1)
	{
		return STK_ERROR;
	}
	i_fileIndex = -1;
	i_fileOffset = ((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock[0].blockOffset;
	i_fileLength = ((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock[0].blockLength;
	return STK_SUCCESS;
}

STK_STATUS Stk_Model::GetLOD2Accessor(STK_INT32& i_fileIndex, STK_UINT32& i_fileOffset, STK_UINT32& i_fileLength)
{
	i_fileIndex = -1;
	i_fileOffset = i_fileLength = 0;

	if (((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock.size() < 2)
	{
		return STK_ERROR;
	}
	i_fileIndex = -1;
	i_fileOffset = ((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock[1].blockOffset;
	i_fileLength = ((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock[1].blockLength;
	return STK_SUCCESS;
}

void Stk_Model::SetBoundingBox(float* i_pBBox)
{
#if 0
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_ModelImpl*)m_pImpl)->m_pDoc->m_pImpl;
	string strBBox;
	for (int i =0; i < 6; i++)
	{
		char charBBox[256];
		sprintf(charBBox, "%f,", i_pBBox[i]);
		strBBox += charBBox;
	}
	Value* valModel = ((Stk_ModelImpl*)m_pImpl)->m_pJsonModel;
	Value::MemberIterator bboxIter = valModel->FindMember("bbox");
	if (bboxIter !=valModel->MemberEnd())
	{
		bboxIter->value.SetString(strBBox, docImpl->m_nodeDoc.GetAllocator());
	}
	else
	{
		Value name(StringRef(strBBox), docImpl->m_nodeDoc.GetAllocator());
		valModel->AddMember("bbox", name, docImpl->m_nodeDoc.GetAllocator());
	}
#endif
	for (int i = 0; i < 6; i++)
	{
		((Stk_ModelImpl*)m_pImpl)->m_modelBlock.BBox[i] = i_pBBox[i];
	}
}

int Stk_Model::GetBoundingBox(float* o_pBBox, int i_size)
{
	float fBBox[6] = {0};
#if 0
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_ModelImpl*)m_pImpl)->m_pDoc->m_pImpl;
	Value* valModel = ((Stk_ModelImpl*)m_pImpl)->m_pJsonModel;
	if(!(*valModel).HasMember("bbox"))
		return -1;
	string strBBox;
	strBBox = (*valModel)["bbox"].GetString();
	if("" == strBBox)
	{
		return -1;
	}
	for (int i = 0; i < i_size; i++)
	{
		string strTemp = strBBox.substr(0, strBBox.find_first_of(","));
		o_pBBox[i] = atof(strTemp.c_str());
		strBBox = strBBox.substr(strBBox.find_first_of(",") + 1, strBBox.length() - strTemp.length() - 1);
	}
#endif
	for (int i = 0; i < i_size; i++)
	{
		o_pBBox[i] = ((Stk_ModelImpl*)m_pImpl)->m_modelBlock.BBox[i];
	}
	return 0;
}

int Stk_Model::GetBoundingBoxByJson(float* o_pBBox, int i_size)
{
	float fBBox[6] = {0};
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_ModelImpl*)m_pImpl)->m_pDoc->m_pImpl;
	Value* valModel = ((Stk_ModelImpl*)m_pImpl)->m_pJsonModel;
	if(!(*valModel).HasMember("bbox"))
		return -1;
	string strBBox;
	strBBox = (*valModel)["bbox"].GetString();
	if("" == strBBox)
	{
		return -1;
	}
	for (int i = 0; i < i_size; i++)
	{
		string strTemp = strBBox.substr(0, strBBox.find_first_of(","));
		o_pBBox[i] = atof(strTemp.c_str());
		strBBox = strBBox.substr(strBBox.find_first_of(",") + 1, strBBox.length() - strTemp.length() - 1);
	}
	return 0;
}

STK_STATUS Stk_Model::WriteMesh(Stk_FilePtr& binFile)
{
	STK_STATUS status;
	WriteLod1Mesh(binFile);
	WriteLod2Mesh(binFile);
	return STK_SUCCESS;
}

STK_STATUS Stk_Model::WriteLod1Mesh(Stk_FilePtr& binFile)
{
	//创建MeshBlock
	Stk_MeshLodBlock* pStkMeshLodBlock = new Stk_MeshLodBlock();
	//LOD1
	int numMeshLOD1 = ((Stk_ModelImpl*)m_pImpl)->m_mapLOD1Mesh.size();
	if(0 == numMeshLOD1)
		return STK_ERROR;
	map<int, Stk_MeshPrimPtr>::iterator it;
	for(  it= ((Stk_ModelImpl*)m_pImpl)->m_mapLOD1Mesh.begin(); it!= ((Stk_ModelImpl*)m_pImpl)->m_mapLOD1Mesh.end(); it++)
	{
		pStkMeshLodBlock->AddMesh(it->second);
	}
	//创建访问器对象
	Stk_CommonAccessorPtr stkMeshAccessor = Stk_CommonAccessorPtr::CreateObject();
	pStkMeshLodBlock->WrieteFile(stkMeshAccessor, binFile);
	//创建访问器信息
	StkAccessorBlock stkLod1AccessorBlock;
	stkLod1AccessorBlock.fileIndex = -1;
	stkLod1AccessorBlock.blockOffset =  stkMeshAccessor->GetByteOffset();
	stkLod1AccessorBlock.blockLength = stkMeshAccessor->GetByteLength();
	((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock.push_back(stkLod1AccessorBlock);
	delete pStkMeshLodBlock;
    return STK_SUCCESS;
}

STK_STATUS Stk_Model::WriteLod2Mesh(Stk_FilePtr& binFile)
{
	//创建MeshBlock
	Stk_MeshLodBlock* pStkMeshLodBlock = new Stk_MeshLodBlock();
	//LOD1
	int numMeshLOD2 = ((Stk_ModelImpl*)m_pImpl)->m_mapLOD2Mesh.size();
	if(0 == numMeshLOD2)
		return STK_ERROR;
	map<int, Stk_MeshPrimPtr>::iterator it;
	for(  it= ((Stk_ModelImpl*)m_pImpl)->m_mapLOD2Mesh.begin(); it!= ((Stk_ModelImpl*)m_pImpl)->m_mapLOD2Mesh.end(); it++)
	{
		pStkMeshLodBlock->AddMesh(it->second);
	}
	//创建访问器对象
	Stk_CommonAccessorPtr stkMeshAccessor = Stk_CommonAccessorPtr::CreateObject();
	pStkMeshLodBlock->WrieteFile(stkMeshAccessor, binFile);
	//创建访问器信息
	StkAccessorBlock stkLod2AccessorBlock;
	stkLod2AccessorBlock.fileIndex = -1;
	stkLod2AccessorBlock.blockOffset =  stkMeshAccessor->GetByteOffset();
	stkLod2AccessorBlock.blockLength = stkMeshAccessor->GetByteLength();
	((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock.push_back(stkLod2AccessorBlock);
	delete pStkMeshLodBlock;
    return STK_SUCCESS;
}

STK_STATUS Stk_Model::LoadLOD1Mesh()
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	((Stk_ModelImpl*)m_pImpl)->m_vecLOD1Mesh.clear();
#if 0
	//解析Mesh信息
	Value* valModel = ((Stk_ModelImpl*)m_pImpl)->m_pJsonModel;
	if( !(*valModel).HasMember(MESH))
		return STK_ERROR;
	//获取文件信息
	string GeoFileName = (*valModel)[MESH][LOD1][ACCESOR][FILENAME].GetString();
#endif	
	//获取文件信息
	if (((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock.size() == 0)
	{
		return STK_ERROR;
	}
	string GeoFileName;
	if (-1 == ((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock[0].fileIndex)
	{
		GeoFileName = docImpl->m_strOutputName + ".mesh";
	}
	
	//创建MeshBlock
	Stk_MeshLodBlock* pStkMeshLodBlock = new Stk_MeshLodBlock();
	Stk_FilePtr stk_filePtr;
	switch (docImpl->m_enumFileOpenMode)
	{
	case STK_FILE_OPENMODE_LOCAL:
		{
			stk_filePtr = docImpl->m_resourceManagerPtr->GetFile(GeoFileName);
			stk_filePtr->OpenAsBin("rb"); 
		}
		break;
	case STK_FILE_OPENMODE_BUFFER:
		{
			if (!docImpl->m_hasExtractMeshData)
			{
				docImpl->loadMeshFromBuff();
			}
			stk_filePtr = docImpl->m_meshFilePtr;
		}
		break;
	}
	//获取资源管理器
	Stk_CommonAccessorPtr stkMeshAccessor = Stk_CommonAccessorPtr::CreateObject();
	//stkMeshAccessor->Uncode( (*valModel)[MESH][LOD1][ACCESOR]);
	stkMeshAccessor->Uncode(GeoFileName, ((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock[0].blockOffset, ((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock[0].blockLength);
	pStkMeshLodBlock->ReadFile(stkMeshAccessor, stk_filePtr);
	pStkMeshLodBlock->GetMeshes(((Stk_ModelImpl*)m_pImpl)->m_vecLOD1Mesh);

	delete pStkMeshLodBlock;

	return STK_SUCCESS;
}

STK_STATUS Stk_Model::LoadLOD2Mesh()
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	((Stk_ModelImpl*)m_pImpl)->m_vecLOD2Mesh.clear();
	//获取文件信息
	if (((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock.size() < 2)
	{
		return STK_ERROR;
	}
	string GeoFileName;
	if (-1 == ((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock[1].fileIndex)
	{
		GeoFileName = docImpl->m_strOutputName + ".mesh";
	}

	//创建MeshBlock
	Stk_MeshLodBlock* pStkMeshLodBlock = new Stk_MeshLodBlock();
	Stk_FilePtr stk_filePtr;
	switch (docImpl->m_enumFileOpenMode)
	{
	case STK_FILE_OPENMODE_LOCAL:
		{
			stk_filePtr = docImpl->m_resourceManagerPtr->GetFile(GeoFileName);
			stk_filePtr->OpenAsBin("rb"); 
		}
		break;
	case STK_FILE_OPENMODE_BUFFER:
		{
			if (!docImpl->m_hasExtractMeshData)
			{
				docImpl->loadMeshFromBuff();
			}
			stk_filePtr = docImpl->m_meshFilePtr;
		}
		break;
	}
	//获取资源管理器
	Stk_CommonAccessorPtr stkMeshAccessor = Stk_CommonAccessorPtr::CreateObject();
	//stkMeshAccessor->Uncode( (*valModel)[MESH][LOD1][ACCESOR]);
	stkMeshAccessor->Uncode(GeoFileName, ((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock[1].blockOffset, ((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock[1].blockLength);
	pStkMeshLodBlock->ReadFile(stkMeshAccessor, stk_filePtr);
	pStkMeshLodBlock->GetMeshes(((Stk_ModelImpl*)m_pImpl)->m_vecLOD2Mesh);

	delete pStkMeshLodBlock;

	return STK_SUCCESS;
}

STK_STATUS Stk_Model::UnloadLOD1Mesh()
{
	int numLod1Mesh = GetNbLOD1Mesh();
	for (int i = 0; i < numLod1Mesh; i++)
	{
		 Stk_MeshPrimPtr meshPrimPtr =  GetLOD1Mesh(i);
		////清除面的信息
		//int numMeshFace = meshPrimPtr->GetNbMeshFace();
		//for (int j = 0; j < numMeshFace; j++)
		//{
		//	Stk_MeshFacePtr meshFacePtr = meshPrimPtr->GetMeshFaceByIdx(j);
		//	meshFacePtr->Clear();
		//}
		////清楚边的信息
		//int numMeshEdge = meshPrimPtr->GetNbMeshEdge();
		//for (int j = 0; j < numMeshEdge; j++)
		//{
		//	Stk_MeshEdgePtr meshEdgePtr = meshPrimPtr->GetMeshEdgeByIdx(j);
		//	meshEdgePtr->Clear();
		//}
		//清除Mesh信息
		meshPrimPtr->Clear();
	}
	//释放Mesh信息
	((Stk_ModelImpl*)m_pImpl)->m_vecLOD1Mesh.shrink_to_fit();
	((Stk_ModelImpl*)m_pImpl)->m_vecLOD1Mesh.clear();
	return STK_SUCCESS;
}

STK_STATUS Stk_Model::UnloadLOD2Mesh()
{
	int numLod2Mesh = GetNbLOD2Mesh();
	for (int i = 0; i < numLod2Mesh; i++)
	{
		Stk_MeshPrimPtr meshPrimPtr =  GetLOD2Mesh(i);
		meshPrimPtr->Clear();
	}
	//释放Mesh信息
	((Stk_ModelImpl*)m_pImpl)->m_vecLOD2Mesh.shrink_to_fit();
	((Stk_ModelImpl*)m_pImpl)->m_vecLOD2Mesh.clear();
	return STK_SUCCESS;
}

Stk_MeshPrimPtr Stk_Model::AddLOD2Mesh(unsigned int uiID /*= -1*/)
{
	//创建Mesh对象
	Stk_MeshPrimPtr meshPtr = Stk_MeshPrimPtr::CreateObject();
	if (-1 != uiID) {
		meshPtr->SetID(uiID);
	}
	meshPtr->Init(((Stk_ModelImpl*)m_pImpl)->m_pDoc, uiID);
	map<int, Stk_MeshPrimPtr>::iterator it;
	it = ((Stk_ModelImpl*)m_pImpl)->m_mapLOD2Mesh.find(meshPtr->GetID());
	if (((Stk_ModelImpl*)m_pImpl)->m_mapLOD2Mesh.end() == it)
		((Stk_ModelImpl*)m_pImpl)->m_mapLOD2Mesh.insert(make_pair(meshPtr->GetID(), meshPtr));
	return meshPtr;
}

void Stk_Model::BindModel(void* i_bindedModel)
{
	((Stk_ModelImpl*)m_pImpl)->m_pJsonModel = (Value*)i_bindedModel;
}

void Stk_Model::BindModelBlock(void* i_bindedModelBlock)
{
	((Stk_ModelImpl*)m_pImpl)->m_modelBlock = *(StkModelBlock*)i_bindedModelBlock;
}

void Stk_Model::BindLodAccessorBlock(void* i_lodAccessorBlock)
{
	((Stk_ModelImpl*)m_pImpl)->m_vecAccessorBlock.push_back(*(StkAccessorBlock*)i_lodAccessorBlock);
}

STK_UINT32 Stk_Model::GetMeshLodBlockCount()
{
	return this->m_lodMeshesBlocks.size();
}

Stk_MeshLodBlock* Stk_Model::GetMeshLodBlock(STK_UINT32 lodLevel)
{
	if (this->m_lodMeshesBlocks.size()>lodLevel)
	{
		return this->m_lodMeshesBlocks.at(lodLevel);
	}

	return NULL;
}

void Stk_Model::Init(Stk_Document* pDoc, unsigned int parentID, bool bForWrite/* = true*/)
{
	((Stk_ModelImpl*)m_pImpl)->m_pDoc = pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	if (bForWrite) {
		//MeshPrim* meshPrim = docImpl->GetMeshPrim(this->GetID());
		//meshPrim->mutable_baseprim()->set_assemblyid(parentID);
	}else{
		//初始化Mesh信息
		((Stk_ModelImpl*)m_pImpl)->m_mapMeshIDToStkMesh.clear();
	}
}

STK_STATUS	Stk_Model::AddPMI(Stk_PMIPtr i_StkPMIP)
{
	if (!((Stk_ModelImpl*)m_pImpl)->m_pDoc || !(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl)
		return STK_ERROR;
	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	if (pDocImpl->m_pPMIDoc == nullptr)
		return  STK_ERROR;
	pDocImpl->m_pPMIDoc->AddPMI(GetID(), i_StkPMIP);
	return STK_SUCCESS;
}

STK_STATUS Stk_Model::GetAllPMI(vector<Stk_PMIPtr>& vcPMI)
{
	if (!((Stk_ModelImpl*)m_pImpl)->m_pDoc || !(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl)
		return STK_ERROR;
	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	if (pDocImpl->m_pPMIDoc == nullptr)
		return  STK_ERROR;
	pDocImpl->m_pPMIDoc->GetPMIListByModelID(GetID(), vcPMI);
	return STK_SUCCESS;
}

STK_STATUS Stk_Model::GetAllView(vector<Stk_ViewPtr>& vcViews)
{
	if (!((Stk_ModelImpl*)m_pImpl)->m_pDoc || !(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl)
		return STK_ERROR;
	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	if (pDocImpl->m_pPMIDoc == nullptr)
		return  STK_ERROR;
	pDocImpl->m_pViewDoc->GetViewListByModelId(GetID(), vcViews);
    return STK_SUCCESS;
}

STK_STATUS  Stk_Model::AddView(Stk_ViewPtr i_StkViewP)
{
	if (!((Stk_ModelImpl*)m_pImpl)->m_pDoc || !(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl)
		return STK_ERROR;
	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	if (pDocImpl->m_pViewDoc == nullptr)
		return  STK_ERROR;
	pDocImpl->m_pViewDoc->AddView(GetID(), i_StkViewP);
	return STK_SUCCESS;
}

wstring  Stk_Model::GetModelName()
{
	wstring wstrName = L"";
	if (!((Stk_ModelImpl*)m_pImpl)->m_pDoc || !(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl)
		return wstrName;
	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	if (pDocImpl->m_pAttrDoc == nullptr)
		return wstrName;
	wstrName = pDocImpl->m_pAttrDoc->GetModelAttribute(GetID(), L"name");

	return wstrName;
}

void  Stk_Model::SetModelName(const wstring& i_wstrName)
{
	if (!((Stk_ModelImpl*)m_pImpl)->m_pDoc || !(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl)
		return;
	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	if (pDocImpl->m_pAttrDoc == nullptr)
		return;
	pDocImpl->m_pAttrDoc->AddModelAttribute(GetID(), L"name", i_wstrName);
}

void  Stk_Model::AddAttribute(const std::wstring& i_wstrAttrName, const std::wstring& i_wstrAttrValue)
{
	if (!((Stk_ModelImpl*)m_pImpl)->m_pDoc || !(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl)
		return;
	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	if (pDocImpl->m_pAttrDoc == nullptr)
		return;
	pDocImpl->m_pAttrDoc->AddModelAttribute(GetID(), i_wstrAttrName, i_wstrAttrValue);
}

std::wstring   Stk_Model::GetAttribute(const std::wstring& i_wstrAttrName)
{
	wstring wstrName = L"";
	if (!((Stk_ModelImpl*)m_pImpl)->m_pDoc || !(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl)
		return wstrName;
	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	if (pDocImpl->m_pAttrDoc == nullptr)
		return wstrName;
	wstrName = pDocImpl->m_pAttrDoc->GetModelAttribute(GetID(), i_wstrAttrName);

	return wstrName;
}

map<wstring, wstring>   Stk_Model::GetAllAttribute()
{
	map<wstring, wstring> mapAttribute;
	wstring wstrName = L"";
	if (!((Stk_ModelImpl*)m_pImpl)->m_pDoc || !(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl)
		return mapAttribute;
	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_ModelImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	if (pDocImpl->m_pAttrDoc == nullptr)
		return mapAttribute;
	mapAttribute = pDocImpl->m_pAttrDoc->GetModelAllAttributes(GetID());
	return mapAttribute;
}

HoteamSoft::SVLLib::Stk_LineSetPtr Stk_Model::AddLineSet(unsigned int uiID /*= -1*/)
{
	Stk_LineSetPtr lineSetPtr = nullptr;
	if (-1 != uiID) {
		map<int, Stk_LineSetPtr>::iterator it;
		it = ((Stk_ModelImpl*)m_pImpl)->m_mapLineSet.find(uiID);
		if (((Stk_ModelImpl*)m_pImpl)->m_mapLineSet.end() == it)
		{
			lineSetPtr = Stk_LineSetPtr::CreateObject();
			lineSetPtr->SetID(uiID);
			((Stk_ModelImpl*)m_pImpl)->m_mapLineSet.insert(make_pair(lineSetPtr->GetID(), lineSetPtr));
		}
		else
		{
			lineSetPtr = it->second;
		}
	}
	else {
		lineSetPtr = Stk_LineSetPtr::CreateObject();
		((Stk_ModelImpl*)m_pImpl)->m_mapLineSet.insert(make_pair(lineSetPtr->GetID(), lineSetPtr));
	}
	//meshPtr->Init(pNodeImp->m_pDoc, GetID(),  - 1 != uiID ? false : true);
	return lineSetPtr;
}

STK_STATUS Stk_Model::DeleteLineSet(unsigned int uiID)
{
	map<int, Stk_LineSetPtr>::iterator it;
	it = ((Stk_ModelImpl*)m_pImpl)->m_mapLineSet.find(uiID);
	if (((Stk_ModelImpl*)m_pImpl)->m_mapLineSet.end() == it)
	{
		((Stk_ModelImpl*)m_pImpl)->m_mapLineSet.erase(it);
	}
	return STK_SUCCESS;
}

map<int, Stk_LineSetPtr>& Stk_Model::GetAllLineSet()
{
	return ((Stk_ModelImpl*)m_pImpl)->m_mapLineSet;
}

STK_STATUS Stk_Model::AddSubLine(Stk_CurveEntityPtr linePtr)
{
	if(linePtr == nullptr)
		return STK_ERROR;
	STK_ID uiID = linePtr->GetID();
	
	map<int, Stk_CurveEntityPtr>::iterator it;
	it = ((Stk_ModelImpl*)m_pImpl)->m_mapSubLine.find(uiID);
	if (((Stk_ModelImpl*)m_pImpl)->m_mapSubLine.end() == it)
	{
		((Stk_ModelImpl*)m_pImpl)->m_mapSubLine.insert(make_pair(linePtr->GetID(), linePtr));
	}
	else
	{
		return STK_SUCCESS_EXIST;
	}

	
	return STK_SUCCESS;
}

STK_STATUS Stk_Model::DeleteSubLine(unsigned int uiID)
{
	map<int, Stk_CurveEntityPtr>::iterator it;
	it = ((Stk_ModelImpl*)m_pImpl)->m_mapSubLine.find(uiID);
	if (((Stk_ModelImpl*)m_pImpl)->m_mapSubLine.end() == it)
	{
		((Stk_ModelImpl*)m_pImpl)->m_mapSubLine.erase(it);
	}
	return STK_SUCCESS;
}

map<int, Stk_CurveEntityPtr>& Stk_Model::GetAllSubLine()
{
	return ((Stk_ModelImpl*)m_pImpl)->m_mapSubLine;
}

HoteamSoft::SVLLib::Stk_SpacialPointEntityPtr Stk_Model::AddSpacialPoint(unsigned int uiID /*= -1*/)
{
	Stk_SpacialPointEntityPtr spacialPointEntityPtr = nullptr;
	if (-1 != uiID) {
		map<int, Stk_SpacialPointEntityPtr>::iterator it;
		it = ((Stk_ModelImpl*)m_pImpl)->m_mapSpacialPoint.find(uiID);
		if (((Stk_ModelImpl*)m_pImpl)->m_mapSpacialPoint.end() == it)
		{
			spacialPointEntityPtr = Stk_SpacialPointEntityPtr::CreateObject();
			spacialPointEntityPtr->SetID(uiID);
			((Stk_ModelImpl*)m_pImpl)->m_mapSpacialPoint.insert(make_pair(spacialPointEntityPtr->GetID(), spacialPointEntityPtr));
		}
		else
		{
			spacialPointEntityPtr = it->second;
		}
	}
	else {
		spacialPointEntityPtr = Stk_SpacialPointEntityPtr::CreateObject();
		((Stk_ModelImpl*)m_pImpl)->m_mapSpacialPoint.insert(make_pair(spacialPointEntityPtr->GetID(), spacialPointEntityPtr));
	}
	//meshPtr->Init(pNodeImp->m_pDoc, GetID(),  - 1 != uiID ? false : true);
	return spacialPointEntityPtr;
}

STK_STATUS Stk_Model::DeleteSpacialPoint(unsigned int uiID)
{
	map<int, Stk_SpacialPointEntityPtr>::iterator it;
	it = ((Stk_ModelImpl*)m_pImpl)->m_mapSpacialPoint.find(uiID);
	if (((Stk_ModelImpl*)m_pImpl)->m_mapSpacialPoint.end() == it)
	{
		((Stk_ModelImpl*)m_pImpl)->m_mapSpacialPoint.erase(it);
	}
	return STK_SUCCESS;
}

map<int, Stk_SpacialPointEntityPtr>& Stk_Model::GetAllSpacialPoint()
{
	return ((Stk_ModelImpl*)m_pImpl)->m_mapSpacialPoint;
}


HoteamSoft::SVLLib::Stk_LayerInfosSetPtr Stk_Model::AddLayerInfosSet(unsigned int uiID /*= -1*/)
{
	Stk_LayerInfosSetPtr layerInfosSetPtr = nullptr;
	if (-1 != uiID) {
		map<int, Stk_LayerInfosSetPtr>::iterator it;
		it = ((Stk_ModelImpl*)m_pImpl)->m_mapLayerInfosSet.find(uiID);
		if (((Stk_ModelImpl*)m_pImpl)->m_mapLayerInfosSet.end() == it)
		{
			layerInfosSetPtr = Stk_LayerInfosSetPtr::CreateObject();
			layerInfosSetPtr->SetID(uiID);
			((Stk_ModelImpl*)m_pImpl)->m_mapLayerInfosSet.insert(make_pair(layerInfosSetPtr->GetID(), layerInfosSetPtr));
		}
		else
		{
			layerInfosSetPtr = it->second;
		}
	}
	else {
		layerInfosSetPtr = Stk_LayerInfosSetPtr::CreateObject();
		((Stk_ModelImpl*)m_pImpl)->m_mapLayerInfosSet.insert(make_pair(layerInfosSetPtr->GetID(), layerInfosSetPtr));
	}
	//meshPtr->Init(pNodeImp->m_pDoc, GetID(),  - 1 != uiID ? false : true);
	return layerInfosSetPtr;
}

HoteamSoft::SVLLib::STK_STATUS Stk_Model::DeleteLayerInfosSet(unsigned int uiID)
{
	map<int, Stk_LayerInfosSetPtr>::iterator it;
	it = ((Stk_ModelImpl*)m_pImpl)->m_mapLayerInfosSet.find(uiID);
	if (((Stk_ModelImpl*)m_pImpl)->m_mapLayerInfosSet.end() == it)
	{
		((Stk_ModelImpl*)m_pImpl)->m_mapLayerInfosSet.erase(it);
	}
	return STK_SUCCESS;
}

map<int, Stk_LayerInfosSetPtr>& Stk_Model::GetAllLayerInfosSet()
{
	return ((Stk_ModelImpl*)m_pImpl)->m_mapLayerInfosSet;
}

Stk_LayerPtr Stk_Model::GetLayerById(unsigned int uiID)
{
	Stk_LayerPtr pRetLayer = nullptr;
	map<int, Stk_LayerPtr>::iterator it = ((Stk_ModelImpl*)m_pImpl)->m_mapLayer.find(uiID);
	if (it != ((Stk_ModelImpl*)m_pImpl)->m_mapLayer.end())
	{
		pRetLayer = it->second;
	}
	else
	{
		map<int, Stk_LayerInfosSetPtr>::iterator it;
		for (it = ((Stk_ModelImpl*)m_pImpl)->m_mapLayerInfosSet.begin(); it != ((Stk_ModelImpl*)m_pImpl)->m_mapLayerInfosSet.end(); it++)
		{
			Stk_LayerInfosSetPtr layerInfosSetPtr = it->second;
			if (layerInfosSetPtr == nullptr)
				continue;
			Stk_LayerPtr pCurLayer = layerInfosSetPtr->GetLayerById(uiID);
			if (pCurLayer != nullptr)
			{
				pRetLayer = pCurLayer;
				((Stk_ModelImpl*)m_pImpl)->m_mapLayer.insert(make_pair(uiID, pRetLayer));
				break;
			}
		}
	}
	return pRetLayer;
}

HoteamSoft::SVLLib::Stk_FeatPtr Stk_Model::AddFeat(unsigned int uiID /*= -1*/)
{
	Stk_FeatPtr featPtr = nullptr;
	if (-1 != uiID) {
		map<int, Stk_FeatPtr>::iterator it;
		it = ((Stk_ModelImpl*)m_pImpl)->m_mapFeat.find(uiID);
		if (((Stk_ModelImpl*)m_pImpl)->m_mapFeat.end() == it)
		{
			featPtr = Stk_FeatPtr::CreateObject();
			featPtr->SetID(uiID);
			((Stk_ModelImpl*)m_pImpl)->m_mapFeat.insert(make_pair(featPtr->GetID(), featPtr));
		}
		else
		{
			featPtr = it->second;
		}
	}
	else {
		featPtr = Stk_FeatPtr::CreateObject();
		((Stk_ModelImpl*)m_pImpl)->m_mapFeat.insert(make_pair(featPtr->GetID(), featPtr));
	}
	//meshPtr->Init(pNodeImp->m_pDoc, GetID(),  - 1 != uiID ? false : true);
	return featPtr;
}

HoteamSoft::SVLLib::STK_STATUS Stk_Model::DeleteFeat(unsigned int uiID)
{
	map<int, Stk_FeatPtr>::iterator it;
	it = ((Stk_ModelImpl*)m_pImpl)->m_mapFeat.find(uiID);
	if (((Stk_ModelImpl*)m_pImpl)->m_mapFeat.end() == it)
	{
		((Stk_ModelImpl*)m_pImpl)->m_mapFeat.erase(it);
	}
	return STK_SUCCESS;
}

map<int, Stk_FeatPtr>& Stk_Model::GetAllFeat()
{
	return ((Stk_ModelImpl*)m_pImpl)->m_mapFeat;
}

	}//svllib
}//hoteamsoft
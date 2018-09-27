// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_Part.cpp
 *
****************************************************************************/
#include "Stk_ObjectImpl.h"
#include "Stk_Part.h"
#include "Stk_DocumentImpl.h"
#include "Stk_Document.h"
namespace HoteamSoft {
	namespace SVLLib {
class Stk_PartImpl : public Stk_ObjectImpl
{
	STK_CLASS_IMPL_DECLARE(Stk_Part)
public:
	Stk_PartImpl(void);
	virtual ~Stk_PartImpl(void);

	Stk_Document* m_pDoc;
	Stk_Document* GetDoc() { return m_pDoc; }
    
    Value*				m_pJsonPart;
	map<int, Stk_MeshPrimPtr> m_mapMeshIDToStkMesh;
};

STK_CLASS_IMPL_IMPLEMENT(Stk_Part)
Stk_PartImpl::Stk_PartImpl(void)
{

}

Stk_PartImpl::~Stk_PartImpl(void)
{
}


STK_IMPLEMENT_CLASS(Stk_Part, Stk_Object)

Stk_Part::Stk_Part(void):Stk_Object(new Stk_PartImpl())
{
}

Stk_Part::~Stk_Part(void)
{
	DELETE_STK_CLASS_IMPL
}

int Stk_Part::Add()
{
	int iRet = 0;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_PartImpl*)m_pImpl)->m_pDoc->m_pImpl;
	//ÃÌº”PartΩ⁄µ„
	Value valPartRoot;
	Value valPart;
	valPartRoot.SetObject();
	valPart.SetObject();
	if (!docImpl->m_nodeDoc.HasMember("Part"))
	{
		docImpl->m_nodeDoc.AddMember("Part", valPartRoot, docImpl->m_nodeDoc.GetAllocator());
	}
	Value::MemberIterator partRootIter = docImpl->m_nodeDoc.FindMember("Part");
	if (partRootIter != docImpl->m_nodeDoc.MemberEnd())
	{
		char charID[32];
		//itoa(this->GetID(), charID, 10);
		sprintf(charID,"%d",this->GetID());
		string strID = charID;
		valPart.AddMember("name", "default", docImpl->m_nodeDoc.GetAllocator());
		Value key(StringRef(strID.c_str()), docImpl->m_nodeDoc.GetAllocator());
		partRootIter->value.AddMember(key, valPart, docImpl->m_nodeDoc.GetAllocator());
		//Ω´JsonΩ⁄µ„∫ÕPart∂‘œÛ∞Û∂®
		Value::MemberIterator childNodeIter = partRootIter->value.MemberEnd();
		childNodeIter--;
		BindPart(&(childNodeIter->value));
	}
	return iRet;
}

HoteamSoft::SVLLib::Stk_MeshPrimPtr Stk_Part::AddMeshPrim(unsigned int uiID /*= -1*/)
{
	Value* pValPart = ((Stk_PartImpl*)m_pImpl)->m_pJsonPart;
	Stk_MeshPrimPtr meshPtr = Stk_MeshPrimPtr::CreateObject();
	if (-1 != uiID) {
		meshPtr->SetID(uiID);
	}
	else {
		Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_PartImpl*)m_pImpl)->m_pDoc->m_pImpl;
		docImpl->m_vecMeshes.push_back(meshPtr);
		//¥¥Ω®MeshΩ⁄µ„
		Value valMeshRoot;
		Value valMesh;
		valMeshRoot.SetObject();
		valMesh.SetObject();
		if (!pValPart->HasMember("Meshes"))
		{
			pValPart->AddMember("Meshes", valMeshRoot, docImpl->m_nodeDoc.GetAllocator());
		}
		Value::MemberIterator meshRootIter = pValPart->FindMember("Meshes");
		if (meshRootIter !=pValPart->MemberEnd())
		{
			char charID[32];
			//itoa(meshPtr->GetID(), charID, 10);
			sprintf(charID,"%d",meshPtr->GetID());
			string strID = charID;
			valMesh.AddMember("materialID", 1, docImpl->m_nodeDoc.GetAllocator());
			Value key(StringRef(strID.c_str()), docImpl->m_nodeDoc.GetAllocator());
			meshRootIter->value.AddMember(key, valMesh, docImpl->m_nodeDoc.GetAllocator());
			//∞Û∂®MeshΩ⁄µ„
			Value::MemberIterator meshIter = meshRootIter->value.MemberEnd();
			meshIter--;
			meshPtr->BindMesh(&meshIter->value);
		}
	}
	//meshPtr->Init(pNodeImp->m_pDoc, GetID(),  - 1 != uiID ? false : true);
	return meshPtr;
}

STK_STATUS Stk_Part::DelViewByID(unsigned int viewID /*= -1*/)
{
	return STK_SUCCESS;
}
unsigned int Stk_Part::GetNbMesh()
{
	int numMesh = 0;
	Value* valPart = ((Stk_PartImpl*)m_pImpl)->m_pJsonPart;
	Value::MemberIterator meshRootIter = valPart->FindMember("Meshes");
	if (meshRootIter !=valPart->MemberEnd())
	{
		numMesh = meshRootIter->value.MemberCount();
	}
	return numMesh;
}

Stk_MeshPrimPtr Stk_Part::GetMesh(int iIdx)
{
	Value* valPart = ((Stk_PartImpl*)m_pImpl)->m_pJsonPart;
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
	//≈–∂œ∏√Mesh∂‘œÛ±ª¥¥Ω®
	map<int, Stk_MeshPrimPtr>::iterator it;
	it = ((Stk_PartImpl*)m_pImpl)->m_mapMeshIDToStkMesh.find(meshID);
	if(it != ((Stk_PartImpl*)m_pImpl)->m_mapMeshIDToStkMesh.end())
		return it->second;
	//¥¥Ω®Mesh∂‘œÛ
	Stk_MeshPrimPtr meshPtr = Stk_MeshPrimPtr::CreateObject();
	meshPtr->SetID(meshID);
	meshPtr->Init(((Stk_PartImpl*)m_pImpl)->m_pDoc, this->GetID(), true);
	Value* valMesh = &(meshIter+iIdx)->value;
	meshPtr->BindMesh(valMesh);
	//ÃÌº”Map
	((Stk_PartImpl*)m_pImpl)->m_mapMeshIDToStkMesh.insert(make_pair(meshID, meshPtr));
	return meshPtr;
}

void Stk_Part::BindPart(void* i_bindedPart)
{
	((Stk_PartImpl*)m_pImpl)->m_pJsonPart = (Value*)i_bindedPart;
}

void Stk_Part::Init(Stk_Document* pDoc, unsigned int parentID, bool bForWrite/* = true*/)
{
	((Stk_PartImpl*)m_pImpl)->m_pDoc = pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	if (bForWrite) {
		//MeshPrim* meshPrim = docImpl->GetMeshPrim(this->GetID());
		//meshPrim->mutable_baseprim()->set_assemblyid(parentID);
	}else{
		//≥ı ºªØMesh–≈œ¢
		((Stk_PartImpl*)m_pImpl)->m_mapMeshIDToStkMesh.clear();
	}
}
	}//svllib
}//hoteamsoft

// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_ComponentFeature.cpp
 *
 *		2016/11/22	÷ÿππ°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_MeshPrim.h"
#include "Stk_ObjectImpl.h"
#include "Stk_DocumentImpl.h"
#include "Stk_Document.h"
#include "Stk_ResourceManager.h"
#include "Stk_MeshAccessor.h"
//#include "Stk_NodeImpl.h"

namespace HoteamSoft {
	namespace SVLLib {


class Stk_MeshPrimImpl : public Stk_ObjectImpl
        {
            STK_CLASS_IMPL_DECLARE(Stk_MeshPrim)
public:
	Stk_MeshPrimImpl(void);
	virtual ~Stk_MeshPrimImpl(void);

	Stk_Document* m_pDoc;
	Stk_Document* GetDoc() { return m_pDoc; }
	Value* m_pJsonMesh;
	bool m_bHasRead;
};

STK_CLASS_IMPL_IMPLEMENT(Stk_MeshPrim)
Stk_MeshPrimImpl::Stk_MeshPrimImpl(void)
{
	m_bHasRead = false;
	m_pDoc = nullptr;
}

Stk_MeshPrimImpl::~Stk_MeshPrimImpl(void)
{
}


STK_IMPLEMENT_CLASS(Stk_MeshPrim, Stk_Object)

Stk_MeshPrim::Stk_MeshPrim(void):Stk_Object(new Stk_MeshPrimImpl())
{
}

Stk_MeshPrim::~Stk_MeshPrim(void)
{
	DELETE_STK_CLASS_IMPL
	m_mapMeshFace.clear();
	m_mapMeshEdge.clear();
}

void Stk_MeshPrim::Init(Stk_Document* pDoc, STK_UINT32 parentID, bool bForWrite/* = true*/)
{
	((Stk_MeshPrimImpl*)m_pImpl)->m_pDoc = pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	if (bForWrite) {
		//MeshPrim* meshPrim = docImpl->GetMeshPrim(this->GetID());
		//meshPrim->mutable_baseprim()->set_assemblyid(parentID);
	}
}

void Stk_MeshPrim::SetQuaternion(const STK_FLOAT32& w, const STK_FLOAT32& x, const STK_FLOAT32& y, const STK_FLOAT32& z)
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_MeshPrimImpl*)m_pImpl)->m_pDoc->m_pImpl;
	//MeshPrim* meshPrim = docImpl->GetMeshPrim(this->GetID());
	//Quaternion* pQuat = meshPrim->mutable_baseprim()->mutable_transform()->mutable_quate();
	//‘∂≥Ã–¥»Î¥¶¿Ì
	/*pQuat->set_w(w);
	pQuat->set_x(x);
	pQuat->set_y(y);
	pQuat->set_z(z);*/
}

void Stk_MeshPrim::SetTranslation(const STK_FLOAT32& x, const STK_FLOAT32& y, const STK_FLOAT32& z)
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_MeshPrimImpl*)m_pImpl)->m_pDoc->m_pImpl;
	//MeshPrim* meshPrim = docImpl->GetMeshPrim(this->GetID());
	//Vec3f* pPostion = meshPrim->mutable_baseprim()->mutable_transform()->mutable_position();
	////‘∂≥Ã–¥»Î¥¶¿Ì
	//pPostion->set_x(x);
	//pPostion->set_y(y);
	//pPostion->set_z(z);
}

void Stk_MeshPrim::SetBoundBox(const STK_FLOAT32* minPnt, const STK_FLOAT32* maxPnt)
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)(((Stk_MeshPrimImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	//MeshPrim* meshPrim = docImpl->GetMeshPrim(this->GetID());
	//BoundBox* pBoundBox = meshPrim->mutable_baseprim()->mutable_boundbox();
	////‘∂≥Ã–¥»Î¥¶¿Ì
	//pBoundBox->mutable_min()->set_x(minPnt[0]);
	//pBoundBox->mutable_min()->set_y(minPnt[1]);
	//pBoundBox->mutable_min()->set_z(minPnt[2]);

	//pBoundBox->mutable_max()->set_x(maxPnt[0]);
	//pBoundBox->mutable_max()->set_y(maxPnt[1]);
	//pBoundBox->mutable_max()->set_z(maxPnt[2]);
}

void Stk_MeshPrim::SetMeshBuf(const std::vector<STK_FLOAT32>& vecVert, 
													const std::vector<STK_FLOAT32>& vecNormals, 
													const std::vector<STK_UINT32>& vecTrians)
{
	if (0 == vecVert.size() || 0 == vecNormals.size() || 0 == vecTrians.size() || vecVert.size() != vecNormals.size()) {
		return;
	}

	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)(((Stk_MeshPrimImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	/*MeshPrim* meshPrim = docImpl->GetMeshPrim(this->GetID());
	Buffer* meshBuf = docImpl->GetRVMBuffer(0);
	if (!meshBuf) 
	{
	meshBuf = docImpl->AddRVMBuffer();
	meshBuf->set_uiqid(Stk_Object::objectId());
	}

	std::vector<STK_FLOAT32>::const_iterator fItor;

	Accessor* pVertAcc = meshPrim->mutable_accvert();
	pVertAcc->set_uiqid(Stk_Object::objectId());
	pVertAcc->set_bufurn(docImpl->GetBinNameUTF8());
	pVertAcc->set_type(Accessor_Type_VERT);
	pVertAcc->set_offset(meshBuf->vertex_size());
	pVertAcc->set_length(vecVert.size());
	pVertAcc->set_bufferid(meshBuf->uiqid());

	fItor = vecVert.begin();
	for (; fItor != vecVert.end(); fItor++) {
	meshBuf->add_vertex(*fItor);
	}

	Accessor* pNorAcc = meshPrim->mutable_accnor();
	pNorAcc->set_uiqid(Stk_Object::objectId());
	pNorAcc->set_bufurn(docImpl->GetBinNameUTF8());
	pNorAcc->set_type(Accessor_Type_NORM);
	pNorAcc->set_offset(meshBuf->normals_size());
	pNorAcc->set_length(vecNormals.size());
	pNorAcc->set_bufferid(meshBuf->uiqid());

	fItor = vecNormals.begin();
	for (; fItor != vecNormals.end(); fItor++) {
	meshBuf->add_normals(*fItor);
	}

	Accessor* pTriaAcc = meshPrim->mutable_acctria();
	pTriaAcc->set_uiqid(Stk_Object::objectId());
	pTriaAcc->set_bufurn(docImpl->GetBinNameUTF8());
	pTriaAcc->set_type(Accessor_Type_TRIAN);
	pTriaAcc->set_offset(meshBuf->trianidx_size());
	pTriaAcc->set_length(vecTrians.size());
	pTriaAcc->set_bufferid(meshBuf->uiqid());

	std::vector<STK_UINT32>::const_iterator uiItor = vecTrians.cbegin();
	for (;  uiItor != vecTrians.end(); uiItor++) {
	meshBuf->add_trianidx(*uiItor);
	}*/
}

STK_INT32 Stk_MeshPrim::GetQuaternion(STK_FLOAT32& w, STK_FLOAT32& x, STK_FLOAT32& y, STK_FLOAT32& z)
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)(((Stk_MeshPrimImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	/*MeshPrim* meshPrim = docImpl->GetMeshPrim(this->GetID());
	if (!meshPrim) {
		return -1;
	}

	const Quaternion& qua = meshPrim->baseprim().transform().quate();
	w = qua.w();
	x = qua.x();
	y = qua.y();
	z = qua.z();*/
	return 0;
}

STK_INT32 Stk_MeshPrim::GetTranslation(STK_FLOAT32& x, STK_FLOAT32& y, STK_FLOAT32& z)
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)(((Stk_MeshPrimImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	/*MeshPrim* meshPrim = docImpl->GetMeshPrim(this->GetID());
	if (!meshPrim) {
		return -1;
	}
	const Vec3f& pnt = meshPrim->baseprim().transform().position();
	x = pnt.x();
	y = pnt.y();
	z = pnt.z();*/
	return 0;
}

void Stk_MeshPrim::SetVertexs(std::vector<STK_FLOAT32>& vecVert)
{
	this->m_vecVert.swap(vecVert);
}

void Stk_MeshPrim::SetNormals(std::vector<STK_FLOAT32>& vecNormals)
{
	this->m_vecNormals.swap(vecNormals);
}

void Stk_MeshPrim::SetTextuercoordinate(std::vector<STK_FLOAT32>& vecTextureCoordinate)
{
	this->m_vecTextureCoordinate.swap(vecTextureCoordinate);
}

void Stk_MeshPrim::SetIndex(std::vector<STK_UINT32>& vecTrians)
{
	this->m_vecTrians.swap(vecTrians);
}

std::vector<STK_FLOAT32>& Stk_MeshPrim::GetVertexs()
{
	return this->m_vecVert;
}
std::vector<STK_FLOAT32>& Stk_MeshPrim::GetNormals()
{
	return this->m_vecNormals;
}
std::vector<STK_FLOAT32>& Stk_MeshPrim::GetTextuercoordinate()
{
	return this->m_vecTextureCoordinate;
}
std::vector<STK_UINT32>& Stk_MeshPrim::GetIndex()
{
	return this->m_vecTrians;
}

void Stk_MeshPrim::BindMesh(void* i_bindedMesh)
{
	((Stk_MeshPrimImpl*)m_pImpl)->m_pJsonMesh = (Value*)i_bindedMesh;
}

void* Stk_MeshPrim::GetBindedData()
{
	return ((Stk_MeshPrimImpl*)m_pImpl)->m_pJsonMesh;
}

Stk_MeshFacePtr Stk_MeshPrim::AddMeshFace(STK_UINT32 uiID/* = -1*/)
{
	Stk_DocumentImpl* docImpl;
	if (((Stk_MeshPrimImpl*)m_pImpl)->m_pDoc)
	{
		docImpl = (Stk_DocumentImpl*)(((Stk_MeshPrimImpl*)m_pImpl)->m_pDoc)->m_pImpl; 
	}
	else
	{
		docImpl = nullptr;
	}

	if (-1 != uiID)
	{
		map<STK_INT32, Stk_MeshFacePtr>::iterator it;
		it = m_mapMeshFace.find(uiID);
		if (m_mapMeshFace.end() != it)
		{
			return it->second;
		}
	}
	
	Stk_MeshFacePtr stkMeshFace = Stk_MeshFacePtr::CreateObject();
	if (-1 != uiID)
	{
		stkMeshFace->SetID(uiID);
	}
	stkMeshFace->Init(((Stk_MeshPrimImpl*)m_pImpl)->m_pDoc, uiID);
	//Ω´MeshFaceµƒ∂‘œÛÃÌº”µΩMap÷–
	m_mapMeshFace.insert(make_pair(stkMeshFace->GetID(), stkMeshFace));
	if(docImpl)
		docImpl->m_meshFaceCount++;
	return stkMeshFace;
}

STK_INT32 Stk_MeshPrim::GetNbMeshFace()
{
	return m_mapMeshFace.size();
}

std::map<STK_INT32, Stk_MeshFacePtr>& Stk_MeshPrim::GetMeshFaces( )
{
	return m_mapMeshFace;
}

Stk_MeshFacePtr Stk_MeshPrim::GetMeshFaceByIdx(STK_INT32 i_idx)
{
	map<STK_INT32, Stk_MeshFacePtr>::iterator it;
	it = m_mapMeshFace.begin();
	for (int i = 0; i < i_idx; i++)
	{
		it++;
	}
	return it->second;
}

Stk_MeshEdgePtr Stk_MeshPrim::AddMeshEdge(STK_UINT32 uiID /*= -1*/)
{
	Stk_DocumentImpl* docImpl  = nullptr;
	if (((Stk_MeshPrimImpl*)m_pImpl)->m_pDoc)
	{
		docImpl = (Stk_DocumentImpl*)(((Stk_MeshPrimImpl*)m_pImpl)->m_pDoc)->m_pImpl; 
	}
	else
	{
		docImpl = nullptr;
	}
	if (-1 != uiID)
	{
		map<STK_INT32, Stk_MeshEdgePtr>::iterator it;
		it = m_mapMeshEdge.find(uiID);
		if (m_mapMeshEdge.end() != it)
		{
			return it->second;
		}
	}

	Stk_MeshEdgePtr stkMeshEdge = Stk_MeshEdgePtr::CreateObject();
	if (-1 != uiID)
	{
		stkMeshEdge->SetID(uiID);
	}
	stkMeshEdge->Init(((Stk_MeshPrimImpl*)m_pImpl)->m_pDoc, uiID);
	//Ω´MeshEdge∂‘œÛÃÌº”µΩMap÷–
	m_mapMeshEdge.insert(make_pair(stkMeshEdge->GetID(), stkMeshEdge));
	if(docImpl)
		docImpl->m_meshEdgeCount++;

	return stkMeshEdge;
}

std::map<STK_INT32, Stk_MeshEdgePtr>& Stk_MeshPrim::GetMeshEdges( )
{
	return m_mapMeshEdge;
}

STK_INT32 Stk_MeshPrim::GetNbMeshEdge()
{
	return m_mapMeshEdge.size();
}

Stk_MeshEdgePtr Stk_MeshPrim::GetMeshEdgeByIdx(STK_INT32 i_idx)
{
	map<STK_INT32, Stk_MeshEdgePtr>::iterator it;
	it = m_mapMeshEdge.begin();
	for (int i = 0; i < i_idx; i++)
	{
		it++;
	}
	return it->second;
}
 
Stk_MeshEdgePtr Stk_MeshPrim::GetMeshEdgeByID(STK_INT32 meshEdgeID)
{
	map<STK_INT32, Stk_MeshEdgePtr>::iterator it;
	it = m_mapMeshEdge.find(meshEdgeID);
	if (it == m_mapMeshEdge.end())
		return NULL;
	return it->second;
}

void Stk_MeshPrim::WriteToBlock(StkMeshBlock& meshBlock)
{
	meshBlock.meshID = this->GetID();
	meshBlock.numEdge = this->GetNbMeshEdge();
	meshBlock.numFace = this->GetNbMeshFace();
	meshBlock.numVertex = m_vecVert.size();
	meshBlock.numNormal = m_vecNormals.size();
	meshBlock.numUV = m_vecTextureCoordinate.size();
	meshBlock.numPadding = 0;
}

void Stk_MeshPrim::ReadFromBlock(const StkMeshBlock meshBlock)
{

}

void Stk_MeshPrim::Clear()
{
	m_vecVert.clear();
	m_vecNormals.clear();
	m_vecTextureCoordinate.clear();
	m_vecTrians.clear();
	m_mapMeshFace.clear();
	m_mapMeshEdge.clear();

	m_vecVert.shrink_to_fit();
	m_vecNormals.shrink_to_fit();
	m_vecTextureCoordinate.shrink_to_fit();
	m_vecTrians.shrink_to_fit();
}

STK_STATUS Stk_MeshPrim::GetMeshBuf(std::vector<STK_FLOAT32>& vecVert, std::vector<STK_FLOAT32>& vecNormals, std::vector<STK_UINT32>& vecTrians)
{
	//≈–∂œ «∑Ò“—æ≠∂¡»°
	if(((Stk_MeshPrimImpl*)m_pImpl)->m_bHasRead)
		return STK_SUCCESS;
	//ªÒ»°Œƒº˛√˚≥∆
	Value*valMesh = ((Stk_MeshPrimImpl*)m_pImpl)->m_pJsonMesh;
	Value::MemberIterator accessorIter = valMesh->FindMember("Accesor");
	if (accessorIter == valMesh->MemberEnd())
		return STK_ERROR;
	//string GeoFileName = "Geom_01.bin";
	string GeoFileName = accessorIter->value.FindMember("geomFile")->value.GetString();
	//ªÒ»°◊ ‘¥π‹¿Ì∆˜
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)(((Stk_MeshPrimImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	Stk_FilePtr stk_filePtr = docImpl->m_resourceManagerPtr->GetFile(GeoFileName);
	stk_filePtr->OpenAsBin("rb"); 
    Stk_MeshAccessorPtr stk_MeshAccessorPtr = Stk_MeshAccessorPtr::CreateObject();
	stk_MeshAccessorPtr->Uncode(accessorIter->value);
	Stk_MeshPrimPtr mesh(this);
	stk_MeshAccessorPtr->ReadFile(mesh,stk_filePtr);
	vecVert.assign(this->m_vecVert.begin(), this->m_vecVert.end());
	vecNormals.assign(this->m_vecNormals.begin(), this->m_vecNormals.end());
	m_vecTextureCoordinate.assign(this->m_vecTextureCoordinate.begin(), this->m_vecTextureCoordinate.end());
	vecTrians.assign(this->m_vecTrians.begin(), this->m_vecTrians.end());
	//Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)(((Stk_MeshPrimImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	/*MeshPrim* meshPrim = docImpl->GetMeshPrim(this->GetID());
	if (!meshPrim || !meshPrim->has_accvert() || !meshPrim->has_accnor() || !meshPrim->has_acctria()) {
	return STK_ERROR;
	}


	std::vector<Accessor> vecAccessor;
	vecAccessor.push_back(meshPrim->accvert());
	vecAccessor.push_back(meshPrim->accnor());
	vecAccessor.push_back(meshPrim->acctria());

	Buffer meshBuf;
	STK_STATUS ststus = (STK_STATUS)docImpl->GetRVMBuffer(vecAccessor, meshBuf);
	if (STK_SUCCESS !=ststus) {
	return STK_ERROR_FILERD;
	}

	vecVert.clear();
	vecNormals.clear();
	vecTrians.clear();
	STK_INT32 iSize = meshBuf.vertex_size();
	vecVert.resize(iSize, 0.0);
	memcpy_s(&vecVert[0], iSize * sizeof(STK_FLOAT32), meshBuf.vertex().data(), iSize * sizeof(STK_FLOAT32));

	iSize = meshBuf.normals_size();
	vecNormals.resize(iSize, 0.0);
	memcpy_s(&vecNormals[0], iSize * sizeof(STK_FLOAT32), meshBuf.normals().data(), iSize * sizeof(STK_FLOAT32));

	iSize = meshBuf.trianidx_size();
	vecTrians.resize(iSize, 0.0);
	memcpy_s(&vecTrians[0], iSize * sizeof(STK_UINT32), meshBuf.trianidx().data(), iSize * sizeof(STK_UINT32));*/

	return STK_SUCCESS;
}
	}//svllib
}//hoteamsoft

// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_ComponentFeature.cpp
 *
 *		2016/11/22	÷ÿππ°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_MeshFace.h"
#include "Stk_ObjectImpl.h"
#include "Stk_DocumentImpl.h"
#include "Stk_Document.h"
#include "Stk_ResourceManager.h"
#include "Stk_MeshAccessor.h"
#include "Stk_MeshAttribute.h"
#include "Stk_GeometryAttribute.h"
#include "ChineseCode.h"
#include "StringTools.h"

namespace HoteamSoft {
	namespace SVLLib {
class Stk_MeshFaceImpl : public Stk_ObjectImpl
        {
            STK_CLASS_IMPL_DECLARE(Stk_MeshFace)
public:
	Stk_Document* m_pDoc;
	Stk_Document* GetDoc() { return m_pDoc; }
public:
	Stk_MeshFaceImpl(void);
	virtual ~Stk_MeshFaceImpl(void);
	map<int, int> m_mapMeshEdgeID;
};

STK_CLASS_IMPL_IMPLEMENT(Stk_MeshFace)
Stk_MeshFaceImpl::Stk_MeshFaceImpl(void)
{
	m_pDoc = NULL;
}

Stk_MeshFaceImpl::~Stk_MeshFaceImpl(void)
{
}


STK_IMPLEMENT_CLASS(Stk_MeshFace, Stk_Object)

Stk_MeshFace::Stk_MeshFace(void):Stk_Object(new Stk_MeshFaceImpl())
{
}

Stk_MeshFace::~Stk_MeshFace(void)
{
	DELETE_STK_CLASS_IMPL
	map<int, int> tmpMap;
	m_mapMeshEdgeID.swap(tmpMap);
	m_mapMeshEdgeID.clear();
	m_vecTrians.shrink_to_fit();
	m_vecTrians.clear();
}

void Stk_MeshFace::SetIndex(std::vector<unsigned int>& vecTrians)
{
	Stk_DocumentImpl* docImpl = nullptr;
	if (((Stk_MeshFaceImpl*)m_pImpl)->m_pDoc)
	{
		docImpl = (Stk_DocumentImpl*)(((Stk_MeshFaceImpl*)m_pImpl)->m_pDoc)->m_pImpl; 
	}
	else
	{
		docImpl = nullptr;
	}
	if(docImpl)
		docImpl->m_triangleCount += vecTrians.size()/3;
	this->m_vecTrians.swap(vecTrians);
}

STK_STATUS Stk_MeshFace::GetIndex(std::vector<unsigned int>& vecTrians)
{
	vecTrians.assign(this->m_vecTrians.begin(), this->m_vecTrians.end());
	return STK_SUCCESS;
}

void Stk_MeshFace::AddMeshEdgeID(STK_UINT32 i_meshEdgeID)
{
	map<int, int>::iterator it;
	it = m_mapMeshEdgeID.find(i_meshEdgeID);
	if (m_mapMeshEdgeID.end() == it)
		m_mapMeshEdgeID.insert(make_pair(i_meshEdgeID, i_meshEdgeID));
}

STK_STATUS Stk_MeshFace::GetEdges(std::vector<int>& vecMeshEdgeID)
{
	map<int, int>::iterator it;
	for(  it=m_mapMeshEdgeID.begin(); it!=m_mapMeshEdgeID.end(); it++)
	{
		vecMeshEdgeID.push_back(it->first);
	}
	return STK_SUCCESS;
}

void Stk_MeshFace::WriteToBlock(StkMeshFaceBlock& meshFaceBlock)
{
	meshFaceBlock.meshFaceID = this->GetID();
	meshFaceBlock.materialID = m_materialID;
	meshFaceBlock.numEdge = m_mapMeshEdgeID.size();
	meshFaceBlock.numIndex = m_vecTrians.size();
}

void Stk_MeshFace::ReadFromBlock(const StkMeshFaceBlock meshFaceBlock)
{

}

void Stk_MeshFace::Clear()
{
	m_vecTrians.clear();
	m_mapMeshEdgeID.clear();
	m_vecTrians.shrink_to_fit();
}

void Stk_MeshFace::Init(Stk_Document* pDoc, unsigned int parentID, bool bForWrite/* = true*/)
{
	((Stk_MeshFaceImpl*)m_pImpl)->m_pDoc = pDoc;
}

Stk_MaterialPtr Stk_MeshFace::GetMaterialByID(unsigned int i_materialID)
{
	if (!((Stk_MeshFaceImpl*)m_pImpl)->m_pDoc)
		return nullptr;
	return ((Stk_MeshFaceImpl*)m_pImpl)->m_pDoc->GetMaterialByID(i_materialID);
}
	}//svllib
}//hoteamsoft

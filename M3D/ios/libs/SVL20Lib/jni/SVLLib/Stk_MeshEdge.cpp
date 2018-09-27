// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_ComponentFeature.cpp
 *
 *		2016/11/22	÷ÿππ°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_MeshEdge.h"
#include "Stk_ObjectImpl.h"
#include "Stk_DocumentImpl.h"
#include "Stk_Document.h"
#include "Stk_ResourceManager.h"
#include "Stk_MeshAccessor.h"
#include "Stk_MeshAttribute.h"
#include "ChineseCode.h"
#include "StringTools.h"
#include "Stk_GeometryAttribute.h"
#include "JsonMacro.h"

namespace HoteamSoft {
	namespace SVLLib {
class Stk_MeshEdgeImpl : public Stk_ObjectImpl
{
	STK_CLASS_IMPL_DECLARE(Stk_MeshEdge)

public:
	Stk_Document* m_pDoc;
	Stk_Document* GetDoc() { return m_pDoc; }
public:
	Stk_MeshEdgeImpl(void);
	virtual ~Stk_MeshEdgeImpl(void);

};

STK_CLASS_IMPL_IMPLEMENT(Stk_MeshEdge)
Stk_MeshEdgeImpl::Stk_MeshEdgeImpl(void)
{
	m_pDoc = NULL;
}

Stk_MeshEdgeImpl::~Stk_MeshEdgeImpl(void)
{
}


STK_IMPLEMENT_CLASS(Stk_MeshEdge, Stk_Object)

Stk_MeshEdge::Stk_MeshEdge(void):Stk_Object(new Stk_MeshEdgeImpl())
{
}

Stk_MeshEdge::~Stk_MeshEdge(void)
{
	DELETE_STK_CLASS_IMPL
	m_vecEdgeIndex.clear();
}

void Stk_MeshEdge::SetIndex(std::vector<unsigned int>& vecEdgeIndex)
{
	this->m_vecEdgeIndex.swap(vecEdgeIndex);
}

STK_STATUS Stk_MeshEdge::GetIndex(std::vector<unsigned int>& vecEdgeIndex)
{
	vecEdgeIndex.assign(this->m_vecEdgeIndex.begin(), this->m_vecEdgeIndex.end());
	return STK_SUCCESS;
}

void Stk_MeshEdge::WriteToBlock(StkMeshEdgeBlock& meshEdgeBlock)
{
	meshEdgeBlock.meshEdgeID = this->GetID();
	meshEdgeBlock.numIndex = m_vecEdgeIndex.size();
}

void Stk_MeshEdge::ReadFromBlock(const StkMeshEdgeBlock meshEdgeBlock)
{

}

void Stk_MeshEdge::Clear()
{
	m_vecEdgeIndex.clear();
}

void Stk_MeshEdge::Init(Stk_Document* pDoc, unsigned int parentID, bool bForWrite/* = true*/)
{
	((Stk_MeshEdgeImpl*)m_pImpl)->m_pDoc = pDoc;
}

map<wstring, wstring> Stk_MeshEdge::GetAllAttribute()
{
	map<wstring, wstring> mapAttribute;
	wstring wstrName = L"";
	if (!((Stk_MeshEdgeImpl*)m_pImpl)->m_pDoc || !(((Stk_MeshEdgeImpl*)m_pImpl)->m_pDoc)->m_pImpl)
		return mapAttribute;
	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_MeshEdgeImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	if (pDocImpl->m_pAttrDoc == nullptr)
		return mapAttribute;
	//mapAttribute = pDocImpl->m_pAttrDoc->GetMeshEdgeAllAttribute(GetID());
	return mapAttribute;
}

Stk_GeometryAttributePtr  Stk_MeshEdge::GetAttribute()
{
	Stk_GeometryAttributePtr pGeoAttr = nullptr;
	if (!((Stk_MeshEdgeImpl*)m_pImpl)->m_pDoc || !(((Stk_MeshEdgeImpl*)m_pImpl)->m_pDoc)->m_pImpl)
		return pGeoAttr;
	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_MeshEdgeImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	if (pDocImpl->m_pAttrDoc == nullptr)
		return pGeoAttr;
	wstring wstrValue;
	string strValue;
	vector<std::string> vecValue;
#if 0
	wstring wstrGeoType = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_TYPE);
	if (wcscmp(wstrGeoType.c_str(), L"1") == 0)
	{
		pGeoAttr = Stk_PlaneFaceAttributePtr::CreateObject();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_NORMAL);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_DIR32 dirNormal;
		dirNormal.X = atof(vecValue[0].c_str());
		dirNormal.Y = atof(vecValue[1].c_str());
		dirNormal.Z = atof(vecValue[2].c_str());
		Stk_PlaneFaceAttributePtr::DynamicCast(pGeoAttr)->SetNormal(dirNormal);
		vecValue.clear();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_ORIGIN);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_PNT32 pntOrigin;
		pntOrigin.X = atof(vecValue[0].c_str());
		pntOrigin.Y = atof(vecValue[1].c_str());
		pntOrigin.Z = atof(vecValue[2].c_str());
		Stk_PlaneFaceAttributePtr::DynamicCast(pGeoAttr)->SetOrigin(pntOrigin);
	}
	else if (wcscmp(wstrGeoType.c_str(), L"2") == 0 || wcscmp(wstrGeoType.c_str(), L"3") == 0
		|| wcscmp(wstrGeoType.c_str(), L"4") == 0 || wcscmp(wstrGeoType.c_str(), L"5") == 0)
	{
		pGeoAttr = Stk_RevolutionFaceAttributePtr::CreateObject();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_AXISORIGIN);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_PNT32 pntAxisOrigin;
		pntAxisOrigin.X = atof(vecValue[0].c_str());
		pntAxisOrigin.Y = atof(vecValue[1].c_str());
		pntAxisOrigin.Z = atof(vecValue[2].c_str());
		Stk_RevolutionFaceAttributePtr::DynamicCast(pGeoAttr)->SetAxisOrigin(pntAxisOrigin);
		vecValue.clear();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_AXISDIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_DIR32 dirRevoAxis;
		dirRevoAxis.X = atof(vecValue[0].c_str());
		dirRevoAxis.Y = atof(vecValue[1].c_str());
		dirRevoAxis.Z = atof(vecValue[2].c_str());
		Stk_RevolutionFaceAttributePtr::DynamicCast(pGeoAttr)->SetRevoAxis(dirRevoAxis);
		vecValue.clear();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_RADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_RevolutionFaceAttributePtr::DynamicCast(pGeoAttr)->SetOriginRadius(atof(strValue.c_str()));
	}
	else if (wcscmp(wstrGeoType.c_str(), L"6") == 0)
	{
		pGeoAttr = Stk_ToroidalFaceAttributePtr::CreateObject();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_AXISORIGIN);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_PNT32 pntAxisOrigin;
		pntAxisOrigin.X = atof(vecValue[0].c_str());
		pntAxisOrigin.Y = atof(vecValue[1].c_str());
		pntAxisOrigin.Z = atof(vecValue[2].c_str());
		Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->SetAxisOrigin(pntAxisOrigin);
		vecValue.clear();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_AXISDIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_DIR32 dirRevoAxis;
		dirRevoAxis.X = atof(vecValue[0].c_str());
		dirRevoAxis.Y = atof(vecValue[1].c_str());
		dirRevoAxis.Z = atof(vecValue[2].c_str());
		Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->SetRevoAxis(dirRevoAxis);
		vecValue.clear();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_ORIGINRADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->SetOriginRadius(atof(strValue.c_str()));
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_MINORRADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->SetMinorRadius(atof(strValue.c_str()));
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_MAJORRADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->SetMajorRaius(atof(strValue.c_str()));
	}
	else if (wcscmp(wstrGeoType.c_str(), L"7") == 0)
	{
		pGeoAttr = Stk_LineAttributePtr::CreateObject();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_CENTEREPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_PNT32 pntCenterPoint, pntStartPoint, pntEndPoint;
		pntCenterPoint.X = atof(vecValue[0].c_str());
		pntCenterPoint.Y = atof(vecValue[1].c_str());
		pntCenterPoint.Z = atof(vecValue[2].c_str());
		Stk_LineAttributePtr::DynamicCast(pGeoAttr)->SetCenterPoint(pntCenterPoint);
		vecValue.clear();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_DIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_DIR32 dirDirection;
		dirDirection.X = atof(vecValue[0].c_str());
		dirDirection.Y = atof(vecValue[1].c_str());
		dirDirection.Z = atof(vecValue[2].c_str());
		Stk_LineAttributePtr::DynamicCast(pGeoAttr)->SetDirection(dirDirection);
		vecValue.clear();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_STARTPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		pntStartPoint.X = atof(vecValue[0].c_str());
		pntStartPoint.Y = atof(vecValue[1].c_str());
		pntStartPoint.Z = atof(vecValue[2].c_str());
		Stk_LineAttributePtr::DynamicCast(pGeoAttr)->SetStartPoint(pntStartPoint);
		vecValue.clear();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_ENDPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		pntEndPoint.X = atof(vecValue[0].c_str());
		pntEndPoint.Y = atof(vecValue[1].c_str());
		pntEndPoint.Z = atof(vecValue[2].c_str());
		Stk_LineAttributePtr::DynamicCast(pGeoAttr)->SetStartPoint(pntEndPoint);
		vecValue.clear();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_LENGTH);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_LineAttributePtr::DynamicCast(pGeoAttr)->SetLength(atof(strValue.c_str()));
	}
	if (wcscmp(wstrGeoType.c_str(), L"8") == 0)
	{
		pGeoAttr = Stk_EllipseAttributePtr::CreateObject();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_CENTEREPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_PNT32 pntCenterPoint, pntStartPoint, pntEndPoint;
		pntCenterPoint.X = atof(vecValue[0].c_str());
		pntCenterPoint.Y = atof(vecValue[1].c_str());
		pntCenterPoint.Z = atof(vecValue[2].c_str());
		Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetCenterPoint(pntCenterPoint);
		vecValue.clear();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_STARTPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		pntStartPoint.X = atof(vecValue[0].c_str());
		pntStartPoint.Y = atof(vecValue[1].c_str());
		pntStartPoint.Z = atof(vecValue[2].c_str());
		Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetStartPoint(pntStartPoint);
		vecValue.clear();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_ENDPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		pntEndPoint.X = atof(vecValue[0].c_str());
		pntEndPoint.Y = atof(vecValue[1].c_str());
		pntEndPoint.Z = atof(vecValue[2].c_str());
		Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetStartPoint(pntEndPoint);
		vecValue.clear();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_MINORRADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetMinorRadius(atof(strValue.c_str()));
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_MAJORRADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetMajorRadius(atof(strValue.c_str()));
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_XDIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_DIR32 xDirection, yDirection, zDirection;
		xDirection.X = atof(vecValue[0].c_str());
		xDirection.Y = atof(vecValue[1].c_str());
		xDirection.Z = atof(vecValue[2].c_str());
		vecValue.clear();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_YDIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		yDirection.X = atof(vecValue[0].c_str());
		yDirection.Y = atof(vecValue[1].c_str());
		yDirection.Z = atof(vecValue[2].c_str());
		vecValue.clear();
		wstrValue = pDocImpl->m_pAttrDoc->GetMeshFaceAttribute(GetID(), MESH_ZDIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		zDirection.X = atof(vecValue[0].c_str());
		zDirection.Y = atof(vecValue[1].c_str());
		zDirection.Z = atof(vecValue[2].c_str());
		Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetXYZDir(xDirection, yDirection, zDirection);
	}
#endif
	return pGeoAttr;
}

//STK_STATUS Stk_MeshEdge::AddAttribute(const std::wstring& i_wstrAttrName, const std::wstring& i_wstrAttrValue)
//{
//	if (!((Stk_MeshEdgeImpl*)m_pImpl)->m_pDoc || !(((Stk_MeshEdgeImpl*)m_pImpl)->m_pDoc)->m_pImpl)
//		return STK_ERROR;
//	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_MeshEdgeImpl*)m_pImpl)->m_pDoc)->m_pImpl;
//	if (pDocImpl->m_pAttrDoc == nullptr)
//		return STK_ERROR;
//	return pDocImpl->m_pAttrDoc->AddMeshEdgeAttribute(GetID(), i_wstrAttrName, i_wstrAttrValue);
//}
//
//wstring  Stk_MeshEdge::GetAttribute(const std::wstring& i_wstrAttrName)
//{
//	wstring wstrName = L"";
//	if (!((Stk_MeshEdgeImpl*)m_pImpl)->m_pDoc || !(((Stk_MeshEdgeImpl*)m_pImpl)->m_pDoc)->m_pImpl)
//		return wstrName;
//	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_MeshEdgeImpl*)m_pImpl)->m_pDoc)->m_pImpl;
//	if (pDocImpl->m_pAttrDoc == nullptr)
//		return wstrName;
//	wstrName = pDocImpl->m_pAttrDoc->GetMeshEdgeAttribute(GetID(), i_wstrAttrName);
//
//	return wstrName;
//}
	}//svllib
}//hoteamsoft
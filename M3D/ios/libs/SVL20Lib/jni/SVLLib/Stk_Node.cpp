// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_ComponentFeature.cpp
 *
 *		2016/11/22	÷ÿππ°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_Node.h"
#include "Stk_NodeImpl.h"
#include "Stk_DocumentImpl.h"
#include "ChineseCode.h"
#include "Stk_API.h"
#ifdef WIN32
#include <windows.h>
#endif
#include "StringTools.h"
using namespace std;

namespace HoteamSoft {
	namespace SVLLib {
        
        STK_IMPLEMENT_CLASS(Stk_Node, Stk_Object)

void Stk_Node::Init(Stk_Document* pDoc, unsigned int parentID, bool bForWrite/* = true*/)
{
	((Stk_NodeImpl*)m_pImpl)->m_pDoc = pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	if (bForWrite) {
	}
}

Stk_Node::Stk_Node(void):Stk_Object(new Stk_NodeImpl())
{
}

Stk_Node::~Stk_Node(void)
{
	DELETE_STK_CLASS_IMPL
}

Stk_NodePtr Stk_Node::AddChild(unsigned int uiID/* = -1*/)
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_NodeImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_NodeImpl* pNodeImp = (Stk_NodeImpl*)m_pImpl;
	//≈–∂œ◊”Ω⁄µ„∂‘œÛ «∑Ò±ªÃÌº”
	map<int, Stk_NodePtr>::iterator it;
	it = docImpl->m_mapIdToNode.find(uiID);
	if (it != docImpl->m_mapIdToNode.end())
	{
		return it->second;
	}
	Stk_NodePtr childNode = Stk_NodePtr::CreateObject();
	if (-1 != uiID) {
		childNode->SetID(uiID);
	}
	else {
		//Value* valueNode = docImpl->GetNode(this->GetID());
		Value* valueNode = ((Stk_NodeImpl*)m_pImpl)->m_pJsonNode;
		if (!valueNode)
			return NULL;
		//…Ë÷√µ±«∞NodeΩ⁄µ„µƒ◊”Ω⁄µ„–≈œ¢
		if (!valueNode->HasMember("Children"))
		{
			Value valChildren;
			valChildren.SetArray();
			valueNode->AddMember("Children", valChildren, docImpl->m_nodeDoc.GetAllocator());
		}
		Value::MemberIterator childrenNodeIter = valueNode->FindMember("Children");
		if ((childrenNodeIter!= valueNode->MemberEnd() && childrenNodeIter->value.IsArray()))
		{
			childrenNodeIter->value.PushBack(childNode->GetID(), docImpl->m_nodeDoc.GetAllocator());
		}
		//¥¥Ω®∫¢◊”Ω⁄µ„
		Value::MemberIterator nodeIter = docImpl->m_nodeDoc.FindMember("Nodes");
		if ((nodeIter!= docImpl->m_nodeDoc.MemberEnd() && nodeIter->value.IsObject()))
		{
			char charID[32];
			//itoa(childNode->GetID(), charID, 10);
			sprintf(charID,"%d",childNode->GetID());
			string strID = charID;
			Value valChildNode;
			valChildNode.SetObject();
			//ÃÌº”√˚≥∆¿‡–Õ∏∏ID–≈œ¢
			valChildNode.AddMember("name", "default", docImpl->m_nodeDoc.GetAllocator());
			valChildNode.AddMember("type", 0, docImpl->m_nodeDoc.GetAllocator());
			//valChildNode.AddMember("parentID", this->GetID(), docImpl->m_nodeDoc.GetAllocator());
			Value key(StringRef(strID), docImpl->m_nodeDoc.GetAllocator());
			nodeIter->value.AddMember(key, valChildNode, docImpl->m_nodeDoc.GetAllocator());
			//Ω´JsonΩ⁄µ„∫ÕNodeΩ⁄µ„∞Û∂®
			Value::MemberIterator childNodeIter = nodeIter->value.MemberEnd();
			childNodeIter--;
			childNode->BindingNode(&(childNodeIter->value));
		}
		//ÃÌº”Map
		docImpl->m_mapIdToNode.insert(make_pair(childNode->GetID(), childNode));
	}
	childNode->Init(pNodeImp->m_pDoc, GetID(), -1 != uiID ? false : true);
	return childNode;
}

void Stk_Node::SetNodeName( const std::wstring& i_wstrNodeName )
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_NodeImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	string nodeName;
	ChineseCode::UnicodeToUTF_8(i_wstrNodeName.c_str(), nodeName);
	//Value* valueNode = docImpl->GetNode(this->GetID());
	Value* valueNode = ((Stk_NodeImpl*)m_pImpl)->m_pJsonNode;
	if (!valueNode)
		return;
	Value::MemberIterator nameIter = valueNode->FindMember("name");
	if (nameIter!= valueNode->MemberEnd())
	{
		nameIter->value.SetString(StringRef(nodeName), nodeName.size(), docImpl->m_nodeDoc.GetAllocator());
	}
	//‘∂≥ÃÃÌº”µƒ¥¶¿Ì
}

const std::wstring Stk_Node::GetNodeName()
{
	wstring nodeName;
	Stk_Document* pDoc = (Stk_Document*)((Stk_NodeImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueNode;
	if (!((Stk_NodeImpl*)m_pImpl)->m_pJsonNode)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valueNode = docImpl->GetJsonNode(this->GetID());
		((Stk_NodeImpl*)m_pImpl)->m_pJsonNode = valueNode;
	} 
	else
	{
		valueNode = ((Stk_NodeImpl*)m_pImpl)->m_pJsonNode;
	}
	if (!valueNode)
		return L"";
	Value::MemberIterator nameIter = valueNode->FindMember("name");
	if ((nameIter!= valueNode->MemberEnd() && nameIter->value.IsString()))
	{
		string strNodeName = nameIter->value.GetString();
		nodeName = ChineseCode::UTF_8ToUnicode(strNodeName.c_str());
	}
	return nodeName;
}

void Stk_Node::SetColorIdx(unsigned int colorIdx)
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_NodeImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	//Value* valueNode = docImpl->GetNode(this->GetID());
	Value* valueNode = ((Stk_NodeImpl*)m_pImpl)->m_pJsonNode;
	if (!valueNode)
		return;
	Value::MemberIterator materialIDIter = valueNode->FindMember("materialID");
	if (materialIDIter!= valueNode->MemberEnd())
	{
		materialIDIter->value.SetInt(colorIdx);
	}
	else
	{
		valueNode->AddMember("materialID", colorIdx, docImpl->m_nodeDoc.GetAllocator());
	}
}

STK_RGBA32 Stk_Node::GetColor()
{
	STK_RGBA32 nodeColor;
	Stk_Document* pDoc = (Stk_Document*)((Stk_NodeImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	//ªÒ»°—’…´–≈œ¢
	Value* valueNode;
	if (!((Stk_NodeImpl*)m_pImpl)->m_pJsonNode)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valueNode = docImpl->GetJsonNode(this->GetID());
		((Stk_NodeImpl*)m_pImpl)->m_pJsonNode = valueNode;
	} 
	else
	{
		valueNode = ((Stk_NodeImpl*)m_pImpl)->m_pJsonNode;
	}
	if (!valueNode)
		return nodeColor;
	Value::MemberIterator materialIDIter = valueNode->FindMember("materialID");
	if (materialIDIter!= valueNode->MemberEnd())
	{
		unsigned int colorIdx = materialIDIter->value.GetInt();
		int stkNodeColor = Stk_API::GetAPI()->GetColor(colorIdx);
		nodeColor.Red = GetRValue(stkNodeColor) / 255.0;
		nodeColor.Green = GetGValue(stkNodeColor) / 255.0;
		nodeColor.Blue = GetBValue(stkNodeColor) / 255.0;
		nodeColor.Alpha = 0.0;
	}
	return nodeColor;
}

//_stk_rgba32 Stk_Node::GetColor()
//{
//	return _stk_rgba32();
//}

unsigned int Stk_Node::GetNbChild()
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_NodeImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueNode;
	if (!((Stk_NodeImpl*)m_pImpl)->m_pJsonNode)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valueNode = docImpl->GetJsonNode(this->GetID());
		((Stk_NodeImpl*)m_pImpl)->m_pJsonNode = valueNode;
	} 
	else
	{
		valueNode = ((Stk_NodeImpl*)m_pImpl)->m_pJsonNode;
	}
	
	if (!valueNode)
		return 0;
	Value::MemberIterator childrenIter = valueNode->FindMember("Children");
	if ((childrenIter!= valueNode->MemberEnd() && childrenIter->value.IsArray()))
	{
		return childrenIter->value.Size();
	}
	return 0;
}

Stk_NodePtr Stk_Node::GetChild( int iIdx )
{
	if (iIdx < 0 || iIdx >= GetNbChild()) {
		return nullptr;
	}
	Stk_NodePtr retPtr = nullptr;

	Stk_Document* pDoc = (Stk_Document*)((Stk_NodeImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueNode;
	if (!((Stk_NodeImpl*)m_pImpl)->m_pJsonNode)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valueNode = docImpl->GetJsonNode(this->GetID());
		((Stk_NodeImpl*)m_pImpl)->m_pJsonNode = valueNode;
	} 
	else
	{
		valueNode = ((Stk_NodeImpl*)m_pImpl)->m_pJsonNode;
	}
	if (!valueNode)
		return 0;
	Value::MemberIterator childrenIter = valueNode->FindMember("Children");
	if ((childrenIter!= valueNode->MemberEnd() && childrenIter->value.IsArray()))
	{
		unsigned int childID = childrenIter->value[iIdx].GetInt();
		retPtr = AddChild(childID);
	}
	return retPtr;
}

Stk_MeshPrimPtr Stk_Node::AddMeshPrim(unsigned int uiID /*= -1*/)
{
	Stk_NodeImpl* pNodeImp = (Stk_NodeImpl*)m_pImpl;
	Stk_MeshPrimPtr meshPtr = Stk_MeshPrimPtr::CreateObject();
	if (-1 != uiID) {
		meshPtr->SetID(uiID);
	}
	else {
		Stk_Document* pDoc = (Stk_Document*)((Stk_NodeImpl*)m_pImpl)->m_pDoc;
		Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
		//‘∂≥ÃÃÌº”µƒ¥¶¿Ì
	}
	meshPtr->Init(pNodeImp->m_pDoc, GetID(),  - 1 != uiID ? false : true);
	return meshPtr;
}

int Stk_Node::GetNbMeshPrim(){
	Stk_Document* pDoc = (Stk_Document*)((Stk_NodeImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	return 0;
}

Stk_MeshPrimPtr Stk_Node::GetMeshPrim(unsigned int iIdx)
{
	if (iIdx < 0 || iIdx >= GetNbMeshPrim()) {
		return nullptr;
	}

	Stk_MeshPrimPtr retPtr = nullptr;
	Stk_Document* pDoc = (Stk_Document*)((Stk_NodeImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	return retPtr;
}

void Stk_Node::SetMatrix(STK_MTX32& iMatrix)
{
	//((Stk_NodeImpl*)m_pImpl)->SetQuaternion(w, x, y, z);
	Stk_Document* pDoc = (Stk_Document*)((Stk_NodeImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueNode = ((Stk_NodeImpl*)m_pImpl)->m_pJsonNode;
	if (!valueNode)
		return;
	char cMatrix[1000];
	sprintf(cMatrix, "%.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f", iMatrix.PlcMatrix[0][0], iMatrix.PlcMatrix[0][1], iMatrix.PlcMatrix[0][2],iMatrix.PlcMatrix[0][3]
		, iMatrix.PlcMatrix[1][0], iMatrix.PlcMatrix[1][1], iMatrix.PlcMatrix[1][2], iMatrix.PlcMatrix[1][3]
		, iMatrix.PlcMatrix[2][0], iMatrix.PlcMatrix[2][1], iMatrix.PlcMatrix[2][2], iMatrix.PlcMatrix[2][3]);
	string strMatrix = cMatrix;

	Value::MemberIterator matrixIter = valueNode->FindMember("Matrix");
	if (matrixIter != valueNode->MemberEnd())
	{
		matrixIter->value.SetString(StringRef(strMatrix), strMatrix.size(), docImpl->m_nodeDoc.GetAllocator());
	}
	else
	{
		//¥¥Ω®MatrixΩ⁄µ„
		valueNode->AddMember("Matrix", strMatrix, docImpl->m_nodeDoc.GetAllocator());
	}
}

int Stk_Node::GetMatrix(STK_MTX32& iMatrix)
{
	//return ((Stk_NodeImpl*)m_pImpl)->GetQuaternion(w, x, y, z);
	for (int i = 0; i < MTX_LINE_COUNT; i++)
	{
		for (int j = 0; j < MTX_COL_COUNT; j++)
		{
			iMatrix.PlcMatrix[i][j] = 0.0f;
		}
	}
	iMatrix.PlcMatrix[0][3] = iMatrix.PlcMatrix[1][3] = iMatrix.PlcMatrix[2][3] = 1.0f;
	
	Stk_Document* pDoc = (Stk_Document*)((Stk_NodeImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueNode = ((Stk_NodeImpl*)m_pImpl)->m_pJsonNode;
	if (!valueNode)
		return -1;
	Value::MemberIterator matrixIter = valueNode->FindMember("Matrix");
	if (matrixIter != valueNode->MemberEnd())
	{
		string strMatrix = matrixIter->value.GetString();
		std::vector<std::string> vecMatrixValue;
		CStringTools::StringSplit(strMatrix, " ", vecMatrixValue);
		if (vecMatrixValue.size() == MTX_LINE_COUNT*MTX_COL_COUNT)
		{
			for (int i = 0; i < MTX_LINE_COUNT; i++)
			{
				for (int j = 0; j < MTX_COL_COUNT; j++)
				{
					iMatrix.PlcMatrix[i][j] = atof(vecMatrixValue[i*MTX_COL_COUNT + j].c_str());
				}
			}
		}
	}
	
	return 0;
}

void Stk_Node::SetBoundBox( const float* minPnt, const float* maxPnt )
{
	((Stk_NodeImpl*)m_pImpl)->SetBoundBox(minPnt, maxPnt);
}

void Stk_Node::SetNodeType( const StkNodeTypeEnum& i_enumNodetype )
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_NodeImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	string nodeName;
	Value* valueNode = docImpl->GetNode(this->GetID());
	if (!valueNode)
		return;
	Value::MemberIterator nameIter = valueNode->FindMember("type");
	if (nameIter!= valueNode->MemberEnd())
	{
		nameIter->value.SetInt(i_enumNodetype);
	}
}

void Stk_Node::BindingNode( void* i_bingdedNode )
{
	 ((Stk_NodeImpl*)m_pImpl)->m_pJsonNode = (Value*)i_bingdedNode;
}

void Stk_Node::SetPart(Stk_PartPtr i_pStkPart)
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_NodeImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	m_pPart = i_pStkPart;
	//¥¥Ω®refΩ⁄µ„
	Value* valueNode = ((Stk_NodeImpl*)m_pImpl)->m_pJsonNode;
	if (!valueNode)
		return ;
	Value::MemberIterator refIter = valueNode->FindMember("Ref");
	if (refIter== valueNode->MemberEnd())
		return;
	valueNode->AddMember("ref", i_pStkPart->GetID(), docImpl->m_nodeDoc.GetAllocator());
}

Stk_PartPtr Stk_Node::SetPart(unsigned int uiID /*= -1*/)
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_NodeImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_NodeImpl* pNodeImp = (Stk_NodeImpl*)m_pImpl;
	//»∑»œPart «∑Ò±ªÃÌº”
	map<int, Stk_PartPtr>::iterator it;
	it = docImpl->m_mapIdToStkPart.find(uiID);
	if (it != docImpl->m_mapIdToStkPart.end())
	{
		return it->second;
	}

	Stk_PartPtr partPtr = Stk_PartPtr::CreateObject();
	partPtr->SetID(uiID);
	//Ω´PartΩ⁄µ„ÃÌº”µΩMap÷–
	docImpl->m_mapIdToStkPart.insert(make_pair(uiID, partPtr));

	partPtr->Init(pNodeImp->m_pDoc, GetID(), -1 != uiID ? false : true);
	partPtr->Add();
	//¥¥Ω®refΩ⁄µ„
	Value* valueNode = ((Stk_NodeImpl*)m_pImpl)->m_pJsonNode;
	if (!valueNode)
		return partPtr;
	Value::MemberIterator refIter = valueNode->FindMember("Ref");
	if (refIter != valueNode->MemberEnd())
		return partPtr;
	valueNode->AddMember("ref", partPtr->GetID(), docImpl->m_nodeDoc.GetAllocator());
	return partPtr;
}

Stk_PartPtr Stk_Node::GetPart()
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_NodeImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_NodeImpl* pNodeImp = (Stk_NodeImpl*)m_pImpl;
	//ªÒ»°Node∂‘”¶µƒJsonΩ⁄µ„
	Value* valueNode;
	if (!((Stk_NodeImpl*)m_pImpl)->m_pJsonNode)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valueNode = docImpl->GetJsonNode(this->GetID());
		((Stk_NodeImpl*)m_pImpl)->m_pJsonNode = valueNode;
	} 
	else
	{
		valueNode = ((Stk_NodeImpl*)m_pImpl)->m_pJsonNode;
	}

	if (!valueNode)
		return NULL;
	//ªÒ»°refID
	int partID;
	Value::MemberIterator refIter = valueNode->FindMember("ref");
	if (refIter == valueNode->MemberEnd())
		return NULL;
	partID =  refIter->value.GetInt();
	//»∑»œPart «∑Ò±ªÃÌº”
	map<int, Stk_PartPtr>::iterator it;
	it = docImpl->m_mapIdToStkPart.find(partID);
	if (it != docImpl->m_mapIdToStkPart.end())
	{
		return it->second;
	}
	//¥¥Ω®Part∂‘œÛ
	Stk_PartPtr stkPart = Stk_PartPtr::CreateObject();
	stkPart->SetID(partID);
	Value* valPart = docImpl->GetJsonPart(partID);
	stkPart->BindPart(valPart);
	//Ω´PartΩ⁄µ„ÃÌº”µΩMap÷–
	docImpl->m_mapIdToStkPart.insert(make_pair(partID, stkPart));
	stkPart->Init(pDoc, 0, false);
	return stkPart;
}

	}//svllib
}//hoteamsoft

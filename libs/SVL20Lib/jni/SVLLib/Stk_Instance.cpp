// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_ComponentFeature.cpp
 *
 *		2016/11/22	÷ÿππ°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_ObjectImpl.h"
#include "Stk_Instance.h"
#include "Stk_Document.h"
#include "Stk_DocumentImpl.h"
#include "ChineseCode.h"
#include "Stk_API.h"
#ifdef WIN32
#include <windows.h>
#endif
#include "JsonMacro.h"
#include "Stk_Attribute.h"
using namespace std;

namespace HoteamSoft {
    namespace SVLLib {
        // µœ÷¿‡
        class Stk_InstanceImpl : public Stk_ObjectImpl
	{
		STK_CLASS_IMPL_DECLARE(Stk_Instance)
	public:
		Stk_InstanceImpl(void);
		virtual ~Stk_InstanceImpl(void);

		Stk_Document* m_pDoc;
		Stk_Document* GetDoc() { return m_pDoc; }

		Value*				m_pJsonInstance;
		StkBomBlock m_bomBlock;
	};

	STK_CLASS_IMPL_IMPLEMENT(Stk_Instance)
	Stk_InstanceImpl::Stk_InstanceImpl(void)
	{
		m_pJsonInstance = NULL;
	}

	Stk_InstanceImpl::~Stk_InstanceImpl(void)
	{
	}

STK_IMPLEMENT_CLASS(Stk_Instance, Stk_Object)

void Stk_Instance::Init(Stk_Document* pDoc, unsigned int parentID, bool bForWrite/* = true*/)
{
	((Stk_InstanceImpl*)m_pImpl)->m_pDoc = pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	if (bForWrite) {

	}
}

void Stk_Instance::SetDocument(void* i_DocumentPtr)
{
	((Stk_InstanceImpl*)m_pImpl)->m_pDoc = (Stk_Document*)i_DocumentPtr;
}

Stk_Instance::Stk_Instance(void):Stk_Object(new Stk_InstanceImpl())
{
}

Stk_Instance::~Stk_Instance(void)
{
	DELETE_STK_CLASS_IMPL
}

Stk_InstancePtr Stk_Instance::AddChild(unsigned int uiID/* = -1*/)
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_InstanceImpl* pInstanceImp = (Stk_InstanceImpl*)m_pImpl;
	//≈–∂œ◊”Ω⁄µ„∂‘œÛ «∑Ò±ªÃÌº”
	map<int, Stk_InstancePtr>::iterator it;
	it = docImpl->m_mapIdToInstance.find(uiID);
	if (it != docImpl->m_mapIdToInstance.end())
	{
		return it->second;
	}
	Stk_InstancePtr stkChildInstance = Stk_InstancePtr::CreateObject();
	if (-1 != uiID) {
		stkChildInstance->SetID(uiID);
	}
#if 0
	Value* valueInstance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstance)
		return NULL;
	//…Ë÷√µ±«∞NodeΩ⁄µ„µƒ◊”Ω⁄µ„–≈œ¢
	(*valueInstance)[5].PushBack(stkChildInstance->GetID(), docImpl->m_nodeDoc.GetAllocator());
	//¥¥Ω®◊” µ¿˝Ω⁄µ„
	Value valChildInstance;
	valChildInstance.SetArray();
	valChildInstance.PushBack(stkChildInstance->GetID(), docImpl->m_nodeDoc.GetAllocator());		//id
	valChildInstance.PushBack(-1, docImpl->m_nodeDoc.GetAllocator());				   //plcid
	valChildInstance.PushBack(-1, docImpl->m_nodeDoc.GetAllocator());					//modelID
	valChildInstance.PushBack(1, docImpl->m_nodeDoc.GetAllocator());			   //œ‘“˛◊¥Ã¨
	valChildInstance.PushBack("", docImpl->m_nodeDoc.GetAllocator());				   //≈‰÷√æÿ’Û
	Value valInstaceChildren;
	valInstaceChildren.SetArray();
	valChildInstance.PushBack(valInstaceChildren, docImpl->m_nodeDoc.GetAllocator());	//◊”Ω⁄µ„–≈œ¢
	valChildInstance.PushBack(-1, docImpl->m_nodeDoc.GetAllocator());				   //≤ƒ÷ ID
	valChildInstance.PushBack(-1, docImpl->m_nodeDoc.GetAllocator());					//∏∏Ω⁄µ„ID
	//Ω´◊” µ¿˝Ω⁄µ„ÃÌº”µΩ µ¿˝ ˝◊È
	docImpl->m_nodeDoc[BOM][INSTANCES].PushBack(valChildInstance, docImpl->m_nodeDoc.GetAllocator());
	//Ω´JsonΩ⁄µ„∫ÕNodeΩ⁄µ„∞Û∂®
	stkChildInstance->BindingNode(&docImpl->m_nodeDoc[BOM][INSTANCES][docImpl->m_nodeDoc[BOM][INSTANCES].Size() -1]);
#endif
	StkBomBlock tmpBomBlock;
	tmpBomBlock.instanceID = stkChildInstance->GetID();
	tmpBomBlock.materialID = -1;
	tmpBomBlock.modelID = -1;
	tmpBomBlock.parentID = -1;
	tmpBomBlock.plcId = -1;
	tmpBomBlock.visible = -1;
	for (int i = 0; i < 16; i++)
	{
		tmpBomBlock.Matrix[i] = 0;
	}
	tmpBomBlock.Matrix[0] = tmpBomBlock.Matrix[5] = tmpBomBlock.Matrix[10] = 1;
	tmpBomBlock.Matrix[15] = 1;
	stkChildInstance->BindBomBlock(&tmpBomBlock);
	//ÃÌº”Map
	docImpl->m_mapIdToInstance.insert(make_pair(stkChildInstance->GetID(), stkChildInstance));
	//}
	stkChildInstance->Init(pInstanceImp->m_pDoc, GetID(), -1 != uiID ? false : true);
	return stkChildInstance;
}

void Stk_Instance::SetInstanceName( const std::wstring& i_wstrNodeName )
{
	if (!((Stk_InstanceImpl*)m_pImpl)->m_pDoc || !(((Stk_InstanceImpl*)m_pImpl)->m_pDoc)->m_pImpl)
		return;
	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_InstanceImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	if(pDocImpl->m_pAttrDoc == nullptr)
		return;
	pDocImpl->m_pAttrDoc->AddInstanceAttribute(GetID(), L"name", i_wstrNodeName);
}

void  Stk_Instance::AddAttribute(const std::wstring& i_wstrAttrName, const std::wstring& i_wstrAttrValue)
{
	if (!((Stk_InstanceImpl*)m_pImpl)->m_pDoc || !(((Stk_InstanceImpl*)m_pImpl)->m_pDoc)->m_pImpl)
		return;
	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_InstanceImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	if (pDocImpl->m_pAttrDoc == nullptr)
		return;
	pDocImpl->m_pAttrDoc->AddInstanceAttribute(GetID(), i_wstrAttrName, i_wstrAttrValue);
}

std::wstring   Stk_Instance::GetAttribute(const std::wstring& i_wstrAttrName)
{
	wstring wstrName = L"";
	if (!((Stk_InstanceImpl*)m_pImpl)->m_pDoc || !(((Stk_InstanceImpl*)m_pImpl)->m_pDoc)->m_pImpl)
		return wstrName;
	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_InstanceImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	if (pDocImpl->m_pAttrDoc == nullptr)
		return wstrName;
	wstrName = pDocImpl->m_pAttrDoc->GetInstanceAttribute(GetID(), i_wstrAttrName);

	return wstrName;
}

map<wstring, wstring>   Stk_Instance::GetAllAttribute()
{
	map<wstring, wstring> mapAttribute;
	wstring wstrName = L"";
	if (!((Stk_InstanceImpl*)m_pImpl)->m_pDoc || !(((Stk_InstanceImpl*)m_pImpl)->m_pDoc)->m_pImpl)
		return mapAttribute;
	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_InstanceImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	if (pDocImpl->m_pAttrDoc == nullptr)
		return mapAttribute;
	mapAttribute = pDocImpl->m_pAttrDoc->GetInstanceAllAttributes(GetID());
	return mapAttribute;
}

std::wstring  Stk_Instance::GetInstanceName()
{
	wstring wstrName = L"";
	if (!((Stk_InstanceImpl*)m_pImpl)->m_pDoc || !(((Stk_InstanceImpl*)m_pImpl)->m_pDoc)->m_pImpl)
		return wstrName;
	Stk_DocumentImpl* pDocImpl = (Stk_DocumentImpl*)(((Stk_InstanceImpl*)m_pImpl)->m_pDoc)->m_pImpl;
	if (pDocImpl->m_pAttrDoc == nullptr)
		return wstrName;
	wstrName = pDocImpl->m_pAttrDoc->GetInstanceAttribute(GetID(), L"name");
	
	return wstrName;
}

const std::wstring Stk_Instance::GetNodeName()
{
	wstring nodeName;
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueNode;
	if (!((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valueNode = docImpl->GetJsonNode(this->GetID());
		((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance = valueNode;
	} 
	else
	{
		valueNode = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
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

void Stk_Instance::SetPlcID(unsigned int inPlcId)
{
#if 0
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueInstnance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstnance)
		return;
	(*valueInstnance)[INDEX_PLCID].SetInt(inPlcId);
#endif
	((Stk_InstanceImpl*)m_pImpl)->m_bomBlock.plcId = inPlcId;
}

STK_UINT32 Stk_Instance::GetPlcID()
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
#if 0
	Value* valueInstnance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstnance)
		return -1;
	return (*valueInstnance)[INDEX_PLCID].GetInt();
#else
	return ((Stk_InstanceImpl*)m_pImpl)->m_bomBlock.plcId;
#endif
	
}

void Stk_Instance::SetParentID(STK_INT32 i_parentID)
{
#if 0
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueInstnance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstnance)
		return;
	(*valueInstnance)[INDEX_PARENTID].SetInt(i_parentID);
#endif
	((Stk_InstanceImpl*)m_pImpl)->m_bomBlock.parentID = i_parentID;
}

STK_INT32 Stk_Instance::GetParentID()
{
#if 0
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueInstnance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstnance)
		return -1;
	return (*valueInstnance)[INDEX_PARENTID].GetInt();
#endif
	return ((Stk_InstanceImpl*)m_pImpl)->m_bomBlock.parentID;
}

void Stk_Instance::SetMaterialID(STK_UINT32 inMaterialId)
{
#if 0
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueInstnance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstnance)
		return;
	(*valueInstnance)[INDEX_MATERIAL].SetInt(inMaterialId);
#endif
	((Stk_InstanceImpl*)m_pImpl)->m_bomBlock.materialID = inMaterialId;
}

unsigned int Stk_Instance::GetMaterialID()
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
#if 0
	Value* valueInstnance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstnance)
		return -1;
	return (*valueInstnance)[INDEX_MATERIAL].GetInt();
#else
	return ((Stk_InstanceImpl*)m_pImpl)->m_bomBlock.materialID;
#endif
}

Stk_MaterialPtr Stk_Instance::GetMaterial()
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
#if 0
	Value* valueInstnance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstnance)
		return nullptr;
	int iMaterialID = (*valueInstnance)[INDEX_MATERIAL].GetInt();
#else
	int iMaterialID = ((Stk_InstanceImpl*)m_pImpl)->m_bomBlock.materialID;
#endif
	return pDoc->GetMaterialByID(iMaterialID);
}

void Stk_Instance::SetColorIdx(unsigned int colorIdx)
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	//Value* valueNode = docImpl->GetNode(this->GetID());
	Value* valueInstance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstance)
		return;
	Value::MemberIterator materialIDIter = valueInstance->FindMember("materialID");
	if (materialIDIter!= valueInstance->MemberEnd())
	{
		materialIDIter->value.SetInt(colorIdx);
	}
	else
	{
		valueInstance->AddMember("materialID", colorIdx, docImpl->m_nodeDoc.GetAllocator());
	}
}

void Stk_Instance::SetMatrix(float* inMatrix)
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
#if 0
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueInstnance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstnance)
		return;
	string strMatrix;
	for (int i =0; i < 12; i++)
	{
		char charMatrix[32];
		sprintf(charMatrix, "%f,", inMatrix[i]);
		strMatrix += charMatrix;
	}
	(*valueInstnance)[INDEX_MATRIX].SetString(strMatrix, docImpl->m_nodeDoc.GetAllocator());
#endif
	for (int i = 0; i  < 16; i++)
	{
		((Stk_InstanceImpl*)m_pImpl)->m_bomBlock.Matrix[i] = inMatrix[i];
	}
}
#if 0
float* Stk_Instance::GetMatrix()
{
	float fMatrix[12] = {0};
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueInstnance;
	if (!((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valueInstnance = docImpl->GetJsonInstance(this->GetID());
		((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance = valueInstnance;
	} 
	else
	{
		valueInstnance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	}
	
	if (!valueInstnance)
		return fMatrix;
	string strMatrix;
	strMatrix = (*valueInstnance)[INDEX_MATRIX].GetString();
	if("" == strMatrix)
		return fMatrix;
	for (int i = 0; i < 12; i++)
	{
		string strTemp = strMatrix.substr(0, strMatrix.find_first_of(","));
		fMatrix[i] = atof(strTemp.c_str());
		strMatrix = strMatrix.substr(strMatrix.find_first_of(",") + 1, strMatrix.length() - strTemp.length() - 1);
	}
	return fMatrix;
}
#endif

STK_STATUS Stk_Instance::GetMatrix(float* o_inMatrix, int i_size)
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
#if 0
	Value* valueInstnance;
	if (!((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valueInstnance = docImpl->GetJsonInstance(this->GetID());
		((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance = valueInstnance;
	} 
	else
	{
		valueInstnance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	}

	if (!valueInstnance)
		return STK_ERROR;
	string strMatrix;
	strMatrix = (*valueInstnance)[INDEX_MATRIX].GetString();
	if("" == strMatrix)
		return STK_ERROR;
	for (int i = 0; i < i_size; i++)
	{
		string strTemp = strMatrix.substr(0, strMatrix.find_first_of(","));
		o_inMatrix[i] = atof(strTemp.c_str());
		strMatrix = strMatrix.substr(strMatrix.find_first_of(",") + 1, strMatrix.length() - strTemp.length() - 1);
	}
#endif
	for (int i = 0; i < i_size; i++)
	{
		o_inMatrix[i] = ((Stk_InstanceImpl*)m_pImpl)->m_bomBlock.Matrix[i];
	}
	return STK_SUCCESS;
}

void Stk_Instance::SetVisible(bool i_bVisible)
{
#if 0
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueInstnance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstnance)
		return;
	(*valueInstnance)[INDEX_VISIBLE].SetInt(i_bVisible);
#endif
	((Stk_InstanceImpl*)m_pImpl)->m_bomBlock.visible = i_bVisible;
}

bool Stk_Instance::GetVisible()
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
#if 0
	Value* valueInstnance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstnance)
		return true;
	return (*valueInstnance)[INDEX_VISIBLE].GetInt();
#else
	return ((Stk_InstanceImpl*)m_pImpl)->m_bomBlock.visible;
#endif
}

STK_RGBA32 Stk_Instance::GetColor()
{
	STK_RGBA32 nodeColor;
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	//ªÒ»°—’…´–≈œ¢
	Value* valueNode;
	if (!((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valueNode = docImpl->GetJsonNode(this->GetID());
		((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance = valueNode;
	} 
	else
	{
		valueNode = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
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

//_stk_rgba32 Stk_Instance::GetColor()
//{
//	return _stk_rgba32();
//}

unsigned int Stk_Instance::GetNbChild()
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueInstance;
	if (!((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valueInstance = docImpl->GetJsonInstance(this->GetID());
		((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance = valueInstance;
	} 
	else
	{
		valueInstance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	}
	
	if (!valueInstance)
		return 0;
	return (*valueInstance)[INDEX_INSCHILDREN].Size();
}

Stk_InstancePtr Stk_Instance::GetChild( int iIdx )
{
	if (iIdx < 0 || iIdx >= GetNbChild()) {
		return nullptr;
	}
	Stk_InstancePtr retPtr = nullptr;

	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueInstance;
	if (!((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valueInstance = docImpl->GetJsonInstance(this->GetID());
		((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance = valueInstance;
	} 
	else
	{
		valueInstance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	}
	if (!valueInstance)
		return 0;

	unsigned int childID = (*valueInstance)[INDEX_INSCHILDREN][iIdx].GetInt();
	//retPtr = AddChild(childID);
	//¥¥Ω®Instance∂‘œÛ
	//≈–∂œ◊”Ω⁄µ„∂‘œÛ «∑Ò±ªÃÌº”
	map<int, Stk_InstancePtr>::iterator it;
	it = docImpl->m_mapIdToInstance.find(childID);
	if (it != docImpl->m_mapIdToInstance.end())
	{
		return it->second;
	}
	Stk_InstancePtr stkChildInstance = Stk_InstancePtr::CreateObject();
	if (-1 != childID) {
		stkChildInstance->SetID(childID);
	}
	docImpl->m_mapIdToInstance.insert(make_pair(stkChildInstance->GetID(), stkChildInstance));
	//}
	stkChildInstance->Init(((Stk_InstanceImpl*)m_pImpl)->m_pDoc, GetID(), -1 != childID ? false : true);
	return stkChildInstance;
}

vector<Stk_InstancePtr>& Stk_Instance::getChildren()
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	return docImpl->m_parentInstanceIdToStkInstance[this->GetID()];
}

void Stk_Instance::SetQuaternion( const float& w, const float& x, const float& y, const float& z )
{
	//((Stk_InstanceImpl*)m_pImpl)->SetQuaternion(w, x, y, z);
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueInstance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstance)
		return;
	Value::MemberIterator matrixIter = valueInstance->FindMember("Matrix");
	if (matrixIter!= valueInstance->MemberEnd())
	{
		Value* pValMatrix = &matrixIter->value;
		Value::MemberIterator quaternionIter = pValMatrix->FindMember("Quaternion");
		if (quaternionIter!= pValMatrix->MemberEnd())
		{
			//±‡º≠Àƒ‘™ ˜–≈œ¢
			quaternionIter->value[0].SetFloat(w);
			quaternionIter->value[1].SetFloat(x);
			quaternionIter->value[2].SetFloat(y);
			quaternionIter->value[3].SetFloat(z);
		} 
		else
		{
			//¥¥Ω®QuaternionΩ⁄µ„
			Value valQuaternion;
			valQuaternion.SetArray();
			valQuaternion.PushBack(w, docImpl->m_nodeDoc.GetAllocator());
			valQuaternion.PushBack(x, docImpl->m_nodeDoc.GetAllocator());
			valQuaternion.PushBack(y, docImpl->m_nodeDoc.GetAllocator());
			valQuaternion.PushBack(z, docImpl->m_nodeDoc.GetAllocator());
			pValMatrix->AddMember("Quaternion", valQuaternion, docImpl->m_nodeDoc.GetAllocator());
		}
	}
	else
	{
		//¥¥Ω®MatrixΩ⁄µ„
		Value valMatrix;
		valMatrix.SetObject();
		//¥¥Ω®QuaternionΩ⁄µ„
		Value valQuaternion;
		valQuaternion.SetArray();
		valQuaternion.PushBack(w, docImpl->m_nodeDoc.GetAllocator());
		valQuaternion.PushBack(x, docImpl->m_nodeDoc.GetAllocator());
		valQuaternion.PushBack(y, docImpl->m_nodeDoc.GetAllocator());
		valQuaternion.PushBack(z, docImpl->m_nodeDoc.GetAllocator());
		valMatrix.AddMember("Quaternion", valQuaternion, docImpl->m_nodeDoc.GetAllocator());
		valueInstance->AddMember("Matrix", valMatrix, docImpl->m_nodeDoc.GetAllocator());
	}
}

void Stk_Instance::SetTranslation( const float& x, const float& y, const float& z )
{
	//((Stk_InstanceImpl*)m_pImpl)->SetTranslation(x, y, z);
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueInstance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstance)
		return;
	Value::MemberIterator matrixIter = valueInstance->FindMember("Matrix");
	if (matrixIter!= valueInstance->MemberEnd())
	{
		Value* pValMatrix = &matrixIter->value;
		Value::MemberIterator positionIter = pValMatrix->FindMember("Position");
		if (positionIter!= pValMatrix->MemberEnd())
		{
			//±‡º≠Àƒ‘™ ˜–≈œ¢
			positionIter->value[0].SetFloat(x);
			positionIter->value[1].SetFloat(y);
			positionIter->value[2].SetFloat(z);
		} 
		else
		{
			//¥¥Ω®QuaternionΩ⁄µ„
			Value valPosition;
			valPosition.SetArray();
			valPosition.PushBack(x, docImpl->m_nodeDoc.GetAllocator());
			valPosition.PushBack(y, docImpl->m_nodeDoc.GetAllocator());
			valPosition.PushBack(z, docImpl->m_nodeDoc.GetAllocator());
			pValMatrix->AddMember("Position", valPosition, docImpl->m_nodeDoc.GetAllocator());
		}
	}
	else
	{
		//¥¥Ω®MatrixΩ⁄µ„
		Value valMatrix;
		valMatrix.SetObject();
		//¥¥Ω®QuaternionΩ⁄µ„
		Value valPosition;
		valPosition.SetArray();
		valPosition.PushBack(x, docImpl->m_nodeDoc.GetAllocator());
		valPosition.PushBack(y, docImpl->m_nodeDoc.GetAllocator());
		valPosition.PushBack(z, docImpl->m_nodeDoc.GetAllocator());
		valMatrix.AddMember("Position", valPosition, docImpl->m_nodeDoc.GetAllocator());
		valueInstance->AddMember("Matrix", valMatrix, docImpl->m_nodeDoc.GetAllocator());
	}
}

int Stk_Instance::GetQuaternion( float& w, float& x, float& y, float& z )
{
	//return ((Stk_InstanceImpl*)m_pImpl)->GetQuaternion(w, x, y, z);
	w = 1.0;
	x = y = z = 0.0;
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueInstance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstance)
		return -1;
	Value::MemberIterator matrixIter = valueInstance->FindMember("Matrix");
	if (matrixIter== valueInstance->MemberEnd())
		return -2;
	Value* pValMatrix = &matrixIter->value;
	Value::MemberIterator quaternionIter = pValMatrix->FindMember("Quaternion");
	if (quaternionIter == valueInstance->MemberEnd())
		return -3;
	w = quaternionIter->value[0].GetFloat();
	x = quaternionIter->value[1].GetFloat();
	y = quaternionIter->value[2].GetFloat();
	z = quaternionIter->value[3].GetFloat();
	return 0;
}

int Stk_Instance::GetTranslation( float& x, float& y, float& z )
{
	//return ((Stk_InstanceImpl*)m_pImpl)->GetTranslation(x, y, z);
	x = y = z = 0.0;
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Value* valueInstance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstance)
		return -1;
	Value::MemberIterator matrixIter = valueInstance->FindMember("Matrix");
	if (matrixIter== valueInstance->MemberEnd())
		return -2;
	Value* pValMatrix = &matrixIter->value;
	Value::MemberIterator positionIter = pValMatrix->FindMember("Position");
	if (positionIter == valueInstance->MemberEnd())
		return -3;
	x = positionIter->value[0].GetFloat();
	y = positionIter->value[1].GetFloat();
	z = positionIter->value[2].GetFloat();
	return 0;
}

void Stk_Instance::SetBoundBox( const float* minPnt, const float* maxPnt )
{
	//((Stk_InstanceImpl*)m_pImpl)->SetBoundBox(minPnt, maxPnt);
}

void Stk_Instance::SetNodeType( const StkNodeTypeEnum& i_enumNodetype )
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	string nodeName;
	Value* valueInstance = docImpl->GetNode(this->GetID());
	if (!valueInstance)
		return;
	Value::MemberIterator nameIter = valueInstance->FindMember("type");
	if (nameIter!= valueInstance->MemberEnd())
	{
		nameIter->value.SetInt(i_enumNodetype);
	}
}

void Stk_Instance::BindingNode( void* i_bingdedNode )
{
	 ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance = (Value*)i_bingdedNode;
}

void Stk_Instance::BindBomBlock(void* i_bindedBomBlock)
{
	((Stk_InstanceImpl*)m_pImpl)->m_bomBlock = *(StkBomBlock*)i_bindedBomBlock;
}

void Stk_Instance::SetModel(Stk_ModelPtr i_pStkModel)
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	m_pModel = i_pStkModel;
	//¥¥Ω®refΩ⁄µ„
	Value* valueInstance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	if (!valueInstance)
		return ;
	Value::MemberIterator refIter = valueInstance->FindMember("Ref");
	if (refIter== valueInstance->MemberEnd())
		return;
	valueInstance->AddMember("ref", i_pStkModel->GetID(), docImpl->m_nodeDoc.GetAllocator());
}

Stk_ModelPtr Stk_Instance::SetModel( int uiID /*= -1*/)
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_InstanceImpl* pInstanceImp = (Stk_InstanceImpl*)m_pImpl;

	//Value* valueInstance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	//if (!valueInstance)
		//return nullptr;
	
	//»∑»œPart «∑Ò±ªÃÌº”
	map<int, Stk_ModelPtr>::iterator it;
	it = docImpl->m_mapIdToStkModel.find(uiID);
	if (it != docImpl->m_mapIdToStkModel.end())
	{
		//(*valueInstance)[2].SetInt(uiID);
		 ((Stk_InstanceImpl*)m_pImpl)->m_bomBlock.modelID = uiID;
		return it->second;
	}

	Stk_ModelPtr modelPtr = Stk_ModelPtr::CreateObject();
	if(uiID != -1)
	{
		modelPtr->SetID(uiID);
	}
	//Ω´PartΩ⁄µ„ÃÌº”µΩMap÷–
	docImpl->m_mapIdToStkModel.insert(make_pair(modelPtr->GetID(), modelPtr));
	modelPtr->Init(pInstanceImp->m_pDoc, GetID(), -1 != uiID ? false : true);
	//modelPtr->Add();
	//¥¥Ω®refΩ⁄µ„
	//(*valueInstance)[2].SetInt(modelPtr->GetID());
	 ((Stk_InstanceImpl*)m_pImpl)->m_bomBlock.modelID = modelPtr->GetID();
	return modelPtr;
}

Stk_ModelPtr Stk_Instance::GetModel()
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_InstanceImpl* pNodeImp = (Stk_InstanceImpl*)m_pImpl;
#if 0
	//ªÒ»°Instance∂‘”¶µƒJsonΩ⁄µ„
	Value* valueInstance;
	if (!((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valueInstance = docImpl->GetJsonInstance(this->GetID());
		((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance = valueInstance;
	} 
	else
	{
		valueInstance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	}

	if (!valueInstance)
		return NULL;
	//ªÒ»°refID
	int modelID;
	modelID =  (*valueInstance)[INDEX_MODELID].GetInt();
	if (-1 == modelID)
		return NULL;
	//»∑»œPart «∑Ò±ªÃÌº”
	map<int, Stk_ModelPtr>::iterator it;
	it = docImpl->m_mapIdToStkModel.find(modelID);
	if (it != docImpl->m_mapIdToStkModel.end())
	{
		return it->second;
	}
	//¥¥Ω®Part∂‘œÛ
	//Stk_PartPtr stkPart = Stk_PartPtr::CreateObject();
	//stkPart->SetID(modelID);
	//Value* valPart = docImpl->GetJsonPart(modelID);
	//stkPart->BindPart(valPart);
	////Ω´PartΩ⁄µ„ÃÌº”µΩMap÷–
	//docImpl->m_mapIdToStkPart.insert(make_pair(modelID, stkPart));
	//stkPart->Init(pDoc, 0, false);
	//¥¥Ω®model∂‘œÛ
	Stk_ModelPtr stkModel = Stk_ModelPtr::CreateObject();
	stkModel->SetID(modelID);
	Value* valModel = docImpl->GetJsonModel(modelID);
	stkModel->BindModel(valModel);
	docImpl->m_mapIdToStkModel.insert(make_pair(modelID, stkModel));
	stkModel->Init(pDoc, 0, false);
#endif
	Stk_ModelPtr stkModel = nullptr;
	map<int, Stk_ModelPtr>::iterator it;
	it = docImpl->m_mapIdToStkModel.find(pNodeImp->m_bomBlock.modelID);
	if (it != docImpl->m_mapIdToStkModel.end())
		stkModel = it->second;
	return stkModel;
}

int Stk_Instance::GetModelID()
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_InstanceImpl* pNodeImp = (Stk_InstanceImpl*)m_pImpl;
#if 0
	//ªÒ»°Instance∂‘”¶µƒJsonΩ⁄µ„
	Value* valueInstance;
	if (!((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valueInstance = docImpl->GetJsonInstance(this->GetID());
		((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance = valueInstance;
	} 
	else
	{
		valueInstance = ((Stk_InstanceImpl*)m_pImpl)->m_pJsonInstance;
	}

	if (!valueInstance)
		return NULL;
	//ªÒ»°refID
	int modelID;
	modelID =  (*valueInstance)[INDEX_MODELID].GetInt();
	return modelID;
#else
	return pNodeImp->m_bomBlock.modelID;
#endif
}

Stk_ModelPtr Stk_Instance::GetModelByID(int i_ModelID)
{
	if (-1 == i_ModelID)
	{
		return nullptr;
	}
	Stk_Document* pDoc = (Stk_Document*)((Stk_InstanceImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_InstanceImpl* pNodeImp = (Stk_InstanceImpl*)m_pImpl;
	//»∑»œPart «∑Ò±ªÃÌº”
	map<int, Stk_ModelPtr>::iterator it;
	it = docImpl->m_mapIdToStkModel.find(i_ModelID);
	if (it != docImpl->m_mapIdToStkModel.end())
	{
		if (it->second.isNotNull())
			it->second->Init(pDoc, 0, false);
		return it->second;
	}
	//¥¥Ω®model∂‘œÛ
	Stk_ModelPtr stkModel = Stk_ModelPtr::CreateObject();
	stkModel->SetID(i_ModelID);
	Value* valModel = docImpl->GetJsonModel(i_ModelID);
	stkModel->BindModel(valModel);
	docImpl->m_mapIdToStkModel.insert(make_pair(i_ModelID, stkModel));
	stkModel->Init(pDoc, 0, false);
	return stkModel;
}

	}//svllib
}//hoteamsoft


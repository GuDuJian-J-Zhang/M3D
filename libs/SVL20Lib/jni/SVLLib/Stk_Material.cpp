// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_ComponentFeature.cpp
 *
 *
****************************************************************************/
#include "Stk_Material.h"
#include "Stk_ObjectImpl.h"
#include "Stk_DocumentImpl.h"
#include "Stk_Document.h"
#include "JsonMacro.h"
#include "ChineseCode.h"
namespace HoteamSoft {
	namespace SVLLib {
class Stk_Document;
class Stk_MaterialImpl : public Stk_ObjectImpl
{
	STK_CLASS_IMPL_DECLARE(Stk_Material)
public:
	Stk_MaterialImpl(void);
	virtual ~Stk_MaterialImpl(void);

	Stk_Document* m_pDoc;
	Stk_Document* GetDoc() { return m_pDoc; }

	Value*				m_pJsonMaterial;

};

STK_CLASS_IMPL_IMPLEMENT(Stk_Material)
Stk_MaterialImpl::Stk_MaterialImpl(void)
{
	m_pJsonMaterial = NULL;
}

Stk_MaterialImpl::~Stk_MaterialImpl(void)
{
}


STK_IMPLEMENT_CLASS(Stk_Material, Stk_Object)

Stk_Material::Stk_Material(void):Stk_Object(new Stk_MaterialImpl())
{
}

Stk_Material::~Stk_Material(void)
{
	DELETE_STK_CLASS_IMPL
}

void Stk_Material::BindingMaterial(void* i_bingdedNode)
{
	((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial = (Value*)i_bingdedNode;
}


void Stk_Material::SetMaterialType(StkMaterialTypeEnum i_enmuMaterialType)
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc->m_pImpl;
	Value* valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	if (!valMaterial)
		return;
	if (!valMaterial->HasMember(TYPE))
	{
		valMaterial->AddMember(TYPE, i_enmuMaterialType, docImpl->m_materialDoc.GetAllocator());
	}else{
		(*valMaterial)[TYPE] = i_enmuMaterialType;
	}
}


void Stk_Material::SetTransparency(float i_fTransparency)
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc->m_pImpl;
	Value* valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	if (!valMaterial)
		return;
	if (!valMaterial->HasMember(TRANSPARENCY))
	{
		valMaterial->AddMember(TRANSPARENCY, i_fTransparency, docImpl->m_materialDoc.GetAllocator());
	}else{
		(*valMaterial)[TRANSPARENCY] = i_fTransparency;
	}
}

void Stk_Material::SetDiffuseColor(float* i_diffuseColor)
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc->m_pImpl;
	Value* valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	if (!valMaterial)
		return;
	if (!valMaterial->HasMember(DIFFUSECOLOR))
	{
		Value valDiffuseColor;
		valDiffuseColor.SetArray();
		valDiffuseColor.PushBack(i_diffuseColor[0], docImpl->m_materialDoc.GetAllocator());
		valDiffuseColor.PushBack(i_diffuseColor[1], docImpl->m_materialDoc.GetAllocator());
		valDiffuseColor.PushBack(i_diffuseColor[2], docImpl->m_materialDoc.GetAllocator());
		valMaterial->AddMember(DIFFUSECOLOR, valDiffuseColor, docImpl->m_materialDoc.GetAllocator());
	}else{
		(*valMaterial)[DIFFUSECOLOR][0] = i_diffuseColor[0];
		(*valMaterial)[DIFFUSECOLOR][1] = i_diffuseColor[1];
		(*valMaterial)[DIFFUSECOLOR][2] = i_diffuseColor[2];
	}
}

StkMaterialTypeEnum Stk_Material::GetMaterialType()
{
	Stk_Document* pDoc = (Stk_Document*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_MaterialImpl* pMaterialImp = (Stk_MaterialImpl*)m_pImpl;
	//��ȡInstance��Ӧ��Json�ڵ�
	Value* valMaterial;
	if (!((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valMaterial = docImpl->GetJsonMaterial(this->GetID());
		((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial = valMaterial;
	} 
	else
	{
		valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	}

	if (!valMaterial)
		return StkMaterialTypeEnum::STK_MATERIALTYPE_SIMPLAE;
	//��ȡ��������
	return (StkMaterialTypeEnum)(*valMaterial)[TYPE].GetInt();
}

void Stk_Material::GetTransparency(float& i_fTransparency)
{
	i_fTransparency = 0.0;
	Stk_Document* pDoc = (Stk_Document*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_MaterialImpl* pMaterialImp = (Stk_MaterialImpl*)m_pImpl;
	//��ȡInstance��Ӧ��Json�ڵ�
	Value* valMaterial;
	if (!((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valMaterial = docImpl->GetJsonMaterial(this->GetID());
		((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial = valMaterial;
	} 
	else
	{
		valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	}

	if (!valMaterial)
		return;
	//��ȡ��������
	i_fTransparency = (*valMaterial)[TRANSPARENCY].GetFloat();
}

void Stk_Material::GetDiffuseColor(float* i_diffuseColor)
{
	i_diffuseColor[0] = i_diffuseColor[1] = i_diffuseColor[2] = 0.0;
	Stk_Document* pDoc = (Stk_Document*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_MaterialImpl* pMaterialImp = (Stk_MaterialImpl*)m_pImpl;
	//��ȡInstance��Ӧ��Json�ڵ�
	Value* valMaterial;
	if (!pMaterialImp->m_pJsonMaterial)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valMaterial = docImpl->GetJsonMaterial(this->GetID());
		((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial = valMaterial;
	} 
	else
	{
		valMaterial = pMaterialImp->m_pJsonMaterial;
	}

	if (!valMaterial)
		return;
	//��ȡ��������
	i_diffuseColor[0] = (*valMaterial)[DIFFUSECOLOR][0].GetFloat();
	i_diffuseColor[1] = (*valMaterial)[DIFFUSECOLOR][1].GetFloat();
	i_diffuseColor[2] = (*valMaterial)[DIFFUSECOLOR][2].GetFloat();
}

void Stk_Material::Init(Stk_Document* pDoc, unsigned int parentID, bool bForWrite/* = true*/)
{
	((Stk_MaterialImpl*)m_pImpl)->m_pDoc = pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	if (bForWrite) {

	}else{
		//��ʼ��Mesh��Ϣ
	}
}

void Stk_Material::SetDiffuseTexture(Stk_TexturePtr i_DiffuseTextureP)
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc->m_pImpl;
	Value* valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	if (!valMaterial)
		return;
	if (!valMaterial->HasMember(DIFFUSETEXTURE))
	{
		Value diffuseDiffuseTexture;
		diffuseDiffuseTexture.SetObject();
		Value diffuseDiffuseTextureRef;
		diffuseDiffuseTextureRef.SetArray();
		diffuseDiffuseTextureRef.PushBack(i_DiffuseTextureP->GetTextureImage()->GetID(), docImpl->m_materialDoc.GetAllocator());
		diffuseDiffuseTexture.AddMember(REF, diffuseDiffuseTextureRef, docImpl->m_materialDoc.GetAllocator());
		valMaterial->AddMember(DIFFUSETEXTURE, diffuseDiffuseTexture, docImpl->m_materialDoc.GetAllocator());
	}
	else {
		(*valMaterial)[DIFFUSETEXTURE].RemoveAllMembers();
		Value diffuseDiffuseTextureRef;
		diffuseDiffuseTextureRef.SetArray();
		diffuseDiffuseTextureRef.PushBack(i_DiffuseTextureP->GetTextureImage()->GetID(), docImpl->m_materialDoc.GetAllocator());
		(*valMaterial)[DIFFUSETEXTURE].AddMember(REF, diffuseDiffuseTextureRef, docImpl->m_materialDoc.GetAllocator());
	}
}

Stk_TexturePtr Stk_Material::GetDiffuseTexture()
{
	Stk_TexturePtr retStkTexturePtrP = nullptr;
	Stk_Document* pDoc = (Stk_Document*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_MaterialImpl* pMaterialImp = (Stk_MaterialImpl*)m_pImpl;
	//��ȡInstance��Ӧ��Json�ڵ�
	Value* valMaterial;
	if (!((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valMaterial = docImpl->GetJsonMaterial(this->GetID());
		((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial = valMaterial;
	}
	else
	{
		valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	}

	if (!valMaterial || !valMaterial->HasMember(DIFFUSETEXTURE))
		return retStkTexturePtrP;
	//��ȡ��������
	Value* valDiffusetexture = &(*valMaterial)[DIFFUSETEXTURE];
	if(!valDiffusetexture->HasMember(REF))
		return retStkTexturePtrP;
	int iImageId = (*valDiffusetexture)[REF][0].GetInt();

	retStkTexturePtrP = Stk_TexturePtr::CreateObject();
	retStkTexturePtrP->SetTextureImage(pDoc->GetImageByID(iImageId));
	return retStkTexturePtrP;
}

void Stk_Material::GetAmbientColor(float* i_diffuseColor)
{

}

void Stk_Material::GetReflectiveTexture(vector<Stk_TexturePtr>& o_frontTexture)
{
	Stk_TexturePtr retStkTexturePtrP = nullptr;
	Stk_Document* pDoc = (Stk_Document*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_MaterialImpl* pMaterialImp = (Stk_MaterialImpl*)m_pImpl;
	Value* valMaterial;
	if (!((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valMaterial = docImpl->GetJsonMaterial(this->GetID());
		((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial = valMaterial;
	}
	else
	{
		valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	}

	if (!valMaterial || !valMaterial->HasMember(REFLECTIVETEXTURE))
		return;
	Value* valFronttexture = &(*valMaterial)[REFLECTIVETEXTURE];
	if (!valFronttexture->HasMember(REF))
		return;
	for (int i = 0; i < (*valFronttexture)[REF].Size(); i++)
	{
		int iImageId = (*valFronttexture)[REF][i].GetInt();

		retStkTexturePtrP = Stk_TexturePtr::CreateObject();
		retStkTexturePtrP->SetTextureImage(pDoc->GetImageByID(iImageId));
		o_frontTexture.push_back(retStkTexturePtrP);
	}
}

void Stk_Material::SetMixingFactor(float mixingFactor)
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc->m_pImpl;
	Value* valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	if (!valMaterial)
		return;
	if (!valMaterial->HasMember(MIXINGFACTOR))
	{
		valMaterial->AddMember(MIXINGFACTOR, mixingFactor, docImpl->m_materialDoc.GetAllocator());
	}
	else {
		(*valMaterial)[MIXINGFACTOR] = mixingFactor;
	}
}

void Stk_Material::GetMixingFactor(float & mixingFactor)
{
	mixingFactor = 0.0;
	Stk_Document* pDoc = (Stk_Document*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_MaterialImpl* pMaterialImp = (Stk_MaterialImpl*)m_pImpl;
	//��ȡInstance��Ӧ��Json�ڵ�
	Value* valMaterial;
	if (!((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valMaterial = docImpl->GetJsonMaterial(this->GetID());
		((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial = valMaterial;
	}
	else
	{
		valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	}

	if (!valMaterial)
		return;
	//��ȡ��������
	mixingFactor = (*valMaterial)[MIXINGFACTOR].GetFloat();
}

void Stk_Material::SetFrontTexture(Stk_TexturePtr i_frontTexture)
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc->m_pImpl;
	Value* valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	if (!valMaterial)
		return;
	if (!valMaterial->HasMember(FRONTTEXTURE))
	{
		Value frontTexture;
		frontTexture.SetObject();
		Value frontTextureRef;
		frontTextureRef.SetArray();
		frontTextureRef.PushBack(i_frontTexture->GetTextureImage()->GetID(), docImpl->m_materialDoc.GetAllocator());
		frontTexture.AddMember(REF, frontTextureRef, docImpl->m_materialDoc.GetAllocator());
		valMaterial->AddMember(FRONTTEXTURE, frontTexture, docImpl->m_materialDoc.GetAllocator());
	}
	else {
		Value* valFronttexture = &(*valMaterial)[FRONTTEXTURE];
		(*valFronttexture)[REF].PushBack(i_frontTexture->GetTextureImage()->GetID(), docImpl->m_materialDoc.GetAllocator());
	}
}

void Stk_Material::GetFrontTexture(vector<Stk_TexturePtr>& o_frontTexture)
{
	Stk_TexturePtr retStkTexturePtrP = nullptr;
	Stk_Document* pDoc = (Stk_Document*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_MaterialImpl* pMaterialImp = (Stk_MaterialImpl*)m_pImpl;
	//��ȡInstance��Ӧ��Json�ڵ�
	Value* valMaterial;
	if (!((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valMaterial = docImpl->GetJsonMaterial(this->GetID());
		((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial = valMaterial;
	}
	else
	{
		valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	}

	if (!valMaterial || !valMaterial->HasMember(FRONTTEXTURE))
		return ;
	//��ȡ��������
	Value* valFronttexture = &(*valMaterial)[FRONTTEXTURE];
	if (!valFronttexture->HasMember(REF))
		return ;
	for (int i = 0;i<(*valFronttexture)[REF].Size();i++)
	{
		int iImageId = (*valFronttexture)[REF][i].GetInt();

		retStkTexturePtrP = Stk_TexturePtr::CreateObject();
		retStkTexturePtrP->SetTextureImage(pDoc->GetImageByID(iImageId));
		o_frontTexture.push_back(retStkTexturePtrP);
	}	
}

void Stk_Material::SetBackTexture(Stk_TexturePtr i_backTexture)
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc->m_pImpl;
	Value* valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	if (!valMaterial)
		return;
	if (!valMaterial->HasMember(BACKTEXTURE))
	{
		Value backTexture;
		backTexture.SetObject();
		Value backTextureRef;
		backTextureRef.SetArray();
		backTextureRef.PushBack(i_backTexture->GetTextureImage()->GetID(), docImpl->m_materialDoc.GetAllocator());
		backTexture.AddMember(REF, backTextureRef, docImpl->m_materialDoc.GetAllocator());
		valMaterial->AddMember(BACKTEXTURE, backTexture, docImpl->m_materialDoc.GetAllocator());
	}
	else {
		Value* valBacktexture = &(*valMaterial)[BACKTEXTURE];
		(*valBacktexture)[REF].PushBack(i_backTexture->GetTextureImage()->GetID(), docImpl->m_materialDoc.GetAllocator());
	}
}

void Stk_Material::GetBackTexture(vector<Stk_TexturePtr>& o_backTexture)
{
	Stk_TexturePtr retStkTexturePtrP = nullptr;
	Stk_Document* pDoc = (Stk_Document*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_MaterialImpl* pMaterialImp = (Stk_MaterialImpl*)m_pImpl;
	//��ȡInstance��Ӧ��Json�ڵ�
	Value* valMaterial;
	if (!((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valMaterial = docImpl->GetJsonMaterial(this->GetID());
		((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial = valMaterial;
	}
	else
	{
		valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	}

	if (!valMaterial || !valMaterial->HasMember(BACKTEXTURE))
		return;
	//��ȡ��������
	Value* valBacktexture = &(*valMaterial)[BACKTEXTURE];
	if (!valBacktexture->HasMember(REF))
		return;
	for (int i = 0; i < (*valBacktexture)[REF].Size(); i++)
	{
		int iImageId = (*valBacktexture)[REF][i].GetInt();

		retStkTexturePtrP = Stk_TexturePtr::CreateObject();
		retStkTexturePtrP->SetTextureImage(pDoc->GetImageByID(iImageId));
		o_backTexture.push_back(retStkTexturePtrP);
	}
}

void Stk_Material::SetSpecularTexture(Stk_TexturePtr i_SpecularTextureP)
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc->m_pImpl;
	Value* valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	if (!valMaterial)
		return;
	if (!valMaterial->HasMember(SPECULARTEXTURE))
	{
		Value specularTexture;
		specularTexture.SetObject();
		Value specularTextureRef;
		specularTextureRef.SetArray();
		specularTextureRef.PushBack(i_SpecularTextureP->GetTextureImage()->GetID(), docImpl->m_materialDoc.GetAllocator());
		specularTexture.AddMember(REF, specularTextureRef, docImpl->m_materialDoc.GetAllocator());
		valMaterial->AddMember(SPECULARTEXTURE, specularTexture, docImpl->m_materialDoc.GetAllocator());
	}
	else {
		(*valMaterial)[SPECULARTEXTURE].RemoveAllMembers();
		Value specularTextureRef;
		specularTextureRef.SetArray();
		specularTextureRef.PushBack(i_SpecularTextureP->GetTextureImage()->GetID(), docImpl->m_materialDoc.GetAllocator());
		(*valMaterial)[SPECULARTEXTURE].AddMember(REF, specularTextureRef, docImpl->m_materialDoc.GetAllocator());
	}
}

Stk_TexturePtr Stk_Material::GetSpecularTexture()
{
	Stk_TexturePtr retStkTexturePtrP = nullptr;
	Stk_Document* pDoc = (Stk_Document*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_MaterialImpl* pMaterialImp = (Stk_MaterialImpl*)m_pImpl;
	//��ȡInstance��Ӧ��Json�ڵ�
	Value* valMaterial;
	if (!((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valMaterial = docImpl->GetJsonMaterial(this->GetID());
		((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial = valMaterial;
	}
	else
	{
		valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	}

	if (!valMaterial || !valMaterial->HasMember(SPECULARTEXTURE))
		return retStkTexturePtrP;
	//��ȡ��������
	Value* valSpeculartexture = &(*valMaterial)[SPECULARTEXTURE];
	if (!valSpeculartexture->HasMember(REF))
		return retStkTexturePtrP;
	int iImageId = (*valSpeculartexture)[REF][0].GetInt();

	retStkTexturePtrP = Stk_TexturePtr::CreateObject();
	retStkTexturePtrP->SetTextureImage(pDoc->GetImageByID(iImageId));
	return retStkTexturePtrP;
}

void Stk_Material::SetMaterialName(wstring i_name)
{
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc->m_pImpl;
	Value* valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	if (!valMaterial)
		return;
	string strNameUTF8 = "";
	ChineseCode::UnicodeToUTF_8(i_name.c_str(), strNameUTF8);
	if (!valMaterial->HasMember(NAME))
	{
		valMaterial->AddMember(NAME, strNameUTF8, docImpl->m_materialDoc.GetAllocator());
	}
	else {
		(*valMaterial)[NAME].SetString(strNameUTF8,docImpl->m_materialDoc.GetAllocator());
	}
}

wstring Stk_Material::GetMaterialName()
{
	wstring retName = L"";
	Stk_Document* pDoc = (Stk_Document*)((Stk_MaterialImpl*)m_pImpl)->m_pDoc;
	Stk_DocumentImpl* docImpl = (Stk_DocumentImpl*)pDoc->m_pImpl;
	Stk_MaterialImpl* pMaterialImp = (Stk_MaterialImpl*)m_pImpl;
	//��ȡInstance��Ӧ��Json�ڵ�
	Value* valMaterial;
	if (!((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial)
	{
		//valueNode = docImpl->GetNode(this->GetID()); 
		valMaterial = docImpl->GetJsonMaterial(this->GetID());
		((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial = valMaterial;
	}
	else
	{
		valMaterial = ((Stk_MaterialImpl*)m_pImpl)->m_pJsonMaterial;
	}

	if (!valMaterial)
		return retName;
	//��ȡ��������
	Value::MemberIterator nameIter = valMaterial->FindMember(NAME);
	if ((nameIter != valMaterial->MemberEnd() && nameIter->value.IsString()))
	{
		string strNodeName = nameIter->value.GetString();
		retName = ChineseCode::UTF_8ToUnicode(strNodeName.c_str());
	}
	return retName;
}

	}//svllib
}//hoteamsoft
